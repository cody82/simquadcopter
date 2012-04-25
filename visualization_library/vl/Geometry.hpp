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

#ifndef Geometry_INCLUDE_DEFINE
#define Geometry_INCLUDE_DEFINE

#include "vl/GPUBuffer.hpp"
#include "vl/vec2.hpp"
#include "vl/vec3d.hpp"
#include "vl/vec4.hpp"
#include "vl/mat4d.hpp"
#include "vl/Drawable.hpp"
#include "vl/OpenGL.hpp"
#include "vl/Object.hpp"
#include "vl/enums.hpp"
#include "vlut/Colors.hpp"

namespace vl
{

  typedef GPUArrayBuffer<1, vec2> GPUArrayVec2;

  typedef GPUArrayBuffer<1, vec3> GPUArrayVec3;

  typedef GPUArrayBuffer<1, vec4> GPUArrayVec4;

  typedef GPUArrayBuffer<1, GLfloat> GPUArrayFloat;
  typedef GPUArrayBuffer<2, GLfloat> GPUArrayFloat2;
  typedef GPUArrayBuffer<3, GLfloat> GPUArrayFloat3;
  typedef GPUArrayBuffer<4, GLfloat> GPUArrayFloat4;

  typedef GPUArrayBuffer<1, GLfloat> GPUArrayDouble;
  typedef GPUArrayBuffer<2, GLfloat> GPUArrayDouble2;
  typedef GPUArrayBuffer<3, GLfloat> GPUArrayDouble3;
  typedef GPUArrayBuffer<4, GLfloat> GPUArrayDouble4;

  typedef GPUArrayBuffer<1, GLbyte> GPUArrayByte;
  typedef GPUArrayBuffer<2, GLbyte> GPUArrayByte2;
  typedef GPUArrayBuffer<3, GLbyte> GPUArrayByte3;
  typedef GPUArrayBuffer<4, GLbyte> GPUArrayByte4;

  typedef GPUArrayBuffer<1, GLshort> GPUArrayShort;
  typedef GPUArrayBuffer<2, GLshort> GPUArrayShort2;
  typedef GPUArrayBuffer<3, GLshort> GPUArrayShort3;
  typedef GPUArrayBuffer<4, GLshort> GPUArrayShort4;

  typedef GPUArrayBuffer<1, GLint> GPUArrayInt;
  typedef GPUArrayBuffer<2, GLint> GPUArrayInt2;
  typedef GPUArrayBuffer<3, GLint> GPUArrayInt3;
  typedef GPUArrayBuffer<4, GLint> GPUArrayInt4;

  class DrawElementsAbstract: public Object
  {
  public:

    virtual ~DrawElementsAbstract() { }
    void setPrimitiveType(EPrimitiveType type) { mType = type; }
    EPrimitiveType primitiveType() const { return mType; }

    virtual void draw(bool use_vbo = true) = 0;

    virtual void clearGPUBuffer() = 0;
    virtual void createLocalBufferFromGPUBuffer() = 0;
    virtual void createGPUBufferFromLocalBuffer(EGPUBufferUsage usage = BUF_STATIC_DRAW)  = 0;
    virtual GLuint handle() const = 0;

    virtual int indexCount() const = 0;
    virtual int index(int i) const = 0;

    protected:
      EPrimitiveType mType;
  };

  class DrawArrays: public DrawElementsAbstract
  {
  public:
    DrawArrays(): mStart(0), mCount(0) 
    { 
      mType = PT_TRIANGLES; 
    }
    DrawArrays(EPrimitiveType primitive, int start, int count): mStart(start), mCount(count) 
    { 
      mType = primitive;
    }

    virtual ~DrawArrays() { }

    virtual void clearGPUBuffer() {}
    virtual void createLocalBufferFromGPUBuffer() {}
    virtual void createGPUBufferFromLocalBuffer(EGPUBufferUsage) {}
    virtual GLuint handle() const { return 0; }
    virtual int indexCount() const { return count(); }
    virtual int index(int i) const { return start() + i; }

    virtual void draw(bool)
    {
      CHECK(start() >= 0)
      CHECK(count() >= 0)
      glDrawArrays( primitiveType(), start(), count() );
    }

    void setStart(int start) { mStart = start; }

    int start() const { return mStart; }

    void setCount(int count) { mCount = count; }

    int count() const { return mCount; }

    protected:
      int mStart;
      int mCount;
  };

  template <typename index_type>
  class DrawElements: public DrawElementsAbstract
  {
  public:
    DrawElements(EPrimitiveType primitive = PT_TRIANGLES) { mType = primitive; }

    void setPrimitiveType(EPrimitiveType type) { mType = type; }

    EPrimitiveType primitiveType() const { return mType; }

    virtual GLuint handle() const
    {
      return mIndexBuffer.handle();
    }

    virtual int index(int i) const
    {
      return mIndexBuffer.object(i);
    }

    virtual int indexCount() const
    {
      return mIndexBuffer.localBufferObjects();
    }

    void addIndex(index_type index)
    {
      mIndexBuffer.push_back(index);
    }

    index_type& at(int index)
    {
      return mIndexBuffer[index];
    }

    const index_type& at(int index) const
    {
      return mIndexBuffer[index];
    }

    void resize(int entry_count)
    {
      mIndexBuffer.resize( entry_count );
    }

    const index_type* localPtr() const
    {
      return mIndexBuffer.localBufferPtr();
    }

    index_type* localPtr()
    {
      return mIndexBuffer.localBufferPtr();
    }

