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

#ifndef RenderPipeline_INCLUDE_DEFINE
#define RenderPipeline_INCLUDE_DEFINE

#include "vl/KdTreeCuller.hpp"
#include "vl/Renderer.hpp"

namespace vlut
{

  class RenderPipeline: public vl::Object
  {
  public:
    RenderPipeline();

    void executeRendering();

    vl::RenderListCompiler* renderListCompiler() const { return mRenderListCompiler.get(); }
    vl::Renderer* renderer() const { return mRenderer.get(); }
    vl::ShaderNode* shaderNode() const { return mShaderNode.get(); }
    vl::Transform* transform() const { return mTransform.get(); }
    vl::Camera* camera() const { return mCamera.get(); }
    vl::ActorList* actorList() const { return mActorList.get(); }
    vl::KdTreeCuller* kdtreeCuller() const { return mKdTreeCuller.get(); }

    void setRenderListCompiler(vl::RenderListCompiler* render_list_compiler) { mRenderListCompiler = render_list_compiler; }
    void setRenderer(vl::Renderer* renderer) { mRenderer = renderer; }
    void setShaderNode(vl::ShaderNode* shader_node) { mShaderNode = shader_node; }
    void setTransform(vl::Transform* transform) { mTransform = transform; }
    void setCamera(vl::Camera* camera) { mCamera = camera; }
    void setActorList(vl::ActorList* actor_list) { mActorList = actor_list; }
    void setKdTreeCuller(vl::KdTreeCuller* kdtree_culler) { mKdTreeCuller = kdtree_culler; }

    vl::ref<vl::RenderListCompiler> mRenderListCompiler;
    vl::ref<vl::Renderer> mRenderer;
    vl::ref<vl::ShaderNode> mShaderNode;
    vl::ref<vl::Transform> mTransform;
    vl::ref<vl::Camera> mCamera;
    vl::ref<vl::ActorList> mActorList;
    vl::ref<vl::KdTreeCuller> mKdTreeCuller;
  };
}

#endif
