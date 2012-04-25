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

#include "vlut/RenderPipeline.hpp"
#include "vl/Renderer.hpp"
#include "vl/Time.hpp"

using namespace vlut;
using namespace vl;

RenderPipeline::RenderPipeline()
{
  mRenderListCompiler = new RenderListCompiler;
  mRenderer           = new Renderer;
  mShaderNode         = new ShaderNode;
  mTransform          = new Transform;
  mCamera             = new Camera;

  mActorList          = new ActorList;
  mKdTreeCuller       = new KdTreeCuller;

  mRenderListCompiler->setActorListExtraction(ActorListExtractAlways);
  mRenderListCompiler->setFrustumCullingEnabled(true);
  mRenderListCompiler->setActorAnimationEnabled(true);
  mRenderListCompiler->setShaderAnimationEnabled(true);
}

void RenderPipeline::executeRendering()
{

  mRenderListCompiler->setCamera(mCamera.get());
  mRenderListCompiler->setShaderNode(mShaderNode.get());
  mRenderListCompiler->setActorList(mActorList.get());
  mRenderer->setCamera(mCamera.get());
  mKdTreeCuller->setCamera( mCamera.get() );
  mKdTreeCuller->setActorList( mActorList.get() );

  mTransform->computeLocalToWorldMatrixRecursive( mCamera.get() );

  if (mCamera->followedTransform())
    mCamera->setViewMatrix( mCamera->followedTransform()->localToWorldMatrix( mCamera->renderStream() ) );

  CLEAR_GL_ERROR()

  CHECK(mCamera->renderTarget())
  CHECK(mCamera->viewport())
  mCamera->renderTarget()->activate(); // this must be the first
  GLCHECK4()
  mCamera->renderTarget()->activateDrawBuffers();
  GLCHECK4()
  mCamera->viewport()->activate();
  GLCHECK4()
  mCamera->activate();
  GLCHECK4()

  mKdTreeCuller->executeCulling();
  GLCHECK4()

  mRenderListCompiler->setUpdateTime( Time::timerSeconds() ); // used to animate Actors and Shaders
  GLCHECK4()

  mRenderListCompiler->compileRenderLists();
  GLCHECK4()

  mRenderer->draw( mRenderListCompiler->renderLists() );
  GLCHECK4()

}

