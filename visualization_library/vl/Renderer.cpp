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

#include "vl/Renderer.hpp"
#include "vl/Painter.hpp"
#include "vl/Transform.hpp"
#include "vl/CHECK.hpp"
#include "vl/OpenGL.hpp"
#include "vl/GlobalState.hpp"
#include "vl/Time.hpp"

using namespace vl;

RenderListCompiler::RenderListCompiler()
{
  mFrustumCullingEnabled = true;
  mActorListExtraction = ActorListExtractAlways;
  mAnimateActors = true;
  mAnimateShaders = true;
  mEvaluateLOD = true;
  mUpdateTime = 0;

  mDefaultTokenSorter = new RenderListSorterLazy;

  mMarkMode = DisableMarkedLists;
}

void RenderListCompiler::compileRenderLists()
{
  CHECK( camera() );
  CHECK( scede() );
  CHECK( mActorList );

  if (!scede())
    return;

  if (!camera())
    return;

  TRenderListMap::iterator it;
  for(it = mRenderListSet.begin(); it!=mRenderListSet.end(); it++)
    it->second->clear();

  mShaderAllocator.clear();

  if (actorListExtraction() == ActorListExtractAlways || actorListExtraction() == ActorListExtractOnce)
  {
    if ( actorListExtraction() == ActorListExtractOnce )
      setActorListExtraction(ActorListNeverExtract);

    mActorList->clear();
    extractActorList( mScede.get() );
  }

  if (mActorList->empty())
    return;

  compileRenderListsFromActorList();

  std::vector<float> unused_list;
  for(it = mRenderListSet.begin(); it!=mRenderListSet.end(); it++) 
  {
    if ( it->second->empty() )
      unused_list.push_back(it->first); // schedule deletion of unused lists
    else 
    {
      CHECK(it->second->listSorter())

      it->second->sort(); // sort the render token 

      if ( renderListSorter(it->first) == NULL )
        it->second->setListSorter(NULL);
    }
  }

  for(int i=0; i<(int)unused_list.size(); i++)
    mRenderListSet.erase(unused_list[i]);
}

void RenderListCompiler::setShaderNode(ShaderNode* scede)
{
	mScede = scede;
}

ShaderNode* RenderListCompiler::scede()
{
	return mScede.get();
}

RenderListSorter* RenderListCompiler::renderListSorter(float listname)
{
  std::map< float, ref<RenderListSorter> >::iterator rts = mTokenSorters.find(listname);
  if (rts != mTokenSorters.end())
    return rts->second.get();
  else
    return NULL;
}

void RenderListCompiler::setRenderListSorter(float listname, RenderListSorter* sort)
{
  if (sort == NULL)
    mTokenSorters.erase(listname);
  else
    mTokenSorters[listname] = sort;
}

void RenderListCompiler::extractActorList(ShaderNode* scede)
{
  if (!scede)
    return;

  for( int i=0; i<scede->childCount(); i++ )
    extractActorList( scede->child(i) );

  ref<Painter> painter = dynamic_cast<Painter*>(scede);

  if (painter)
    for( int i=0; i<painter->actorCount(); i++ )
      mActorList->push_back(painter->actor(i));
}

