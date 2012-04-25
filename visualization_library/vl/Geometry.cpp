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

#include "vl/Geometry.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"
#include "vl/Shader.hpp"
#include "vl/GlobalState.hpp"
#include <cmath>

using namespace vl;

#define TEX0 (1<<0)
#define TEX1 (1<<1)
#define TEX2 (1<<2)
#define TEX3 (1<<3)
#define TEX4 (1<<4)
#define TEX5 (1<<5)
#define TEX6 (1<<6)
#define TEX7 (1<<7)

#define TEX8 (1<<7)
#define TEX9 (1<<7)
#define TEX10 (1<<7)
#define TEX11 (1<<7)
#define TEX12 (1<<7)
#define TEX13 (1<<7)
#define TEX14 (1<<7)
#define TEX15 (1<<7)

Geometry::Geometry()
{
  mUseVBO = true;
}
Geometry::~Geometry()
{
  if (GLEW_ARB_vertex_buffer_object)
    clearGPUBufferArrays();
}

void Geometry::computeAABB()
{
  if(!mVertexArray)
  {
    mAABB.setEmpty();
    return;
  }

  GPUArrayVec2*    avec2    = dynamic_cast<GPUArrayVec2*>( mVertexArray.get() );
  GPUArrayVec3*    avec3    = dynamic_cast<GPUArrayVec3*>( mVertexArray.get() );
  GPUArrayVec4*    avec4    = dynamic_cast<GPUArrayVec4*>( mVertexArray.get() );
  GPUArrayByte2*   abyte2   = dynamic_cast<GPUArrayByte2*>( mVertexArray.get() );
  GPUArrayShort2*  ashort2  = dynamic_cast<GPUArrayShort2*>( mVertexArray.get() );
  GPUArrayInt2*    aint2    = dynamic_cast<GPUArrayInt2*>( mVertexArray.get() );
  GPUArrayFloat2*  afloat2  = dynamic_cast<GPUArrayFloat2*>( mVertexArray.get() );
  GPUArrayDouble2* adouble2 = dynamic_cast<GPUArrayDouble2*>( mVertexArray.get() );
  GPUArrayByte3*   abyte3   = dynamic_cast<GPUArrayByte3*>( mVertexArray.get() );
  GPUArrayShort3*  ashort3  = dynamic_cast<GPUArrayShort3*>( mVertexArray.get() );
  GPUArrayInt3*    aint3    = dynamic_cast<GPUArrayInt3*>( mVertexArray.get() );
  GPUArrayFloat3*  afloat3  = dynamic_cast<GPUArrayFloat3*>( mVertexArray.get() );
  GPUArrayDouble3* adouble3 = dynamic_cast<GPUArrayDouble3*>( mVertexArray.get() );

  if (avec2)
    mAABB.compute(avec2->localBufferPtr()->ptr(), avec2->size()*2, 2);
  else
  if (avec3)
    mAABB.compute(avec3->localBufferPtr()->ptr(), avec3->size()*3, 3);
  else
  if (avec4)
    mAABB.compute(avec4->localBufferPtr()->ptr(), avec4->size()*4, 4);
  else
  if(abyte2)
    mAABB.compute(abyte2->localBufferPtr(), abyte2->size(), abyte2->components());
  else
  if(ashort2)
    mAABB.compute(ashort2->localBufferPtr(), ashort2->size(), ashort2->components());
  else
  if(aint2)
    mAABB.compute(aint2->localBufferPtr(), aint2->size(), aint2->components());
  else
  if(afloat2)
    mAABB.compute(afloat2->localBufferPtr(), afloat2->size(), afloat2->components());
  else
  if(adouble2)
    mAABB.compute(adouble2->localBufferPtr(), adouble2->size(), adouble2->components());
  else
  if(abyte3)
    mAABB.compute(abyte3->localBufferPtr(), abyte3->size(), abyte3->components());
  else
  if(ashort3)
    mAABB.compute(ashort3->localBufferPtr(), ashort3->size(), ashort3->components());
  else
  if(aint3)
    mAABB.compute(aint3->localBufferPtr(), aint3->size(), aint3->components());
  else
  if(afloat3)
    mAABB.compute(afloat3->localBufferPtr(), afloat3->size(), afloat3->components());
  else
  if(adouble3)
    mAABB.compute(adouble3->localBufferPtr(), adouble3->size(), adouble3->components());
  else
    Log::error("Geometry::computeAABB(): unknown vertex array format.");
}

