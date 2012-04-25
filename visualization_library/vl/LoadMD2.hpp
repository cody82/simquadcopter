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

#ifndef LoadMD2_INCLUDE_DEFINE
#define LoadMD2_INCLUDE_DEFINE

#include "vl/ShaderNode.hpp"
#include "vl/Actor.hpp"
#include "vl/Geometry.hpp"

namespace vl
{

  class MD2Actor: public Actor
  {
  public:
    MD2Actor();

    void blendFrames(int a, int b, float t);

    virtual void prerenderCallback(const Camera* camera, GLSLProgram* glslprogram);

    virtual void update(Camera*, double delta_t);

    void setAnimation(int start, int end, float period);

    void startAnimation(double time = -1);

    void stopAnimation();

    void initFrom(MD2Actor* md2);

    void resetGLSLBindings();

    bool glslVertexBlendEnabled() const { return mGLSLVertexBlendEnabled; }
    void setGLSLVertexBlendEnabled(bool enable) { mGLSLVertexBlendEnabled = enable; }

    ref<Geometry> mGeometry;
    ref<DrawElementsUInt> mPolygons;
    ref<GPUArrayVec2> mTexCoords;
    ref<GPUArrayVec3> mVertices;
    ref<GPUArrayVec3> mNormals;
    std::vector< ref<GPUArrayVec3> > mVertexFrames;
    std::vector< ref<GPUArrayVec3> > mNormalFrames;

  protected:
    double mLastUpdate;
    double mElapsed;
    int mAnimationStart;
    int mAnimationEnd;
    float mAnimationPeriod;
    bool mAnimationStarted;
    int mFrame1;
    int mFrame2;
    float mAnimationT;

    bool mGLSLVertexBlendEnabled;
    int mVertex2_Binding;
    int mNormal2_Binding;

    ref<Uniform> mAnim_t_Uniform;
  };

  class LoadMD2
  {
  public:
    LoadMD2();

    ref<MD2Actor> loadMD2(const std::string& path);

  protected:
	  enum {
		  MD2_MAX_TRIANGLES = 4096,
		  MD2_MAX_VERTICES  = 2048,
		  MD2_MAX_TEXCOORDS = 2048,
		  MD2_MAX_FRAMES    = 512,
		  MD2_MAX_SKINS     = 32,
		  MD2_MAX_FRAMESIZE = MD2_MAX_VERTICES * 4 + 128
	  };

	  class md2_header_info
    {
    public:
	     int magic;
	     int version;
	     int skinWidth;
	     int skinHeight;
	     int frameSize;
	     int numSkins;
	     int numVertices;
	     int numTexCoords;
	     int numTriangles;
	     int numGlCommands;
	     int numFrames;
	     int offsetSkins;
	     int offsetTexCoords;
	     int offsetTriangles;
	     int offsetFrames;
	     int offsetGlCommands;
	     int offsetEnd;
	  };

	  class md2_vertex_info
    {
    public:
      unsigned char vertex[3];
      unsigned char light_norm_index;
	  };

	  class md2_triangle_info
    {
    public:
	     short vert_idx[3];
	     short uv_idx[3];
	  };

	  class md2_uv_info
    {
    public:
	     short u, v;
	  };

	  class md2_frame_info
    {
    public:
	     float scale[3];
	     float translate[3];
	     char name[16];
	     md2_vertex_info vertices[1];
	  };

	  class md2_skin_info
    {
    public:
		  unsigned char name[64];
	  };

    md2_header_info header;
    std::vector<md2_frame_info*> md2_frame;
    std::vector<md2_uv_info> md2_uv;
    std::vector<md2_triangle_info> md2_triangle;
    std::vector<md2_skin_info> md2_skin;
    bool verbose;
  };

}

#endif
