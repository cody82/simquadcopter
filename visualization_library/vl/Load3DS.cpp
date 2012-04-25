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

#include "vl/Load3DS.hpp"
#include "vl/Painter.hpp"
#include "vl/Geometry.hpp"
#include "vl/Actor.hpp"
#include "vl/Texture.hpp"
#include "vl/Image.hpp"
#include "vl/Camera.hpp"
#include <map>

using namespace vl;

void Load3DS::material_info::print() 
{
	Log::print("\n");
	Log::print(Say("material: %s\n") << name.c_str());
	Log::print(Say("ambient color: %n %n %n\n")<<ambient.r()<<ambient.g()<<ambient.b());
	Log::print(Say("diffuse color: %n %n %n\n")<<diffuse.r()<<diffuse.g()<<diffuse.b());
	Log::print(Say("specular color: %n %n %n\n")<<specular.r()<<specular.g()<<specular.b());
	Log::print(Say("shininess: %n (%n)\n")<<shininess<<shininess_strength);
	Log::print(Say("transparency: %n\n")<<transparency);
	Log::print(Say("double sided: %s\n")<< (double_sided ? "yes" : "no") );
	Log::print("\n");
	if ( !texture1.filename.empty() ) {
		Log::print(Say("texture1: %s\n")<<texture1.filename.c_str());
		Log::print(Say("\tuscale: %n\n")<<texture1.uscale);
		Log::print(Say("\tvscale: %n\n")<<texture1.vscale);
		Log::print(Say("\tuscale: %n\n")<<texture1.uoffset);
		Log::print(Say("\tvscale: %n\n")<<texture1.voffset);
		Log::print(Say("\trotation: %n\n")<<texture1.rotation);
	}
	if ( !texture2.filename.empty() ) {
		Log::print(Say("texture2: %s\n")<<texture2.filename.c_str());
		Log::print(Say("\tuscale: %n\n")<<texture2.uscale);
		Log::print(Say("\tvscale: %n\n")<<texture2.vscale);
		Log::print(Say("\tuscale: %n\n")<<texture2.uoffset);
		Log::print(Say("\tvscale: %n\n")<<texture2.voffset);
		Log::print(Say("\trotation: %n\n")<<texture2.rotation);
	}
	Log::print("\n");
}

Load3DS::coord_system_info::coord_system_info() 
{
	xaxis.x() = 1;
	yaxis.y() = 1;
	zaxis.z() = 1;
}

void Load3DS::object_info::print() 
{
	Log::print("\n");
	Log::print( Say("object: %s\n")<< name.c_str());
	Log::print("\n");

	Log::print( Say("vertices: %n\n") << (int)vert_list.size());
	for(unsigned int i=0; i<vert_list.size(); i++) {
		Log::print( Say("%n %n %n\n")<< vert_list[i].x()<<vert_list[i].y()<<vert_list[i].z());
	}
	Log::print("\n");

	Log::print( Say("faces: %n\n") << (int)face_list.size());
	for(unsigned int i=0; i<face_list.size(); i++) {
		Log::print( Say("%n %n %n, %x\n")<< face_list[i].a<<face_list[i].b<<face_list[i].c<<face_list[i].flags);
	}
	Log::print("\n");

	Log::print( Say("uv coords: %n\n")<<(int)uv_coords.size());
	for(unsigned int i=0; i<uv_coords.size(); i++) {
		Log::print(Say("%n %n\n")<< uv_coords[i].s()<<uv_coords[i].t());
	}
	Log::print("\n");

	Log::print("Local coordinate system:\n");
	Log::print( Say("x: %n %n %n\n")<< coord_system.xaxis.x()<<coord_system.xaxis.y()<<coord_system.xaxis.z());
	Log::print( Say("y: %n %n %n\n")<< coord_system.yaxis.x()<<coord_system.yaxis.y()<<coord_system.yaxis.z());
	Log::print( Say("z: %n %n %n\n")<< coord_system.zaxis.x()<<coord_system.zaxis.y()<<coord_system.zaxis.z());
	Log::print( Say("origin: %n %n %n\n")<< coord_system.origin.x()<<coord_system.origin.y()<<coord_system.origin.z());
	Log::print("\n");

	Log::print( Say("material face mapping: %n\n")<<(int)mat_face_map.size());
	Log::print("\n");
}