int Geometry::triangleCount() const
{
  return 0;

}

int Geometry::lineCount() const
{
  return 0;

}

int Geometry::pointCount() const
{
  return 0;

}

void Geometry::setVertexArray(GPUBuffer* data) 
{ 
  CHECK( data->glSize() >=2 && data->glSize()<=4 )
  CHECK( data->glType() == GL_SHORT || 
         data->glType() == GL_INT || 
         data->glType() == GL_FLOAT || 
         data->glType() == GL_DOUBLE );

  mVertexArray = data; 
}

void Geometry::setNormalArray(GPUBuffer* data) 
{ 
  CHECK( data->glSize() == 3 )
  CHECK( data->glType() == GL_BYTE|| 
         data->glType() == GL_SHORT || 
         data->glType() == GL_INT || 
         data->glType() == GL_FLOAT || 
         data->glType() == GL_DOUBLE );

  mNormalArray = data; 
}

void Geometry::setColorArray(GPUBuffer* data) 
{ 
  CHECK( data->glSize() >=3 && data->glSize()<=4 )
  CHECK( data->glType() == GL_BYTE|| 
         data->glType() == GL_SHORT || 
         data->glType() == GL_INT || 
         data->glType() == GL_UNSIGNED_BYTE || 
         data->glType() == GL_UNSIGNED_SHORT || 
         data->glType() == GL_UNSIGNED_INT || 
         data->glType() == GL_FLOAT || 
         data->glType() == GL_DOUBLE );

  mColorArray = data; 
}

void Geometry::setSecondaryColorArray(GPUBuffer* data) 
{ 
  CHECK( data->glSize() >=3 && data->glSize()<=4 )
  CHECK( data->glType() == GL_BYTE|| 
         data->glType() == GL_SHORT || 
         data->glType() == GL_INT || 
         data->glType() == GL_UNSIGNED_BYTE || 
         data->glType() == GL_UNSIGNED_SHORT || 
         data->glType() == GL_UNSIGNED_INT || 
         data->glType() == GL_FLOAT || 
         data->glType() == GL_DOUBLE );

  mSecondaryColorArray = data; 
}

void Geometry::setFogCoordArray(GPUBuffer* data) 
{ 
  CHECK( data->glSize() == 1 )
  CHECK( data->glType() == GL_FLOAT || 
         data->glType() == GL_DOUBLE );

  mFogCoordArray = data; 
}

void Geometry::setTexCoordArray(unsigned char tex_unit, GPUBuffer* data) 
{ 
  CHECK(tex_unit<MAX_TEXTURE_UNITS); 
  mTexCoordArray[tex_unit] = data; 
}

void Geometry::clearGPUBufferArrays(bool clear_primitives)
{
  CHECK(GLEW_ARB_vertex_buffer_object)
  if (!GLEW_ARB_vertex_buffer_object)
    return;

  if (clear_primitives)
  {
    for(int i=0; i<(int)mDrawCalls.size(); i++)
      mDrawCalls[i]->clearGPUBuffer();
  }

  if (mVertexArray)
    mVertexArray->clearGPUBuffer();

  if (mNormalArray)
	  mNormalArray->clearGPUBuffer();

  if (mColorArray)
	  mColorArray->clearGPUBuffer();

  if (mSecondaryColorArray)
	  mSecondaryColorArray->clearGPUBuffer();

  if (mFogCoordArray)
    mFogCoordArray->clearGPUBuffer();

  for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    if (mTexCoordArray[i])
      mTexCoordArray[i]->clearGPUBuffer();
}

