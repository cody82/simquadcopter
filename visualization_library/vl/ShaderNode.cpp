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

#include "vl/ShaderNode.hpp"
#include "vl/Painter.hpp"
#include "vl/Shader.hpp"
#include "vl/GlobalState.hpp"

using namespace vl;

ShaderNode::~ShaderNode() 
{
  detachFromParent();
  removeAllChildrenRecursive();
}

int ShaderNode::childCount() const 
{ 
  return (int)mChildren.size(); 
}

ShaderNode* ShaderNode::lastChild()
{
  return mChildren.back().get();
}

ShaderNode* ShaderNode::child(int i)
{ 
  CHECK(i>=0 && i<(int)mChildren.size())
  return mChildren[i].get(); 
}

void ShaderNode::detachFromParent()
{
  if (mParent != NULL)
    mParent->removeChild(this);
}

void ShaderNode::setChild(int index, ShaderNode* child)
{
  CHECK( index < (int)mChildren.size() )
  mChildren[index] = child;
}

void ShaderNode::addChild(ShaderNode* child)
{
  CHECK(child != NULL)
  CHECK(child->mParent == NULL)
  CHECK(std::find(mChildren.begin(), mChildren.end(), child) == mChildren.end());
  mChildren.push_back(child);
  child->mParent = this;
}

void ShaderNode::removeChild(ShaderNode* child)
{
  CHECK(child != NULL)
  std::vector< ref<ShaderNode> >::iterator it;
  it = std::find(mChildren.begin(), mChildren.end(), child);
  CHECK(it != mChildren.end())
  if (it != mChildren.end())
  {
    (*it)->mParent = NULL;
    mChildren.erase(it);
  }
}

void ShaderNode::removeChildren(int index, int count)
{
  CHECK( index + count <= (int)mChildren.size() );

  for(int i=index+count, j=index; i<(int)mChildren.size(); i++, j++)
    mChildren[j] = mChildren[i];

  mChildren.resize( mChildren.size() - count );
}

void ShaderNode::removeAllChildren()
{
  for(int i=0; i<(int)mChildren.size(); i++)
    mChildren[i]->mParent = NULL;
  mChildren.clear();
}

void ShaderNode::removeAllChildrenRecursive()
{
  for(int i=0; i<(int)mChildren.size(); i++)
  {
    mChildren[i]->removeAllChildrenRecursive();
    mChildren[i]->mParent = NULL;
  }
  mChildren.clear();
}

void ShaderNode::setToDefaultStates()
{

  mShader->setToDefaults();
}

void ShaderNode::setShader(Shader* shader)
{
  mShader = shader;
}

Shader* ShaderNode::shader()
{
  if (!mShader)
    mShader = new Shader;
  return mShader.get();
}

Shader* ShaderNode::finalShader()
{
  if (!mFinalShader)
    mFinalShader = new Shader;
  return mFinalShader.get();
}

