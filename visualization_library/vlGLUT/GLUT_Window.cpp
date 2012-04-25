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

#include "vlGLUT/GLUT_Window.hpp"
#include "vlut/Program.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"

using namespace vlGLUT;

std::map< int, GLUT_Window* > GLUT_Window::mWinMap;

GLUT_Window::GLUT_Window(const std::string& title, const vl::OpenGLContextInfo& info, int x, int y, int width, int height)
{
  setOpenGLWidgetInfo(info);

  int flags = GLUT_RGB;
  if (info.rgbaBits().r()) flags |= GLUT_ALPHA;
  if (info.accumRGBABits().r()|info.accumRGBABits().g()|info.accumRGBABits().b()|info.accumRGBABits().a()) flags |= GLUT_ACCUM;
  if (info.hasDoubleBuffer()) flags |= GLUT_DOUBLE;
  if (info.zBufferBits()) flags |= GLUT_DEPTH;
  if (info.stencilBufferBits()) flags |= GLUT_STENCIL;
  if (info.hasMultisample()) flags |= GLUT_MULTISAMPLE;
  if (info.stereo()) flags |= GLUT_STEREO;

  glutInitDisplayMode ( flags ) ;
  glutInitWindowSize ( width, height ) ;
  glutInitWindowPosition( x, y );
  mHandle = glutCreateWindow ( title.c_str() ) ;
  if (info.fullscreen())
    glutFullScreen();

  mWidth = width;
  mHeight = height;

  setVSyncEnabled(info.vSync());

  glutSetIconTitle(title.c_str());
  mWinMap[handle()] = this;

  FILE*fout = fopen(::vl::logfile_name.c_str(),"at");
  vl::print_system_info(fout);
  fclose(fout);

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  setName(title);
  initKeymap();

  glutDisplayFunc( glut_display_func );
  glutReshapeFunc( glut_reshape_func );
  glutKeyboardFunc( glut_keyboard_func );
  glutKeyboardUpFunc( glut_keyboard_up_func );
  glutMouseFunc( glut_mouse_func );
  glutMotionFunc( glut_motion_func );
  glutPassiveMotionFunc( glut_passive_motion_func );
  glutVisibilityFunc( glut_visibility_func );
  glutSpecialFunc( glut_special_func );
  glutSpecialUpFunc( glut_special_up_func );
  glutMouseWheelFunc( glut_mouse_wheel_func );

  mGlewOK = true;
  glutSetWindow( handle() );
  glutReshapeWindow(width, height);
  vl::init_glew();

  glutWMCloseFunc( glut_wmclose_func );
  glutCloseFunc( glut_close_func );

}

void GLUT_Window::setMouseVisible(bool visible)
{
  mMouseVisible = visible;
  if (visible)
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
  else
    glutSetCursor(GLUT_CURSOR_NONE);
}

void GLUT_Window::setMousePosition(int x, int y)
{
  glutWarpPointer(x,y);
}

void GLUT_Window::update()
{
  glutPostWindowRedisplay( handle() );
}

void GLUT_Window::quit()
{

  if ( handle() )
  {
    glutDestroyWindow( handle() );
    mHandle = 0;
    mKeymap.clear();
  }
}

void GLUT_Window::updateOverlay()
{
  glutPostOverlayRedisplay();
}

void GLUT_Window::swapBuffers()
{
  glutSwapBuffers();
}

void GLUT_Window::setWindowTitle(const std::string& title)
{
  int prev = glutGetWindow();
  glutSetWindow( handle() );
  glutSetWindowTitle( title.c_str() );
  glutSetWindow( prev );
}

void GLUT_Window::setPosition(int x, int y)
{
  int prev = glutGetWindow();
  glutSetWindow( handle() );
  glutPositionWindow(x, y);
  glutSetWindow( prev );
}

void GLUT_Window::setSize(int w, int h)
{
  int prev = glutGetWindow();
  glutSetWindow( handle() );
  glutReshapeWindow(w, h);
  glutSetWindow( prev );
}

vl::ivec2 GLUT_Window::position() const
{
  int prev = glutGetWindow();
  glutSetWindow( handle() );
  vl::ivec2 v( glutGet(GLUT_WINDOW_X), glutGet(GLUT_WINDOW_Y) );
  glutSetWindow( prev );
  return v;
}

