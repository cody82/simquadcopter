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

#include "vl/Transform.hpp"
#include "vl/Camera.hpp"
#include <algorithm>

using namespace vl;

Transform::~Transform()
{
  detachFromParent();
  removeAllChildren();
}

int Transform::childCount() const 
{ 
  return (int)mChildren.size(); 
}

Transform* Transform::child(int i) 
{ 
  CHECK(i>=0 && i<(int)mChildren.size())
  return mChildren[i].get(); 
}

Transform* Transform::lastChild()
{
  return mChildren.back().get();
}

void Transform::detachFromParent()
{
  if (mParent != NULL)
    mParent->removeChild(this);
}

void Transform::setChild(int index, Transform* child)
{
  CHECK( index < (int)mChildren.size() )
  mChildren[index] = child;
}

void Transform::addChild(Transform* child)
{
  CHECK(child != NULL)
  CHECK(child->mParent == NULL)
  CHECK(std::find(mChildren.begin(), mChildren.end(), child) == mChildren.end());
  mChildren.push_back(child);
  child->mParent = this;
}

void Transform::removeChild(Transform* child)
{
  CHECK(child != NULL)
  std::vector< ref<Transform> >::iterator it;
  it = std::find(mChildren.begin(), mChildren.end(), child);
  CHECK(it != mChildren.end())
  if (it != mChildren.end())
  {
    (*it)->mParent = NULL;
    mChildren.erase(it);
  }
}

void Transform::removeChildren(int index, int count)
{
  CHECK( index + count <= (int)mChildren.size() );

  for(int i=index+count, j=index; i<(int)mChildren.size(); i++, j++)
    mChildren[j] = mChildren[i];

  mChildren.resize( mChildren.size() - count );
}

void Transform::removeAllChildren()
{
  for(int i=0; i<(int)mChildren.size(); i++)
    mChildren[i]->mParent = NULL;
  mChildren.clear();
}

void Transform::removeAllChildrenRecursive()
{
  for(int i=0; i<(int)mChildren.size(); i++)
  {
    mChildren[i]->removeAllChildrenRecursive();
    mChildren[i]->mParent = NULL;
  }
  mChildren.clear();
}

void Transform::setName(std::string n) 
{ 
  mName = n; 
}

std::string Transform::name() 
{
  return mName; 
}

void Transform::computeLocalToWorldMatrix(Camera* camera)
{
  mat4d m;
  if( parent() )
    m = parent()->localToWorldMatrix(camera ? camera->renderStream() : 0);
  m = m * localMatrix();
  setLocalToWorldMatrix(m, camera ? camera->renderStream() : 0);
}

void Transform::setLocalToWorldMatrix(const mat4d& matrix, int thread_slot) 
{ 
  if (!mLocalToWorldMatrix[thread_slot]) 
    mLocalToWorldMatrix[thread_slot] = new MatrixObject; 
  mLocalToWorldMatrix[thread_slot]->mMatrix = matrix; 
}

const mat4d& Transform::localToWorldMatrix(int thread_slot)
{ 
  if (!mLocalToWorldMatrix[thread_slot]) 
    mLocalToWorldMatrix[thread_slot] = new MatrixObject;
  return mLocalToWorldMatrix[thread_slot]->mMatrix; 
}

MatrixObject* Transform::localToWorldMatrixPtr(int thread_slot)
{ 
  if (!mLocalToWorldMatrix[thread_slot]) 
    mLocalToWorldMatrix[thread_slot] = new MatrixObject;
  return mLocalToWorldMatrix[thread_slot].get(); 
}

void Transform::setLocalMatrix(const mat4d& matrix) 
{ 
  if (!mLocalMatrix) 
    mLocalMatrix = new MatrixObject; 
  mLocalMatrix->mMatrix = matrix; 
}

mat4d& Transform::localMatrix()
{ 
  if (!mLocalMatrix) 
    mLocalMatrix = new MatrixObject;
  return mLocalMatrix->mMatrix; 
}

mat4d Transform::getComputedLocalToWorld()
{
  mat4d world = localMatrix();
  ref<Transform> par = parent();
  while(par)
  {
    world = par->localMatrix() * world;
    par = par->parent();
  }
  return world;
}

Billboard::Billboard()
{
  setAxis(vec3d(0,1,0));
  mType = SphericalBillboard;
}

vec3d Billboard::position()
{
  return localMatrix().getT();
}

void Billboard::setPosition(const vec3d& pos)
{
  localMatrix().setIdentity();
  localMatrix().translate(pos);
}

void Billboard::computeLocalToWorldMatrix(Camera* camera)
{
  if (!camera)
  {
    Transform::computeLocalToWorldMatrix(NULL);
  }
  else
  {
    mat4d rot;
    if ( type() == SphericalBillboard )
    {
      rot.setZ( (camera->viewMatrix().getT() - position()).normalize() );
      rot.setY( camera->viewMatrix().getY() );
      rot.setX( cross(rot.getY(), rot.getZ()) );
    }
    else
    if ( type() == AxisAlignedBillboard )
    {
      rot.setZ( (camera->viewMatrix().getT() - position()).normalize() );
      rot.setY( axis() );
      if ( ( cross(rot.getY(), rot.getZ())).length() < 0.001f )
        rot = mat4d::I();
      else
      {
        rot.setX( (cross(rot.getY(), rot.getZ())).normalize() );
        rot.setZ( (cross(rot.getX(), rot.getY())).normalize() );
      }
    }

    mat4d m;
    if( parent() )
      m = parent()->localToWorldMatrix(camera->renderStream());
    m = m * localMatrix() * rot;
    setLocalToWorldMatrix(m,camera->renderStream());
  }
}

void Transform::computeLocalToWorldMatrixRecursive(Camera* camera)
{
  computeLocalToWorldMatrix(camera);
  for(int i=0; i<(int)mChildren.size(); i++)
    mChildren[i]->computeLocalToWorldMatrixRecursive(camera);
}

