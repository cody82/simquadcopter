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

#ifndef RenderStage_INCLUDE_DEFINE
#define RenderStage_INCLUDE_DEFINE

#include "vl/Camera.hpp"
#include "vl/QuickList.hpp"
#include "vl/mat4.hpp"
#include "vl/Drawable.hpp"
#include "vl/Shader.hpp"
#include "vl/Painter.hpp"
#include "vl/ObjectVector.hpp"
#include "vl/Actor.hpp"
#include <vector>
#include <map>
#include <set>

namespace vl
{
  class RenderListSorter;
  class Camera;

  class RenderToken: public Object
  {
  public:
    RenderToken(): mLightCount(0), mPlaneCount(0), mCameraDistance(0) {}

    ref<RenderToken> mNextPass;

    ref<Actor> mActor;

    ref<Shader> mShader;

    int mLightCount;

    int mPlaneCount;

    double mCameraDistance;
  };

  class RenderListSorter: public Object
  {
  public:
    virtual bool operator()(const RenderToken& a, const RenderToken& b) = 0;
    virtual bool requiresZCameraDistance(const RenderToken&) = 0;
  };

  class RenderTokenSortByActor: public RenderListSorter
  {
  public:
    virtual bool requiresZCameraDistance(const RenderToken&) { return false; }
    virtual bool operator()(const RenderToken& A, const RenderToken& B)
    {
      if (A.mActor->renderRank() != B.mActor->renderRank())
        return A.mActor->renderRank() < B.mActor->renderRank();
      else

      if (A.mShader.get() != B.mShader.get())
        return A.mShader.get() < B.mShader.get();
      else

      if (A.mActor->painter() != B.mActor->painter())
        return A.mActor->painter() < B.mActor->painter();
      else

        return A.mActor.get() < B.mActor.get();
    }
  };

  class RenderTokenSortByScedeActor: public RenderListSorter
  {
  public:
    virtual bool requiresZCameraDistance(const RenderToken&) { return false; }
    virtual bool operator()(const RenderToken& A, const RenderToken& B)
    {

      if (A.mActor->painter()->renderRank() != B.mActor->painter()->renderRank())
        return A.mActor->painter()->renderRank() < B.mActor->painter()->renderRank();
      else

      if (A.mActor->renderRank() != B.mActor->renderRank())
        return A.mActor->renderRank() < B.mActor->renderRank();
      else

      if (A.mShader.get() != B.mShader.get())
        return A.mShader.get() < B.mShader.get();
      else

      if (A.mActor->painter() != B.mActor->painter())
        return A.mActor->painter() < B.mActor->painter();
      else

        return A.mActor.get() < B.mActor.get();
    }
  };

  class RenderListSorterAggressive: public RenderTokenSortByScedeActor
  {
  public:
    RenderListSorterAggressive(): mDepthSortMode(AlphaDepthSort) { }

    virtual bool requiresZCameraDistance(const RenderToken& A) 
    { return mDepthSortMode != NeverDepthSort && (mDepthSortMode == AlwaysDepthSort  || 
      (A.mShader->isEnabled(EN_BLEND) && (mDepthSortMode == AlphaDepthSort)) ); }

    virtual bool operator()(const RenderToken& A, const RenderToken& B)
    {

      if (A.mActor->painter()->renderRank() != B.mActor->painter()->renderRank())
        return A.mActor->painter()->renderRank() < B.mActor->painter()->renderRank();
      else
      if (A.mActor->renderRank() != B.mActor->renderRank())
        return A.mActor->renderRank() < B.mActor->renderRank();
      else

      if ( mDepthSortMode != AlwaysDepthSort && (A.mShader->isEnabled(EN_BLEND) != B.mShader->isEnabled(EN_BLEND)))
      {
        return !A.mShader->isEnabled(EN_BLEND); // first draw opaque objects
      }
      else // A/B.mShader->isEnabled(OGL_BLEND) are equal or AlwaysDepthSort
      if ( requiresZCameraDistance(A) )
      {
        return A.mCameraDistance > B.mCameraDistance; // draw first far objects then the close ones
      }
      else

      if (A.mShader->getDepthMask() != B.mShader->getDepthMask())
        return A.mShader->getDepthMask(); // first draw if zwrite is on
      else

      if ( A.mShader->sortByGLSLProgram(B.mShader.get()) )
        return true;
      else
      if ( B.mShader->sortByGLSLProgram(A.mShader.get()) )
        return false; // A.mShader->sortByGLSLProgram(B.mShader) was "really" false 
      else // both were false so they are actually equal...

      if ( A.mShader->sortByTextUnits(B.mShader.get()) )
        return true;
      else
      if ( B.mShader->sortByTextUnits(A.mShader.get()) )
        return false; // A.mShader->sortByTextUnits(B.mShader) was "really" false 
      else // both were false so they are actually equal...

      if ( A.mShader->sortByMaterial(B.mShader.get()) ) 
        return true;
      else
      if ( B.mShader->sortByMaterial(A.mShader.get()) ) 
        return false; // A.mShader->sortByMaterial(B.mShader) waw "really" false
      else // both were false so they are actually equal...

      if (A.mShader->isEnabled(EN_LIGHTING) != B.mShader->isEnabled(EN_LIGHTING))
        return A.mShader->isEnabled(EN_LIGHTING); // first draw lit objects
      else

      if (A.mShader.get() != B.mShader.get())
        return A.mShader.get() < B.mShader.get();
      else

      if (A.mActor->painter() != B.mActor->painter())
        return A.mActor->painter() < B.mActor->painter();
      else

        return A.mActor.get() < B.mActor.get();
    }