vl::ivec2 GLUT_Window::size() const
{
  int prev = glutGetWindow();
  glutSetWindow( handle() );
  vl::ivec2 v( glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) );
  glutSetWindow( prev );
  return v;
}

void GLUT_Window::initKeymap()
{
  mKeymap.clear();

  mKeymap[27]  = vl::Key_Escape;
  mKeymap[127] = vl::Key_Delete;
  mKeymap[8] = vl::Key_BackSpace;
  mKeymap[13] = vl::Key_Return;

  mKeymap[' '] = vl::Key_Space;
  mKeymap['`'] = vl::Key_QuoteLeft;
  mKeymap['-'] = vl::Key_Minus;
  mKeymap['='] = vl::Key_Equal;
  mKeymap['['] = vl::Key_LeftBracket;
  mKeymap[']'] = vl::Key_RightBracket;
  mKeymap[';'] = vl::Key_Semicolon;
  mKeymap['\''] = vl::Key_Quote;
  mKeymap['\\'] = vl::Key_BackSlash;
  mKeymap[','] = vl::Key_Comma;
  mKeymap['.'] = vl::Key_Period;
  mKeymap['/'] = vl::Key_Slash;
  mKeymap['\t'] = vl::Key_Tab;
  mKeymap['!'] = vl::Key_Exclam;
  mKeymap['"'] = vl::Key_QuoteDbl;
  mKeymap['#'] = vl::Key_Hash;
  mKeymap['$'] = vl::Key_Dollar;
  mKeymap['&'] = vl::Key_Ampersand;
  mKeymap['('] = vl::Key_LeftParen;
  mKeymap[')'] = vl::Key_RightParen;
  mKeymap['*'] = vl::Key_Asterisk;
  mKeymap['+'] = vl::Key_Plus;
  mKeymap[':'] = vl::Key_Colon;
  mKeymap['<'] = vl::Key_Less;
  mKeymap['>'] = vl::Key_Greater;
  mKeymap['?'] = vl::Key_Question;
  mKeymap['@'] = vl::Key_At;
  mKeymap['|'] = vl::Key_Caret;
  mKeymap['_'] = vl::Key_Underscore;

  mKeymap['q'] = vl::Key_Q;
  mKeymap['w'] = vl::Key_W;
  mKeymap['e'] = vl::Key_E;
  mKeymap['r'] = vl::Key_R;
  mKeymap['t'] = vl::Key_T;
  mKeymap['y'] = vl::Key_Y;
  mKeymap['u'] = vl::Key_U;
  mKeymap['i'] = vl::Key_I;
  mKeymap['o'] = vl::Key_O;
  mKeymap['p'] = vl::Key_P;
  mKeymap['a'] = vl::Key_A;
  mKeymap['s'] = vl::Key_S;
  mKeymap['d'] = vl::Key_D;
  mKeymap['f'] = vl::Key_F;
  mKeymap['g'] = vl::Key_G;
  mKeymap['h'] = vl::Key_H;
  mKeymap['j'] = vl::Key_J;
  mKeymap['k'] = vl::Key_K;
  mKeymap['l'] = vl::Key_L;
  mKeymap['z'] = vl::Key_Z;
  mKeymap['x'] = vl::Key_X;
  mKeymap['c'] = vl::Key_C;
  mKeymap['v'] = vl::Key_V;
  mKeymap['b'] = vl::Key_B;
  mKeymap['n'] = vl::Key_N;
  mKeymap['m'] = vl::Key_M;

  mKeymap['Q'] = vl::Key_Q;
  mKeymap['W'] = vl::Key_W;
  mKeymap['E'] = vl::Key_E;
  mKeymap['R'] = vl::Key_R;
  mKeymap['T'] = vl::Key_T;
  mKeymap['Y'] = vl::Key_Y;
  mKeymap['U'] = vl::Key_U;
  mKeymap['I'] = vl::Key_I;
  mKeymap['O'] = vl::Key_O;
  mKeymap['P'] = vl::Key_P;
  mKeymap['A'] = vl::Key_A;
  mKeymap['S'] = vl::Key_S;
  mKeymap['D'] = vl::Key_D;
  mKeymap['F'] = vl::Key_F;
  mKeymap['G'] = vl::Key_G;
  mKeymap['H'] = vl::Key_H;
  mKeymap['J'] = vl::Key_J;
  mKeymap['K'] = vl::Key_K;
  mKeymap['L'] = vl::Key_L;
  mKeymap['Z'] = vl::Key_Z;
  mKeymap['X'] = vl::Key_X;
  mKeymap['C'] = vl::Key_C;
  mKeymap['V'] = vl::Key_V;
  mKeymap['B'] = vl::Key_B;
  mKeymap['N'] = vl::Key_N;
  mKeymap['M'] = vl::Key_M;

  mKeymap['0'] = vl::Key_0;
  mKeymap['1'] = vl::Key_1;
  mKeymap['2'] = vl::Key_2;
  mKeymap['3'] = vl::Key_3;
  mKeymap['4'] = vl::Key_4;
  mKeymap['5'] = vl::Key_5;
  mKeymap['6'] = vl::Key_6;
  mKeymap['7'] = vl::Key_7;
  mKeymap['8'] = vl::Key_8;
  mKeymap['9'] = vl::Key_9;
}