void ShaderNode::setupShaders(Camera* camera, double cur_time, double internal_time)
{

  ref<Painter> painter = dynamic_cast<Painter*>(this);

  bool ready = finalStatesUpdateTime() == internal_time;

  if ( painter && painter->multipassDrawLODCount() )
    ready &= painter->multipassDrawLOD()->finalShaderUpdateTime() == internal_time;

  if ( ready )
    return;

  if ( finalStatesUpdateTime() != internal_time )
  {
    setFinalStatesUpdateTime( internal_time );

    mFinalEnabled = mEnabled;

    if (mFinalEnabled == false)
      return;

    if (mParent)
    {
      if ( mParent->finalStatesUpdateTime() != internal_time )
        mParent->setupShaders(camera, cur_time, internal_time);
      mFinalEnabled &= mParent->mFinalEnabled;
      if (mFinalEnabled == false)
        return;
    }

    if ( cur_time != -1 )
    {
        double t = shader()->lastUpdateTime();
        if ( cur_time != t )
        {
          double dt = t == -1 ? 0 : cur_time - t;
          shader()->setLastUpdateTime( cur_time );
          shader()->update(camera, dt);
        }
    }

    *finalShader() = *shader();

    finalShader()->removeAllLights();
    finalShader()->removeAllPlanes();

    if (mParent)
    {

      finalShader()->combine(mParent->finalShader());

      if ( shader()->inheritParentLights() )
      {
        for(int i=0; i<(int)mParent->finalShader()->lightCount() && finalShader()->lightCount() <= MAX_LIGHT_COUNT; i++)
          finalShader()->addLight(mParent->finalShader()->light(i));
      }

      if ( shader()->inheritParentPlanes() )
      {
        for(int i=0; i<(int)mParent->finalShader()->planeCount() && finalShader()->planeCount() <= MAX_CLIPPING_PLANES; i++)
          finalShader()->addPlane(mParent->finalShader()->plane(i));
      }
    }
    else
      finalShader()->combine( GlobalState::renderStream( camera->renderStream() )->defaultShader() );

    for(int i=0; i<(int)shader()->lightCount() && finalShader()->lightCount() <= MAX_LIGHT_COUNT; i++)
      finalShader()->addLight( shader()->light(i) );

    for(int i=0; i<(int)shader()->planeCount() && finalShader()->planeCount() <= MAX_CLIPPING_PLANES; i++)
      finalShader()->addPlane( shader()->plane(i) );
  }

  if ( painter && painter->multipassDrawLODCount() && painter->multipassDrawLOD()->finalShaderUpdateTime() != internal_time )
  {
    painter->multipassDrawLOD()->setFinalShaderUpdateTime( internal_time );
    for(int j=0; j<painter->multipassDrawLOD()->passCount(); j++)
    {
      Shader* final_sh = painter->multipassDrawLOD()->pass( j ).finalShader();
      Shader* sh       = painter->multipassDrawLOD()->pass( j ).shader();

      if ( cur_time != -1 )
      {
          double t = sh->lastUpdateTime();
          if ( cur_time != t )
          {
            double dt = t == -1 ? 0 : cur_time - t;
            sh->setLastUpdateTime( cur_time );
            sh->update(camera, dt);
          }
      }

      *final_sh = *sh;

      final_sh->removeAllLights();
      final_sh->removeAllPlanes();

      if (mParent)
      {

        final_sh->combine(mParent->finalShader());

        if ( sh->inheritParentLights() )
        {
          for(int i=0; i<(int)mParent->finalShader()->lightCount() && final_sh->lightCount() <= MAX_LIGHT_COUNT; i++)
            final_sh->addLight(mParent->finalShader()->light(i));
        }

        if ( sh->inheritParentPlanes() )
        {
          for(int i=0; i<(int)mParent->finalShader()->planeCount() && final_sh->planeCount() <= MAX_CLIPPING_PLANES; i++)
            final_sh->addPlane(mParent->finalShader()->plane(i));
        }
      }
      else
        final_sh->combine( GlobalState::renderStream( camera->renderStream() )->defaultShader() );

      for(int i=0; i<(int)sh->lightCount() && final_sh->lightCount() <= MAX_LIGHT_COUNT; i++)
        final_sh->addLight( sh->light(i) );

      for(int i=0; i<(int)sh->planeCount() && final_sh->planeCount() <= MAX_CLIPPING_PLANES; i++)
        final_sh->addPlane( sh->plane(i) );
    }
  }
}

void ShaderNode::extractActors( std::vector< ref<Actor> >& actors, bool append )
{
  if (!append)
    actors.clear();

  for( int i=0; i<childCount(); i++ )
    child(i)->extractActors( actors, true );

  ref<Painter> painter = dynamic_cast<Painter*>(this);

  if (painter)
  {

    for( int i=0; i<painter->actorCount(); i++ )
      actors.push_back(painter->actor(i));
  }
}