    virtual void createGPUBufferFromLocalBuffer(EGPUBufferUsage usage = BUF_STATIC_DRAW)
    {
      mIndexBuffer.createGPUBufferFromLocalBuffer(usage);
    }

    virtual void createLocalBufferFromGPUBuffer()
    {
      mIndexBuffer.createLocalBufferFromGPUBuffer();
    }

    virtual void clearGPUBuffer()
    {
      mIndexBuffer.clearGPUBuffer();
    }

    virtual void clearLocalBuffer()
    {
      mIndexBuffer.clearLocalBuffer();
    }

    virtual void draw(bool use_vbo)
    {
      GLvoid* ptr = 0;
      if (use_vbo)
      {
        CHECK( GLEW_ARB_vertex_buffer_object )
        CHECK(mIndexBuffer.handle())
        CHECK(mIndexBuffer.gpuBufferObjects())
        if (mIndexBuffer.gpuBufferObjects() == 0 || mIndexBuffer.handle() == 0)
          return;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.handle() );
        ptr = 0;
      }
      else
      {
        CHECK(mIndexBuffer.localBufferObjects())
        if (mIndexBuffer.localBufferObjects() == 0)
          return;
        ptr = localPtr();
      }
      glDrawElements( mType, mIndexBuffer.localBufferObjects(), mIndexBuffer.glType(), ptr ); GLCHECK4()
      if (use_vbo)
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

  protected:
    GPUBufferTemplate<BUF_ELEMENT_ARRAY_BUFFER, 1, index_type> mIndexBuffer;
  };

  typedef DrawElements<GLuint> DrawElementsUInt;

  typedef DrawElements<GLushort> DrawElementsUShort;

  typedef DrawElements<GLubyte> DrawElementsUByte;

  class VertexAttribInfo
  {
  public:
    VertexAttribInfo(GLuint location, bool normalize, GPUBuffer* data): mData(data), mName(location), mNormalize(normalize) {}
    VertexAttribInfo(): mName((GLuint)-1), mNormalize(false) {}
    ref<GPUBuffer> mData;
    GLuint mName;
    bool mNormalize;
  };

  class Geometry: public Drawable
  {
  public:
    Geometry();
    virtual ~Geometry();
    virtual void computeAABB();

    int triangleCount() const;
    int lineCount() const;
    int pointCount() const;
    int vboTriangleCount() const;
    int vboLineCount() const;
    int vboPointCount() const;

	  virtual void computeSmoothNormals();

	  virtual void draw(Actor*, int thread, unsigned int tex_units);

    virtual void clearArrays(bool clear_primitives=true);

    void setColorArray(const vec4& col);
    void setNormalArray(const vec3& norm);

    void setVertexArray(GPUBuffer* data);
    void setNormalArray(GPUBuffer* data);
    void setColorArray(GPUBuffer* data);
    void setSecondaryColorArray(GPUBuffer* data);
    void setFogCoordArray(GPUBuffer* data);
    void setTexCoordArray(unsigned char tex_unit, GPUBuffer* data);

    GPUBuffer* vertexArray() { return mVertexArray.get(); }
    GPUBuffer* normalArray() { return mNormalArray.get(); }
    GPUBuffer* colorArray() { return mColorArray.get(); }
    GPUBuffer* secondaryColorArray() { return mSecondaryColorArray.get(); }
    GPUBuffer* fogCoordArray() { return mFogCoordArray.get(); }
    GPUBuffer* texCoordArray(unsigned char tex) { return mTexCoordArray[tex].get(); }

    void setVertexAttrib(GLuint name, bool normalize, GPUBuffer* data) { setVertexAttrib(VertexAttribInfo(name, normalize, data)); }
    void setVertexAttrib(const VertexAttribInfo& info);
    GPUBuffer* getVertexAttrib(GLuint name);
    void removeVertexAttrib(GLuint name);
    int vertexAttribInfoCount() const { return (int)mVertexAttribInfo.size(); }
    const VertexAttribInfo& vertexAttribInfo(int index) const { return mVertexAttribInfo[index]; }

    void addDrawCall( DrawElementsAbstract* draw_call);
    int drawCallCount() const { return (int)mDrawCalls.size(); }
    DrawElementsAbstract* drawCall(int i) const { return mDrawCalls[i].get(); }
    void removeDrawCall( DrawElementsAbstract* draw_call );
    void removeAllDrawCalls() { mDrawCalls.clear(); }

    void setUseVBO(bool use) { mUseVBO = use; }
    bool useVBO() const { return mUseVBO && GLEW_ARB_vertex_buffer_object; }
    void clearGPUBufferArrays(bool clear_primitives = true);
    void createArraysFromVBO();
    void createGPUBuffersFromLocalBuffers(EGPUBufferUsage vert_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage norm_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage col_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage sec_col_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage fog_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage tex_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage vert_attrib_usage = BUF_STATIC_DRAW, 
                             EGPUBufferUsage prim_usage = BUF_STATIC_DRAW
                             );

    void transform(const mat4d&matr, bool normalize = true, int start=0, int count=-1);

  protected:
    bool mUseVBO;
    
    ref<GPUBuffer> mVertexArray;
	  ref<GPUBuffer> mNormalArray;
	  ref<GPUBuffer> mColorArray;
	  ref<GPUBuffer> mSecondaryColorArray;
	  ref<GPUBuffer> mFogCoordArray;
	  ref<GPUBuffer> mTexCoordArray[MAX_TEXTURE_UNITS];

    std::vector<VertexAttribInfo> mVertexAttribInfo;

	  std::vector< ref<DrawElementsAbstract> > mDrawCalls;

  };

}

#endif
