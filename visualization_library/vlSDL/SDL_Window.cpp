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

#include "vlSDL/SDL_Window.hpp"
#include "vl/OpenGL.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"
#include "vlut/Program.hpp"
#include <algorithm>
#include <SDL.h>
#include <map>

using namespace vlSDL;

namespace
{
  SDL_Window* sdl_window = NULL;
  bool update_flag = true;
  bool quit_flag = false;

  std::map<int, vl::EKey> key_translation_map;

  int key_translation_vec[] =
  {
    SDLK_0, vl::Key_0,
    SDLK_1, vl::Key_1,
    SDLK_2, vl::Key_2,
    SDLK_3, vl::Key_3,
    SDLK_4, vl::Key_4,
    SDLK_5, vl::Key_5,
    SDLK_6, vl::Key_6,
    SDLK_7, vl::Key_7,
    SDLK_8, vl::Key_8,
    SDLK_9, vl::Key_9,

    SDLK_a, vl::Key_A,
    SDLK_b, vl::Key_B,
    SDLK_c, vl::Key_C,
    SDLK_d, vl::Key_D,
    SDLK_e, vl::Key_E,
    SDLK_f, vl::Key_F,
    SDLK_g, vl::Key_G,
    SDLK_h, vl::Key_H,
    SDLK_i, vl::Key_I,
    SDLK_j, vl::Key_J,
    SDLK_k, vl::Key_K,
    SDLK_l, vl::Key_L,
    SDLK_m, vl::Key_M,
    SDLK_n, vl::Key_N,
    SDLK_o, vl::Key_O,
    SDLK_p, vl::Key_P,
    SDLK_q, vl::Key_Q,
    SDLK_r, vl::Key_R,
    SDLK_s, vl::Key_S,
    SDLK_t, vl::Key_T,
    SDLK_u, vl::Key_U,
    SDLK_v, vl::Key_V,
    SDLK_w, vl::Key_W,
    SDLK_x, vl::Key_X,
    SDLK_y, vl::Key_Y,
    SDLK_z, vl::Key_Z,

    SDLK_RETURN,       vl::Key_Return,
    SDLK_BACKSPACE,    vl::Key_BackSpace,
    SDLK_SPACE,        vl::Key_Space,
    SDLK_TAB,          vl::Key_Tab,
    SDLK_CLEAR,        vl::Key_Clear,
    SDLK_ESCAPE,       vl::Key_Escape,
    SDLK_EXCLAIM,      vl::Key_Exclam,
    SDLK_QUOTEDBL,     vl::Key_QuoteDbl,
    SDLK_HASH,         vl::Key_Hash,
    SDLK_DOLLAR,       vl::Key_Dollar,
    SDLK_AMPERSAND,    vl::Key_Ampersand,
    SDLK_QUOTE,        vl::Key_Quote,
    SDLK_LEFTPAREN,    vl::Key_LeftParen,
    SDLK_RIGHTPAREN,   vl::Key_RightParen,
    SDLK_ASTERISK,     vl::Key_Asterisk,
    SDLK_PLUS,         vl::Key_Plus,
    SDLK_COMMA,        vl::Key_Comma,
    SDLK_MINUS,        vl::Key_Minus,
    SDLK_PERIOD,       vl::Key_Period,
    SDLK_SLASH,        vl::Key_Slash,
    SDLK_COLON,        vl::Key_Colon,
    SDLK_SEMICOLON,    vl::Key_Semicolon,
    SDLK_LESS,         vl::Key_Less,
    SDLK_EQUALS,       vl::Key_Equal,
    SDLK_GREATER,      vl::Key_Greater,
    SDLK_QUESTION,     vl::Key_Question,
    SDLK_AT,           vl::Key_At,
    SDLK_LEFTBRACKET,  vl::Key_LeftBracket,
    SDLK_BACKSLASH,    vl::Key_BackSlash,
    SDLK_RIGHTBRACKET, vl::Key_RightBracket,
    SDLK_CARET,        vl::Key_Caret,
    SDLK_UNDERSCORE,   vl::Key_Underscore,
    SDLK_BACKQUOTE,    vl::Key_QuoteLeft,

    SDLK_LEFT,     vl::Key_ArrowLeft,
    SDLK_RIGHT,    vl::Key_ArrowRight,
    SDLK_UP,       vl::Key_ArrowUp,
    SDLK_DOWN,     vl::Key_ArrowDown,
    SDLK_LCTRL,    vl::Key_LeftCtrl,
    SDLK_RCTRL,    vl::Key_RightCtrl,
    SDLK_LSHIFT,   vl::Key_LeftShift,
    SDLK_RSHIFT,   vl::Key_RightShift,
    SDLK_LALT,     vl::Key_LeftAlt,
    SDLK_RALT,     vl::Key_RightAlt,
    SDLK_INSERT,   vl::Key_Insert,
    SDLK_DELETE,   vl::Key_Delete,
    SDLK_HOME,     vl::Key_Home,
    SDLK_END,      vl::Key_End,
    SDLK_PAGEUP,   vl::Key_PageUp,
    SDLK_PAGEDOWN, vl::Key_PageDown,
    SDLK_PAUSE,    vl::Key_Pause,
    SDLK_PRINT,    vl::Key_Print,
    SDLK_F1,       vl::Key_F1,
    SDLK_F2,       vl::Key_F2,
    SDLK_F3,       vl::Key_F3,
    SDLK_F4,       vl::Key_F4,
    SDLK_F5,       vl::Key_F5,
    SDLK_F6,       vl::Key_F6,
    SDLK_F7,       vl::Key_F7,
    SDLK_F8,       vl::Key_F8,
    SDLK_F9,       vl::Key_F9,
    SDLK_F10,      vl::Key_F10,
    SDLK_F11,      vl::Key_F11,
    SDLK_F12,      vl::Key_F12,
    0,0
  };
}