Load3DS::Load3DS()
{
	verbose = true;
	input_file = NULL;
}

unsigned char Load3DS::readByte() 
{
	unsigned char b;
	fread(&b, sizeof(unsigned char), 1, input_file);
	return b;
}

unsigned short Load3DS::readWord() 
{
	unsigned short w;
	fread(&w, sizeof(unsigned short), 1, input_file);
	return w;
}

unsigned int Load3DS::readDWord() 
{
	unsigned int dw;
	fread(&dw, sizeof(unsigned int), 1, input_file);
	return dw;
}

float Load3DS::readFloat() 
{
	float f;
	fread(&f, sizeof(float), 1, input_file);
	return f;
}

vec3 Load3DS::readVec3() 
{
	vec3 v;
	v.x() = readFloat();
	v.y() = readFloat();
	v.z() = readFloat();

	return v;
}

vec3 Load3DS::readColByte3() 
{
	vec3 c;
	c.r() = readByte() / 255.0f;
	c.g() = readByte() / 255.0f;;
	c.b() = readByte() / 255.0f;;
	return c;
}

vec3 Load3DS::readColFloat3() 
{
	vec3 c;
	c.r() = readFloat();
	c.g() = readFloat();
	c.b() = readFloat();
	return c;
}

std::string Load3DS::readString() 
{
	std::vector<unsigned char> str;
	unsigned char b;
	do {
		b= readByte();
		str.push_back(b);
	} while(b);
	
	return (char*)&str[0];
}

float Load3DS::readWordPercent() 
{
	return readWord() / 100.0f;
}

float Load3DS::readFloatPercent() 
{
	return readFloat() / 100.0f;
}

void Load3DS::readChunk() 
{
	if (verbose) Log::print(Say("%08d: ")<<(unsigned int)ftell(input_file));
	chunk_id  = readWord();
	if (verbose) Log::print(Say("%04X")<<(unsigned int)chunk_id);
	chunk_len = readDWord();
	if (verbose) Log::print(Say(")<<length: %u\n")<<chunk_len);
}

void Load3DS::skipChunk() 
{
	if (verbose) Log::print(Say("Skipping Chunk %04X ==> %n\n")<<(unsigned int)chunk_id<<chunk_len - 6);
	fseek(input_file, chunk_len - 6, SEEK_CUR);
}

void Load3DS::read0x3D3D() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	unsigned int id = chunk_id;		
	if (verbose) 
    Log::print(Say("Entering %04X\n")<<id);
	
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		
		switch(chunk_id) 
    {
		case 0x4000: 
			read0x4000();
		break;
		
		case 0xAFFF: 
			read0xAFFF();
		break;
		
		default:
			skipChunk();
		}
	}
	if (verbose) Log::print(Say("<== Exiting %04X\n")<<id);
}

vec3 Load3DS::readColChunk() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	vec3 color;
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		switch(chunk_id) 
    {
		case 0x0010:
			color = readColFloat3();
		break;
		case 0x0011:
			color = readColByte3();
		break;

		case 0x0012:
			readColByte3();
		break;

		case 0x0013:
			readColFloat3();
		break;
		}
	}
	return color;
}

float Load3DS::readPercentChunk() 
{
	readChunk();
	float perc=0;
	switch(chunk_id) 
  {
	case 0x0030:
		perc = readWordPercent();
	break;
	case 0x0031:
		perc = readFloatPercent();
	break;
	}
	return perc;
}

