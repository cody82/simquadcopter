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

#ifndef GLSL_INCLUDE_DEFINE
#define GLSL_INCLUDE_DEFINE

#include "vl/mat4.hpp"
#include "vl/mat3.hpp"
#include "vl/mat2.hpp"
#include "vl/vec4.hpp"
#include "vl/vec3.hpp"
#include "vl/vec2.hpp"
#include "vl/ivec4.hpp"
#include "vl/ivec3.hpp"
#include "vl/ivec2.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <cmath> // sqrt

namespace vl
{

  typedef enum
  {
    ST_VERTEX_SHADER   = GL_VERTEX_SHADER,
    ST_FRAGMENT_SHADER = GL_FRAGMENT_SHADER
  } EShaderType;

  class GLSLShader: public Object
  {
  public:
    GLSLShader(EShaderType type, const std::string& source="", const std::string& name="");

    ~GLSLShader();

    EShaderType type() const { return mType; }

    std::string loadFromFile( const std::string& path );

    bool setSource( const std::string& multi_source, const std::string& name = std::string() );

    const std::string& source() const { return mSource; }

    bool compile();

    bool compileStatus() const;

    std::string infoLog() const;

    void deleteShader();

    GLuint handle() const { return mHandle; }

  protected:
    EShaderType mType;
    std::string mSource;
    GLuint mHandle;
    bool mCompiled;
  };

  class GLSLVertexShader: public GLSLShader
  {
  public:
    GLSLVertexShader(const std::string& source="", const std::string& name=""): GLSLShader(ST_VERTEX_SHADER, source, name) { }
  };

  class GLSLFragmentShader: public GLSLShader
  {
  public:
    GLSLFragmentShader(const std::string& source="", const std::string& name=""): GLSLShader(ST_FRAGMENT_SHADER, source, name) {}
  };

  class GLSLProgram: public Object
  {
    friend class Renderer;
  public:
    GLSLProgram();

    ~GLSLProgram();

    void createProgram();

    void deleteProgram();

    bool attachShader(GLSLShader* shader);

    bool detachShader(GLSLShader* shader);

    bool linkProgram(bool force_relink = false);

    bool linkStatus();

    std::string infoLog() const;

    bool validateProgram();

    void bindAttribLocation(GLuint index, const std::string& name);

    GLint getAttribLocation(const std::string& name);

    static GLint maxVertexAttribs();

    GLint getUniformLocation(const std::string& name);

    void getUniformfv(GLint location, GLfloat* params);

    void getUniformfv(const std::string& name, GLfloat* params);

    void getUniformiv(GLint location, GLint* params);

    void getUniformiv(const std::string& name, GLint* params);

    void setUniform1f(GLint location, int count, const GLfloat* v0);
    void setUniform1f(const std::string& name, int count, GLfloat* v0);

    void setUniform2f(GLint location, int count, const GLfloat* v0);
    void setUniform2f(const std::string& name, int count, GLfloat* v0);

    void setUniform3f(GLint location, int count, const GLfloat* v0);
    void setUniform3f(const std::string& name, int count, GLfloat* v0);

    void setUniform4f(GLint location, int count, const GLfloat* v0);
    void setUniform4f(const std::string& name, int count, GLfloat* v0);

    void setUniform(GLint location, int count, const vec2* vec);
    void setUniform(const std::string& name, int count, const vec2* vec);

    void setUniform(GLint location, int count, const vec3* vec);
    void setUniform(const std::string& name, int count, const vec3* vec);

    void setUniform(GLint location, int count, const vec4* vec);
    void setUniform(const std::string& name, int count, const vec4* vec);

    void setUniform1i(GLint location, int count, const GLint* v0);
    void setUniform1i(const std::string& name, int count, GLint* v0);

    void setUniform2i(GLint location, int count, const GLint* v0);
    void setUniform2i(const std::string& name, int count, GLint* v0);

    void setUniform3i(GLint location, int count, const GLint* v0);
    void setUniform3i(const std::string& name, int count, GLint* v0);

    void setUniform4i(GLint location, int count, const GLint* v0);
    void setUniform4i(const std::string& name, int count, GLint* v0);

    void setUniform(GLint location, int count, const ivec2* vec);
    void setUniform(const std::string& name, int count, const ivec2* vec);

    void setUniform(GLint location, int count, const ivec3* vec);
    void setUniform(const std::string& name, int count, const ivec3* vec);

    void setUniform(GLint location, int count, const ivec4* vec);
    void setUniform(const std::string& name, int count, const ivec4* vec);

