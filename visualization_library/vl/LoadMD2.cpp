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

#include "vl/LoadMD2.hpp"
#include "vl/Time.hpp"

using namespace vl;

MD2Actor::MD2Actor()
{
  mGeometry = new Geometry;
  mGeometry->setUseVBO(false);
  mTexCoords = new GPUArrayVec2;
  mVertices = new GPUArrayVec3;
  mNormals = new GPUArrayVec3;
  setDrawable(mGeometry.get());
  mGeometry->setVertexArray(mVertices.get());
  mGeometry->setNormalArray(mNormals.get());
  mGeometry->setTexCoordArray(0, mTexCoords.get());
  mPolygons = new DrawElementsUInt;
  mGeometry->addDrawCall( mPolygons.get() );

  setAnimation(0,1,1);
  resetGLSLBindings();
  setGLSLVertexBlendEnabled(false);

  mAnim_t_Uniform = new Uniform("anim_t");
  addUniform(mAnim_t_Uniform.get());
}

void MD2Actor::blendFrames(int a, int b, float t)
{

  if (mVertices->size() != mVertexFrames[0]->size() || mNormals->size() != mNormalFrames[0]->size() )
  {
    mVertices->resize( mVertexFrames[0]->size() );
    mNormals->resize( mNormalFrames[0]->size() );

    if (mGeometry->useVBO())
    {
      mVertices->createGPUBufferFromLocalBuffer(BUF_DYNAMIC_DRAW);
      mNormals ->createGPUBufferFromLocalBuffer(BUF_DYNAMIC_DRAW);
    }
  }

#if(0)
  float Ha = 2*t*t*t - 3*t*t + 1; 
  float Hb = -2*t*t*t + 3*t*t;
#else
  float Ha = 1-t;
  float Hb = t;
#endif

  for(int i=0; i<mVertices->size(); i++)
  {
    mVertices->object(i) = mVertexFrames[ a ]->object(i)*Ha + mVertexFrames[ b ]->object(i)*Hb;
    mNormals->object(i)  = mNormalFrames[ a ]->object(i)*Ha + mNormalFrames[ b ]->object(i)*Hb;
  }

  if (mGeometry->useVBO())
  {
    mVertices->updateGPUBufferFromLocalBuffer();
    mNormals ->updateGPUBufferFromLocalBuffer();
  }
    
}

void MD2Actor::prerenderCallback(const Camera*, GLSLProgram* glslprogram)
{

  if ( glslVertexBlendEnabled() )
  {
    if (mVertex2_Binding == -1)
      mVertex2_Binding = glslprogram->getAttribLocation("vertex2");
    if (mNormal2_Binding == -1)
      mNormal2_Binding = glslprogram->getAttribLocation("normal2");

    CHECK(mVertex2_Binding != -1)
    CHECK(mNormal2_Binding != -1)

    mGeometry->setUseVBO(true);
    mGeometry->setVertexArray( mVertexFrames[mFrame1].get() );
    mGeometry->setNormalArray( mNormalFrames[mFrame1].get() );
    mGeometry->setVertexAttrib( mNormal2_Binding, false, mNormalFrames[mFrame2].get() );
    mGeometry->setVertexAttrib( mVertex2_Binding, false, mVertexFrames[mFrame2].get() );
  }
}

void MD2Actor::update(Camera*, double delta_t) 
{
  if (!mAnimationStarted)
    return;

  mElapsed += delta_t;

  if ( mLastUpdate == -1 || (mElapsed - mLastUpdate) > 1.0f/30.0f)
  {
    mLastUpdate = mElapsed;
    double ft = mElapsed / mAnimationPeriod;
    ft = ft - (int)ft;

    int frame_count = mAnimationEnd - mAnimationStart + 1;
    float t  = (float)(ft * frame_count - (int)(ft * frame_count));
    mFrame1 = (int)(ft * frame_count);
    mFrame2 = (mFrame1 + 1) % frame_count;
    mFrame1 += mAnimationStart;
    mFrame2 += mAnimationStart;
    mAnim_t_Uniform->set(1,&t);
    if (glslVertexBlendEnabled() == false)
      blendFrames(mFrame1, mFrame2, t);
  }
}

void MD2Actor::setAnimation(int start, int end, float period)
{
  mLastUpdate = -1;
  mElapsed = 0;
  mAnimationStart   = start;
  mAnimationEnd     = end;
  mAnimationPeriod  = period;
  mAnimationStarted = false;
}

void MD2Actor::startAnimation(double time)
{
  mAnimationStarted = true;
  mLastUpdate = -1;
  mElapsed = 0;
  setLastUpdateTime( time >= 0 ? time : Time::timerSeconds() );
}

void MD2Actor::stopAnimation()
{
  mAnimationStarted = false;
}

void MD2Actor::initFrom(MD2Actor* md2)
{
  mGeometry = new Geometry;

  mTexCoords = md2->mTexCoords;
  mVertices = new GPUArrayVec3;
  mNormals = new GPUArrayVec3;
  setDrawable(mGeometry.get());
  mGeometry->setVertexArray(mVertices.get());
  mGeometry->setNormalArray(mNormals.get());
  mGeometry->setTexCoordArray(0, mTexCoords.get());
  mGeometry->addDrawCall( md2->mPolygons.get() );

  mVertexFrames = md2->mVertexFrames;
  mNormalFrames = md2->mNormalFrames;
  mGeometry->setAABB(md2->mGeometry->aabb());

  setAnimation(40,45,1.5);
}

