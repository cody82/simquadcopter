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

#include "vl/Light.hpp"
#include "vl/Transform.hpp"

using namespace vl;

Light::Light()
{
  mAmbient = vec4(0,0,0,1);
  mDiffuse = vec4(1,1,1,1);
  mSpecular = vec4(1,1,1,1);
  mPosition = vec4(0,0,0,1);
  mSpotDirection = vec3(0,0,-1);
  mSpotExponent = 0;
  mSpotCutoff = 180.0f;
  mConstantAttenuation = 1.0f;
  mLinearAttentuation = 0.0f;
  mQuadraticAttentuation = 0.0f;
  mFollowedTransform = NULL;
}

void Light::applyLight(GLint lightindex)
{
  glLightfv(GL_LIGHT0+lightindex, GL_AMBIENT, mAmbient.ptr());
  glLightfv(GL_LIGHT0+lightindex, GL_DIFFUSE, mDiffuse.ptr());
  glLightfv(GL_LIGHT0+lightindex, GL_SPECULAR, mSpecular.ptr());
  glLightfv(GL_LIGHT0+lightindex, GL_POSITION, mPosition.ptr());

  glLightf(GL_LIGHT0+lightindex, GL_SPOT_CUTOFF, mSpotCutoff);

  if (mSpotCutoff != 180.0f) 
  {
    CHECK(mSpotCutoff>=0.0f && mSpotCutoff<=90.0f);
    glLightfv(GL_LIGHT0+lightindex, GL_SPOT_DIRECTION, mSpotDirection.ptr());
    glLightf(GL_LIGHT0+lightindex, GL_SPOT_EXPONENT, mSpotExponent);
  }

  if (mSpotCutoff != 180.0f || mPosition.w() != 0)
  {
    glLightf(GL_LIGHT0+lightindex, GL_CONSTANT_ATTENUATION, mConstantAttenuation);
    glLightf(GL_LIGHT0+lightindex, GL_LINEAR_ATTENUATION, mLinearAttentuation);
    glLightf(GL_LIGHT0+lightindex, GL_QUADRATIC_ATTENUATION, mQuadraticAttentuation);
  }
}

void Light::followTransform(Transform* transform) 
{ 
  mFollowedTransform = transform; 
}

Transform* Light::followedTransform()
{ 
  return mFollowedTransform.get(); 
}

const Transform* Light::followedTransform() const
{ 
  return mFollowedTransform.get(); 
}

