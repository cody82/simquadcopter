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

#include "vl/GLSL.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"

using namespace vl;

GLSLShader::GLSLShader(EShaderType type, const std::string& source, const std::string& name)
{
  mType = type;
  mHandle = 0;
  mCompiled = false;
  if (source.length()) 
  {
    if (name.length())
    {
      setSource(source, name);
    }
    else
      setSource(source);
  }
}

GLSLShader::~GLSLShader()
{
  if (::vl::can_use_opengl)
    deleteShader();
}

std::string GLSLShader::loadFromFile( const std::string& path )
{
  mCompiled = false;

  mSource.clear();
  FILE* fin = fopen(path.c_str(), "rb");
  if (fin == NULL)
  {
    Log::error( Say("\nGLSLShader::loadFromFile(\"%s\"): file not found.\n") << path.c_str());
    CHECK(0);
    return false;
  }
  fseek(fin,0,SEEK_END);
  int bytes = (int)ftell(fin);
  fseek(fin,0,SEEK_SET);
  std::vector<char> buffer;
  buffer.resize(bytes);
  fread(&buffer[0], 1, bytes, fin);
  buffer.push_back(0);
  std::string src = &buffer[0];
  fclose(fin);
  return src;
}

bool GLSLShader::setSource( const std::string& multi_source, const std::string& name )
{
  mCompiled = false;
  mSource.clear();

  std::string src = multi_source;
  if ( strstr(multi_source.c_str(), "file://") == multi_source.c_str() )
  {
    src = loadFromFile(multi_source.c_str() + 7);
  }
  else
  {
    setName("<NONAME>");
    mSource = multi_source;
    return true;
  }
  if ( name.empty() )
  {
    setName(multi_source);
    mSource = src;
    return true;
  }
  else
    setName(multi_source + ": " + name);

  std::vector<char> buffer;
  bool found = false;
  std::string full_name = '<' + name + '>';

  int index = 0;
  while( index < (int)src.size() )
  {
    buffer.clear();
    for( ; index < (int)src.size(); index++ )
    {
      if ( src[index] == 13 || src[index] == 10 )
      {
        if ( src[index] == 13 && index+1 < (int)src.size() && src[index+1] == 10 )
          index++;
        buffer.push_back( '\n' );
        buffer.push_back( '\0' );
        index++;
        break;
      }
      else
        buffer.push_back( src[index] );
    }
    buffer.push_back( src[index] );

    if (strstr(&buffer[0],"// #SHADER") == &buffer[0] && strstr(&buffer[0],full_name.c_str()) )
      found = true;

    if (strstr(&buffer[0],"// #SHADER") == &buffer[0] && strstr(&buffer[0],full_name.c_str()) == NULL )
      found = false;

    if ( found )
      mSource += &buffer[0];
  }

  if (mSource.empty())
    Log::error( Say("couldn't load the shader: %s\n") << this->name() );

  CHECK(mSource.empty() == false)

  return !mSource.empty();
}

bool GLSLShader::compile()
{
  if (GLEW_ARB_shading_language_100 == false)
    return false;

  if (mCompiled == false)
  {

    if (mHandle == 0)
    {
      mHandle = glCreateShader(mType);
      GLCHECK4()
    }
    CHECK(mHandle);

    const GLchar* source[] = { mSource.c_str() };
    glShaderSource(mHandle, 1, source, NULL);

    glCompileShader(mHandle);

    if ( compileStatus() )
    {
      mCompiled = true;
      #ifndef NDEBUG
        Log::info( Say("%s\n%s\n\n") << name() << infoLog() );
      #endif
    }
    else
    {
      Log::error( Say("\nShader compilation error: '%s':\n\n") << name() );
      Log::print( Say("%s\n\n") << mSource.c_str() );
      Log::print( Say("%s\n\n") << infoLog().c_str() );
    }
  }

  return mCompiled;
}

bool GLSLShader::compileStatus() const
{
  if (GLEW_ARB_shading_language_100 == false)
    return false;

  CHECK(mHandle)
  if (!mHandle)
    return false;

  GLint status = 0;
  glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);
  return status == GL_TRUE;
}

