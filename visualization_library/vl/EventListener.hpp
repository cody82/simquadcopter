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

#ifndef EventListener_INCLUDE_DEFINE
#define EventListener_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/ivec2.hpp"
#include "vl/ivec4.hpp"

namespace vl
{
  typedef enum
  {
    Key_None = 0,

    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    Key_Return,
    Key_BackSpace,
    Key_Tab,
    Key_Space,

    Key_Clear,
    Key_Escape,
    Key_Exclam,
    Key_QuoteDbl,
    Key_Hash,
    Key_Dollar,
    Key_Ampersand,
    Key_Quote,
    Key_LeftParen,
    Key_RightParen,
    Key_Asterisk,
    Key_Plus,
    Key_Comma,
    Key_Minus,
    Key_Period,
    Key_Slash,
    Key_Colon,
    Key_Semicolon,
    Key_Less,
    Key_Equal,
    Key_Greater,
    Key_Question,
    Key_At,
    Key_LeftBracket,
    Key_BackSlash,
    Key_RightBracket,
    Key_Caret,
    Key_Underscore,
    Key_QuoteLeft,

    Key_Ctrl,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_Alt,
    Key_LeftAlt,
    Key_RightAlt,
    Key_Shift,
    Key_LeftShift,
    Key_RightShift,
    Key_Insert,
    Key_Delete,
    Key_Home,
    Key_End,
    Key_Print,
    Key_Pause,
    Key_PageUp,
    Key_PageDown,
    Key_ArrowLeft,
    Key_ArrowRight,
    Key_ArrowUp,
    Key_ArrowDown,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,

    Key_Unknown,

    Key_NumberOfKeys

  } EKey;

  typedef enum
  {
    NoButton = 0,

    LeftButton   = 1,
    RightButton  = 2,
    MiddleButton = 4,

    UnknownButton,
  } EMouseButton;

  class OpenGLContextInfo
  {
  public:
    OpenGLContextInfo():
      mRGBABits ( ivec4(0,0,0,0) ),
      mAccumRGBABits ( ivec4(0,0,0,0) ),
      mHasDoubleBuffer ( 0 ),
      mZBufferBits ( 0 ),
      mStencilBufferBits  ( 0 ),
      mHasMultisample  ( 0 ),
      mMultisampleSamples  ( 0 ),
      mStereo ( false ),
      mFullscreen ( false ),
      mVSync ( false ) {}

    void setRGBABits(const ivec4& rgba) { mRGBABits = rgba; }
    void setAccumRGBABits(const ivec4& rgba) { mAccumRGBABits = rgba; }
    void setHasDoubleBuffer(bool double_buffer_on) { mHasDoubleBuffer = double_buffer_on; }
    void setZBufferBits(int bits) { mZBufferBits = bits; }
    void setStencilBufferBits(int bits) { mStencilBufferBits = bits; }
    void setHasMultisample(bool multisample_on) { mHasMultisample = multisample_on; }
    void setMultisampleSamples(int samples) { mMultisampleSamples = samples; }
    void setStereo(bool stereo_on) { mStereo = stereo_on; }
    void setFullscreen(bool fullscreent) { mFullscreen = fullscreent; }
    void setVSync(bool vsync_on) { mVSync = vsync_on; }

    const ivec4& rgbaBits() const { return mRGBABits; }
    const ivec4& accumRGBABits() const { return mAccumRGBABits; }
    bool hasDoubleBuffer() const { return mHasDoubleBuffer; }
    int zBufferBits() const { return mZBufferBits; }
    int stencilBufferBits() const { return mStencilBufferBits; }
    bool hasMultisample() const { return mHasMultisample; }
    int multisampleSamples() const { return mMultisampleSamples; }
    bool stereo() const { return mStereo; }
    bool fullscreen() const { return mFullscreen; }
    bool vSync() const { return mVSync; }

  protected:
    ivec4 mRGBABits;
    ivec4 mAccumRGBABits;
    bool mHasDoubleBuffer;
    int mZBufferBits;
    int mStencilBufferBits;
    bool mHasMultisample;
    int mMultisampleSamples;
    bool mStereo;
    bool mFullscreen;
    bool mVSync;
  };
  class OpenGLWidget;

  class EventListener: public Object
  {
  friend class OpenGLWidget;
  public:
    EventListener(): mOpenGLWidget(NULL), mRank(0), mEnabled(true) {}
    virtual ~EventListener() {}

    virtual void init() {}
    virtual void update() {}
    virtual void quit() {}
    virtual void setWindowTitle(const std::string&) {}
    void setPosition(int /*x*/, int /*y*/) {}
    void setSize(int /*w*/, int /*h*/) {}
    ivec2 position() const { return ivec2(); }
    ivec2 size() const { return ivec2(); }

    virtual void mouseMoveEvent(int /*x*/, int /*y*/) {}
    virtual void mouseUpEvent(EMouseButton /*button*/, int /*x*/, int /*y*/) {}
    virtual void mouseDownEvent(EMouseButton /*button*/, int /*x*/, int /*y*/) {}
    virtual void mouseWheelEvent(int /*n*/) {}
    virtual void keyPressEvent(unsigned int /*unicode_ch*/, EKey /*key*/) {}
    virtual void keyReleaseEvent(unsigned int /*unicode_ch*/, EKey /*key*/) {}
    virtual void resizeEvent(int /*x*/, int /*y*/) {}
    virtual void closeEvent() {}
    virtual void renderEvent() {}
    virtual void visibilityEvent(bool /*visible*/) {}

    virtual void boundToOpenGLWidget(OpenGLWidget*) {}

    void setRank(int rank);
    int rank() const;

    OpenGLWidget* openglWidget();

    void setEnabled(bool enabled) { mEnabled = enabled; }

    bool enabled() const { return mEnabled; }

  protected:
    OpenGLWidget* mOpenGLWidget;
    int mRank;
    bool mEnabled;
  };

  class EventForwarder: public EventListener
  {
  public:
    EventForwarder(EventListener* el=NULL): mEventListener(el) {}

    virtual void mouseMoveEvent(int x, int y) { if (mEventListener) mEventListener->mouseMoveEvent(x, y); }
    virtual void mouseUpEvent(EMouseButton button, int x, int y) { if (mEventListener) mEventListener->mouseUpEvent(button, x, y); }
    virtual void mouseDownEvent(EMouseButton button, int x, int y) { if (mEventListener) mEventListener->mouseDownEvent(button, x, y); }
    virtual void mouseWheelEvent(int n) { if (mEventListener) mEventListener->mouseWheelEvent(n); }
    virtual void keyPressEvent(unsigned int unicode_ch, EKey key) { if (mEventListener) mEventListener->keyPressEvent(unicode_ch, key); }
    virtual void keyReleaseEvent(unsigned int unicode_ch, EKey key) { if (mEventListener) mEventListener->keyReleaseEvent(unicode_ch, key); }
    virtual void resizeEvent(int x, int y) { if (mEventListener) mEventListener->resizeEvent(x, y); }
    virtual void closeEvent() { if (mEventListener) mEventListener->closeEvent(); }
    virtual void renderEvent() { if (mEventListener) mEventListener->renderEvent(); }
    virtual void visibilityEvent(bool visible) { if (mEventListener) mEventListener->visibilityEvent(visible); }

    EventListener* eventListener() const { return mEventListener; }
    void setEventListener(EventListener* el) { mEventListener = el; }

  protected:
    EventListener* mEventListener;
  };
}

#endif
