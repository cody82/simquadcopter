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

#ifndef GPUBuffer_INCLUDE_DEFINE
#define GPUBuffer_INCLUDE_DEFINE

#include "vl/OpenGL.hpp"
#include "vl/vec2.hpp"
#include "vl/vec3.hpp"
#include "vl/vec4.hpp"
#include "vl/ivec2.hpp"
#include "vl/ivec3.hpp"
#include "vl/ivec4.hpp"
#include "vl/mat4.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"
#include "vl/enums.hpp"
#include <typeinfo>
#include <vector>

namespace vl
{

  class GPUBuffer: public Object
  {
  public:
    GPUBuffer() {}
    virtual ~GPUBuffer() {}

    void setGPUBufferType(EGPUBufferType type) { mGPUBufferType = type; }
    EGPUBufferType gpuBufferType() const { return mGPUBufferType; }

    virtual bool isElementType(const std::type_info& type) const = 0;

    virtual std::string elementTypeString() const = 0;

    virtual int glSize() const = 0;
    virtual int glType() const = 0;
    virtual int components() const = 0;

    virtual void createGPUBuffer(int entry_count, EGPUBufferUsage usage = BUF_STATIC_DRAW) = 0;

    virtual void createGPUBufferFromLocalBuffer(EGPUBufferUsage usage = BUF_STATIC_DRAW) = 0;

    virtual void createLocalBufferFromGPUBuffer() = 0;

    virtual void clearGPUBuffer() = 0;

    virtual void updateGPUBuffer(void* data, int gpu_byte_offset=0, int byte_count=-1) = 0;

    virtual void updateGPUBufferFromLocalBuffer(int local_byte_offset=0, int gpu_byte_offset=0, int byte_count=-1) = 0;

    virtual void scheduleGPUBufferUpdate(EGPUBufferUsage usage, int local_byte_offset=0, int gpu_byte_offset=0, int byte_count=-1) = 0;

    virtual bool isGPUBufferUpdateScheduled() = 0;

    virtual void execScheduledGPUBufferUpdate() = 0;

    virtual void unlockGPUBuffer() = 0;

    virtual GLuint handle() const = 0;

    virtual int gpuBufferBytes() const  = 0;

    virtual int gpuBufferObjects() const = 0;

    virtual int gpuBufferEntries() const  = 0;

    virtual bool gpuBufferEmpty() const  = 0;

    virtual int localBufferBytes() const  = 0;

    virtual int localBufferObjects() const = 0;

    virtual int localBufferEntries() const  = 0;

    virtual bool localAndGPUBufferSizesAreEqual() const = 0;

    virtual void createLocalBuffer(int entry_count) = 0;

    virtual void* localBufferVoidPtr() = 0;

    virtual void clearLocalBuffer() = 0;

    virtual void resize(int entries) = 0;

    virtual void clear(bool deep_clear = false) = 0;

    virtual int size() const = 0;

    virtual bool empty() const = 0;
    
  protected:
    EGPUBufferType mGPUBufferType;
  private:
    GPUBuffer(const GPUBuffer& other): Object(other) {}
    void operator=(const GPUBuffer&) {}
  };

  template< EGPUBufferType buffer_type, unsigned int data_components, typename data_type >
  class GPUBufferTemplate: public GPUBuffer
  {
  public:
    GPUBufferTemplate(): mHandle(0), mGPUBytesUsed(0), mGPUBufferLocked(0), mGPUBufferLockedPointer(0), mScheduledGPUBufferOffset(-1), mScheduledLocalOffset(-1), mScheduledByteCount(-1), mScheduledGPUBufferUsage(BUF_STATIC_DRAW) 
    { 
      CHECK(data_components) 
      mGPUBufferType = buffer_type;
    }
    ~GPUBufferTemplate() { clearGPUBuffer(); }

    virtual bool isElementType(const std::type_info& type) const { return typeid(data_type) == type; }
    std::type_info elementType() const { return typeid(data_type); }
    virtual std::string elementTypeString() const { return typeid(data_type).name(); }

