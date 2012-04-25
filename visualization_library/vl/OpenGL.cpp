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

#include "vl/OpenGL.hpp"
#include <string.h>

namespace vl
{
  void print_system_info(FILE* file)
  {
    if (VERBOSITY_LEVEL >= 1)
    {
      if (GLEW_VERSION_2_1)
        fprintf(file,"OpenGL 2.1 supported\n");
      else
      if (GLEW_VERSION_2_0)
        fprintf(file,"OpenGL 2.0 supported\n");
      else
      if (GLEW_VERSION_1_5)
        fprintf(file,"OpenGL 1.5 supported\n");
      else
      if (GLEW_VERSION_1_4)
        fprintf(file,"OpenGL 1.4 supported\n");
      else
      if (GLEW_VERSION_1_3)
        fprintf(file,"OpenGL 1.3 supported\n");
      else
      if (GLEW_VERSION_1_2)
        fprintf(file,"OpenGL 1.2 supported\n");
      else
      if (GLEW_VERSION_1_1)
        fprintf(file,"OpenGL 1.1 supported\n");
      else
        fprintf(file,"Unknown OpenGL version supported\n");
      fprintf(file,"GL_VENDOR = %s\n", glGetString(GL_VENDOR) );
      fprintf(file,"GL_RENDERER = %s\n", glGetString(GL_RENDERER) );
      fprintf(file,"GL_VERSION = %s\n", glGetString(GL_VERSION) );
      if (GLEW_VERSION_2_0)
        fprintf(file,"GL_SHADING_LANGUAGE_VERSION = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
      fprintf(file,"GLEW_VERSION = %s\n", glewGetString(GLEW_VERSION) );
      GLint tex_max = 0;
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tex_max);
      fprintf(file,"Max texture size: %d\n", tex_max);
      tex_max = 1;
      if (GLEW_ARB_multitexture)
        glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tex_max);
      fprintf(file,"%s: Multitexturing, %d unit%s\n", GLEW_ARB_multitexture ? "OK" : "NO", tex_max, tex_max>1?"s":"");
      tex_max = 0;
      if (GLEW_ARB_multitexture)
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &tex_max);
      fprintf(file,"%s: Anisotropic Texture Filter ", GLEW_EXT_texture_filter_anisotropic? "OK" : "NO");
      GLEW_EXT_texture_filter_anisotropic ? fprintf(file,"%dX\n", tex_max) : fprintf(file,"\n");
      fprintf(file,"%s: S3 Texture Compression\n", GLEW_EXT_texture_compression_s3tc? "OK" : "NO" );
      fprintf(file,"%s: Vertex Buffer Object\n", GLEW_ARB_vertex_buffer_object ? "OK" : "NO");
      fprintf(file,"%s: Pixel Buffer Object\n", GLEW_ARB_pixel_buffer_object ? "OK" : "NO");
      fprintf(file,"%s: Frame Buffer Object\n", GLEW_EXT_framebuffer_object ? "OK" : "NO");
      fprintf(file,"%s: GLSL 1.0\n", GLEW_ARB_shading_language_100 ? "OK" : "NO");
      if (VERBOSITY_LEVEL >= 2)
      {
        fprintf(file,"\nExtensions:\n");
        const char* ext_str = (const char*)glGetString(GL_EXTENSIONS);
        if (ext_str)
        {
          size_t max_offset = strlen( ext_str );
          for(size_t offset=0; offset<max_offset; )
          {
            char buffer[128];
            memset(buffer,0,128);
            sscanf(ext_str+offset,"%s",buffer);
            fprintf(file,"%s\n", buffer );
            offset += strlen(buffer)+1;
          }
        }
      }
      fprintf(file,"\n");
    }
  }

  void init_glew()
  {

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      fprintf(stderr, "Error calling glewInit(): %s\n", glewGetErrorString(err));
      CHECK(0)
    }

    print_system_info(stdout);

    glGetError();
  }

}
