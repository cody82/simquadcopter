/*
  Copyright (C) 2008 Michele Bosi

  This file is part of Visualization Library.

  Visualization Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Visualization Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Visualization Library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "vlut/GeometricalPrimitives.hpp"
#include "vl/Geometry.hpp"
#include "vl/vec3d.hpp"

using namespace vl;

vl::ref<Geometry> vlut::makeIcosphere(const vec3d& pos, double radius, int detail)
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();

  ref<GPUArrayVec3> coords = new GPUArrayVec3;
  ref<GPUArrayVec3> norms = new GPUArrayVec3;
  ref<DrawElementsUInt> polys = new DrawElementsUInt(PT_TRIANGLES);

  const double X = 0.525731112119133606f;
  const double Z = 0.850650808352039932f;
  std::vector< vec3d > verts;
  verts.push_back( vec3d(-X, 0, Z) );
  verts.push_back( vec3d(X, 0, Z) );
  verts.push_back( vec3d(-X, 0, -Z) );
  verts.push_back( vec3d(X, 0, -Z) );
  verts.push_back( vec3d(0, Z, X) );
  verts.push_back( vec3d(0, Z, -X) );
  verts.push_back( vec3d(0, -Z, X) );
  verts.push_back( vec3d(0, -Z, -X) );
  verts.push_back( vec3d(Z, X, 0) );
  verts.push_back( vec3d(-Z, X, 0) );
  verts.push_back( vec3d(Z, -X, 0) );
  verts.push_back( vec3d(-Z, -X, 0) );

  int idxs[] = { 
    1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4, 
    1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2, 
    3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
    10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
  };

  std::vector<int> indices;
  for(int i=0; i<4*5*3; ++i)
    indices.push_back(idxs[i]);

  if (detail>8)
    detail = 8;
  if (detail<0)
    detail = 0;
  for(int i=0; i<detail; i++)
  {
    std::vector<int> indices2;
    std::vector< vec3d > verts2;
    for( int i=0, idx=0; i<(int)indices.size(); i+=3)
    {
      indices2.push_back(idx++); indices2.push_back(idx++); indices2.push_back(idx++);
      indices2.push_back(idx++); indices2.push_back(idx++); indices2.push_back(idx++);
      indices2.push_back(idx++); indices2.push_back(idx++); indices2.push_back(idx++);
      indices2.push_back(idx++); indices2.push_back(idx++); indices2.push_back(idx++);

      vec3d v1 = verts[ indices[i+0] ]; v1.normalize();
      vec3d v2 = verts[ indices[i+1] ]; v2.normalize();
      vec3d v3 = verts[ indices[i+2] ]; v3.normalize();
      vec3d a = (v1 + v2) * 0.5f; a.normalize();
      vec3d b = (v2 + v3) * 0.5f; b.normalize();
      vec3d c = (v3 + v1) * 0.5f; c.normalize();
      verts2.push_back(v1); verts2.push_back( a); verts2.push_back(c);
      verts2.push_back( a); verts2.push_back(v2); verts2.push_back(b);
      verts2.push_back( a); verts2.push_back( b); verts2.push_back(c);
      verts2.push_back( c); verts2.push_back( b); verts2.push_back(v3);
    }
    verts = verts2;
    indices = indices2;
  }

  for( int i=0; i<(int)verts.size(); ++i )
  {
    coords->push_back( (verts[i]*radius + pos).to_vec3() );
    vec3d n = verts[i];
    n.normalize();
    norms->push_back(n.to_vec3());
  }

  for(int i=0; i<(int)indices.size(); ++i)
  {
    CHECK( indices[i] < (int)coords->size() )
    polys->addIndex( indices[i] );
  }

  geom->setVertexArray(coords.get());
  geom->setNormalArray(norms.get());
  geom->addDrawCall(polys.get());

  return geom;
}

vl::ref<Geometry> vlut::makeUVSphere( const vec3d& origin, double diameter, int phi, int theta)
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  diameter = diameter / 2.0f;
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray(vert3.get());

  vert3->push_back( (vec3d(0,1*diameter,0) + origin).to_vec3() );
  for(int i=0; i<theta; i++)
  {
    for(int j=0; j<phi; j++)
    {

      vec3d v(0,1*diameter,0);
      v = mat4d::rotation(180.0f/(theta+1)*(i+1),0,0,1) * v;
      v = mat4d::rotation(360.0f/phi*j,0,1,0)  * v;
      vert3->push_back( (v+origin).to_vec3() );
    }
  }
  vert3->push_back( (vec3d(0,-1*diameter,0) + origin).to_vec3() );

  ref<DrawElementsUInt> polys = new DrawElementsUInt;

  for(int i=0; i<theta-1; i++)
  {
    for(int j=0; j<phi; j++)
    {
      polys->addIndex( 1+phi*(i+0)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+1)%phi );
      polys->addIndex( 1+phi*(i+0)+(j+1)%phi );

      polys->addIndex( 1+phi*(i+0)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+1)%phi );
    }
  }

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( 0 );
    polys->addIndex( 1+(j+0)%phi );
    polys->addIndex( 1+(j+1)%phi );
  }

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( (int)geom->vertexArray()->size()-1 );
    polys->addIndex( 1+phi*(theta-1)+(j+1)%phi );
    polys->addIndex( 1+phi*(theta-1)+(j+0)%phi );
  }
  geom->addDrawCall(polys.get());
  return geom;
}

vl::ref<Geometry> vlut::makeCylinder( const vec3d& origin, double diameter, double height, int phi, int theta, bool top, bool bottom)
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  diameter = diameter / 2;
  height = height / 2;
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray(vert3.get());

  vert3->push_back( (vec3d(0, height, 0) + origin).to_vec3() );
  for(int i=0; i<theta; i++)
  {
    for(int j=0; j<phi; j++)
    {
      vec3d v(1*diameter, 1*height - 2*height*((double)i/(theta-1)), 0);
      v = mat4d::rotation(360.0f/phi*j,0,1,0) * v;
      vert3->push_back( (v + origin).to_vec3() );
    }
  }
  vert3->push_back( (vec3d(0, -height, 0) + origin).to_vec3() );

  ref<DrawElementsUInt> polys = new DrawElementsUInt;

  for(int i=0; i<theta-1; i++)
  {
    for(int j=0; j<phi; j++)
    {
      polys->addIndex( 1+phi*(i+0)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+1)%phi );
      polys->addIndex( 1+phi*(i+0)+(j+1)%phi );

      polys->addIndex( 1+phi*(i+0)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+0)%phi );
      polys->addIndex( 1+phi*(i+1)+(j+1)%phi );
    }
  }
  if (top)

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( 0 );
    polys->addIndex( 1+(j+0)%phi );
    polys->addIndex( 1+(j+1)%phi );
  }
  if (bottom)

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( (int)geom->vertexArray()->size()-1 );
    polys->addIndex( 1+phi*(theta-1)+(j+1)%phi );
    polys->addIndex( 1+phi*(theta-1)+(j+0)%phi );
  }
  geom->addDrawCall(polys.get());
  return geom;
}

vl::ref<Geometry> vlut::makeTorus( const vec3d& origin, double diameter, double thickness, int phi, int theta )
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();

  thickness /= 2.0f;
  const double radius = diameter / 2.0f - thickness;

  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray(vert3.get());

  for(int i=0; i<theta; i++)
  {
    for(int j=0; j<phi; j++)
    {
      vec3d v(thickness, 0, 0);
      vec3d o(radius, 0, 0);
      v = mat4d::rotation(360.0f/phi*j,0,1,0) * v;
      v = mat4d::rotation(360.0f/theta*i,0,0,1) * v;
      o = mat4d::rotation(360.0f/theta*i,0,0,1) * o;
      vert3->push_back( (v + o + origin).to_vec3() );
    }
  }

  ref<DrawElementsUInt> polys = new DrawElementsUInt;

  for(int i=0; i<theta; i++)
  {
    for(int j=0; j<phi; j++)
    {
      int i1 = (i+1) % theta;
      polys->addIndex( phi*i +(j+0)%phi );
      polys->addIndex( phi*i +(j+1)%phi );
      polys->addIndex( phi*i1+(j+1)%phi );

      polys->addIndex( phi*i +(j+0)%phi );
      polys->addIndex( phi*i1+(j+1)%phi );
      polys->addIndex( phi*i1+(j+0)%phi );
    }
  }

  geom->addDrawCall(polys.get());
  return geom;
}

vl::ref<Geometry> vlut::makeBox( const vec3d& origin, double xside, double yside, double zside, bool tex_coords)
{

  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  ref<GPUArrayVec2> texc2 = new GPUArrayVec2;
  geom->setVertexArray(vert3.get());
  geom->setTexCoordArray(0, texc2.get());

  double x=xside/2.0f;
  double y=yside/2.0f;
  double z=zside/2.0f;

  vec3 a0( (vec3d(+x,+y,+z) + origin).to_vec3() );
  vec3 a1( (vec3d(-x,+y,+z) + origin).to_vec3() );
  vec3 a2( (vec3d(-x,-y,+z) + origin).to_vec3() );
  vec3 a3( (vec3d(+x,-y,+z) + origin).to_vec3() );
  vec3 a4( (vec3d(+x,+y,-z) + origin).to_vec3() );
  vec3 a5( (vec3d(-x,+y,-z) + origin).to_vec3() );
  vec3 a6( (vec3d(-x,-y,-z) + origin).to_vec3() );
  vec3 a7( (vec3d(+x,-y,-z) + origin).to_vec3() );

  ref<DrawArrays> polys = new DrawArrays(PT_TRIANGLES,0,36);
  geom->addDrawCall( polys.get() );

  vert3->push_back(a1); vert3->push_back(a2); vert3->push_back(a3); vert3->push_back(a1); vert3->push_back(a3); vert3->push_back(a0);

  vert3->push_back(a2); vert3->push_back(a6); vert3->push_back(a7); vert3->push_back(a2); vert3->push_back(a7); vert3->push_back(a3);

  vert3->push_back(a6); vert3->push_back(a5); vert3->push_back(a4); vert3->push_back(a6); vert3->push_back(a4); vert3->push_back(a7);

  vert3->push_back(a5); vert3->push_back(a1); vert3->push_back(a0); vert3->push_back(a5); vert3->push_back(a0); vert3->push_back(a4);

  vert3->push_back(a3); vert3->push_back(a7); vert3->push_back(a4); vert3->push_back(a3); vert3->push_back(a4); vert3->push_back(a0);

  vert3->push_back(a1); vert3->push_back(a5); vert3->push_back(a6); vert3->push_back(a1); vert3->push_back(a6); vert3->push_back(a2);

  if (tex_coords)
  {
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,0) ); 
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(1,1) );
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,0) ); 
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(1,1) );
    texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(1,1) ); texc2->push_back( vec2(0,1) ); 
    texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(0,0) );
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,0) ); 
    texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(1,1) );
    texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,0) ); texc2->push_back( vec2(1,1) ); 
    texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,1) ); texc2->push_back( vec2(0,1) );
    texc2->push_back( vec2(1,1) ); texc2->push_back( vec2(0,1) ); texc2->push_back( vec2(0,0) ); 
    texc2->push_back( vec2(1,1) ); texc2->push_back( vec2(0,0) ); texc2->push_back( vec2(1,0) );
  }

  return geom;
}

vl::ref<Geometry> vlut::makePyramid( const vec3d& origin, double side, double height, bool bottom)
{
  double s2 = side/2.0f;
  double pitagora = sqrt(s2*s2 + s2*s2);
  ref<Geometry> geom = makeCone(vec3d(0,0,0), 2.0f*pitagora, height, 4, bottom);
  geom->transform( mat4d::translation(origin) * mat4d::rotation(45,0,1,0) );
  return geom;
}

vl::ref<Geometry> vlut::makeCone( const vec3d& origin, double diameter, double height, int phi, bool bottom)
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray( vert3.get() );

  diameter = diameter / 2;

  vert3->push_back( (vec3d(0, height/2.0f, 0) + origin).to_vec3() );
  for(int j=0; j<phi; j++)
  {
    vec3d v(1*diameter, -height/2.0f, 0);
    v = mat4d::rotation(360.0f/phi*j,0,1,0) * v;
    vert3->push_back( (v + origin).to_vec3()  );
  }
  vert3->push_back( (vec3d(0, -height/2.0f, 0) + origin).to_vec3() );

  ref<DrawElementsUInt> polys = new DrawElementsUInt;

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( 0 );
    polys->addIndex( 1+(j+0)%phi );
    polys->addIndex( 1+(j+1)%phi );
  }
  if (bottom)

  for(int j=0; j<phi; j++)
  {
    polys->addIndex( (int)geom->vertexArray()->size()-1 );
    polys->addIndex( 1+(j+1)%phi );
    polys->addIndex( 1+(j+0)%phi );
  }
  geom->addDrawCall(polys.get());
  return geom;
}

vl::ref<Geometry> vlut::makeGrid( const vec3d& origin, double xside, double zside, int x, int z, bool gen_tex_coord )
{
  ref<Geometry> geom = new Geometry;
  geom->setName("Grid");

  geom->clearArrays();
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  ref<GPUArrayVec2> text2 = new GPUArrayVec2;
  geom->setVertexArray( vert3.get() );
  geom->setTexCoordArray( 0, text2.get() );

  CHECK(x>=2)
  CHECK(z>=2)
  double du = 1.0f / (x-1);
  double dv = 1.0f / (z-1);
  double dx = xside / (x-1);
  double dz = zside / (z-1);
  xside /= 2.0f;
  zside /= 2.0f;

  for(int i=0; i<z; i++)
    for(int j=0; j<x; j++)
    {
      vert3->push_back( (vec3d(-xside+j*dx, 0, -zside+i*dz) + origin).to_vec3() );
      if (gen_tex_coord)
        text2->push_back( vec2( float(j*du), float(i*dv) ) );
    }

  ref<DrawElementsUInt> polys = new DrawElementsUInt;

  for(int i=0; i<z-1; i++)
    for(int j=0; j<x-1; j++)
    {
      polys->addIndex( j+0 + x*(i+0) );
      polys->addIndex( j+1 + x*(i+1) );
      polys->addIndex( j+1 + x*(i+0) );

      polys->addIndex( j+0 + x*(i+0) );
      polys->addIndex( j+0 + x*(i+1) );
      polys->addIndex( j+1 + x*(i+1) );
    }

  geom->addDrawCall(polys.get());

  return geom;
}

vl::ref<Geometry> vlut::makePoints( const std::vector< vec3d>& pos, const vec4& color )
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray( vert3.get() );
  ref<GPUArrayVec4> col4 = new GPUArrayVec4;
  geom->setColorArray( col4.get() );

  vert3->reserve((int)pos.size());
  col4->reserve((int)pos.size());
  for(unsigned i=0; i<pos.size(); i++)
  {
    vert3->push_back( pos[i].to_vec3() );
    col4->push_back( color );
  }

  geom->addDrawCall( new DrawArrays(vl::PT_POINTS, 0, vert3->size() ));

  return geom;

}

vl::ref<Geometry> vlut::makeIcosahedron( const vec3d& origin, double diameter )
{
  ref<Geometry> geom = new Geometry;

  geom->clearArrays();
  ref<GPUArrayVec3> vert3 = new GPUArrayVec3;
  geom->setVertexArray(vert3.get());

  const double x = 0.525731112119133606f / 1.0f;
  const double z = 0.850650808352039932f / 1.0f;
  const double radius = diameter / 2.0f;

  vert3->push_back( (origin + vec3d(-x, 0.0, z)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(x, 0.0, z)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(-x, 0.0, -z)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(x, 0.0, -z)*radius).to_vec3() );

  vert3->push_back( (origin + vec3d(0.0, z, x)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(0.0, z, -x)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(0.0, -z, x)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(0.0, -z, -x)*radius).to_vec3() );

  vert3->push_back( (origin + vec3d(z, x, 0.0)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(-z, x, 0.0)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(z, -x, 0.0)*radius).to_vec3() );
  vert3->push_back( (origin + vec3d(-z, -x, 0.0)*radius).to_vec3() );

  GLint faces[20][3] = {
    {1,4,0}, {4,9,0}, {4,5,9}, {8,5,4}, {1,8,4}, 
    {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2}, 
    {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0}, 
    {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
  };

  ref<DrawElementsUInt> polys = new DrawElementsUInt;
  polys->resize(20*3);
  memcpy(polys->localPtr(), faces, sizeof(GLint)*20*3);
  geom->addDrawCall(polys.get());

  return geom;
}

ref<Geometry> vlut::makeCircle( vec3d origin, double radius, int slices )
{
  ref< Geometry > belt = new Geometry;
  ref< GPUArrayVec3 > points = new GPUArrayVec3;
  belt->setVertexArray(points.get());
  for(int i=0; i<slices; i++)
  {
    double t = 360.0f * i / slices;
    vec3d v = mat4d::rotation(t,0,1,0) * vec3d(radius,0,0) + origin;
    points->push_back(v.to_vec3());
  }
  belt->addDrawCall( new DrawArrays(PT_LINE_LOOP, 0, points->size()) );
  return belt;
}