std::string GLSLShader::infoLog() const
{
  if (GLEW_ARB_shading_language_100 == false)
    return "";

  CHECK(mHandle)
  if (!mHandle)
    return "";

  GLint max_length = 0;
  glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &max_length);
  std::vector<GLchar> log_buffer;
  log_buffer.resize(max_length+1);
  glGetShaderInfoLog(mHandle, max_length, NULL, &log_buffer[0]);
  std::string log_string(&log_buffer[0]);
  return log_string;
}

void GLSLShader::deleteShader()
{
  if (GLEW_ARB_shading_language_100 == false)
    return;

  if (mHandle)
  {
    glDeleteShader(mHandle);
    mHandle = 0;
    mCompiled = false;
    GLCHECK4()
  }
}

GLSLProgram::GLSLProgram()
{
  mSafeSetUniform = true;
  mNeedsLink = true;
  mHandle = 0;
}

GLSLProgram::~GLSLProgram()
{
  if (handle() && ::vl::can_use_opengl)
    deleteProgram();
}

void GLSLProgram::createProgram()
{
  if (GLEW_ARB_shading_language_100 == false)
    return;

  if (mHandle == 0)
  {
    mHandle = glCreateProgram(); GLCHECK4();
    CHECK(mHandle)
    GLCHECK4()
  }
}

void GLSLProgram::deleteProgram()
{
  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(mHandle)
  if(mHandle)
  {
    glDeleteProgram(mHandle); GLCHECK4();
    mHandle = 0;
    mNeedsLink = true;
  }
}

bool GLSLProgram::attachShader(GLSLShader* shader) 
{ 
  mUniformLocationMap.clear();
  mAttribLocationMap.clear();

  if (GLEW_ARB_shading_language_100 == false)
    return false;

  if(std::find(mShaders.begin(), mShaders.end(), shader) != mShaders.end())
  {
    if ( shader->handle() )
      glDetachShader( handle(), shader->handle() ); GLCHECK4();
  }
  else
    mShaders.push_back(shader);

  if ( shader->compile() )
  {
    createProgram();

    glAttachShader( handle(), shader->handle() ); GLCHECK4();

    mNeedsLink = true; 
    return true;
  }
  else
  {
    CHECK(0);
    return false;
  }
}

bool GLSLProgram::detachShader(GLSLShader* shader) 
{
  mUniformLocationMap.clear();
  mAttribLocationMap.clear();

  if (GLEW_ARB_shading_language_100 == false)
    return false;

  CHECK( shader->handle() )
  if ( shader->compile() )
  {
    for(int i=0; i<(int)mShaders.size(); ++i)
    {
      if (mShaders[i] == shader)
      {
        mShaders.erase(mShaders.begin() + i);
        createProgram();
        glDetachShader( handle(), shader->handle() ); GLCHECK4();
        mNeedsLink = true; 
        break;
      }
    }
  }
  else 
    return false;

  return true;
}

bool GLSLProgram::linkProgram(bool force_relink)
{
  mUniformLocationMap.clear();
  mAttribLocationMap.clear();

  if (GLEW_ARB_shading_language_100 == false)
    return false;

  if (mNeedsLink || force_relink)
  {
    CHECK(mHandle) // no shaders attached
    glLinkProgram(mHandle); GLCHECK4();
    if ( linkStatus() )
    {
      mNeedsLink = false;
    }
  }

  if(mNeedsLink)
  {
    Log::error("\nGLSLProgram::linkProgram() error:\n\n");
    Log::error( Say("%s\n\n") << infoLog().c_str());
    CHECK( 0 )
    return false;
  }
  else
    return true;
}

bool GLSLProgram::linkStatus()
{
  if (GLEW_ARB_shading_language_100 == false)
    return false;

  CHECK(mHandle) // no shaders attached
  if (mHandle == 0)
    return false;
  GLint status = 0;
  glGetProgramiv(mHandle, GL_LINK_STATUS, &status); GLCHECK4();
  return status == GL_TRUE;
}