    void setUniformMatrix2f(GLint location, int count, bool transpose, const GLfloat* mat);
    void setUniformMatrix2f(const std::string& name, int count, bool transpose, const GLfloat* mat);

    void setUniformMatrix3f(GLint location, int count, bool transpose, const GLfloat* mat);
    void setUniformMatrix3f(const std::string& name, int count, bool transpose, const GLfloat* mat);

    void setUniformMatrix4f(GLint location, int count, bool transpose, const GLfloat* mat);
    void setUniformMatrix4f(const std::string& name, int count, bool transpose, const GLfloat* mat);

    void setUniformMatrix(GLint location, int count, bool transpose, const mat4* mat);
    void setUniformMatrix(const std::string& name, int count, bool transpose, const mat4* mat);

    void setUniformMatrix(GLint location, int count, bool transpose, const mat3* mat);
    void setUniformMatrix(const std::string& name, int count, bool transpose, const mat3* mat);

    void setUniformMatrix(GLint location, int count, bool transpose, const mat2* mat);
    void setUniformMatrix(const std::string& name, int count, bool transpose, const mat2* mat);

    int shaderCount() { return (int)mShaders.size(); }

    GLSLShader* shader(int index) { return mShaders[index].get(); }

    bool useProgram();

    void applyGLState();

    GLuint handle() const { return mHandle; }

    bool needsLink() const { return mNeedsLink; }

  protected:
    std::vector< ref<GLSLShader> > mShaders;
    std::map<std::string, int> mAttribLocationMap;
    std::map<std::string, int> mUniformLocationMap;
    GLuint mHandle;
    bool mNeedsLink;
    bool mSafeSetUniform;
  };

  inline float abs(float a) { return a >= 0 ? a : -a; }
  inline vec2 abs(const vec2& a) 
  { 
    return vec2( a.x() >= 0 ? a.x() : -a.x(),
                 a.y() >= 0 ? a.y() : -a.y() );
  }
  inline vec3 abs(const vec3& a) 
  { 
    return vec3( a.x() >= 0 ? a.x() : -a.x(),
                 a.y() >= 0 ? a.y() : -a.y(),
                 a.z() >= 0 ? a.z() : -a.z() );
  }
  inline vec4 abs(const vec4& a) 
  { 
    return vec4( a.x() >= 0 ? a.x() : -a.x(),
                 a.y() >= 0 ? a.y() : -a.y(),
                 a.z() >= 0 ? a.z() : -a.z(),
                 a.w() >= 0 ? a.w() : -a.w() );
  }

  inline float sign(float a) { return a > 0 ? 1.0f : a == 0 ? 0 : -1.0f; }
  inline vec2 sign(const vec2& a) 
  { 
    return vec2( a.x() > 0 ? 1.0f : a.x() == 0 ? 0 : -1.0f,
                 a.y() > 0 ? 1.0f : a.y() == 0 ? 0 : -1.0f );
  }
  inline vec3 sign(const vec3& a) 
  { 
    return vec3( a.x() > 0 ? 1.0f : a.x() == 0 ? 0 : -1.0f,
                 a.y() > 0 ? 1.0f : a.y() == 0 ? 0 : -1.0f,
                 a.z() > 0 ? 1.0f : a.z() == 0 ? 0 : -1.0f );
  }
  inline vec4 sign(const vec4& a) 
  { 
    return vec4( a.x() > 0 ? 1.0f : a.x() == 0 ? 0 : -1.0f,
                 a.y() > 0 ? 1.0f : a.y() == 0 ? 0 : -1.0f,
                 a.z() > 0 ? 1.0f : a.z() == 0 ? 0 : -1.0f,
                 a.w() > 0 ? 1.0f : a.w() == 0 ? 0 : -1.0f );
  }

  inline float floor(float a) { return float(int(a)); }
  inline vec2 floor(const vec2& a) 
  {
    return vec2( float(int(a.x())), float(int(a.y())) );
  }
  inline vec3 floor(const vec3& a) 
  {
    return vec3( float(int(a.x())), float(int(a.y())), float(int(a.z())) );
  }
  inline vec4 floor(const vec4& a) 
  {
    return vec4( float(int(a.x())), float(int(a.y())), float(int(a.z())), float(int(a.w())) );
  }