SDL_Surface* SDL_Window::mScreen = NULL;

SDL_Window::~SDL_Window()
{
  SDL_Quit();
  quit_flag = true;
}

SDL_Window::SDL_Window(const std::string& title, const vl::OpenGLContextInfo& info, int width, int height)
{
  if (mScreen || sdl_window)
  {
    vl::Log::error("SDL supports only one window.\n");
    CHECK(0);
    return;
  }

  sdl_window = this;

  for(int i=0; key_translation_vec[i]; i+=2)
    key_translation_map[ key_translation_vec[i] ] = (vl::EKey)key_translation_vec[i+1];

  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
      printf("Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   info.rgbaBits().r());
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, info.rgbaBits().g());
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  info.rgbaBits().b());
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, info.rgbaBits().a());

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,   info.accumRGBABits().r());
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, info.accumRGBABits().g());
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,  info.accumRGBABits().b());
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, info.accumRGBABits().a());

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, info.zBufferBits());
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, info.stencilBufferBits());

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, info.hasDoubleBuffer()?1:0);
  SDL_GL_SetAttribute(SDL_GL_STEREO, info.stereo());
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, info.hasMultisample()?1:0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, info.multisampleSamples());
  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, info.vSync());

  int bpp = SDL_GetVideoInfo()->vfmt->BitsPerPixel;
  Uint32 flags = SDL_OPENGL | (info.fullscreen() ? SDL_FULLSCREEN : 0);
  width  = width  !=0 ? width  : SDL_GetVideoInfo()->current_w;
  height = height !=0 ? height : SDL_GetVideoInfo()->current_h;
  mScreen = SDL_SetVideoMode( width, height, bpp, flags );
  if (mScreen == 0)
  {
    vl::Log::print( vl::Say("\n  error: SDL_SetVideoMode(%n, %n, %n, %X) failed: %s\n") << width << height << bpp << flags << SDL_GetError() );
    exit(1);
  }

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,  viewport);
  CHECK(viewport[0] == 0);
  CHECK(viewport[1] == 0);
  if (viewport[2] != mScreen->w || viewport[3] != mScreen->h)
  {
    vl::Log::print( vl::Say("\n  warning: OpenGL reported %nx%n as video size but SDL says %nx%n\n") << viewport[2] << viewport[3] << mScreen->w << mScreen->h );
    CHECK(0)
  }

  mWidth  = mScreen->w;
  mHeight = mScreen->h;

  vl::init_glew();

  FILE*fout = fopen(::vl::logfile_name.c_str(),"at");
  vl::print_system_info(fout);
  fclose(fout);

#ifndef NDEBUG
  vl::Log::print( vl::Say("SDL screen: %n x %n x %n %s\n") << mScreen->w << mScreen->h << mScreen->format->BitsPerPixel << (info.fullscreen() ? "fullscreen" : "windowed") );
#endif

  SDL_EnableUNICODE(1);

  SDL_ShowCursor(true);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  setWindowTitle(title);

  SDL_PumpEvents();
  SDL_Event event;
  while ( SDL_PollEvent(&event) ) {}
}