    EDepthSortMode depthSortMode() const { return mDepthSortMode; }
    void setDepthSortMode(EDepthSortMode mode) { mDepthSortMode = mode; }

  public:
    EDepthSortMode mDepthSortMode;
  };

  class RenderListSorterLazy: public RenderTokenSortByScedeActor
  {
  public:
    RenderListSorterLazy(): mDepthSortMode(AlphaDepthSort) { }

    virtual bool requiresZCameraDistance(const RenderToken& A) 
    { return mDepthSortMode != NeverDepthSort && (mDepthSortMode == AlwaysDepthSort  || 
      (A.mShader->isEnabled(EN_BLEND) && (mDepthSortMode == AlphaDepthSort)) ); }

    virtual bool operator()(const RenderToken& A, const RenderToken& B)
    {

      if (A.mActor->painter()->renderRank() != B.mActor->painter()->renderRank())
        return A.mActor->painter()->renderRank() < B.mActor->painter()->renderRank();
      else
      if (A.mActor->renderRank() != B.mActor->renderRank())
        return A.mActor->renderRank() < B.mActor->renderRank();
      else

      if ( mDepthSortMode != AlwaysDepthSort && (A.mShader->isEnabled(EN_BLEND) != B.mShader->isEnabled(EN_BLEND)))
      {
        return !A.mShader->isEnabled(EN_BLEND); // first draw opaque objects
      }
      else // A/B.mShader->isEnabled(OGL_BLEND) are equal or AlwaysDepthSort
      if ( requiresZCameraDistance(A) )
      {
        return A.mCameraDistance > B.mCameraDistance; // draw first far objects then the close ones
      }
      else

      if (A.mShader->getDepthMask() != B.mShader->getDepthMask())
        return A.mShader->getDepthMask(); // first draw if zwrite is on
      else

      if (A.mShader.get() != B.mShader.get())
        return A.mShader.get() < B.mShader.get();
      else

      if (A.mActor->painter() != B.mActor->painter())
        return A.mActor->painter() < B.mActor->painter();
      else

        return A.mActor.get() < B.mActor.get();
    }

    EDepthSortMode depthSortMode() const { return mDepthSortMode; }
    void setDepthSortMode(EDepthSortMode mode) { mDepthSortMode = mode; }

  public:
    EDepthSortMode mDepthSortMode;
  };

  class RenderList: public Object
  {
  public:

    typedef std::vector< ref<RenderToken> >::iterator iterator; 
  public:
    RenderList() {}
    virtual ~RenderList() {} 

    RenderListSorter* listSorter() { return mTokenSorter.get(); }

    void setListSorter(RenderListSorter* sorter) { mTokenSorter = sorter; }

    RenderToken& newToken(bool multipass)
    {
      if (multipass)
      {
        MultipassList.push_back( Allocator.allocate() );
        return *MultipassList.back();
      }
      else
      {
        List.push_back( Allocator.allocate() );
        return *List.back();
      }
    }

    void clear() { List.clear(); MultipassList.clear(); Allocator.clear(); }
    
    iterator begin() { return List.begin(); }
    
    iterator end() { return List.end(); }
    
    bool empty() { return List.empty(); }

    int size() const { return (int)List.size(); }

    void sort()
    {
      CHECK(mTokenSorter);
      std::sort( List.begin(), List.end(), Sorter(mTokenSorter.get()) );
    }

  protected:
    class Sorter
    {
      ref<RenderListSorter> mTokenSorter;
    public:
      Sorter(RenderListSorter* sorter): mTokenSorter(sorter) {}
      bool operator()(const ref<RenderToken>& a, const ref<RenderToken>& b)
      {
        CHECK(a && b);
        return mTokenSorter->operator()(*a, *b);
      }
    };