  inline float ceil(float a) { return a > 0 ? int(a) == a ? a : int(a+1) : int(a+1)-1.0f; }
  inline vec2 ceil(const vec2& a) 
  {
    return vec2( a.x() > 0 ? int(a.x()) == a.x() ? a.x() : int(a.x()+1) : int(a.x()+1)-1.0f,
                 a.y() > 0 ? int(a.y()) == a.y() ? a.y() : int(a.y()+1) : int(a.y()+1)-1.0f );
  }
  inline vec3 ceil(const vec3& a) 
  {
    return vec3( a.x() > 0 ? int(a.x()) == a.x() ? a.x() : int(a.x()+1) : int(a.x()+1)-1.0f,
                 a.y() > 0 ? int(a.y()) == a.y() ? a.y() : int(a.y()+1) : int(a.y()+1)-1.0f,
                 a.z() > 0 ? int(a.z()) == a.z() ? a.z() : int(a.z()+1) : int(a.z()+1)-1.0f );
  }
  inline vec4 ceil(const vec4& a) 
  {
    return vec4( a.x() > 0 ? int(a.x()) == a.x() ? a.x() : int(a.x()+1) : int(a.x()+1)-1.0f,
                 a.y() > 0 ? int(a.y()) == a.y() ? a.y() : int(a.y()+1) : int(a.y()+1)-1.0f,
                 a.z() > 0 ? int(a.z()) == a.z() ? a.z() : int(a.z()+1) : int(a.z()+1)-1.0f,
                 a.w() > 0 ? int(a.w()) == a.w() ? a.w() : int(a.w()+1) : int(a.w()+1)-1.0f );
  }

  inline float frac(float a) { return a - floor(a); }
  inline vec2 frac(vec2 a) { return a - floor(a); }
  inline vec3 frac(vec3 a) { return a - floor(a); }
  inline vec4 frac(vec4 a) { return a - floor(a); }

  inline float mod(float a, float b) { return a - b * floor(a/b); }
  inline vec2 mod(const vec2& a, float b) { return a - b * floor(a/b); }
  inline vec3 mod(const vec3& a, float b) { return a - b * floor(a/b); }
  inline vec4 mod(const vec4& a, float b) { return a - b * floor(a/b); }
  inline vec2 mod(const vec2& a, const vec2& b) { return a - b * floor(a/b); }
  inline vec3 mod(const vec3& a, const vec3& b) { return a - b * floor(a/b); }
  inline vec4 mod(const vec4& a, const vec4& b) { return a - b * floor(a/b); }

  inline float length(float v) { return v; }
  inline float length(const vec2& v) { return v.length(); }
  inline float length(const vec3& v) { return v.length(); }
  inline float length(const vec4& v) { return v.length(); }

  inline float distance(float p0, float p1) { return length(p0-p1); }
  inline float distance(const vec2& p0, const vec2& p1) { return length(p0-p1); }
  inline float distance(const vec3& p0, const vec3& p1) { return length(p0-p1); }
  inline float distance(const vec4& p0, const vec4& p1) { return length(p0-p1); }

  inline float dot(float a, float b) { return a*b; }

  inline float normalize(float) { return 1; }
  inline vec2 normalize(const vec2& v) { vec2 t = v; t.normalize(); return t; }
  inline vec3 normalize(const vec3& v) { vec3 t = v; t.normalize(); return t; }
  inline vec4 normalize(const vec4& v) { vec4 t = v; t.normalize(); return t; }

  inline float reflect(float I, float N) { return I-2.0f*dot(N,I)*N; }
  inline vec2 reflect(const vec2& I, const vec2& N) { return I-2.0f*dot(N,I)*N; }
  inline vec3 reflect(const vec3& I, const vec3& N) { return I-2.0f*dot(N,I)*N; }
  inline vec4 reflect(const vec4& I, const vec4& N) { return I-2.0f*dot(N,I)*N; }

  inline float refract(float I, float N, float eta) 
  {
    float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    if (k < 0.0f)
      return 0.0f;
    else
      return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
  }
  inline vec2 refract(const vec2& I, const vec2& N, float eta) 
  {
    float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    if (k < 0.0f)
      return vec2(0,0);
    else
      return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
  }
  inline vec3 refract(const vec3& I, const vec3& N, float eta) 
  {
    float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    if (k < 0.0f)
      return vec3(0,0,0);
    else
      return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
  }
  inline vec4 refract(const vec4& I, const vec4& N, float eta) 
  {
    float k = 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    if (k < 0.0f)
      return vec4(0,0,0,0);
    else
      return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
  }

