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

#ifndef Load3DS_INCLUDE_DEFINE
#define Load3DS_INCLUDE_DEFINE

#include "vl/ShaderNode.hpp"
#include <stdio.h>
#include <string>
#include <vector>

namespace vl
{

  class Load3DS 
  {
  public:

	  class texture_info 
    {
	  public:
		  texture_info(): uscale(1), vscale(1), uoffset(1), voffset(1), rotation(0) {}
  		
		  std::string filename;
		  float uscale, vscale, uoffset, voffset, rotation;
	  };

	  class material_info 
    {
	  public:
		  material_info(): shininess(0), shininess_strength(0), transparency(0), double_sided(false) {}
		  void print();
  		
		  std::string name;
		  vec3 ambient, diffuse, specular;
		  float shininess, shininess_strength;
		  float transparency;
		  bool double_sided;
		  texture_info texture1;
		  texture_info texture2;		
	  };

	  class coord_system_info 
    {
	  public:
		  coord_system_info();
  		
		  vec3 xaxis, yaxis, zaxis, origin;
	  };

	  class tri_face_info 
    {
	  public:
		  tri_face_info(): a(0), b(0), c(0), flags(0) {}
  		
		  unsigned short a,b,c,flags;
	  };

	  class material_face_mapping_info 
    {
	  public:
		  std::string material_name;
		  std::vector<unsigned short> mapped_face;
	  };

	  class object_info 
    {
	  public:
		  void print();
  		
		  std::string name;
		  std::vector<vec3> vert_list;
		  std::vector<tri_face_info> face_list;
		  std::vector<material_face_mapping_info> mat_face_map;
		  std::vector<vec2> uv_coords;
		  coord_system_info coord_system;
	  };

	  Load3DS();
	  bool parse3DS(const std::string& name);

    ref<ShaderNode> load3DS(const std::string& file, const std::string& texture_directory  = "./");

  protected:
	  unsigned char readByte();
	  unsigned short readWord();
	  unsigned int readDWord();
	  float  readFloat();
	  vec3 readVec3();
	  vec3 readColByte3();
	  vec3 readColFloat3();
	  std::string readString();
	  float readWordPercent();
	  float readFloatPercent();
	  void readChunk();
	  void skipChunk();
	  void read0x3D3D();
	  vec3 readColChunk();
	  float readPercentChunk();
	  void read0xAFFF();
	  texture_info readMapChunk();
	  void read0x4000();
	  void read0x4100();

  public:	
	  std::vector<object_info> objects;
	  std::vector<material_info> materials;
	  bool verbose;
  	
  protected:
	  FILE *input_file;
	  unsigned short chunk_id;
	  unsigned int chunk_len;	
  };
}

#endif