void SDL_Window::translateEvent( SDL_Event * ev )
{
  vl::EKey key = vl::Key_None;
  int unicode = 0;
  if ( ev && (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP) )
  {
    if( key_translation_map.find(ev->key.keysym.sym) != key_translation_map.end() )
      key = key_translation_map[ ev->key.keysym.sym ];
    else
      key = vl::Key_Unknown;

    unicode = ev->key.keysym.unicode;
  }

  bool update = update_flag;

  update_flag = continuousUpdate();

  std::vector< vl::ref<EventListener> > temp_clients = mEventListener;
  std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin();
  for( ; it != temp_clients.end(); ++it )
  {
    vl::ref<EventListener> client = *it;

    if (client->enabled() == false)
      continue;

    if (update)
      client->renderEvent();

    if (ev == NULL)
      continue;

    if (ev->type == SDL_KEYDOWN)
    {
      switch(key)
      {
      default: break;

      case vl::Key_LeftCtrl:
      case vl::Key_RightCtrl:
        mKeyboard.insert(vl::Key_Ctrl);
        client->keyPressEvent(unicode, vl::Key_Ctrl);
      break;

      case vl::Key_LeftShift:
      case vl::Key_RightShift:
        mKeyboard.insert(vl::Key_Shift);
        client->keyPressEvent(unicode, vl::Key_Shift);
      break;

      case vl::Key_LeftAlt:
      case vl::Key_RightAlt:
        mKeyboard.insert(vl::Key_Alt);
        client->keyPressEvent(unicode, vl::Key_Alt);
      break;
      }
      mKeyboard.insert(key);
      client->keyPressEvent(unicode, key);
    }
    else
    if (ev->type == SDL_KEYUP)
    {
      switch(key)
      {
      default: break;

      case vl::Key_LeftCtrl:
      case vl::Key_RightCtrl:
        mKeyboard.erase(vl::Key_Ctrl);
        client->keyReleaseEvent(unicode, vl::Key_Ctrl);
      break;

      case vl::Key_LeftShift:
      case vl::Key_RightShift:
        mKeyboard.erase(vl::Key_Shift);
        client->keyReleaseEvent(unicode, vl::Key_Shift);
      break;

      case vl::Key_LeftAlt:
      case vl::Key_RightAlt:
        mKeyboard.erase(vl::Key_Alt);
        client->keyReleaseEvent(unicode, vl::Key_Alt);
      break;
      }
      mKeyboard.erase(key);
      client->keyReleaseEvent(unicode, key);
    }
    else
    if (ev->type == SDL_MOUSEBUTTONDOWN)
    {
      if (ev->button.button == SDL_BUTTON_WHEELUP)
        client->mouseWheelEvent(1);
      else
      if (ev->button.button == SDL_BUTTON_WHEELDOWN)
        client->mouseWheelEvent(-1);
      else
      if (ev->button.button == SDL_BUTTON_LEFT)
        client->mouseDownEvent(vl::LeftButton, ev->button.x, ev->button.y);
      else
      if (ev->button.button == SDL_BUTTON_RIGHT)
        client->mouseDownEvent(vl::RightButton, ev->button.x, ev->button.y);
      else
      if (ev->button.button == SDL_BUTTON_MIDDLE)
        client->mouseDownEvent(vl::MiddleButton, ev->button.x, ev->button.y);
    }
    else
    if (ev->type == SDL_MOUSEBUTTONUP)
    {

      if (ev->button.button == SDL_BUTTON_LEFT)
        client->mouseUpEvent(vl::LeftButton, ev->button.x, ev->button.y);
      else
      if (ev->button.button == SDL_BUTTON_RIGHT)
        client->mouseUpEvent(vl::RightButton, ev->button.x, ev->button.y);
      else
      if (ev->button.button == SDL_BUTTON_MIDDLE)
        client->mouseUpEvent(vl::MiddleButton, ev->button.x, ev->button.y);
    }
    else
    if (ev->type == SDL_MOUSEMOTION)
    {
      if (mIgnoreNextMouseMoveEvent)
        mIgnoreNextMouseMoveEvent = false;
      else
        client->mouseMoveEvent(ev->motion.x, ev->motion.y);
    }
    else
    if (ev->type == SDL_VIDEORESIZE)
    {

    }
    else
    if (ev->type == SDL_QUIT)
    {
      client->closeEvent();
      quit();
    }
  }
}

void vlSDL::sdlMainLoop()
{
  SDL_Event ev;
  while(quit_flag == false)
  {
    if ( SDL_PollEvent(&ev) )
      sdl_window->translateEvent(&ev);
    else
    {

      if ( update_flag || sdl_window->continuousUpdate() )
        sdl_window->translateEvent(NULL);
      else
      {

        SDL_Delay(10);
      }
    }
  }
}

void SDL_Window::quit()
{
  quit_flag = true;
}

void SDL_Window::update()
{
  update_flag = true;
}

void SDL_Window::setWindowTitle(const std::string& title)
{
  SDL_WM_SetCaption(title.c_str(), title.c_str());
}

void SDL_Window::swapBuffers()
{
  SDL_GL_SwapBuffers();
}

vl::ref<vlSDL::SDL_Window> vlSDL::open_SDL_Window(const std::string& title, vlut::Program* program, const vl::OpenGLContextInfo& settings, int width, int height, vl::vec4 bkcolor, vl::vec3d pos, vl::vec3d look)
{

  vl::ref<vlut::Program> program_ref = program;

  vl::Log::print(title + "\n\n");
  program->setTitle(title);

  vl::ref<vlSDL::SDL_Window> sdl_window = new vlSDL::SDL_Window(title, settings, width, height);

  program->setRank(0);

  program->pipeline()->camera()->setRenderTarget( new vl::RenderTarget( width, height ) );
  program->pipeline()->camera()->setViewport( new vl::Viewport( 0, 0, width, height) );
  program->pipeline()->camera()->viewport()->setClearColor( bkcolor );

  program->pipeline()->camera()->setProjectionMatrixPerspective();
  vl::mat4d m; m.setAsLookAt(pos, look, vl::vec3d(0,1,0));
  program->pipeline()->camera()->setViewMatrix(m);

  sdl_window->addEventListerner( program );

  program->init();

  return sdl_window;
}

