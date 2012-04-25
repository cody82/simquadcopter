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

#ifndef Actor_INCLUDE_DEFINE
#define Actor_INCLUDE_DEFINE

#include "vl/Drawable.hpp"
#include "vl/LODEvaluator.hpp"
#include "vl/Uniform.hpp"

namespace vl
{
  class GLSLProgram;
  class Painter;
  class Transform;
  class Drawable;
  class LODEvaluator;

  class Actor: public Object
  {
  public:
    Actor(Drawable* drawable = NULL, Transform* transform = NULL, float rank = 0): 
        mPainter(NULL), mTransform(transform), mLastUpdate(-1.0f), mRenderRank(rank), mActiveLOD(0), mLastUpdatedLOD(-1), mEnabled(true) 
        { if(drawable) setDrawable(drawable,0); }
    virtual ~Actor() { }

    Drawable* drawable() { CHECK(mActiveLOD<(int)mDrawable.size()); return mDrawable[ mActiveLOD ].get(); }
    Drawable* drawable(int lod) { CHECK(lod<(int)mDrawable.size()); return mDrawable[lod].get(); }
    void setDrawable(Drawable* drawable, int lod=0) 
    { 
      if (lod >= (int)mDrawable.size())
        mDrawable.resize(lod+1);
      mDrawable[lod] = drawable; 
    }
    void removeAllDrawableLODs() { mDrawable.clear(); }
    int drawableLODCount() const { return (int)mDrawable.size(); }

    Transform* transform()  { return mTransform.get(); }
    void setTransform(Transform* transform) { mTransform = transform; }

    float renderRank() const { return mRenderRank; }
    void setRenderRank(float renderrkank) { mRenderRank = renderrkank; }

    void setScedeDraw(Painter* scededraw) { mPainter = scededraw; }
    const Painter* painter() const { return mPainter; }
    Painter* painter() { return mPainter; }

    void setActiveLOD(int lod) { mActiveLOD = lod; }
    int activeLOD() const { return mActiveLOD; }

    void setLODEvaluator(LODEvaluator* lod_evaluator) { mLODEvaluator = lod_evaluator; }
    LODEvaluator* lodEvaluator() { return mLODEvaluator.get(); }

    void evaluateLOD(Camera* camera);

    int lastUpdatedLOD() const { return mLastUpdatedLOD; }
    void setLastUpdatedLOD(int lod) { mLastUpdatedLOD = lod; }
    double lastUpdateTime() const { return mLastUpdate; }
    void setLastUpdateTime(double time) { mLastUpdate = time; }
    virtual void update(Camera*, double /*delta_t*/) {}

    virtual void prerenderCallback(const Camera*, GLSLProgram*) {}

    void setEnabled(bool enabled) { mEnabled = enabled; }
    bool enabled() const { return mEnabled; }

    void addUniform(Uniform*);
    void removeUniform(Uniform*);
    void removeUniform(const std::string&);
    void removeUniform(int);
    const Uniform* uniform(int) const;
    Uniform* uniform(int);
    int uniformCount();
    void removeAllUniform();

  protected:
    Painter* mPainter;
    std::vector< ref<Drawable> > mDrawable;
    ref<Transform> mTransform;
    ref<LODEvaluator> mLODEvaluator;
    std::vector< ref<Uniform> > mUniform;
    double mLastUpdate;
    float mRenderRank;
    int mActiveLOD;
    int mLastUpdatedLOD;
    bool mEnabled;
  };

}

#endif