void Geometry::createArraysFromVBO()
{
  if (mVertexArray)
    mVertexArray->createLocalBufferFromGPUBuffer();

  if (mNormalArray)
	  mNormalArray->createLocalBufferFromGPUBuffer();

  if (mColorArray)
	  mColorArray->createLocalBufferFromGPUBuffer();

  if (mSecondaryColorArray)
	  mSecondaryColorArray->createLocalBufferFromGPUBuffer();

  if (mFogCoordArray)
    mFogCoordArray->createLocalBufferFromGPUBuffer();

  for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    if (mTexCoordArray[i])
	    mTexCoordArray[i]->createLocalBufferFromGPUBuffer();

  for(int i=0; i<(int)mDrawCalls.size(); i++)
    mDrawCalls[i]->createLocalBufferFromGPUBuffer();
}

void Geometry::createGPUBuffersFromLocalBuffers(EGPUBufferUsage vert_usage,
                                   EGPUBufferUsage norm_usage,
                                   EGPUBufferUsage col_usage,
                                   EGPUBufferUsage sec_col_usage,
                                   EGPUBufferUsage fog_usage,
                                   EGPUBufferUsage tex_usage,                                   
                                   EGPUBufferUsage vert_attrib_usage,
                                   EGPUBufferUsage prim_usage)
{
  if (mVertexArray)
    mVertexArray->createGPUBufferFromLocalBuffer(vert_usage);

  if (mNormalArray)
	  mNormalArray->createGPUBufferFromLocalBuffer(norm_usage);

  if (mColorArray)
	  mColorArray->createGPUBufferFromLocalBuffer(col_usage);

  if (mSecondaryColorArray)
	  mSecondaryColorArray->createGPUBufferFromLocalBuffer(sec_col_usage);

  if (mFogCoordArray)
    mFogCoordArray->createGPUBufferFromLocalBuffer(fog_usage);

  for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    if (mTexCoordArray[i])
	    mTexCoordArray[i]->createGPUBufferFromLocalBuffer(tex_usage);

  for(int i=0; i<vertexAttribInfoCount(); i++)
    vertexAttribInfo(i).mData->createGPUBufferFromLocalBuffer(vert_attrib_usage);

  for(int i=0; i<(int)mDrawCalls.size(); i++)
    mDrawCalls[i]->createGPUBufferFromLocalBuffer(prim_usage);
}

void Geometry::setColorArray(const vec4& col)
{
  if (!mColorArray)
    setColorArray( new GPUArrayVec4 );

  GPUArrayVec3* vec3a = dynamic_cast<GPUArrayVec3*>(mColorArray.get());
  GPUArrayVec4* vec4a = dynamic_cast<GPUArrayVec4*>(mColorArray.get());
  if (vec3a || vec4a)
  {
    mColorArray->resize(mVertexArray->size());
    for(int i=0; i<(int)mColorArray->size(); i++)
    {
      if (vec3a)
        (*vec3a)[i] = col.rgb();
      else
      if (vec4a)
        (*vec4a)[i] = col;
    }
  }
}

void Geometry::setNormalArray(const vec3& norm)
{
  if (!mNormalArray)
    setNormalArray( new GPUArrayVec3 );

  GPUArrayVec3* vec3a = dynamic_cast<GPUArrayVec3*>(mNormalArray.get());
  if (vec3a)
  {
    mNormalArray->resize(mVertexArray->size());
    for(int i=0; i<(int)mNormalArray->size(); i++)
    {
      if (vec3a)
        (*vec3a)[i] = norm;
    }
  }
}

void Geometry::clearArrays(bool clear_primitives)
{
  mVertexArray = NULL;
	mNormalArray = NULL;
	mColorArray = NULL;
	mSecondaryColorArray = NULL;
	mFogCoordArray = NULL;
  for(int i=0; i<MAX_TEXTURE_UNITS; i++)
	  mTexCoordArray[i] = NULL;

  mVertexAttribInfo.clear();

  if (clear_primitives)
	  mDrawCalls.clear();
}