void Load3DS::read0xAFFF() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	unsigned int id = chunk_id;		
	if (verbose) 
    Log::print(Say("Entering %04X\n")<<id);
	
	material_info mat;
	
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		
		switch(chunk_id) 
    {

		case 0xA000: 
		{
			mat.name = readString();
			if (verbose) Log::print(Say("material_info name: '%s'\n")<<mat.name.c_str());
		}
		break;

		case 0xA010: 
		{
			mat.ambient = readColChunk();
		}
		break;

		case 0xA020: 
		{
			mat.diffuse = readColChunk();
		}
		break;

		case 0xA030: 
		{
			mat.specular = readColChunk();
		}
		break;

		case 0xA040: 
		{
			mat.shininess = readPercentChunk();
		}
		break;

		case 0xA041: 
		{
			mat.shininess_strength = readPercentChunk();
		}
		break;

		case 0xA050: 
		{
			mat.transparency = readPercentChunk();
		}
		break;

		case 0xA081: 
		{
			mat.double_sided = true;
		}

		case 0xA200:
		{
			mat.texture1 = readMapChunk();
		}
		break;

		case 0xA33A:
		{
			mat.texture2 = readMapChunk();
		}
		break;
		
		default:
			skipChunk();
		}
	}
	if (verbose) 
    Log::print(Say("<== Exiting %04X\n")<<id);
	materials.push_back(mat);
}

Load3DS::texture_info Load3DS::readMapChunk() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	unsigned int id = chunk_id;		
	
	texture_info tex;
	
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		
		switch(chunk_id) 
    {

		case 0xA300:
		{
			tex.filename = readString();
			if (verbose) Log::print(Say("texture_info file: '%s'\n")<<tex.filename.c_str());
		}
		break;

		case 0xA354:
		{
			tex.vscale = readFloat();
		}
		break;

		case 0xA356:
		{
			tex.uscale = readFloat();
		}
		break;

		case 0xA358:
		{
			tex.uoffset = readFloat();
		}
		break;

		case 0xA35A:
		{
			tex.voffset = readFloat();
		}
		break;

		case 0xA35C:
		{
			tex.rotation = readFloat(); 
		}
		break;
		default:
			skipChunk();
		}
	}
	if (verbose) 
    Log::print(Say("<== Exiting %04X\n")<<id);
	return tex;
}

void Load3DS::read0x4000() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	unsigned int id = chunk_id;		
	if (verbose) 
    Log::print(Say("Entering %04X\n")<<id);
	
	objects.push_back( object_info() );
	objects.back().name = readString();
	
	if (verbose) 
    Log::print(Say("object_info name: '%s'\n")<<objects.back().name.c_str());
	
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		
		switch(chunk_id) 
    {
		case 0x4100:
			read0x4100();
		break;
		default:

			objects.pop_back();
			skipChunk();
		}
	}
	if (verbose) 
    Log::print(Say("<== Exiting %04X\n")<<id);
}