  inline float max(float a, float b) { return a > b ? a : b; }
  inline vec2 max(const vec2& a, const vec2& b)
  { 
    return vec2( a.x() > b.x() ? a.x() : b.x(),
                 a.y() > b.y() ? a.y() : b.y());
  }
  inline vec3 max(const vec3& a, const vec3& b)
  { 
    return vec3( a.x() > b.x() ? a.x() : b.x(),
                 a.y() > b.y() ? a.y() : b.y(),
                 a.z() > b.z() ? a.z() : b.z() );
  }
  inline vec4 max(const vec4& a, const vec4& b)
  { 
    return vec4( a.x() > b.x() ? a.x() : b.x(),
                 a.y() > b.y() ? a.y() : b.y(),
                 a.z() > b.z() ? a.z() : b.z(),
                 a.w() > b.w() ? a.w() : b.w() );
  }
  inline vec2 max(const vec2& a, float b)
  { 
    return vec2( a.x() > b ? a.x() : b,
                 a.y() > b ? a.y() : b);
  }
  inline vec3 max(const vec3& a, float b)
  { 
    return vec3( a.x() > b ? a.x() : b,
                 a.y() > b ? a.y() : b,
                 a.z() > b ? a.z() : b );
  }
  inline vec4 max(const vec4& a, float b)
  { 
    return vec4( a.x() > b ? a.x() : b,
                 a.y() > b ? a.y() : b,
                 a.z() > b ? a.z() : b,
                 a.w() > b ? a.w() : b );
  }

  inline float min(float a, float b) { return a < b ? a : b; }
  inline vec2 min(const vec2& a, const vec2& b)
  { 
    return vec2( a.x() < b.x() ? a.x() : b.x(),
                 a.y() < b.y() ? a.y() : b.y());
  }
  inline vec3 min(const vec3& a, const vec3& b)
  { 
    return vec3( a.x() < b.x() ? a.x() : b.x(),
                 a.y() < b.y() ? a.y() : b.y(),
                 a.z() < b.z() ? a.z() : b.z() );
  }
  inline vec4 min(const vec4& a, const vec4& b)
  { 
    return vec4( a.x() < b.x() ? a.x() : b.x(),
                 a.y() < b.y() ? a.y() : b.y(),
                 a.z() < b.z() ? a.z() : b.z(),
                 a.w() < b.w() ? a.w() : b.w() );
  }
  inline vec2 min(const vec2& a, float b)
  { 
    return vec2( a.x() < b ? a.x() : b,
                 a.y() < b ? a.y() : b);
  }
  inline vec3 min(const vec3& a, float b)
  { 
    return vec3( a.x() < b ? a.x() : b,
                 a.y() < b ? a.y() : b,
                 a.z() < b ? a.z() : b );
  }
  inline vec4 min(const vec4& a, float b)
  { 
    return vec4( a.x() < b ? a.x() : b,
                 a.y() < b ? a.y() : b,
                 a.z() < b ? a.z() : b,
                 a.w() < b ? a.w() : b );
  }

  inline float clamp(float x, float minval, float maxval) { return min(max(x,minval),maxval); }
  inline vec2 clamp(const vec2& x, float minval, float maxval) { return min(max(x,minval),maxval); }
  inline vec3 clamp(const vec3& x, float minval, float maxval) { return min(max(x,minval),maxval); }
  inline vec4 clamp(const vec4& x, float minval, float maxval) { return min(max(x,minval),maxval); }
  inline vec2 clamp(const vec2& x, const vec2& minval, const vec2& maxval) { return min(max(x,minval),maxval); }
  inline vec3 clamp(const vec3& x, const vec3& minval, const vec3& maxval) { return min(max(x,minval),maxval); }
  inline vec4 clamp(const vec4& x, const vec4& minval, const vec4& maxval) { return min(max(x,minval),maxval); }

  inline float mix(float a, float b, float t) { return a*(1-t) + b*t; }
  inline vec2 mix(const vec2& a, const vec2& b, float t) { return a*(1-t) + b*t; }
  inline vec3 mix(const vec3& a, const vec3& b, float t) { return a*(1-t) + b*t; }
  inline vec4 mix(const vec4& a, const vec4& b, float t) { return a*(1-t) + b*t; }
  inline vec2 mix(const vec2& a, const vec2& b, const vec2& t) 
  { 
    return vec2( a.x()*(1-t.x()) + b.x()*t.x(),
                 a.y()*(1-t.y()) + b.y()*t.y() );
  }
  inline vec3 mix(const vec3& a, const vec3& b, const vec3& t) 
  { 
    return vec3( a.x()*(1-t.x()) + b.x()*t.x(),
                 a.y()*(1-t.y()) + b.y()*t.y(),
                 a.z()*(1-t.z()) + b.z()*t.z() );
  }
  inline vec4 mix(const vec4& a, const vec4& b, const vec4& t) 
  { 
    return vec4( a.x()*(1-t.x()) + b.x()*t.x(),
                 a.y()*(1-t.y()) + b.y()*t.y(),
                 a.z()*(1-t.z()) + b.z()*t.z(),
                 a.w()*(1-t.w()) + b.w()*t.w() );
  }