void Geometry::computeSmoothNormals()
{
  if (!mNormalArray)
    setNormalArray(new GPUArrayVec3);

	mNormalArray->resize(mVertexArray->size());
	
  GPUArrayVec3* norm3 = dynamic_cast<GPUArrayVec3*>(mNormalArray.get());
  GPUArrayVec3* vert3 = dynamic_cast<GPUArrayVec3*>(mVertexArray.get());
  GPUArrayVec4* vert4 = dynamic_cast<GPUArrayVec4*>(mVertexArray.get());

  if (!norm3)
    return;

  if (!vert3 && !vert4)
    return;

	for(int prim=0; prim<(int)mDrawCalls.size(); prim++)
	{
    if (mDrawCalls[prim]->primitiveType() == GL_TRIANGLES)
	  {
      for(int i=0; i<(int)mDrawCalls[prim]->indexCount(); i+=3)
	    {
        int a = mDrawCalls[prim]->index(i+0);
	      int b = mDrawCalls[prim]->index(i+1);
	      int c = mDrawCalls[prim]->index(i+2);	      
	      (*norm3)[a] = 0.0f;
	      (*norm3)[b] = 0.0f;
	      (*norm3)[c] = 0.0f;
	    }
	  }
	}
	
	for(int prim=0; prim<(int)mDrawCalls.size(); prim++)
	{
    if (mDrawCalls[prim]->primitiveType() == GL_TRIANGLES)
	  {
	    for(int i=0; i<(int)mDrawCalls[prim]->indexCount(); i+=3)
	    {
	      int a = mDrawCalls[prim]->index(i+0);
	      int b = mDrawCalls[prim]->index(i+1);
	      int c = mDrawCalls[prim]->index(i+2);
	      
        vec3 n, v0, v1, v2;
        if (vert3)
        {
	        v0 = (*vert3)[a];
	        v1 = (*vert3)[b] - v0;
	        v2 = (*vert3)[c] - v0;
        }
        else
        if (vert4)
        {
	        v0 = (*vert4)[a].xyz();
	        v1 = (*vert4)[b].xyz() - v0;
	        v2 = (*vert4)[c].xyz() - v0;
        }

        n = cross(v1, v2).normalize();
	      
	      (*norm3)[a] += n;
	      (*norm3)[b] += n;
	      (*norm3)[c] += n;
	    }
	  }
	}
	
	for(int i=0; i<(int)mVertexArray->size(); i++)
	  (*norm3)[i].normalize();
}