void Load3DS::read0x4100() 
{
	int chunk_end = ftell(input_file) + chunk_len - 6;
	unsigned int id = chunk_id;		
	if (verbose) Log::print(Say("Entering %04X === TRIANGULAR MESH ===\n")<<id);
	
	while(ftell(input_file) < chunk_end) 
  {
		readChunk();
		
		switch(chunk_id) 
    {

		case 0x4110:
		{
			if (verbose) Log::print("Vert list\n");
			unsigned short vertc = readWord();
			objects.back().vert_list.reserve(vertc);
			for(unsigned short i=0; i<vertc; i++) {
				vec3 v = readVec3();
				objects.back().vert_list.push_back(v);
			}
		}
		break;

		case 0x4120:
		{
			if (verbose) Log::print("Face desc\n");
			unsigned short facec= readWord();
			objects.back().face_list.reserve(facec);
			for(unsigned short i=0; i<facec; i++) 
      {
				tri_face_info face;
				face.a = readWord();
				face.b = readWord();
				face.c = readWord();
				face.flags = readWord();
				objects.back().face_list.push_back(face);
			}
		}
		break;

		case 0x4130:
		{
			objects.back().mat_face_map.push_back(material_face_mapping_info());
			objects.back().mat_face_map.back().material_name = readString();
			unsigned short facec = readWord();
			objects.back().mat_face_map.back().mapped_face.reserve(facec);
			for(unsigned short i=0; i<facec; i++) 
      {
				unsigned short face = readWord();
				objects.back().mat_face_map.back().mapped_face.push_back(face);
			}
		}
		break;

		case 0x4140:
		{
			if (verbose) Log::print("Mapping coords\n");
			unsigned short vertc = readWord();
			objects.back().uv_coords.reserve(vertc);
			for(unsigned short i=0; i<vertc; i++) 
      {
				vec2 coord;
				coord.s() = readFloat();
				coord.t() = readFloat(); // 1 - readFloat(); ?
				objects.back().uv_coords.push_back(coord);						
			}
		}
		break;

		case 0x4160:
		{
			if (verbose) 
        Log::print("Local coords system\n");
			objects.back().coord_system.xaxis  = readVec3();
			objects.back().coord_system.yaxis  = readVec3();
			objects.back().coord_system.zaxis  = readVec3();
			objects.back().coord_system.origin = readVec3();
		}
		break;

		default:
			skipChunk();
		}
	}
	if (verbose) Log::print(Say("<== Exiting %04X\n")<<id);
}

bool Load3DS::parse3DS(const std::string& name) 
{	
	input_file = fopen(name.c_str(), "rb");
	if (!input_file) 
  {
		Log::print(Say("Couldn't open the file %s\n")<<name.c_str());
		return false;
	}
		
	readChunk();				
	if (chunk_id == 0x4D4D) 
  {
		if (verbose) 
      Log::print("Opened 3DS File.\n");
	}
	else 
  {
		Log::print("Not a 3DS file.");
		fclose(input_file);
		input_file = NULL;
		return false;
	}
	
	int EndOfFile = chunk_len;
		
	do 
  {

		readChunk();		
		switch(chunk_id) 
    {
		case 0x0002: // 3DS Version
		{
			unsigned int version = readDWord();
			if (verbose) Log::print(Say("3DS Version: %u\n")<<version);
		}
		break;
		
		case 0x3D3D: // 3D Editor
		{
			read0x3D3D();
		}
		break;			
		
		default:
			skipChunk();
		}
					
	} 
  while( ftell(input_file) != EndOfFile );			
	
	fclose(input_file);
	input_file = NULL;
	return true;
}