  inline float step( float edge, float a ) { if (a<edge) return 0.0f; else return 1.0f; }
  inline vec2 step( vec2 edge, vec2 a ) 
  { 
    return vec2( a.x()<edge.x() ? 0.0f : 1.0f,
                 a.y()<edge.y() ? 0.0f : 1.0f );
  }
  inline vec3 step( vec3 edge, vec3 a ) 
  { 
    return vec3( a.x()<edge.x() ? 0.0f : 1.0f,
                 a.y()<edge.y() ? 0.0f : 1.0f,
                 a.z()<edge.z() ? 0.0f : 1.0f );
  }
  inline vec4 step( vec4 edge, vec4 a ) 
  { 
    return vec4( a.x()<edge.x() ? 0.0f : 1.0f,
                 a.y()<edge.y() ? 0.0f : 1.0f,
                 a.z()<edge.z() ? 0.0f : 1.0f,
                 a.w()<edge.w() ? 0.0f : 1.0f );
  }

  inline float smoothstep(float edge0, float edge1, float a)
  {
    float t = clamp( (a - edge0) / (edge1 - edge0), 0, 1);
    return t * t * (3 - 2 * t);
  }
  inline vec2 smoothstep(const vec2& edge0, const vec2& edge1, const vec2& a)
  {
    vec2 v;
    float t;
    t = clamp( (a.x() - edge0.x()) / (edge1.x() - edge0.x()), 0, 1); v.x() = t * t * (3 - 2 * t);
    t = clamp( (a.y() - edge0.y()) / (edge1.y() - edge0.y()), 0, 1); v.y() = t * t * (3 - 2 * t);
    return v;
  }
  inline vec3 smoothstep(const vec3& edge0, const vec3& edge1, const vec3& a)
  {
    vec3 v;
    float t;
    t = clamp( (a.x() - edge0.x()) / (edge1.x() - edge0.x()), 0, 1); v.x() = t * t * (3 - 2 * t);
    t = clamp( (a.y() - edge0.y()) / (edge1.y() - edge0.y()), 0, 1); v.y() = t * t * (3 - 2 * t);
    t = clamp( (a.z() - edge0.z()) / (edge1.z() - edge0.z()), 0, 1); v.z() = t * t * (3 - 2 * t);
    return v;
  }
  inline vec4 smoothstep(const vec4& edge0, const vec4& edge1, const vec4& a)
  {
    vec4 v;
    float t;
    t = clamp( (a.x() - edge0.x()) / (edge1.x() - edge0.x()), 0, 1); v.x() = t * t * (3 - 2 * t);
    t = clamp( (a.y() - edge0.y()) / (edge1.y() - edge0.y()), 0, 1); v.y() = t * t * (3 - 2 * t);
    t = clamp( (a.z() - edge0.z()) / (edge1.z() - edge0.z()), 0, 1); v.z() = t * t * (3 - 2 * t);
    t = clamp( (a.w() - edge0.w()) / (edge1.w() - edge0.w()), 0, 1); v.w() = t * t * (3 - 2 * t);
    return v;
  }

  inline mat2 matrixCompMult(const mat2& a, const mat2& b)
  {
    mat2 t;
    for(int i=0; i<2; i++)
      for(int j=0; j<2; j++)
        t[i][j] = a[i][j] * b[i][j];
    return t;
  }
  inline mat3 matrixCompMult(const mat3& a, const mat3& b)
  {
    mat3 t;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
        t[i][j] = a[i][j] * b[i][j];
    return t;
  }
  inline mat4 matrixCompMult(const mat4& a, const mat4& b)
  {
    mat4 t;
    for(int i=0; i<4; i++)
      for(int j=0; j<4; j++)
        t[i][j] = a[i][j] * b[i][j];
    return t;
  }

  inline mat2 transpose(const mat2& a)
  {
    mat2 t;
    for(int i=0; i<2; i++)
      for(int j=0; j<2; j++)
        t[i][j] = a[j][i];
    return t;
  }
  inline mat3 transpose(const mat3& a)
  {
    mat3 t;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
        t[i][j] = a[j][i];
    return t;
  }
  inline mat4 transpose(const mat4& a)
  {
    mat4 t;
    for(int i=0; i<4; i++)
      for(int j=0; j<4; j++)
        t[i][j] = a[j][i];
    return t;
  }

}

#endif