std::string GLSLProgram::infoLog() const
{
  if (GLEW_ARB_shading_language_100 == false)
    return "";

  CHECK(mHandle) // no shaders attached
  if (mHandle == 0)
    return "GLSLProgram::infoLog(): no shaders bound!";
  GLint max_length = 0;
  glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &max_length); GLCHECK4();
  std::vector<GLchar> log_buffer;
  log_buffer.resize(max_length+1);
  glGetProgramInfoLog(mHandle, max_length, NULL, &log_buffer[0]); GLCHECK4();
  std::string info_log(&log_buffer[0]);
  return info_log;
}

bool GLSLProgram::validateProgram()
{
  if (GLEW_ARB_shading_language_100 == false)
    return false;

  CHECK(mHandle) // no shaders attached
  if (mHandle == 0)
    return GL_FALSE;

  glValidateProgram(mHandle);
  GLint status = 0;
  glGetProgramiv(mHandle, GL_VALIDATE_STATUS, &status); GLCHECK4();
  return status == GL_TRUE;
}

void GLSLProgram::bindAttribLocation(GLuint index, const std::string& name)
{
  if (GLEW_ARB_shading_language_100 == false)
    return;

  createProgram();
  glBindAttribLocation(mHandle, index, name.c_str());
  GLCHECK4()
  mNeedsLink = true;
}

GLint GLSLProgram::getAttribLocation(const std::string& name)
{
  GLCHECK4()
  if (GLEW_ARB_shading_language_100 == false)
    return (GLuint)-1;

  CHECK(mHandle)
  if (!mHandle)
    return (GLuint)-1;
  if ( mAttribLocationMap.find( name ) != mAttribLocationMap.end() )
    return mAttribLocationMap[name];
  else
  {
    GLint location = glGetAttribLocation(mHandle, name.c_str());
    GLCHECK4()
    if (location != -1)
      mAttribLocationMap[name] = location;
    return location;
  }
}

GLint GLSLProgram::maxVertexAttribs()
{
  if (GLEW_ARB_shading_language_100 == false)
    return 0;

  GLint max = 0;
  glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max );
  return max;
}

GLint GLSLProgram::getUniformLocation(const std::string& name)
{
  if (GLEW_ARB_shading_language_100 == false)
    return (GLuint)-1;

  if (mNeedsLink)
    return (GLint)-1;
  CHECK(mHandle)
  if (mUniformLocationMap.find(name) != mUniformLocationMap.end())
    return mUniformLocationMap[name];
  else
  {
    GLint location = glGetUniformLocation(mHandle, name.c_str());
    if (location != -1)
      mUniformLocationMap[name] = location;
    return location;
  }
}

void GLSLProgram::getUniformfv(GLint location, GLfloat* params)
{
  GLCHECK4()
  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(mHandle)
  glGetUniformfv(mHandle, location, params);
  GLCHECK4()
}

void GLSLProgram::getUniformfv(const std::string& name, GLfloat* params)
{
  getUniformfv(getUniformLocation(name), params);
}

void GLSLProgram::getUniformiv(GLint location, GLint* params)
{
  GLCHECK4()
  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(mHandle)
  glGetUniformiv(mHandle, location, params);
  GLCHECK4()
}

void GLSLProgram::getUniformiv(const std::string& name, GLint* params)
{
  getUniformiv(getUniformLocation(name), params);
}

