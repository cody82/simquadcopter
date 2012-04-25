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

#ifndef Transform_INCLUDE_DEFINE
#define Transform_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/mat4d.hpp"
#include <string>
#include <vector>

namespace vl
{
  class Camera;

  typedef enum
  {
    AxisAlignedBillboard = 1,
    SphericalBillboard = 2
  } EBillboardType;

  class Transform: public Object
  {
  public:
    Transform(): mParent(NULL) { }
    Transform(const mat4d& matrix): mParent(NULL) { setLocalMatrix(matrix); }
    virtual ~Transform();

	  void setName(std::string n);
	  std::string name();

    void addChild(Transform* child);
    void setChild(int index, Transform* child);
	  int childCount() const;
	  Transform* child(int i);
    Transform* lastChild();
    void detachFromParent();
    void removeChild(Transform* child);
    void removeChildren(int index, int count);
    void removeAllChildren();
    void removeAllChildrenRecursive();
    Transform* parent() { return mParent; }
    mat4d getComputedLocalToWorld();

    virtual void computeLocalToWorldMatrix(Camera* camera);
    void computeLocalToWorldMatrixRecursive(Camera* camera);
    const mat4d& localToWorldMatrix(int thread_slot);
    MatrixObject* localToWorldMatrixPtr(int thread_slot);
    void setLocalMatrix(const mat4d& matrix);
    mat4d& localMatrix();

  protected:
    void setLocalToWorldMatrix(const mat4d& matrix, int thread_slot);

    ref<MatrixObject> mLocalToWorldMatrix[MAX_CAMERA_THREAD_SLOTS]; 
    ref<MatrixObject> mLocalMatrix; // local to parent matrix
    std::vector< ref<Transform> > mChildren;
    Transform* mParent;
  };

  class Billboard: public Transform
  {
  public:
    Billboard();
    void setPosition(const vec3d& pos);
    vec3d position();
    void setAxis(const vec3d& axis) { mAxis = axis; mAxis.normalize(); }
    const vec3d& axis() const { return mAxis; }
    virtual void computeLocalToWorldMatrix(Camera* camera);
    EBillboardType type() const { return mType; }
    void setType(EBillboardType type) { mType = type; }

  protected:
    vec3d mAxis;
    EBillboardType mType;
  };

}

#endif