void RenderListCompiler::compileRenderListsFromActorList()
{
  double internal_time = Time::timerSeconds();
  float listname = 0;
  ref<RenderList> list = NULL;
  bool listenabled = true;
  if (mActorList->size())
  {
    listname = mActorList->element(0)->painter()->renderList();
    list = mRenderListSet[ listname ];

    if (!list)
    {
      list = new RenderList;
      mRenderListSet[ listname ] = list;
    }

    listenabled = isEnabled(listname);
  }
  
  for(int iactor=0; iactor<mActorList->size(); iactor++)
  {
    const ref<Actor>&     actor = mActorList->element( iactor );
    const ref<Painter>& painter = actor->painter();

    if ( !actor->enabled() )
      continue;

    if ( frustumCullingEnabled() )
    {
      AABB taabb;
      if ( actor->transform() ) 
        actor->drawable()->aabb().transformed( taabb, actor->transform()->localToWorldMatrix( camera()->renderStream() ) );
      else
        taabb = actor->drawable()->aabb();

      if ( camera()->frustumCull(taabb) )
        continue;
    }

    painter->evaluateLOD( mActorList->element( iactor ).get(), camera() );

    painter->setupShaders( camera(), shaderAnimationEnabled() ? updateTime() : -1, internal_time );

    if (painter->finalEnabled() == false)
      continue;

    if ( evaluateLOD() )
      actor->evaluateLOD(camera());

    if ( actorAnimationEnabled() )
    {
      double t = actor->lastUpdateTime();
      double dt = t == -1 ? 0 : updateTime() - t;
      actor->update(camera(), dt);
      actor->setLastUpdateTime( updateTime() );
      actor->setLastUpdatedLOD( actor->activeLOD() );
    }

    if (listname != painter->renderList())
    {
      listname = painter->renderList();
      list = mRenderListSet[ listname ];

      if (!list) 
      {
        list = new RenderList;
        mRenderListSet[ listname ] = list;
      }

      listenabled = isEnabled(listname);
    }

    if (!listenabled)
      continue;

    if (list->listSorter() == NULL)
    {
      ref<RenderListSorter> tok_sorter = renderListSorter(listname);
      if ( tok_sorter )
      {
        list->setListSorter(tok_sorter.get());
      }
      else
        list->setListSorter(mDefaultTokenSorter.get());
    }

    int pass_count = painter->multipassDrawLODCount() ? painter->multipassDrawLOD()->passCount() : 1;
    ref<Shader> final_shader = painter->finalShader();

    ref<RenderToken> prev_pass = NULL;
    for(int ipass=0; ipass<pass_count; ipass++)
    {

      if ( painter->multipassDrawLODCount() )
        final_shader = painter->multipassDrawLOD()->pass(ipass).finalShader();
      else
      {
        CHECK(pass_count == 1)
      }

      RenderToken& tok = list->newToken(prev_pass != NULL);

      if ( prev_pass != NULL )
        prev_pass->mNextPass = &tok;
      prev_pass = &tok;
      tok.mNextPass = NULL;

      tok.mActor = actor;

      tok.mShader = final_shader;

      tok.mLightCount = tok.mShader->isEnabled(EN_LIGHTING) ? tok.mShader->lightCount() : 0;

      if (tok.mLightCount == 0)
      {
        tok.mShader->disable(EN_LIGHTING);
      }

      tok.mPlaneCount = tok.mShader->planeCount();

      if ( list->listSorter()->requiresZCameraDistance(tok) && !tok.mActor->drawable()->aabb().isEmpty() )
      {
        if (tok.mActor->transform())
          tok.mCameraDistance = ( camera()->inverseViewMatrix() * tok.mActor->transform()->localToWorldMatrix(camera()->renderStream()) * tok.mActor->drawable()->aabb().center() ).lengthSquared();
        else
          tok.mCameraDistance = ( camera()->inverseViewMatrix() * /* I* */ tok.mActor->drawable()->aabb().center() ).lengthSquared();
      }
      else
        tok.mCameraDistance = 0;
	  }
  }
}

Renderer::Renderer() 
{ 
  mLightCount = 0;
  mPlaneCount = 0;
  mEnableDraw = true;
  mEnableShader = true;
}

