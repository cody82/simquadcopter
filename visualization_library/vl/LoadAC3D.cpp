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

#include "vl/LoadAC3D.hpp"
#include "vl/CHECK.hpp"
#include "vl/Painter.hpp"
#include "vl/Geometry.hpp"
#include "vl/Image.hpp"
#include "vl/Texture.hpp"
#include <map>

using namespace vl;

LoadAC3D::mesh_info::mesh_info()
{
	TexRep[0] = 1;
	TexRep[1] = 1;
	NumVert = 0;
	NumSurf = 0;
	NumKids = 0;
}

LoadAC3D::LoadAC3D()
{
  verbose = true;
	fin = NULL;
}

bool LoadAC3D::parseAC3D(const std::string& file)
{
  if (verbose)
    Log::info( Say("Loading '%s'\n") << file.c_str());
	fin = fopen(file.c_str(), "rt");
  if (!fin)
    return false;

	char buf[256];

	fscanf(fin, "%s", buf);

	if (std::string(buf) != "AC3Db")
  {
    Log::error("Not an AC3Db file!\n");
		fclose(fin);
		fin = NULL;
		return false;
	}

	while( !feof(fin) )
  {
		fscanf(fin, "%s", buf);
		if (std::string(buf) == "MATERIAL")
			readMaterial();
		else
		if (std::string(buf) == "OBJECT")
			readObject();
		else
    {
			Log::error("AC3D parse error\n");
			fclose(fin);
			fin = NULL;
			return false;
		}
	}

	fclose(fin);
	fin = NULL;
  return true;
}

vec3 LoadAC3D::readColor()
{
	float r,g,b;
	if ( fscanf(fin, "%f %f %f", &r, &g, &b) != 3)
		Log::error("io error reading color\n");
	return vec3(r,g,b);
}

vec3 LoadAC3D::readVector()
{
	float x,y,z;
	if ( fscanf(fin, "%f %f %f", &x, &y, &z) != 3)
		Log::error("io error reading vector\n");
	return vec3(x,y,z);
}

float LoadAC3D::readFloat()
{
	float x;
	if ( fscanf(fin, "%f", &x) != 1 )
		Log::error("io error reading float\n");
	return x;
}

int LoadAC3D::readInt()
{
	int x;
	if ( fscanf(fin, "%d", &x) != 1 )
		Log::error("io error reading float\n");
	return x;
}

int LoadAC3D::readHex()
{
	char buf[256];
	fscanf(fin, "%s", buf);
	buf[0] = ' ';
	buf[1] = ' ';
	int x;
	if ( sscanf(buf, "%x", &x) != 1 )
		Log::error("io error reading hex\n");
	return x;
}

std::string LoadAC3D::readString()
{
	char c=0;
	do
  {
		fread(&c, 1,1, fin);
		if (feof(fin))
			return "";
	}
  while (c == ' ' || c==13 || c==10 || c=='\t' || c==0);

	std::string str;
	char stop = ' ';
	if (c=='"')
  {
		stop = '"';
	}
  else
		str.push_back(c);

	c = 0;
	do
  {
		if ( fread(&c, 1,1, fin) )
    {
			if ( c != stop)
				str.push_back(c);
		}
	}
  while (c != stop && c!=13 && c!=10 && c!='\t' && c!=0);

	return str;
}

void LoadAC3D::readMaterial()
{

	material_info material;

	material.Name = readString();
	readString(); // rgb
	material.Diffuse = readColor();
	readString(); // amb
	material.Ambient = readColor();
	readString(); // emis
	material.Emission = readColor();
	readString(); // spec
	material.Specular = readColor();
	readString(); // shi
	material.Shininess = readFloat();
	readString(); // trans
	material.Trans = 1 - readFloat();

	Material.push_back(material);
}

void LoadAC3D::readObject()
{
	mesh_info mesh;

	readString(); // world poly group

	std::string block;
	do
  {
		block = readString();
		if (feof(fin))
			break;

		if (block == "name")
    {
			mesh.Name = readString();
		} else
		if (block == "data")
    {

			char buf[256];
			fgets(buf, 255, fin);
			int data_size;
			sscanf(buf, "%d", &data_size);
      std::vector<char> data;
      data.resize(data_size);
			memset(&data[0], 0, data_size);
			fread(&data[0], 1, data_size, fin);
			mesh.Data = &data[0];
		}
    else
		if (block == "texture")
    {
			mesh.Texture = readString();
		} else
		if (block == "texrep")
    {
			mesh.TexRep[0] = readFloat();
			mesh.TexRep[1] = readFloat();
		} else
		if (block == "rot")
    {
			mesh.Matrix.setX( (vec3d)readVector() );
			mesh.Matrix.setY( (vec3d)readVector() );
			mesh.Matrix.setZ( (vec3d)readVector() );
		} else
		if (block == "loc")
    {
			mesh.Matrix.setT( (vec3d)readVector() );
		} else
		if (block == "url")
    {
			mesh.Url = readString();
		} else
		if (block == "numvert")
    {
			mesh.NumVert = readInt();
			for(int i=0; i<mesh.NumVert; i++)
      {
				mesh.Vert.push_back( readVector() );
			}
		} else
		if (block == "numsurf")
    {
			mesh.NumSurf = readInt();
			for(int i=0; i<mesh.NumSurf; i++)
      {
				surface_info surf;
				std::string tmp;
				do
        {
					tmp = readString();

					if (tmp == "SURF")
          {
						surf.Flags = readHex();
					}
					if (tmp == "mat")
          {
						surf.Material = readInt();
					}
					if (tmp == "refs")
          {
						surf.VertCount = readInt();
						for(int j=0; j<surf.VertCount; j++)
            {
							vert_info vert;
							vert.Vert = readInt();
							vert.U = readFloat();
							vert.V = readFloat();
							surf.Vertex.push_back( vert );
						}
					}
				}
        while (tmp != "refs");
				mesh.Surface.push_back(surf);
			}
		}
    else
		if (block == "kids")
    {
			mesh.NumKids = readInt();
			Mesh.push_back(mesh);
		}
    else
    {
      if (verbose)
        Log::warning( Say("unknown block '%s'\n") << block.c_str() );

			char c=0;
			do
      {
				fread(&c,1,1,fin);
			}
      while( !feof(fin) && (c != 10) && (c != 13) ) ;
		}
	}
  while(block != "kids");
}

