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

#ifndef Scede_INCLUDE_DEFINE
#define Scede_INCLUDE_DEFINE

#include "vl/Shader.hpp"
#include <algorithm>
#include <vector>

namespace vl
{
  class Actor;

  class ShaderNode;
  class ScedeVisitor: public Object
  {
  public:
    virtual ~ScedeVisitor() { }
  };

  class ShaderNode: public Object
  {
    friend class RenderListCompiler;
    friend class Renderer;
  public:
    ShaderNode(): mParent(NULL), mEnabled(true), mFinalEnabled(true), mFinalStatesReady(false) 
    {
      int a=0;
      a++;
    }
    virtual ~ShaderNode();

    void setupShaders(Camera* camera, double cur_time, double internal_time);

    void addChild(ShaderNode* child);
    void setChild(int index, ShaderNode* child);
    int childCount() const;
	  ShaderNode* child(int i);
	  ShaderNode* lastChild();
    void detachFromParent();
    void removeChild(ShaderNode* child);
    void removeChildren(int index, int count);
    void removeAllChildren();
    void removeAllChildrenRecursive();
    ShaderNode* parent() { return mParent; }

    void setToDefaultStates();

    void setShader(Shader* shader);
    Shader* shader();

    void setEnabled(bool enable) { mEnabled = enable; }
    bool enabled() const { return mEnabled; }
    bool finalEnabled() const { return mFinalEnabled; }

    void setChildEnabled(int child, bool enable) { CHECK(child < (int)mChildren.size()); mChildren[child]->setEnabled(enable); }
    bool childEnabled(int child) { CHECK(child < (int)mChildren.size()); return mChildren[child]->enabled(); }

    void extractActors( std::vector< ref<Actor> >& actors, bool append = false );

  protected:
    Shader* finalShader();
    void setFinalStatesUpdateTime(double ready) { mFinalStatesReady = ready; }
    double finalStatesUpdateTime() const { return mFinalStatesReady; }

  protected:

    ref<Shader> mShader;
    ref<Shader> mFinalShader;

    ShaderNode* mParent;
    std::vector< ref<ShaderNode> > mChildren;

    bool mEnabled;
    bool mFinalEnabled;
    double mFinalStatesReady;
  };

}

#endif
