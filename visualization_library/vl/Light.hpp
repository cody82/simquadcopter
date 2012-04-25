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

#ifndef Light_INCLUDE_DEFINE
#define Light_INCLUDE_DEFINE

#include "vl/OpenGL.hpp"
#include "vl/vec4.hpp"
#include "vl/Transform.hpp"

namespace vl
{

  class Light: public Object
  {
  public:
    Light();
    virtual void applyLight(GLint lightindex);

    void setAmbient(const vec4& ambientcolor) { mAmbient = ambientcolor; }
    void setDiffuse(const vec4& diffusecolor) { mDiffuse = diffusecolor; }
    void setSpecular(const vec4& specularcolor) { mSpecular = specularcolor; }
    void setPosition(const vec4& position) { mPosition = position; }
    void setSpotDirection(const vec3& spotdirection) { mSpotDirection = spotdirection; }
    void setSpotExponent(GLfloat spotexponent) { mSpotExponent = spotexponent; }
    void setSpotCutoff(GLfloat spotcutoff) { mSpotCutoff = spotcutoff; }
    void setLinearAttentuation(GLfloat linearattenuation) { mLinearAttentuation = linearattenuation; }
    void setQuadraticAttentuation(GLfloat quadraticattenuation) { mQuadraticAttentuation = quadraticattenuation; }
    void setConstantAttenuation(GLfloat constantattenuation) { mConstantAttenuation = constantattenuation; }

    const vec4& ambient() const { return mAmbient; }
    const vec4& diffuse() const { return mDiffuse; }
    const vec4& specular() const { return mSpecular; }
    const vec4& position() const { return mPosition; }
    const vec3& spotDirection() const { return mSpotDirection; }
    GLfloat spotExponent() const { return mSpotExponent; }
    GLfloat spotCutoff() const { return mSpotCutoff; }
    GLfloat constantAttenuation() const { return mConstantAttenuation; }
    GLfloat linearAttenuation() const { return mLinearAttentuation; }
    GLfloat quadraticAttenuation() const { return mQuadraticAttentuation; }

    void followTransform(Transform* transform);
    Transform* followedTransform();
    const Transform* followedTransform() const;
    
  protected:
    vec4 mAmbient;
    vec4 mDiffuse;
    vec4 mSpecular;
    vec4 mPosition;
    vec3 mSpotDirection;
    GLfloat mSpotExponent;
    GLfloat mSpotCutoff;
    GLfloat mConstantAttenuation;
    GLfloat mLinearAttentuation;
    GLfloat mQuadraticAttentuation;
    ref<Transform> mFollowedTransform;
  };
}

#endif
