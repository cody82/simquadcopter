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

#ifndef GlobalState_INCLUDE_DEFINE
#define GlobalState_INCLUDE_DEFINE

#include "vl/Camera.hpp"
#include "vl/Shader.hpp"
#include "vl/Object.hpp"

namespace vl
{

  class RenderStreamState: public Object
  {
  public:
    RenderStreamState(): mMaxTextureUnits(0) {}
    Shader* currentShader();
    Shader* defaultShader();
    Camera* currentCamera() { return mCurrentCamera.get(); }
    void setCurrentCamera(Camera* camera) { mCurrentCamera = camera; }

    int maxTextureUnits() const { return mMaxTextureUnits; }
    void setMaxTextureUnits(int max_tex_unit) { mMaxTextureUnits = max_tex_unit; }

  protected:
    ref<Shader> mDefaultShader; // default shader
    ref<Shader> mCurrentShader; // current state
    ref<Camera> mCurrentCamera; // current camera ( from which you can get vieport and render target )
    int mMaxTextureUnits;
  };

  class GlobalState
  {
  public:
    static RenderStreamState* renderStream(int i)
    {
      CHECK(i >=0 && i<MAX_CAMERA_THREAD_SLOTS)
      if (!mRenderStreamState[i])
        mRenderStreamState[i] = new RenderStreamState;
      return mRenderStreamState[i].get();
    }

    static void shutdown()
    {
      for(int i=0; i<MAX_CAMERA_THREAD_SLOTS; i++)
        mRenderStreamState[i] = NULL;
    }

  protected:
    static ref<RenderStreamState> mRenderStreamState[MAX_CAMERA_THREAD_SLOTS];
  };

}

#endif