    int glSize() const
    {
      if ( typeid(data_type) == typeid(GLfloat) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLdouble) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLint) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLuint) )
        return data_components;
      if ( typeid(data_type) == typeid(GLshort) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLushort) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLbyte) )
        return data_components; 
      if ( typeid(data_type) == typeid(GLubyte) )
        return data_components; 

      if ( typeid(data_type) == typeid(vec2) )
        return 2; 
      if ( typeid(data_type) == typeid(vec3) )
        return 3; 
      if ( typeid(data_type) == typeid(vec4) )
        return 4; 
      if ( typeid(data_type) == typeid(mat2) )
        return 2; 
      if ( typeid(data_type) == typeid(mat3) )
        return 3; 
      if ( typeid(data_type) == typeid(mat4) )
        return 4; 
      if ( typeid(data_type) == typeid(ivec2) )
        return 2; 
      if ( typeid(data_type) == typeid(ivec3) )
        return 3; 
      if ( typeid(data_type) == typeid(ivec4) )
        return 4; 
      else
      {

        CHECK(0);
        return 0;
      }
    }

    int glType() const
    {
      if ( typeid(data_type) == typeid(GLfloat) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(GLdouble) )
        return GL_DOUBLE; 
      if ( typeid(data_type) == typeid(GLint) )
        return GL_INT; 
      if ( typeid(data_type) == typeid(GLuint) )
        return GL_UNSIGNED_INT;
      if ( typeid(data_type) == typeid(GLshort) )
        return GL_SHORT; 
      if ( typeid(data_type) == typeid(GLushort) )
        return GL_UNSIGNED_SHORT; 
      if ( typeid(data_type) == typeid(GLbyte) )
        return GL_BYTE; 
      if ( typeid(data_type) == typeid(GLubyte) )
        return GL_UNSIGNED_BYTE; 

      if ( typeid(data_type) == typeid(vec2) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(vec3) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(vec4) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(mat2) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(mat3) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(mat4) )
        return GL_FLOAT; 
      if ( typeid(data_type) == typeid(ivec2) )
        return GL_INT; 
      if ( typeid(data_type) == typeid(ivec3) )
        return GL_INT; 
      if ( typeid(data_type) == typeid(ivec4) )
        return GL_INT; 
      else
      {

        CHECK(0);
        return 0;
      }
    }

    int components() const { return data_components; }
    
    void createGPUBuffer(int entry_count, EGPUBufferUsage usage = BUF_STATIC_DRAW)
    { 
      CHECK( GLEW_ARB_vertex_buffer_object )
      CHECK(entry_count>0);
      clearGPUBuffer(); 
      glGenBuffers(1, &mHandle);
      glBindBuffer(mGPUBufferType, mHandle);
      int size = sizeof(data_type)*data_components*entry_count;
      glBufferData(mGPUBufferType, size, NULL, usage);
      glBindBuffer(mGPUBufferType, 0);
      mGPUBytesUsed = size;
    }

    void createGPUBufferFromLocalBuffer(EGPUBufferUsage usage = BUF_STATIC_DRAW) 
    { 
      CHECK( GLEW_ARB_vertex_buffer_object )

      if (mLocalBuffer.empty())
      {
        if (mHandle)
        {
          CHECK( GLEW_ARB_vertex_buffer_object )
          glDeleteBuffers(1, &mHandle);
          mHandle = 0;
          mGPUBytesUsed = 0;
        }
      }
      else
      {
        if (mHandle == 0)
          glGenBuffers(1, &mHandle);
        glBindBuffer(mGPUBufferType, mHandle);
        mGPUBytesUsed = (int)mLocalBuffer.size()*sizeof(mLocalBuffer[0]);
        glBufferData(mGPUBufferType, mGPUBytesUsed, &mLocalBuffer[0], usage);
        glBindBuffer(mGPUBufferType, 0);
      }
    }

    void createLocalBufferFromGPUBuffer()
    {
      CHECK( GLEW_ARB_vertex_buffer_object )

      createLocalBuffer( gpuBufferEntries() );
      CHECK(localBufferPtr())
      data_type* gpu_ptr = lockGPUBuffer(BUF_READ_ONLY);
      CHECK(gpu_ptr)
      memcpy(localBufferPtr(), gpu_ptr, gpuBufferBytes());
      unlockGPUBuffer();
    }

    void clearGPUBuffer()
    {
      if (mHandle)
      {
        CHECK( GLEW_ARB_vertex_buffer_object )
        glDeleteBuffers(1, &mHandle);
        mHandle = 0;
        mGPUBytesUsed = 0;
      }
    }

    void updateGPUBuffer(void* data, int gpu_byte_offset=0, int byte_count=-1)
    {
      CHECK( GLEW_ARB_vertex_buffer_object )
      if (byte_count == 0)
        return;
      if (byte_count < 0)
        byte_count = gpuBufferBytes();
      CHECK(gpu_byte_offset>=0);
      CHECK(data)
      CHECK(mGPUBufferLocked == 0)
      CHECK(mHandle != 0 && mGPUBytesUsed != 0)
      CHECK(mGPUBytesUsed >= gpu_byte_offset + byte_count)

      glBindBuffer(mGPUBufferType, mHandle);
      glBufferSubData(mGPUBufferType, gpu_byte_offset, byte_count, data);
      glBindBuffer(mGPUBufferType, 0);
    }

    void updateGPUBufferFromLocalBuffer(int local_byte_offset=0, int gpu_byte_offset=0, int byte_count=-1)
    {
      CHECK( GLEW_ARB_vertex_buffer_object )
      CHECK(mHandle);
      CHECK(local_byte_offset >= 0)
      CHECK(gpu_byte_offset   >= 0)
      if (byte_count<0)
        byte_count = gpuBufferBytes();
      CHECK(gpuBufferBytes()         >= gpu_byte_offset   + byte_count);
      CHECK(localBufferBytes() >= local_byte_offset + byte_count);

      updateGPUBuffer( (unsigned char*)localBufferPtr() + local_byte_offset, gpu_byte_offset, byte_count );
    }

    void scheduleGPUBufferUpdate(EGPUBufferUsage usage, int local_byte_offset=0, int gpu_byte_offset=0, int byte_count=-1)
    {
      CHECK( GLEW_ARB_vertex_buffer_object )

      mScheduledGPUBufferUsage    = usage;
      mScheduledGPUBufferOffset   = gpu_byte_offset;
      mScheduledLocalOffset = local_byte_offset;
      mScheduledByteCount   = byte_count;
    }

    bool isGPUBufferUpdateScheduled()
    {
      CHECK( GLEW_ARB_vertex_buffer_object )

      return !(mScheduledLocalOffset == -1 || mScheduledGPUBufferOffset == -1 || mScheduledByteCount == 0);
    }

    void execScheduledGPUBufferUpdate()
    {
      CHECK( GLEW_ARB_vertex_buffer_object )

      if ( isGPUBufferUpdateScheduled() )
      {
        if (mHandle == 0)
          createGPUBuffer(localBufferEntries(), mScheduledGPUBufferUsage);

        updateGPUBufferFromLocalBuffer(mScheduledLocalOffset, mScheduledGPUBufferOffset, mScheduledByteCount);

        mScheduledLocalOffset = -1;
        mScheduledGPUBufferOffset   = -1;
        mScheduledByteCount   =  0;
      }
    }

    data_type* lockGPUBuffer(EGPUBufferAccess access) 
    { 
      CHECK( GLEW_ARB_vertex_buffer_object )
      CHECK(mGPUBufferLocked >= 0)
      CHECK(mGPUBufferLocked < 50)
      CHECK(mHandle != 0 && mGPUBytesUsed != 0)

      if (mHandle == 0 || mGPUBytesUsed == 0)
        return NULL;

      mGPUBufferLocked++;
      if (mGPUBufferLocked == 1)
      {
        glBindBuffer(mGPUBufferType, mHandle);
        mGPUBufferLockedPointer = (data_type*)glMapBuffer(mGPUBufferType, access);
        CHECK(mGPUBufferLockedPointer)
      }
      return mGPUBufferLockedPointer;
    }

    void unlockGPUBuffer()
    {
      CHECK( GLEW_ARB_vertex_buffer_object )
      CHECK(mHandle != 0 && mGPUBytesUsed != 0)
      CHECK(mGPUBufferLocked >= 0)
      mGPUBufferLocked--;
      if (mGPUBufferLocked == 0)
      {
        glBindBuffer(mGPUBufferType, mHandle);
        GLboolean ok = glUnmapBuffer(mGPUBufferType); 
        if (!ok)
          Log::bug( Say("glUnmapBuffer() failed!\nMaybe the right OpenGL context wasn't active or the buffer wasn't mapped?\n") );
        CHECK(ok); 
        mGPUBufferLockedPointer = 0;
      }
    }

    GLuint handle() const { return mHandle; }

    int gpuBufferBytes() const { return mGPUBytesUsed; }

    int gpuBufferObjects() const { return mGPUBytesUsed / (int)sizeof(data_type); }

    int gpuBufferEntries() const { return mGPUBytesUsed / (int)sizeof(data_type) / data_components; }

    bool gpuBufferEmpty() const { return mGPUBytesUsed == 0; }

    int localBufferBytes() const { return (int)mLocalBuffer.size()*sizeof(data_type); }

    int localBufferObjects() const { return (int)mLocalBuffer.size(); }

    int localBufferEntries() const { return (int)mLocalBuffer.size() / data_components; }

    bool localAndGPUBufferSizesAreEqual() const
    {
      return localBufferObjects() == gpuBufferObjects();
    }

    void* localBufferVoidPtr() 
    { 
      return localBufferPtr(); 
    }

    void createLocalBuffer(int entry_count) 
    { 
      CHECK(entry_count);
      clearLocalBuffer();
      mLocalBuffer.resize(entry_count*data_components);
    }

    data_type* localBufferPtr()
    {
      if (mLocalBuffer.empty())
        return NULL;
      return &mLocalBuffer[0];
    }

    void clearLocalBuffer()
    {
      std::vector<data_type> empty;
      mLocalBuffer.swap( empty ); 
    }

    void resize(int entry_count)
    {

      mLocalBuffer.resize(entry_count*data_components);
    }

    void reserve(int entry_count)
    {

      mLocalBuffer.resize(entry_count*data_components);
    }

    void clear(bool deep_clear = false)
    {
      if (deep_clear)
        clearLocalBuffer();
      else
        mLocalBuffer.clear();
    }

    int size() const
    {
      return (int)mLocalBuffer.size();
    }

    bool empty() const
    {
      return mLocalBuffer.empty();
    }

    void push_back(const data_type& data)
    {
      mLocalBuffer.push_back(data);
    }

    data_type& operator[](int i)
    {
      CHECK(i<(int)mLocalBuffer.size())
      return mLocalBuffer[i];
    }

    const data_type& operator[](int i) const
    {
      CHECK(i<(int)mLocalBuffer.size())
      return mLocalBuffer[i];
    }

    data_type& object(int i)
    {
      CHECK(i<(int)mLocalBuffer.size())
      return mLocalBuffer[i];
    }

    const data_type& object(int i) const
    {
      CHECK(i<(int)mLocalBuffer.size())
      return mLocalBuffer[i];
    }

  private:
    GPUBufferTemplate(const GPUBufferTemplate&) {}
    void operator=(const GPUBufferTemplate&) {}

  protected:
    std::string mElementTypeString;
    std::vector<data_type> mLocalBuffer;
    GLuint mHandle;
    int mGPUBytesUsed;
    int mGPUBufferLocked;
    data_type* mGPUBufferLockedPointer;
    int mScheduledGPUBufferOffset;
    int mScheduledLocalOffset;
    int mScheduledByteCount;
    EGPUBufferUsage mScheduledGPUBufferUsage;
  };

  template< unsigned int data_components, typename data_type >
  class GPUArrayBuffer: public GPUBufferTemplate<BUF_ARRAY_BUFFER, data_components, data_type>
  {
  public:
  };

}

#endif