void MD2Actor::resetGLSLBindings()
{
  mVertex2_Binding = -1;
  mNormal2_Binding = -1;

}

LoadMD2::LoadMD2()
{
  verbose = false;
}

ref<MD2Actor> LoadMD2::loadMD2(const std::string& path) 
{

  FILE *fin = fopen(path.c_str(), "rb");
  if (!fin) {
    Log::error( Say("file not found: %s\n") << path );
	  return 0;
  }
		
  fread(&header, 1, sizeof(header), fin);
	
  if (verbose)
  {
    Log::print( Say("tris  %n:\n") <<  header.numTriangles);
    Log::print( Say("verts %n:\n") <<  header.numVertices);
    Log::print( Say("uvs   %n:\n") <<  header.numTexCoords);
    Log::print( Say("offs skins %n:\n") <<  header.offsetSkins);
    Log::print( Say("offs end %n:\n") <<  header.offsetEnd);
    Log::print( Say("offs frames %n:\n") <<  header.offsetFrames);
    Log::print( Say("offs gl comm %n:\n") <<  header.offsetGlCommands);
    Log::print( Say("offs tex coor %n:\n") <<  header.offsetTexCoords);
    Log::print( Say("offs tri %n:\n") <<  header.offsetTriangles);
    Log::print( Say("skinh %n:\n") <<  header.skinHeight);
    Log::print( Say("skinw %n:\n") <<  header.skinWidth);
  }

  fseek(fin, header.offsetFrames, SEEK_SET);
  md2_frame.resize(header.numFrames);
  for(unsigned i=0; i<md2_frame.size(); i++) {
	  md2_frame[i] = (md2_frame_info*)malloc(header.frameSize * sizeof(char) );
	  fread(md2_frame[i], 1, header.frameSize, fin);

  }
		
  md2_uv.resize(header.numTexCoords);
  fseek(fin, header.offsetTexCoords, SEEK_SET);
  fread(&md2_uv[0], 1, header.numTexCoords*sizeof(md2_uv_info), fin);	
	
  md2_triangle.resize(header.numTriangles);
  fseek(fin, header.offsetTriangles, SEEK_SET);
  fread(&md2_triangle[0], 1, header.numTriangles*sizeof(md2_triangle_info), fin);	
	
  if (header.numSkins) 
  {
	  md2_skin.resize(header.numSkins);
	  fseek(fin, header.offsetSkins, SEEK_SET);
	  fread(&md2_skin[0], 1, header.numSkins*sizeof(md2_skin_info), fin);
  }
	
  fclose(fin);
	
  ref<MD2Actor> md2 = new MD2Actor;

  for(int i=0; i<header.numFrames; i++)
  {
    md2->mVertexFrames.push_back( new GPUArrayVec3 );
    md2->mNormalFrames.push_back( new GPUArrayVec3 );
  }

  int vert_idx = 0;
  CHECK( (int)md2_triangle.size() == header.numTriangles )
  for(int itri=0; itri<header.numTriangles; itri++) 
  {
	  for(int ivert=2; ivert>=0; ivert--)
    {

      float u = (float)md2_uv[ md2_triangle[itri].uv_idx[ivert] ].u / header.skinWidth;
      float v = 1.0f - (float)md2_uv[ md2_triangle[itri].uv_idx[ivert] ].v / header.skinHeight;
      md2->mTexCoords->push_back( vec2(u, v) );

		  for(int iframe=0; iframe<header.numFrames; iframe++) 
      {
			  vec3 v;
        v.x() =      md2_frame[iframe]->vertices[ md2_triangle[itri].vert_idx[ivert] ].vertex[0] * md2_frame[iframe]->scale[0] + md2_frame[iframe]->translate[0];
			  v.y() =      md2_frame[iframe]->vertices[ md2_triangle[itri].vert_idx[ivert] ].vertex[2] * md2_frame[iframe]->scale[2] + md2_frame[iframe]->translate[2];
			  v.z() = -1 *(md2_frame[iframe]->vertices[ md2_triangle[itri].vert_idx[ivert] ].vertex[1] * md2_frame[iframe]->scale[1] + md2_frame[iframe]->translate[1]);
        md2->mVertexFrames[iframe]->push_back( v );
		  }
			
      md2->mPolygons->addIndex( vert_idx++ );
	  }
  }

  for(int iframe=0; iframe<header.numFrames; iframe++) {
	  free(md2_frame[iframe]);
  }

  for(int iframe=0; iframe<header.numFrames; iframe++) 
  {
    md2->mGeometry->setVertexArray(md2->mVertexFrames[iframe].get() );
    md2->mGeometry->setNormalArray(md2->mNormalFrames[iframe].get() );
    md2->mGeometry->computeSmoothNormals();
  }

  md2->mGeometry->setVertexArray(md2->mVertexFrames[0].get() );
  md2->mGeometry->setNormalArray(md2->mNormalFrames[0].get() );
  md2->mGeometry->aabb();

  md2->mGeometry->setVertexArray(md2->mVertices.get() );
  md2->mGeometry->setNormalArray(md2->mNormals.get() );
  md2->blendFrames(0,0,0);

  return md2;
}