vl::EKey GLUT_Window::mapAsciiKey(unsigned char ascii)
{
  vl::EKey key;
  if(mKeymap.find(ascii) == mKeymap.end())
    key = vl::Key_Unknown;
  else
    key = mKeymap[ascii];

  return key;
}

vl::EKey GLUT_Window::mapSpecialKey(int special_key)
{
  vl::EKey vl_key = vl::Key_Unknown;
  switch(special_key)
  {
    case GLUT_KEY_F1: vl_key = vl::Key_F1; break;
    case GLUT_KEY_F2: vl_key = vl::Key_F2; break;
    case GLUT_KEY_F3: vl_key = vl::Key_F3; break;
    case GLUT_KEY_F4: vl_key = vl::Key_F4; break;
    case GLUT_KEY_F5: vl_key = vl::Key_F5; break;
    case GLUT_KEY_F6: vl_key = vl::Key_F6; break;
    case GLUT_KEY_F7: vl_key = vl::Key_F7; break;
    case GLUT_KEY_F8: vl_key = vl::Key_F8; break;
    case GLUT_KEY_F9: vl_key = vl::Key_F9; break;
    case GLUT_KEY_F10: vl_key = vl::Key_F10; break;
    case GLUT_KEY_F11: vl_key = vl::Key_F11; break;
    case GLUT_KEY_F12: vl_key = vl::Key_F12; break;
    case GLUT_KEY_LEFT: vl_key = vl::Key_ArrowLeft; break;
    case GLUT_KEY_UP: vl_key = vl::Key_ArrowUp; break;
    case GLUT_KEY_RIGHT: vl_key = vl::Key_ArrowRight; break;
    case GLUT_KEY_DOWN: vl_key = vl::Key_ArrowDown; break;
    case GLUT_KEY_PAGE_UP: vl_key = vl::Key_PageUp; break;
    case GLUT_KEY_PAGE_DOWN: vl_key = vl::Key_PageDown; break;
    case GLUT_KEY_HOME: vl_key = vl::Key_Home; break;
    case GLUT_KEY_END: vl_key = vl::Key_End; break;
    case GLUT_KEY_INSERT: vl_key = vl::Key_Insert; break;
    default:
      vl_key = vl::Key_Unknown;
  }
  return vl_key;
}

void GLUT_Window::updateModifiers()
{
  mKeyboard.erase(vl::Key_Shift);
  mKeyboard.erase(vl::Key_Alt);
  mKeyboard.erase(vl::Key_Ctrl);
  int modifiers = glutGetModifiers();
  if (modifiers & GLUT_ACTIVE_SHIFT)
    mKeyboard.insert(vl::Key_Shift);
  if (modifiers & GLUT_ACTIVE_CTRL)
    mKeyboard.insert(vl::Key_Ctrl);
  if (modifiers & GLUT_ACTIVE_ALT)
    mKeyboard.insert(vl::Key_Alt);
}

void GLUT_Window::glut_keyboard_func(unsigned char ch, int, int)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();
  win->mKeyboard.insert( win->mapAsciiKey(ch) );

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->keyPressEvent(ch, win->mapAsciiKey(ch) );
}

void GLUT_Window::glut_keyboard_up_func(unsigned char ch, int, int)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();
  win->mKeyboard.erase( win->mapAsciiKey(ch) );

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
  {
    if (it->get()->enabled())
      it->get()->keyReleaseEvent(ch, win->mapAsciiKey(ch) );
  }
}

