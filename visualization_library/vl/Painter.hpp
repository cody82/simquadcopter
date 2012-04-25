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

#ifndef Painter_INCLUDE_DEFINE
#define Painter_INCLUDE_DEFINE

#include "vl/Drawable.hpp"
#include "vl/ShaderNode.hpp"
#include "vl/LODEvaluator.hpp"
#include "vl/Actor.hpp"
#include "vl/Shader.hpp"
#include <vector>

namespace vl
{

  class DrawPass
  {
  public:
    DrawPass(Shader* shader): mShader(shader), mFinalShader( new Shader) { CHECK(shader) }
    Shader* shader() { return mShader.get(); }
    const Shader* shader() const { return mShader.get(); }
    Shader* finalShader() { return mFinalShader.get(); }
    const Shader* finalShader() const { return mFinalShader.get(); }
  protected:
    ref<Shader> mShader;
    ref<Shader> mFinalShader;
  };

  class MultipassDraw: public Object
  {
  public:
    MultipassDraw(): mFinalShaderUpdateTime(false) {}
    void setFinalShaderUpdateTime(double ready) { mFinalShaderUpdateTime = ready; }
    double finalShaderUpdateTime() const { return mFinalShaderUpdateTime; }
    void addPass( const DrawPass& pass ) { mPass.push_back(pass); }
    int passCount() const { return (int)mPass.size(); }
    void removeAllPasses() { mPass.clear(); }
    const DrawPass& pass(int i) const { return mPass[i]; }
    DrawPass& pass(int i) { return mPass[i]; }
    void setPass(int i, const DrawPass& pass) { mPass[i] = pass; }
  protected:
    double mFinalShaderUpdateTime;
    std::vector< DrawPass > mPass;
  };

  class Painter: public ShaderNode
  {
    friend class RenderListCompiler;
    friend class Renderer;
  public:
    Painter() { clear(); }
    void clear() { mActors.clear(); mRenderList=0; mRenderRank=0; mActiveLOD=0; }
    ~Painter() { removeAllActors(); }

	  void addActor(Actor* actor);
	  void setActor(int index, Actor* actor);
    void removeActor(Actor* actor);
    void removeActors(int index, int count);
    void removeAllActors();
	  int actorCount() const { return (int)mActors.size(); }
	  Actor* actor(int i) { return mActors[i].get(); }
	  Actor* lastActor() { return mActors.back().get(); }

    void setRenderList(float listname) { mRenderList = listname; }
    float renderList() { return mRenderList; }
    void setRenderRank(float rank) { mRenderRank = rank; }
    float renderRank() { return mRenderRank; }

    void removeAllMultipassDrawLODs() { mMultipassDrawLOD.clear(); }
    void replaceMultipassDrawLOD(int lod, MultipassDraw* multipass) { mMultipassDrawLOD[lod] = multipass; }
    void addMultipassDrawLOD(MultipassDraw* multipass) { mMultipassDrawLOD.push_back(multipass); }
    MultipassDraw* multipassDrawLOD(int lod=-1) { if (lod<0) lod = mActiveLOD; return mMultipassDrawLOD[lod].get(); }
    int multipassDrawLODCount() { return (int)mMultipassDrawLOD.size(); }
    void addDrawPass(Shader* shader, int lod);

    LODEvaluator* lodEvaluator() { return mLODEvaluator.get(); }
    void setLODEvaluator(LODEvaluator* lod_evaluator) { mLODEvaluator = lod_evaluator; }

    void evaluateLOD(Actor* actor, Camera* camera);
    void setActiveLOD(int lod) { mActiveLOD = lod; }
    int activeLOD() const { return mActiveLOD; }

  protected:
    std::vector< ref<Actor> > mActors;
    std::vector< ref<MultipassDraw> > mMultipassDrawLOD;
    ref<LODEvaluator> mLODEvaluator;
    int mActiveLOD;
    float mRenderList;
    float mRenderRank;
  };
}

#endif
