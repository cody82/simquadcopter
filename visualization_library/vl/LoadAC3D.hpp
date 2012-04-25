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

#ifndef LoadAC3D_INCLUDE_DEFINE
#define LoadAC3D_INCLUDE_DEFINE

#include "vl/vec3.hpp"
#include "vl/mat4d.hpp"
#include "vl/ShaderNode.hpp"
#include <string>
#include <vector>

namespace vl
{
  class LoadAC3D 
  {
  public:

	  class vert_info 
    {
	  public:
		  int Vert;
		  float U;
		  float V;
	  };
  	
	  class material_info 
    {
	  public:
		  std::string Name;
		  vec3 Diffuse;
		  vec3 Ambient;
		  vec3 Emission;
		  vec3 Specular;
		  float Shininess;
		  float Trans;
	  };

	  class surface_info 
    {
	  public:
		  int Flags;
		  int Material;
		  int VertCount;
		  std::vector<vert_info> Vertex;
	  };

	  class mesh_info 
    {
	  public:
		  mesh_info();
  	
		  std::string Name;
		  std::string Data;
		  std::string Texture;
		  float TexRep[2];
		  mat4d Matrix;
		  std::string Url;
		  int NumVert;
		  std::vector<vec3> Vert;
		  int NumSurf;
		  std::vector<surface_info> Surface;		
		  int NumKids;
	  };
  	
	  LoadAC3D();	
	  bool parseAC3D(const std::string& file);	

	  ref<ShaderNode> loadAC3D(const std::string& file, const std::string& texture_directory = "./");

	  vec3 readColor();	
	  vec3 readVector();
	  float readFloat();
	  int readInt();
	  int readHex();
	  std::string readString();
	  void readMaterial();
	  void readObject();
  	
	  std::vector<material_info> Material;
	  std::vector<mesh_info> Mesh;
    bool verbose;
  	
  protected:
	  FILE * fin;
  };
}

#endif