void GLUT_Window::glut_special_func(int key, int, int)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();
  win->mKeyboard.insert( mapSpecialKey(key) );

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->keyPressEvent(0, mapSpecialKey(key) );
}

void GLUT_Window::glut_special_up_func(int key, int, int)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();
  win->mKeyboard.erase( mapSpecialKey(key) );

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->keyReleaseEvent(0, mapSpecialKey(key) );
}

void GLUT_Window::glut_mouse_func(int button, int state, int x, int y)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();

  vl::EMouseButton btn;
  if (button == GLUT_LEFT_BUTTON) btn = vl::LeftButton; else
  if (button == GLUT_MIDDLE_BUTTON) btn = vl::MiddleButton; else
  if (button == GLUT_RIGHT_BUTTON) btn = vl::RightButton; else
  btn = vl::UnknownButton;

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
  {
    if (!it->get()->enabled())
      continue;

    if (state == GLUT_DOWN)
      it->get()->mouseDownEvent(btn, x, y);
    else
    if (state == GLUT_UP)
      it->get()->mouseUpEvent(btn, x, y);
  }
}

void GLUT_Window::glut_motion_func(int x, int y)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  if (win->mIgnoreNextMouseMoveEvent)
  {
    win->mIgnoreNextMouseMoveEvent = false;
    return;
  }

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->mouseMoveEvent(x, y);
}

void GLUT_Window::glut_passive_motion_func(int x, int y)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  if (win->mIgnoreNextMouseMoveEvent)
  {
    win->mIgnoreNextMouseMoveEvent = false;
    return;
  }

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->mouseMoveEvent(x, y);
}

void GLUT_Window::glut_mouse_wheel_func(int, int rotation, int, int)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);
  win->updateModifiers();

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->mouseWheelEvent(rotation);
}

void GLUT_Window::glut_visibility_func(int visibility)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->visibilityEvent(visibility == GLUT_VISIBLE);
}

void GLUT_Window::glut_reshape_func(int w, int h)
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);

  if (win->mGlewOK == false)
  {
    win->mGlewOK = true;
    vl::init_glew();
  }

  win->mWidth = w;
  win->mHeight = h;

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->resizeEvent(w, h);
}

void GLUT_Window::glut_display_func()
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->renderEvent();

  if (win->continuousUpdate())
    win->update();
}

void GLUT_Window::glut_close_func()
{
  int cur_win = glutGetWindow();
  GLUT_Window* win = mWinMap[cur_win];
  CHECK(win);

  std::vector< vl::ref<EventListener> > temp_clients = win->mEventListener;
  for( std::vector< vl::ref<EventListener> >::iterator it = temp_clients.begin(); it != temp_clients.end(); ++it )
    if (it->get()->enabled())
      it->get()->closeEvent();

  win->mHandle = 0;
  mWinMap.erase( cur_win );
}

void GLUT_Window::glut_wmclose_func()
{
  glut_close_func();
}

void vlGLUT::atexit_visualization_library_shutdown()
{

  static bool alread_called = false;
  if (alread_called)
    return;
  else
  {
    vl::visualization_library_shutdown();
    alread_called = true;
    return;
  }
}

vl::ref<vlGLUT::GLUT_Window> vlGLUT::open_GLUT_Window(const std::string& title, vlut::Program* program, const vl::OpenGLContextInfo& info, int x, int y, int width, int height, vl::vec4 bkcolor, vl::vec3d pos, vl::vec3d look)
{
  vl::Log::print(title + "\n\n");
  program->setTitle(title);

  vl::ref<vlGLUT::GLUT_Window> glut_window = new vlGLUT::GLUT_Window( title, info, x, y, width, height );

  GLCHECK4()

  program->pipeline()->camera()->setRenderTarget( new vl::RenderTarget( width, height ) );
  program->pipeline()->camera()->setViewport( new vl::Viewport( 0, 0, width, height) );
  program->pipeline()->camera()->viewport()->setClearColor( bkcolor );

  program->pipeline()->camera()->setProjectionMatrixPerspective();
  vl::mat4d m;
  m.setAsLookAt(pos, look, vl::vec3d(0,1,0));
  program->pipeline()->camera()->setViewMatrix( m );

  GLCHECK4()

  glut_window->addEventListerner(program);

  program->init();

  GLCHECK4()

  return glut_window;
}