ref<ShaderNode> Load3DS::load3DS(const std::string& file, const std::string& texture_directory)
{
  verbose = false;
  bool force_double_face = false;
  if (parse3DS(file) == false)
    return NULL;

  ref<ShaderNode> root = new ShaderNode;

  if (verbose) 
  {
    Log::print("3DS file info:\n");
    for (int k=0; k<(int)materials.size(); k++)
	    materials[k].print();
  	
    for (int k=0; k<(int)objects.size(); k++)
	    objects[k].print();
  }

  ref<Painter> default_painter = new Painter;
  root->addChild(default_painter.get());
  default_painter->setName("default painter");
  default_painter->shader()->enable(EN_CULL_FACE);

  std::map< std::string, ref<Painter> > mat_map;

  for(unsigned int i=0; i<objects.size(); i++) 
  {
    if (objects[i].vert_list.size() == 0)
	    continue;
  	
    ref<Geometry> geom = new Geometry;
    ref<Actor> act = new Actor(geom.get());
    act->setName( objects[i].name );

    ref<GPUArrayVec3> verts = new GPUArrayVec3;
    geom->setVertexArray(verts.get());
    verts->resize( (int)objects[i].vert_list.size() );
    for(unsigned int ivert=0; ivert<objects[i].vert_list.size(); ivert++) 
    {
      verts->object(ivert).x() = objects[i].vert_list[ivert].x();
      verts->object(ivert).y() = objects[i].vert_list[ivert].z();
      verts->object(ivert).z() =-objects[i].vert_list[ivert].y();
    }
  	
    ref<DrawElementsUInt> polys = new DrawElementsUInt;
    geom->addDrawCall( polys.get() );
    for(unsigned int iface=0; iface<objects[i].face_list.size(); iface++) 
    {
	    polys->addIndex( objects[i].face_list[iface].a );
	    polys->addIndex( objects[i].face_list[iface].b );
	    polys->addIndex( objects[i].face_list[iface].c );
    }
  	
    ref<GPUArrayFloat2> uvs = new GPUArrayFloat2;
    geom->setTexCoordArray(0, uvs.get());
    for(unsigned int icoord=0; icoord<objects[i].uv_coords.size(); icoord++) 
    {
	    uvs->push_back( objects[i].uv_coords[icoord].s() );
	    uvs->push_back( objects[i].uv_coords[icoord].t() );
    }

    act->setTransform( new Transform() );

    mat4d matrix;
    matrix.setX( (vec3d)objects[i].coord_system.xaxis  );
    matrix.setY( (vec3d)objects[i].coord_system.yaxis  );
    matrix.setZ( (vec3d)objects[i].coord_system.zaxis  );
    matrix.setT( (vec3d)objects[i].coord_system.origin );

    act->transform()->setLocalMatrix(matrix);
    act->transform()->computeLocalToWorldMatrix(NULL);

    bool mat_ok = false;
    if ( objects[i].mat_face_map.size() ) 
    {

	    std::string mat_name = objects[i].mat_face_map[0].material_name;
	    for(int m=0; m<(int)materials.size(); m++) 
      {
		    if (materials[m].name == mat_name) 
        {
          mat_ok = true;

          if (mat_map[mat_name] == NULL)
          {
            mat_map[mat_name] = new Painter;
            root->addChild( mat_map[mat_name].get() );
            mat_map[mat_name]->setName(mat_name);
            mat_map[mat_name]->shader()->enable(EN_BLEND);
            mat_map[mat_name]->shader()->glBlendFunc()->set(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);
            mat_map[mat_name]->shader()->glMaterial()->setAmbient(  vec4( materials[m].ambient.r(), materials[m].ambient.g(), materials[m].ambient.b(), 1 ) );
            mat_map[mat_name]->shader()->glMaterial()->setDiffuse(  vec4( materials[m].diffuse.r(), materials[m].diffuse.g(), materials[m].diffuse.b(), 1 ) );
            mat_map[mat_name]->shader()->glMaterial()->setSpecular( vec4( materials[m].specular.r(), materials[m].specular.g(), materials[m].specular.b(), 1 ) );
            mat_map[mat_name]->shader()->glMaterial()->setShininess( materials[m].shininess );
            mat_map[mat_name]->shader()->glMaterial()->setTransparency( 1.0f - materials[m].transparency );
            mat_map[mat_name]->shader()->enable(EN_CULL_FACE);
            if ( materials[m].double_sided || force_double_face )
            {
              mat_map[mat_name]->shader()->glLightModel()->setTwoSide(true);
              mat_map[mat_name]->shader()->disable(EN_CULL_FACE);
            }
			      if ( !materials[m].texture1.filename.empty() )
            {
              mat_map[mat_name]->shader()->textureUnit(0)->setTexture( new Texture( new Image( texture_directory + materials[m].texture1.filename ) ) );
			      }
          }

          mat_map[mat_name]->addActor(act.get());

			    break;
		    }
	    }
    }

    if (!mat_ok)
      default_painter->addActor( act.get() );

    geom->computeSmoothNormals();
  }

  if (default_painter->actorCount() == 0)
    root->removeChild(default_painter.get());

  return root;
}