void Renderer::draw(const TRenderListMap * renderlist)
{
  GLCHECK4()

  GlobalState::renderStream( camera()->renderStream() )->currentShader()->reset();

  GlobalState::renderStream( camera()->renderStream() )->defaultShader()->applyShader( camera()->renderStream() );

  if (renderlist == NULL)
    renderlist = &mRenderListSet;

  for(int i=0; i<MAX_LIGHT_COUNT; i++)
    mActiveLights[i] = NULL;

  mLightCount = MAX_LIGHT_COUNT;

  for(int i=0; i<MAX_CLIPPING_PLANES; i++)
    mActivePlanes[i] = NULL;

  mPlaneCount = MAX_CLIPPING_PLANES;

  GLint max_tex_units = 1;

  if (glActiveTexture != 0)
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_tex_units);
  max_tex_units = max_tex_units < MAX_TEXTURE_UNITS ? max_tex_units : MAX_TEXTURE_UNITS;
  GlobalState::renderStream( camera()->renderStream() )->setMaxTextureUnits( max_tex_units );

  TRenderListMap::const_iterator it;
  ref<Shader> cur_shader = NULL;
  ref<Transform> cur_transform = NULL;
  camera()->applyViewMatrix();
  for(it = renderlist->begin(); it!=renderlist->end(); it++)
  {    
    for(RenderList::iterator el = it->second->begin(); el != it->second->end(); ++el)
    {
      ref<RenderToken> tok = *el;

      for( ; tok != NULL; tok = tok->mNextPass )
      {

        bool pushed_matrix = false; 

        int i=0;
        for(; i<tok->mLightCount; i++)
        {
          const ref<Shader>& fshader = tok->mShader;

		      glEnable(GL_LIGHT0 + i);

          if ( mActiveLights[i] != fshader->light(i) )
          {
            if (!pushed_matrix)
            {
              glMatrixMode(GL_MODELVIEW);
              glPushMatrix(); GLCHECK4()
              pushed_matrix = true;
            }

            if ( fshader->light(i)->followedTransform())

              camera()->applyModelViewMatrix( fshader->light(i)->followedTransform()->localToWorldMatrix(mCamera->renderStream()) );
            else
            {

              glMatrixMode(GL_MODELVIEW);
              glLoadIdentity();
            }

            fshader->light(i)->applyLight(i);
            mActiveLights[i] = fshader->light(i);
          }
        }

        for(;i<mLightCount; i++)
          glDisable(GL_LIGHT0+i);

        mLightCount = tok->mLightCount;

        CHECK( ( !tok->mShader->isEnabled(EN_LIGHTING) && !tok->mLightCount ) || (tok->mShader->isEnabled(EN_LIGHTING) && tok->mLightCount))

        GLCHECK4()

        i=0;
        for(; i<tok->mPlaneCount; i++)
        {
          const ref<Shader>& fshader = tok->mShader;

		      glEnable(GL_CLIP_PLANE0 + i);

          if ( mActivePlanes[i] != fshader->plane(i) )
          {
            if (!pushed_matrix)
            {
              glMatrixMode(GL_MODELVIEW);
              glPushMatrix(); GLCHECK4()
              pushed_matrix = true;
            }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            mat4d mat;
            if ( fshader->plane(i)->followedTransform() )
            {
              mat = camera()->inverseViewMatrix() * fshader->plane(i)->followedTransform()->localToWorldMatrix(mCamera->renderStream());
            }
            fshader->plane(i)->applyPlane(i, mat);
            mActivePlanes[i] = fshader->plane(i);
          }
        }

        for(;i<mPlaneCount; i++)
        {
          glDisable(GL_CLIP_PLANE0+i);
        }

        mPlaneCount = tok->mPlaneCount;

        if (pushed_matrix)
        {
          glMatrixMode(GL_MODELVIEW);
          glPopMatrix(); GLCHECK4()
        }

        GLCHECK4()

        if ( tok->mActor->transform() != cur_transform )
        {
          cur_transform = tok->mActor->transform();
          if ( cur_transform )
            camera()->applyModelViewMatrix( cur_transform->localToWorldMatrix( camera()->renderStream() ) );
          else
            camera()->applyViewMatrix();
        }

        GLCHECK4()

        if (mEnableShader && (cur_shader != tok->mShader))
        {

		      tok->mShader->applyShader( camera()->renderStream() );
          cur_shader = tok->mShader;
        }

        GLCHECK4()

        if (mEnableDraw)
        {

          unsigned char tex_units = 0;
          for(unsigned char i=0; i<max_tex_units; i++)
          {
            if ( tok->mShader->textureUnit(i)->hasTexture() && tok->mShader->textureUnitEnabled(i) )
            {
              tex_units += (1<<i);
            }
          }

          if ( tok->mShader->hasGLSLProgram() && tok->mShader->glslProgram()->handle() )
          {
            tok->mShader->glslProgram()->mSafeSetUniform = false;
            for(int u=0; u<tok->mActor->uniformCount(); ++u)
              tok->mActor->uniform(u)->applyToGLSLProgram( tok->mShader->glslProgram() );
            tok->mShader->glslProgram()->mSafeSetUniform = true;
          }

          tok->mActor->prerenderCallback( camera(), tok->mShader->glslProgram(false) );

          tok->mActor->drawable()->draw(tok->mActor.get(), camera()->renderStream(), tex_units); GLCHECK4()
        }
      }
    }
  }
  camera()->dispatchRenderFinished();
}

