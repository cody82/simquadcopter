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

#ifndef Uniform_INCLUDE_DEFINE
#define Uniform_INCLUDE_DEFINE

#include "vl/GLSL.hpp"

namespace vl
{

  class Uniform: public Object
  {
  public:
    Uniform(): mType(NONE) {}
    Uniform(const std::string& name): mName(name), mType(NONE) {}

    void set(int count, float* data) { initFloat(count*1); memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = FLOAT; }
    void set(int count, vec2* data)  { initFloat(count*2); memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = VEC2; }
    void set(int count, vec3* data)  { initFloat(count*3); memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = VEC3; }
    void set(int count, vec4* data)  { initFloat(count*4); memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = VEC4; }

    void set(int count, int* data)   { initInt(count*1); memcpy(&mIntData[0],data,sizeof(mIntData[0])*mIntData.size()); mType = INT; }
    void set(int count, ivec2* data) { initInt(count*2); memcpy(&mIntData[0],data,sizeof(mIntData[0])*mIntData.size()); mType = IVEC2; }
    void set(int count, ivec3* data) { initInt(count*3); memcpy(&mIntData[0],data,sizeof(mIntData[0])*mIntData.size()); mType = IVEC3; }
    void set(int count, ivec4* data) { initInt(count*4); memcpy(&mIntData[0],data,sizeof(mIntData[0])*mIntData.size()); mType = IVEC4; }

    void set(int count, mat2* data)  { initFloat(count*4);  memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = MAT2; }
    void set(int count, mat3* data)  { initFloat(count*9);  memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = MAT3; }
    void set(int count, mat4* data)  { initFloat(count*16); memcpy(&mFloatData[0],data,sizeof(mFloatData[0])*mFloatData.size()); mType = MAT4; }

    void get(float* data) { CHECK(mType == FLOAT); memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }
    void get(vec2* data)  { CHECK(mType == VEC2);  memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }
    void get(vec3* data)  { CHECK(mType == VEC3);  memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }
    void get(vec4* data)  { CHECK(mType == VEC4);  memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }

    void get(int* data)   { CHECK(mType == INT);   memcpy(data,&mIntData[0],sizeof(mIntData[0])*mIntData.size()); }
    void get(ivec2* data) { CHECK(mType == IVEC2); memcpy(data,&mIntData[0],sizeof(mIntData[0])*mIntData.size()); }
    void get(ivec3* data) { CHECK(mType == IVEC3); memcpy(data,&mIntData[0],sizeof(mIntData[0])*mIntData.size()); }
    void get(ivec4* data) { CHECK(mType == IVEC4); memcpy(data,&mIntData[0],sizeof(mIntData[0])*mIntData.size()); }

    void get(mat2* data)  { CHECK(mType == MAT2); memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }
    void get(mat3* data)  { CHECK(mType == MAT3); memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }
    void get(mat4* data)  { CHECK(mType == MAT4); memcpy(data,&mFloatData[0],sizeof(mFloatData[0])*mFloatData.size()); }

    const std::string& name() const { return mName; }
    void setName(const std::string& name) { mName = name; }

    void applyToGLSLProgram(GLSLProgram* glslprogram)
    {
      if (mType == FLOAT)
        glslprogram->setUniform1f(name().c_str(), (int)mFloatData.size(), &mFloatData[0]);
      else
      if (mType == VEC2)
        glslprogram->setUniform2f(name().c_str(), (int)mFloatData.size() / 2, &mFloatData[0]);
      else
      if (mType == VEC3)
        glslprogram->setUniform3f(name().c_str(), (int)mFloatData.size() / 3, &mFloatData[0]);
      else
      if (mType == VEC4)
        glslprogram->setUniform4f(name().c_str(), (int)mFloatData.size() / 4, &mFloatData[0]);
      else
      if (mType == MAT2)
        glslprogram->setUniformMatrix2f(name().c_str(), (int)mFloatData.size() / 4, false, &mFloatData[0]);
      else
      if (mType == MAT3)
        glslprogram->setUniformMatrix3f(name().c_str(), (int)mFloatData.size() / 9, false, &mFloatData[0]);
      else
      if (mType == MAT4)
        glslprogram->setUniformMatrix4f(name().c_str(), (int)mFloatData.size() / 16, false, &mFloatData[0]);
      else
      if (mType == INT)
        glslprogram->setUniform1i(name().c_str(), (int)mIntData.size(), &mIntData[0]);
      else
      if (mType == IVEC2)
        glslprogram->setUniform2i(name().c_str(), (int)mIntData.size() / 2, &mIntData[0]);
      else
      if (mType == IVEC3)
        glslprogram->setUniform3i(name().c_str(), (int)mIntData.size() / 3, &mIntData[0]);
      else
      if (mType == IVEC4)
        glslprogram->setUniform4i(name().c_str(), (int)mIntData.size() / 4, &mIntData[0]);
    }

  protected:
    void initFloat(int count) { mFloatData.resize(count); mIntData.clear(); }
    void initInt(int count)   { mFloatData.clear(); mIntData.resize(count); }

    std::vector<float> mFloatData;
    std::vector<int>   mIntData;
    std::string mName;
    enum
    {
      NONE = 0x0,
      FLOAT,
      VEC2,
      VEC3,
      VEC4,
      INT,
      IVEC2,
      IVEC3,
      IVEC4,
      MAT2,
      MAT3,
      MAT4,
    } mType;
  };

}

#endif