ref<ShaderNode> LoadAC3D::loadAC3D(const std::string& file, const std::string& texture_directory)
{
  if (parseAC3D(file) == false)
    return NULL;

  ref<ShaderNode> root = new ShaderNode;

  std::vector< ref<Painter> > mat_map;
  for(unsigned imat=0; imat<Material.size(); imat++)
  {
    mat_map.push_back(new Painter);
    root->addChild(mat_map.back().get());
  }

  for(unsigned imesh=0; imesh<Mesh.size(); imesh++)
  {
    if ( Mesh[imesh].Surface.empty() )
      continue;

    ref<Actor> act = new Actor;
    ref<Geometry> geom = new Geometry;
    ref<GPUArrayVec3> verts = new GPUArrayVec3;
    ref<GPUArrayVec2> uv    = new GPUArrayVec2;
    ref<DrawElementsUInt> polys = new DrawElementsUInt;
    geom->addDrawCall( polys.get() );
    geom->setVertexArray( verts.get() );
    geom->setTexCoordArray(0, uv.get());
    act->setDrawable(geom.get());
    int mat_index = Mesh[imesh].Surface[0].Material;
    mat_map[ mat_index ]->addActor(act.get());

    for(unsigned isurf=0; isurf<Mesh[imesh].Surface.size(); isurf++)
    {
      if ( (Mesh[imesh].Surface[isurf].Flags & 0xF) != 0 ) // not a poly
        continue;
      CHECK( Mesh[imesh].Surface[isurf].VertCount >= 3 )
      for( int ivert=1; ivert<Mesh[imesh].Surface[isurf].VertCount-1; ivert++)
      {
        int vert_idx[] = { 0, ivert, ivert+1 };
        for(int i=0; i<3; i++)
        {
          polys->addIndex( verts->size() );
          int iv = Mesh[imesh].Surface[isurf].Vertex[ vert_idx[i] ].Vert;
          verts->push_back(  Mesh[imesh].Vert[ iv ] );
          uv->push_back( vec2(Mesh[imesh].Surface[isurf].Vertex[ vert_idx[i] ].U, Mesh[imesh].Surface[isurf].Vertex[ vert_idx[i] ].V) );
        }
      }
    }

    geom->computeSmoothNormals();

    mat_map[ mat_index ]->shader()->glMaterial()->setAmbient( vec4(Material[ mat_index ].Ambient, 1.0f) );
    mat_map[ mat_index ]->shader()->glMaterial()->setDiffuse( vec4(Material[ mat_index ].Diffuse, 1.0f) );
    mat_map[ mat_index ]->shader()->glMaterial()->setEmission( vec4(Material[ mat_index ].Emission, 1.0f) );
    mat_map[ mat_index ]->shader()->glMaterial()->setSpecular( vec4(Material[ mat_index ].Specular, 1.0f) );
    mat_map[ mat_index ]->shader()->glMaterial()->setShininess( Material[ mat_index ].Shininess );
    mat_map[ mat_index ]->shader()->glMaterial()->setTransparency( Material[ mat_index ].Trans );

    if ( Material[ mat_index ].Trans < 1.0f )
    {
      mat_map[ mat_index ]->shader()->enable(EN_BLEND);
      mat_map[ mat_index ]->shader()->glBlendFunc()->set(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);
    }

    if ( Mesh[imesh].Texture.length() )
    {
      mat_map[ mat_index ]->shader()->textureUnit(0)->setTexture( new Texture( new Image( texture_directory + Mesh[imesh].Texture ) ) );
    }

    if ( (Mesh[imesh].Surface[0].Flags & 0x20) )
    {
      mat_map[ mat_index ]->shader()->disable(EN_CULL_FACE);
      mat_map[ mat_index ]->shader()->glLightModel()->setTwoSide(true);
    }

  }

  for(unsigned imat=0; imat<Material.size(); imat++)
  {
    if (mat_map[imat]->actorCount() == 0)
      root->removeChild( mat_map[imat].get() );
  }

  return root;
}