void Geometry::draw(Actor*, int render_stream, unsigned int tex_units)
{
  GLCHECK4()

  if (!mVertexArray)
    return;

  bool vbo_on = useVBO() && GLEW_ARB_vertex_buffer_object; 

  if (vbo_on)
  {
    if ( mVertexArray && mVertexArray->handle() == 0 && mVertexArray->size())
      mVertexArray->createGPUBufferFromLocalBuffer();

    if ( mNormalArray && mNormalArray->handle() == 0 && mNormalArray->size())
      mNormalArray->createGPUBufferFromLocalBuffer();

    if ( mColorArray && mColorArray->handle() == 0 && mColorArray->size())
      mColorArray->createGPUBufferFromLocalBuffer();

     if ( mSecondaryColorArray && mSecondaryColorArray->handle() == 0 && mSecondaryColorArray->size())
      mSecondaryColorArray->createGPUBufferFromLocalBuffer();

    if ( mFogCoordArray && mFogCoordArray->handle() == 0 && mFogCoordArray->size())
      mFogCoordArray->createGPUBufferFromLocalBuffer();

    for(int i=0; i<MAX_TEXTURE_UNITS; i++)
      if ( mTexCoordArray[i] && mTexCoordArray[i]->handle() == 0 && mTexCoordArray[i]->size())
        mTexCoordArray[i]->createGPUBufferFromLocalBuffer();

    for(int i=0; i<(int)drawCallCount(); i++)
      if ( drawCall(i)->handle() == 0 && drawCall(i)->indexCount())
        drawCall(i)->createGPUBufferFromLocalBuffer();
    
    for(int i=0; i<vertexAttribInfoCount(); i++)
    {
      const ref<GPUBuffer>& data = vertexAttribInfo(i).mData;
      if ( data && data->handle() == 0 && data->size() )
        data->createGPUBufferFromLocalBuffer();
    }
  }

  bool vertex_on = (vbo_on && mVertexArray->handle()) || (!vbo_on && mVertexArray->size());

  if (!vertex_on)
    return;

  bool normal_on    = true;
  bool color_on     = true; 
  GLboolean sec_color_on = true;
  GLboolean fog_on       = true;

  fog_on       &= GLEW_EXT_fog_coord;
  sec_color_on &= GLEW_EXT_secondary_color;

  bool tex_on[] = 
  {
    (tex_units & TEX0),
    GLEW_ARB_multitexture && (tex_units & TEX1),
    GLEW_ARB_multitexture && (tex_units & TEX2),
    GLEW_ARB_multitexture && (tex_units & TEX3),
    GLEW_ARB_multitexture && (tex_units & TEX4),
    GLEW_ARB_multitexture && (tex_units & TEX5),
    GLEW_ARB_multitexture && (tex_units & TEX6),
    GLEW_ARB_multitexture && (tex_units & TEX7), 
    GLEW_ARB_multitexture && (tex_units & TEX8),
    GLEW_ARB_multitexture && (tex_units & TEX9),
    GLEW_ARB_multitexture && (tex_units & TEX10),
    GLEW_ARB_multitexture && (tex_units & TEX11),
    GLEW_ARB_multitexture && (tex_units & TEX12),
    GLEW_ARB_multitexture && (tex_units & TEX13),
    GLEW_ARB_multitexture && (tex_units & TEX14),
    GLEW_ARB_multitexture && (tex_units & TEX15)
  };

  CHECK(MAX_TEXTURE_UNITS < 16)

  if (vbo_on)
  {
    normal_on    &= mNormalArray && mNormalArray->handle() != 0;
    color_on     &= mColorArray && mColorArray->handle() != 0;
    sec_color_on &= mSecondaryColorArray && mSecondaryColorArray->handle() != 0;
    fog_on       &= mFogCoordArray && mFogCoordArray->handle() != 0;
    for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    {
      tex_on[i]  &= mTexCoordArray[i] && mTexCoordArray[i]->handle() != 0;
    }
  }
  else
  {
    normal_on    &= mNormalArray && mNormalArray->size() != 0;
    color_on     &= mColorArray && mColorArray->size() != 0;
    sec_color_on &= mSecondaryColorArray && mSecondaryColorArray->size() != 0;
    fog_on       &= mFogCoordArray && mFogCoordArray->size() != 0;
    for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    {
      tex_on[i] &= mTexCoordArray[i] && mTexCoordArray[i]->size() != 0;
    }
  }

#ifndef NDEBUG

#endif

  GLvoid* vertex_pointer = 0;
  GLvoid* normal_pointer = 0;
  GLvoid* color_pointer = 0;
  GLvoid* sec_color_pointer = 0;
  GLvoid* fog_pointer = 0;
  GLvoid* tex_pointer[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  GLCHECK4()

  if (vbo_on == false)
  {
    vertex_pointer = mVertexArray->localBufferVoidPtr();
    if (normal_on) normal_pointer = mNormalArray->localBufferVoidPtr();
    if (color_on) color_pointer = mColorArray->localBufferVoidPtr();
    if (sec_color_on) sec_color_pointer = mSecondaryColorArray->localBufferVoidPtr();
    if (fog_on) fog_pointer = mFogCoordArray->localBufferVoidPtr();
    for(int i=0; i<MAX_TEXTURE_UNITS; i++)
      if (tex_on[i]) tex_pointer[i] = mTexCoordArray[i]->localBufferVoidPtr();
  }

  GLCHECK4()

  if (vbo_on == false && GLEW_ARB_vertex_buffer_object)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  GLCHECK4()

  CHECK(mVertexArray->gpuBufferType() == BUF_ARRAY_BUFFER)
  if (vbo_on)
    glBindBuffer( GL_ARRAY_BUFFER, mVertexArray->handle() );
  glVertexPointer( mVertexArray->glSize(), mVertexArray->glType(), 0, vertex_pointer );
  glEnableClientState(GL_VERTEX_ARRAY);

  GLCHECK4()

  for(int i=0; i<vertexAttribInfoCount(); i++)
  {
    const ref<GPUBuffer>& data = vertexAttribInfo(i).mData;
    CHECK(data->gpuBufferType() == BUF_ARRAY_BUFFER)
    GLvoid* attrib_pointer = 0;
    if ( vbo_on && data->handle() )
      glBindBuffer( GL_ARRAY_BUFFER, data->handle() );
    else
      attrib_pointer = data->localBufferVoidPtr();
    CHECK((int)vertexAttribInfo(i).mName != -1)
    glVertexAttribPointer( vertexAttribInfo(i).mName, data->glSize(), data->glType(), vertexAttribInfo(i).mNormalize, 0, attrib_pointer);

    glEnableVertexAttribArray( vertexAttribInfo(i).mName );

  }

  GLCHECK4()

  if (normal_on)
  {
    CHECK(mNormalArray->gpuBufferType() == BUF_ARRAY_BUFFER)
    if (vbo_on)
      glBindBuffer( GL_ARRAY_BUFFER, mNormalArray->handle() );
    CHECK(mNormalArray->glSize() == 3);
    glNormalPointer( mNormalArray->glType(), 0, normal_pointer );
	  glEnableClientState(GL_NORMAL_ARRAY);
  }

  GLCHECK4()

  if (color_on)
  {
    CHECK(mColorArray->gpuBufferType() == BUF_ARRAY_BUFFER)
    if (vbo_on)
      glBindBuffer( GL_ARRAY_BUFFER, mColorArray->handle() );
    glColorPointer( mColorArray->glSize(), mColorArray->glType(), 0, color_pointer );
	  glEnableClientState(GL_COLOR_ARRAY);
  }

  GLCHECK4()

  if (sec_color_on)
  {
    CHECK(mSecondaryColorArray->gpuBufferType() == BUF_ARRAY_BUFFER)
    if (vbo_on)
      glBindBuffer(GL_ARRAY_BUFFER, mSecondaryColorArray->handle() );
    glSecondaryColorPointer( mSecondaryColorArray->glSize(), mSecondaryColorArray->glType(), 0, sec_color_pointer );
    glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
  }

  GLCHECK4()

  if (fog_on)
  {
    CHECK(mFogCoordArray->gpuBufferType() == BUF_ARRAY_BUFFER)
    if (vbo_on)
      glBindBuffer(GL_ARRAY_BUFFER, mFogCoordArray->handle() );
    CHECK(mFogCoordArray->glSize() == 1);
    glFogCoordPointer( mFogCoordArray->glType(), 0, fog_pointer );
	  glEnableClientState(GL_FOG_COORD_ARRAY);
  }

  GLCHECK4()

  if (tex_units)
  {
    for (int i=0; i<GlobalState::renderStream(render_stream)->maxTextureUnits(); i++)
    {
      if (tex_on[i])
      {
        if (GLEW_ARB_multitexture)
          glClientActiveTexture(GL_TEXTURE0 + i);
        CHECK(mTexCoordArray[i]->gpuBufferType() == BUF_ARRAY_BUFFER)
        if (vbo_on)
          glBindBuffer(GL_ARRAY_BUFFER, mTexCoordArray[i]->handle());
        glTexCoordPointer(mTexCoordArray[i]->glSize(), mTexCoordArray[i]->glType(), 0, tex_pointer[i]);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      }
    }
  }

  GLCHECK4()

  for(int prim=0; prim<(int)mDrawCalls.size(); prim++)
  {
    mDrawCalls[prim]->draw(vbo_on);

  }

  GLCHECK4()

  glDisableClientState(GL_VERTEX_ARRAY);
  
  GLCHECK4()

  for(int i=0; i<vertexAttribInfoCount(); i++)
    glDisableVertexAttribArray( vertexAttribInfo(i).mName );

  GLCHECK4()

  if (normal_on) 
    glDisableClientState(GL_NORMAL_ARRAY);
  
  GLCHECK4()

  if (color_on) 
    glDisableClientState(GL_COLOR_ARRAY);
  
  GLCHECK4()

  if (sec_color_on) 
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
  
  GLCHECK4()

  if (fog_on) 
    glDisableClientState(GL_FOG_COORD_ARRAY);
  
  GLCHECK4()

  if (tex_on[0])
  { 
    if (GLEW_ARB_multitexture)
      glClientActiveTexture(GL_TEXTURE0); 
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  for(int i=1; i<GlobalState::renderStream(render_stream)->maxTextureUnits(); i++)
  {
    if (tex_on[i]) 
    { 
      glClientActiveTexture(GL_TEXTURE0+i); 
      glDisableClientState(GL_TEXTURE_COORD_ARRAY); 

    }
  }

  GLCHECK4()

  if (vbo_on)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  GLCHECK4()
}

void Geometry::transform(const mat4d&matr, bool normalize, int start, int count)
{
  if (count < 0)
    count = (int)mVertexArray->size();

  GPUArrayVec3* norm3 = dynamic_cast<GPUArrayVec3*>(mNormalArray.get());
  GPUArrayVec3* vert3 = dynamic_cast<GPUArrayVec3*>(mVertexArray.get());
  GPUArrayVec4* vert4 = dynamic_cast<GPUArrayVec4*>(mVertexArray.get());

  if (vert3)
  {
    for(int i=start; i<start+count; i++)
      (*vert3)[i] = (matr * vec3d((*vert3)[i])).to_vec3();
  }
  else
  if(vert4)
  {
    for(int i=start; i<start+count; i++)
      (*vert4)[i] = (matr * vec4d((*vert4)[i])).to_vec4();
  }

  mat3d nmat = matr.get3x3().invert();
  nmat.transpose();

  if (norm3)
  {
    for(int i=start; i<start+count; i++)
    {
      (*norm3)[i] = (nmat * vec3d((*norm3)[i])).to_vec3();
      if (normalize)
        (*norm3)[i].normalize();
    }
  }
}

void Geometry::addDrawCall( DrawElementsAbstract* draw_call) 
{ 
  CHECK( std::find(mDrawCalls.begin(), mDrawCalls.end(), draw_call) == mDrawCalls.end() )
  mDrawCalls.push_back(draw_call); 
}

void Geometry::removeDrawCall( DrawElementsAbstract* draw_call) 
{
  for(int i=0; i<drawCallCount(); i++)
  {
    if ( drawCall(i) == draw_call )
    {
      mDrawCalls.erase(mDrawCalls.begin() + i);
      break;
    }
  }
}

void Geometry::setVertexAttrib(const VertexAttribInfo& info)
{
  for(int i=0; i<vertexAttribInfoCount(); ++i)
  {
    if (mVertexAttribInfo[i].mName == info.mName)
    {
      mVertexAttribInfo[i] = info;
      return;
    }
  }
  mVertexAttribInfo.push_back(info);
}

GPUBuffer* Geometry::getVertexAttrib(GLuint name)
{
  for(int i=0; i<vertexAttribInfoCount(); ++i)
  {
    if (mVertexAttribInfo[i].mName == name)
    {
      return mVertexAttribInfo[i].mData.get();
    }
  }
  return NULL;
}

void Geometry::removeVertexAttrib(GLuint name)
{
  for(int i=0; i<vertexAttribInfoCount(); ++i)
  {
    if (mVertexAttribInfo[i].mName == name)
    {
      mVertexAttribInfo.erase(mVertexAttribInfo.begin() + i);
      break;
    }
  }
}