void GLSLProgram::setUniform1f(GLint location, int count, const GLfloat* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform1fv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform1f(const std::string& name, int count, GLfloat* v0)
{
  setUniform1f(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform2f(GLint location, int count, const GLfloat* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform2fv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform2f(const std::string& name, int count, GLfloat* v0)
{
  setUniform2f(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform3f(GLint location, int count, const GLfloat* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform3fv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform3f(const std::string& name, int count, GLfloat* v0)
{
  setUniform3f(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform4f(GLint location, int count, const GLfloat* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform4fv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform4f(const std::string& name, int count, GLfloat* v0)
{
  setUniform4f(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform(GLint location, int count, const vec2* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform2fv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const vec2* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniform(GLint location, int count, const vec3* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform3fv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const vec3* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniform(GLint location, int count, const vec4* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform4fv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const vec4* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniform1i(GLint location, int count, const GLint* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform1iv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform1i(const std::string& name, int count, GLint* v0)
{
  setUniform1i(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform2i(GLint location, int count, const GLint* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform2iv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform2i(const std::string& name, int count, GLint* v0)
{
  setUniform2i(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform3i(GLint location, int count, const GLint* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform3iv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform3i(const std::string& name, int count, GLint* v0)
{
  setUniform3i(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform4i(GLint location, int count, const GLint* v0)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform4iv(location, count, v0);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform4i(const std::string& name, int count, GLint* v0)
{
  setUniform4i(getUniformLocation(name), count, v0);
}

void GLSLProgram::setUniform(GLint location, int count, const ivec2* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform2iv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const ivec2* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniform(GLint location, int count, const ivec3* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform3iv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const ivec3* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniform(GLint location, int count, const ivec4* vec)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniform4iv(location, count, vec->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniform(const std::string& name, int count, const ivec4* vec)
{
  setUniform(getUniformLocation(name), count, vec);
}

void GLSLProgram::setUniformMatrix4f(GLint location, int count, bool transpose, const GLfloat* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix4fv(location, count, transpose, mat);

  if (mSafeSetUniform)
    glUseProgram( cur_program );

}

void GLSLProgram::setUniformMatrix4f(const std::string& name, int count, bool transpose, const GLfloat* mat)
{
  setUniformMatrix4f(getUniformLocation(name), count, transpose, mat);
}

void GLSLProgram::setUniformMatrix3f(GLint location, int count, bool transpose, const GLfloat* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix3fv(location, count, transpose, mat);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniformMatrix3f(const std::string& name, int count, bool transpose, const GLfloat* mat)
{
  setUniformMatrix3f(getUniformLocation(name), count, transpose, mat);
}

void GLSLProgram::setUniformMatrix2f(GLint location, int count, bool transpose, const GLfloat* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix2fv(location, count, transpose, mat);

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniformMatrix2f(const std::string& name, int count, bool transpose, const GLfloat* mat)
{
  setUniformMatrix2f(getUniformLocation(name), count, transpose, mat);
}
void GLSLProgram::setUniformMatrix(GLint location, int count, bool transpose, const mat4* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix4fv(location, count, transpose, mat->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniformMatrix(const std::string& name, int count, bool transpose, const mat4* mat)
{
  setUniformMatrix(getUniformLocation(name), count, transpose, mat);
}

void GLSLProgram::setUniformMatrix(GLint location, int count, bool transpose, const mat3* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix3fv(location, count, transpose, mat->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );

}

void GLSLProgram::setUniformMatrix(const std::string& name, int count, bool transpose, const mat3* mat)
{
  setUniformMatrix(getUniformLocation(name), count, transpose, mat);
}

void GLSLProgram::setUniformMatrix(GLint location, int count, bool transpose, const mat2* mat)
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return;

  CHECK(location!=-1)

  CHECK(!mNeedsLink)

  if (mNeedsLink)
    return;
  CHECK(mHandle);

  GLint cur_program = 0;
  if (mSafeSetUniform)
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_program);

  glUseProgram(mHandle); GLCHECK4();

  glUniformMatrix2fv(location, count, transpose, mat->ptr());

  if (mSafeSetUniform)
    glUseProgram( cur_program );
}

void GLSLProgram::setUniformMatrix(const std::string& name, int count, bool transpose, const mat2* mat)
{
  setUniformMatrix(getUniformLocation(name), count, transpose, mat);
}

bool GLSLProgram::useProgram()
{
  GLCHECK4()

  if (GLEW_ARB_shading_language_100 == false)
    return false;

  bool ok = linkProgram();
  CHECK(ok)
  if (!ok)
    return false;
  CHECK(mHandle)
  CHECK(validateProgram())
  glUseProgram(mHandle); GLCHECK4()
  return true;
}

void GLSLProgram::applyGLState()
{
  if (GLEW_ARB_shading_language_100 == false)
    return;

  if ( handle() )
    useProgram();
  else
    glUseProgram(0);
}