  public:
    QuickAllocator<RenderToken> Allocator;
    ref<RenderListSorter> mTokenSorter;  
    std::vector< ref<RenderToken> > List;
    std::vector< ref<RenderToken> > MultipassList;
  };

  typedef std::map< float, ref<RenderList> > TRenderListMap;

  class RenderListCompiler: public ScedeVisitor
  {
  public:
    RenderListCompiler();
    virtual ~RenderListCompiler() {}
    void setActorList(ActorList* actor_list) { mActorList = actor_list; }
    ActorList* actorList() { return mActorList.get(); }

    EActorListExtraction actorListExtraction() const { return mActorListExtraction; }
    void setActorListExtraction(EActorListExtraction mode) { mActorListExtraction = mode; }

    bool evaluateLOD() const { return mEvaluateLOD; }
    void setEvaluateLOD(bool evaluate_lod) { mEvaluateLOD = evaluate_lod; }

    bool actorAnimationEnabled() const { return mAnimateActors; }
    void setActorAnimationEnabled(bool animate_actors) { mAnimateActors = animate_actors; }

    bool shaderAnimationEnabled() const { return mAnimateShaders; }
    void setShaderAnimationEnabled(bool animate_shaders) { mAnimateShaders = animate_shaders; }

    void compileRenderLists();
    const TRenderListMap* renderLists() const { return &mRenderListSet; } 
    const TRenderListMap* renderLists() { return &mRenderListSet; } 
    void setCamera(Camera* camera) { mCamera = camera; }
    Camera* camera() { return mCamera.get(); }
	  void setShaderNode(ShaderNode* root);
	  ShaderNode* scede();

    void setRenderListSorter(float listname, RenderListSorter* sort);

    RenderListSorter* renderListSorter(float listname);
    void setMarkMode(EMarkMode mode) { mMarkMode = mode; }
    EMarkMode markMode() const { return mMarkMode; }
    bool isMarked(float listname) { return mMarkedList.find(listname) != mMarkedList.end(); }
    bool isEnabled(float listname)
    {
      if(mMarkMode == EnableMarkedLists)
        return isMarked(listname) == true;
      else // DisableMarkedLists
        return isMarked(listname) == false; 
    }
    void markList(float listname)
    {
      mMarkedList.insert(listname);
    }
    void unmarkList(float listname)
    {
      mMarkedList.erase(listname);
    }

    double updateTime() const { return mUpdateTime; }
    void setUpdateTime(double cur_time) { mUpdateTime = cur_time; }

    bool frustumCullingEnabled() const { return mFrustumCullingEnabled; }
    void setFrustumCullingEnabled(bool enabled) { mFrustumCullingEnabled = enabled; }

  protected:
    void extractActorList( ShaderNode* scede );

    void compileRenderListsFromActorList();

  protected:

    TRenderListMap mRenderListSet;

    std::set<float> mMarkedList;

    EMarkMode mMarkMode;

	  ref<ShaderNode> mScede;

    ref<Camera> mCamera;

    std::map< float, ref<RenderListSorter> > mTokenSorters;

    ref<RenderListSorter> mDefaultTokenSorter;

    QuickAllocator<Shader> mShaderAllocator;

    ref<ActorList> mActorList;

    double mUpdateTime;
    EActorListExtraction mActorListExtraction;
    bool mFrustumCullingEnabled;
    bool mEvaluateLOD;
    bool mAnimateActors;
    bool mAnimateShaders;
  };

  class Renderer: public Object
  {
  public:
	  Renderer();
	  virtual ~Renderer() {}
    virtual void draw(const TRenderListMap * renderlist = NULL);

    void clearRenderLists() { mRenderListSet.clear(); }

    void addRenderLists(const TRenderListMap* lists) 
    {
      for(TRenderListMap::const_iterator it = lists->begin(); it != lists->end(); ++it)
        mRenderListSet[it->first] = it->second;
    }

    void setCamera(Camera* camera) { mCamera = camera; }
    const Camera* camera() const { return mCamera.get(); }
    Camera* camera() { return mCamera.get(); }

    void setEnableDraw(bool enable) { mEnableDraw = enable; }
    void setEnableShader(bool enable) { mEnableShader = enable; }

  protected:

    ref<Light> mActiveLights[MAX_LIGHT_COUNT];
    ref<Plane> mActivePlanes[MAX_CLIPPING_PLANES];

    TRenderListMap mRenderListSet;

    ref<Camera> mCamera;
    int mLightCount;
    int mPlaneCount;
    bool mEnableDraw;
    bool mEnableShader;
  };

}

#endif
