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

#ifndef Culler_INCLUDE_DEFINE

#include "vl/Camera.hpp"

namespace vl
{
  class Culler: public Object
  {
  public:
    virtual void executeCulling() = 0;
    Camera* camera() { return mCamera.get(); }
    void setCamera(Camera* camera) { mCamera = camera; }

  protected:
    ref<Camera> mCamera;
  };
}

#endif
