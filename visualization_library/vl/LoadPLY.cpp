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

#include "vl/LoadPLY.hpp"

vl::ref<vl::Geometry> LoadPLY::loadPLY(const std::string& plyfile)
{
  vl::ref<vl::Geometry> geom = new vl::Geometry;
  vl::ref<vl::GPUArrayVec3> verts = new vl::GPUArrayVec3;
  geom->setVertexArray( verts.get() );
  vl::ref<vl::DrawElementsUInt> polys = new vl::DrawElementsUInt(vl::PT_TRIANGLES);
  geom->addDrawCall( polys.get() );

  FILE* fin = fopen( plyfile.c_str(), "rt" );
  if(fin == NULL)
  {
    vl::Log::error( vl::Say("Could not open file: %s\n") << plyfile );
    return NULL;
  }

  char line[1024];
  memset(line,0,1024); fgets(line,1023,fin);
  if (strstr(line,"ply") == 0)
  {
    fclose(fin);
    vl::Log::error( vl::Say("Not a PLY file: %s\n") << plyfile );
    return NULL;
  }

  memset(line,0,1024); fgets(line,1023,fin);
  bool ascii_format = true;
  if (strstr(line,"format ascii 1.0"))
    ascii_format = true;

  else
  {
    fclose(fin);
    vl::Log::error( vl::Say("Not a PLY 1.0 file: %s\n") << plyfile );
    return NULL;
  }

  int vert_count = 0;
  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if (strstr(line,"element vertex") == NULL)
      continue;
    else
    {
      vert_count = atoi(line+strlen("element vertex"));
      break;
    }
  }

  int face_count = 0;
  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if (strstr(line,"element face") == NULL)
      continue;
    else
    {
      face_count = atoi(line+strlen("element face"));
      break;
    }
  }

  while(!feof(fin))
  {
    fgets(line,1023,fin);
    if ( strstr(line,"end_header") )
      break;
  }

  if (ascii_format)
  {

    for(int i=0; i<vert_count; i++)
    {
      fgets(line,1023,fin);
      vl::vec3 v;
      sscanf( line, "%f %f %f", &v.x(), &v.y(), &v.z() );
      verts->push_back(v);
    }

    for(int i=0; i<face_count; i++)
    {
      fgets(line,1023,fin);
      int n, a, b, c;
      sscanf( line, "%d %d %d %d", &n, &a, &b, &c );
      if(n==3)
      {
        polys->addIndex(a);
        polys->addIndex(c);
        polys->addIndex(b);
      }
    }
  }
  else
  {

  }

  fclose(fin);
  if (vl::VERBOSITY_LEVEL > 1)
    vl::Log::info( vl::Say("PLY: verts=%n, faces=%n\n") << vert_count << face_count );
  return geom.get();
}
