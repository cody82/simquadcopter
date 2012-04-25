#include "vl/VisualizationLibrary.hpp"
#include "vl/OpenGLWidget.hpp"
#include "vl/GPUBuffer.hpp"
#include "vl/CHECK.hpp"
#include "vl/Renderer.hpp"
#include "vl/Geometry.hpp"
#include "vl/Camera.hpp"
#include "vl/Object.hpp"
#include "vl/ShaderNode.hpp"
#include "vl/Painter.hpp"
#include "vlut/Colors.hpp"
#include "vl/Light.hpp"
#include "vl/Image.hpp"
#include "vl/Texture.hpp"
#include "vl/quat.hpp"
#include "vl/Say.hpp"
#include "vl/Time.hpp"
#include "vl/FlyCameraManipulator.hpp"
#include "vl/TrackballManipulator.hpp"
#include "vl/GlobalState.hpp"
#include "vl/KdTree.hpp"
#include "vl/CameraReadPixels.hpp"
#include "vl/FrameBufferObject.hpp"
#include "vl/CopyTexSubImage.hpp"
#include "vl/Text.hpp"
#include "vl/DistanceLODEvaluator.hpp"
#include "vl/PixelLODEvaluator.hpp"
#include "vl/LoadMD2.hpp"
#include "vl/Load3DS.hpp"
//#include "vl/LoadPLY.hpp"
#include "vl/mat3.hpp"
#include "vl/mat2.hpp"
#include "vlut/Program.hpp"
#include "vlut/GeometricalPrimitives.hpp"
#include "vlGLUT/GLUT_Window.hpp"

//the original vl/LoadPLY.hpp sucks
#include "LoadPLY2.hpp"

#include "quadcopter.h"

#include <iostream>

SimQuadCopter::QuadCopter copter(0.51f);

class TestProgram: public vlut::Program
{
public:
  TestProgram(): mMaxTime(0.0) {}

  virtual void keyPressEvent(unsigned int, vl::EKey key)
  {
    if (key == vl::Key_F10)
    {
      _camera_read_pixels ->setup( 0, 0, pipeline()->camera()->viewport()->width(), pipeline()->camera()->viewport()->width(), vl::RDB_BACK_LEFT );
      pipeline()->camera()->addRenderFinishedCallback(_camera_read_pixels .get());
      _camera_read_pixels ->setRemoveAfterCall(true);
      std::string filename = vl::Say( title() + "-%n.tif") << (int)vl::Time::timerSeconds();
      _camera_read_pixels ->setSavePath( filename );
      vl::Log::print( vl::Say("Screenshot: '%s'\n") << filename );
    }
  }

  void init()
  {
    /* screen shot grabbing */
    _camera_read_pixels = new vl::CameraReadPixels;
    setLogFPS(true);
  }

  void setMaxTime(double time) { mMaxTime = time; }

  void renderEvent()
  {
    vlut::Program::renderEvent();

    if (mMaxTime != 0)
    {
      if ( !timer.isStarted() )
        timer.start();
      else
      if (timer.elapsed() > mMaxTime)
        openglWidget()->quit();
    }
  }

protected:
  vl::ref<vl::CameraReadPixels> _camera_read_pixels;
  vl::Time timer;
  double mMaxTime;
};

class CopterViewer_Program: public TestProgram
{
public:
  CopterViewer_Program()  {}
  virtual void shutdown() {}

  vl::mat4d getOdeBodyMatrix(dBodyID body)
  {
    dReal m[16];
    SimQuadCopter::OpenGL1::getMatrix(m,body);
    //position is multiplied by 100, so units are cm instead of meters. (ODE: meters, OpenGL: centimeters)
    vl::mat4d matrix(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12]*100,m[13]*100,m[14]*100,m[15]);
    return matrix;
  }

  virtual void keyPressEvent(unsigned int, vl::EKey key)
  {
    if (key == vl::Key_F2)
    {
      SimQuadCopter::OdeEngine::simulatePropellerRotation=!SimQuadCopter::OdeEngine::simulatePropellerRotation;
    }
  }

  vl::vec3d eye;
  vl::vec3d pos;
  virtual void run()
  {
    double now=vl::Time::timerSeconds();
    double diff=now-time;

    //do nothing if no time has passed
    if(diff<=0.00001)return;

    if(diff>0.1)diff=0.1;

    //at least 200 updates per second
    int count=(int)((diff/0.005f)+1);
    //std::cout << count << " " << diff << std::endl;
    for(int i=0;i<count;++i)
    	copter.update(diff/(float)count);
    time=now;

    //get transforms for objects from ODE
    vl::mat4d m=getOdeBodyMatrix(copter.physics->body);
    _Transform->setLocalMatrix( m );
    engineXpTransform->setLocalMatrix(getOdeBodyMatrix(copter.physics->engineXp.propeller));
    engineXmTransform->setLocalMatrix(getOdeBodyMatrix(copter.physics->engineXm.propeller));
    engineZpTransform->setLocalMatrix(getOdeBodyMatrix(copter.physics->engineZp.propeller));
    engineZmTransform->setLocalMatrix(getOdeBodyMatrix(copter.physics->engineZm.propeller));

    vl::vec3d wantedPos=m.getT();
    vl::vec3d wantedEye=wantedPos-m.getZ()*60+m.getY()*10;

    vl::vec3d dir=wantedEye-eye;
    vl::vec3d dir2=wantedPos-pos;
    eye+=dir*std::min(1.0,diff*30);
//eye=wantedEye;
//    pos+=dir2*std::min(1.0,diff*30);
pos=wantedPos;

    m.setAsLookAt(eye,pos,vl::vec3d(0,1,0));
    camFollowTransform->setLocalMatrix(m);


    wchar_t text[1024];
    float x,z,rx,rz;
    //this is what the flightcontrol thinks (it is dependend on the acceleration of the copter)
    copter.calcAnglesFromAcceleration(x,z);
    //this is the actual angle
    copter.physics->calcRealAngles(rx,rz);


    x*=180.0f/M_PI;
    z*=180.0f/M_PI;
    rx*=180.0f/M_PI;
    rz*=180.0f/M_PI;


    swprintf(text,1024,L"angle[deg]:\nx=%.02f, real: %.02f\nz=%.02f, real: %.02f\ngyro[deg/s]:\nx=%.02f\ny=%.02f\nz=%.02f\npitch: %.02f\nroll: %.02f\nyaw: %.02f\nspeed: %.01fm/s, %.01fkm/h\nair friction: %.02fN\nthrust: %.02fN\naltitude: %.02fm\nthrottle(l,r,f,b)[%]: %02d %02d %02d %02d\nRPM(l,r,f,b): %04d %04d %04d %04d\npropeller rotation: %s"
      ,x,rx,z,rz,copter.gyroX.getValue()*180.0f/M_PI,copter.gyroY.getValue()*180.0f/M_PI,
      copter.gyroZ.getValue()*180.0f/M_PI,
      copter.control.pitch*180.0f/M_PI,
      copter.control.roll*180.0f/M_PI,
      copter.control.yaw*180.0f/M_PI,
      copter.physics->getSpeed(),
      copter.physics->getSpeed()*3600.0/1000.0,
      copter.physics->currentAirFriction,
      copter.physics->getTotalThrust(),
      copter.physics->getPosition().getY(),
      (int)(copter.physics->engineXp.getThrottle()*100.0f),
      (int)(copter.physics->engineXm.getThrottle()*100.0f),
      (int)(copter.physics->engineZp.getThrottle()*100.0f),
      (int)(copter.physics->engineZm.getThrottle()*100.0f),
      (int)(copter.physics->engineXp.getRPM()),
      (int)(copter.physics->engineXm.getRPM()),
      (int)(copter.physics->engineZp.getRPM()),
      (int)(copter.physics->engineZm.getRPM()),
      SimQuadCopter::OdeEngine::simulatePropellerRotation?"ON":"OFF"
      );
    info->setText(text);
  }

  void init()
  {
    TestProgram::init();
    time=vl::Time::timerSeconds();
    copter.remote->init();

    pipeline()->camera()->setFOV( 70 );
    pipeline()->camera()->setFarPlane( 10000 );

    /* bind Transform */
    _Transform = new vl::Transform;
    pipeline()->transform()->addChild( _Transform.get() );

    engineXpTransform = new vl::Transform;
    engineXmTransform = new vl::Transform;
    engineZpTransform = new vl::Transform;
    engineZmTransform = new vl::Transform;
    floorTransform = new vl::Transform;

    if(copter.physics->mountJoint!=NULL)
      floorTransform->setLocalMatrix( vl::mat4d::translation( vl::vec3d(0,-50,0) ) );
    pipeline()->transform()->addChild( floorTransform.get() );

    
    camFollowTransform =new vl::Transform;
    camMountTransform = new vl::Transform;

    pipeline()->transform()->addChild( camFollowTransform.get() );
    _Transform->addChild( camMountTransform.get() );
    camMountTransform->setLocalMatrix( vl::mat4d::translation( vl::vec3d(0,10,0) ) *vl::mat4d::rotation( 180, 0, 1, 0 ));

    if(1)
    {
      pipeline()->camera()->followTransform(camFollowTransform.get());
    }
    else
    {
      pipeline()->camera()->followTransform(camMountTransform.get());
    }

//base painter
    /* bind Painter */
    vl::ref<vl::Painter> painter = new vl::Painter;
    pipeline()->shaderNode()->addChild(painter.get());

    /* enable depth test and lighting */
    painter->shader()->enable(vl::EN_DEPTH_TEST);
    painter->shader()->enable(vl::EN_LIGHTING);
    painter->shader()->glMaterial()->setDiffuse( vlut::white );
    //painter->shader()->glMaterial()->setAmbient( vl::vec4(1.0f,1.0f,1.0f,1.0f) );
    painter->shader()->glMaterial()->setAmbient( vl::vec4(0.5f,0.5f,0.5f,1.0f) );
    /* since no Transform is associated to the Light it will follow the camera */
    vl::ref<vl::Light> light = new vl::Light;
    painter->shader()->addLight( light.get() );
    light->followTransform(floorTransform.get());
    light->setPosition(vl::vec4(0,10000,0,1));
    light->setAmbient( vl::vec4(0.3f,0.3f,0.3f,1.0f) );
    //propellerpainter->addActor( new vl::Actor( propeller.get(), engineXpTransform.get() ) );


    vl::ref<vl::Geometry> geom = LoadPLY2::loadPLY("models/i4copter-frame.ply");
    painter->addActor( new vl::Actor( geom.get(), _Transform.get() ) );

//propeller
    vl::ref<vl::Geometry> propeller = LoadPLY2::loadPLY("models/i4copter-propeller1.ply");
    vl::ref<vl::Painter> propellerpainter = new vl::Painter;
    painter->addChild(propellerpainter.get());

    vl::ref<vl::Image> propellertex = vl::loadImage("textures/propeller.tga");
    propellerpainter->shader()->textureUnit(0)->setTexture( new vl::Texture(propellertex.get() ) );


    propellerpainter->addActor( new vl::Actor( propeller.get(), engineXpTransform.get() ) );
    pipeline()->transform()->addChild( engineXpTransform.get() );
    propellerpainter->addActor( new vl::Actor( propeller.get(), engineXmTransform.get() ) );
    pipeline()->transform()->addChild( engineXmTransform.get() );
    propellerpainter->addActor( new vl::Actor( propeller.get(), engineZpTransform.get() ) );
    pipeline()->transform()->addChild( engineZpTransform.get() );
    propellerpainter->addActor( new vl::Actor( propeller.get(), engineZmTransform.get() ) );
    pipeline()->transform()->addChild( engineZmTransform.get() );
 

//boards
    vl::ref<vl::Painter> boardpainter = new vl::Painter;
    painter->addChild(boardpainter.get());

    vl::ref<vl::Geometry> boards = LoadPLY2::loadPLY("models/i4copter-boards.ply");
    boards->transform( vl::mat4d::translation( vl::vec3d(0,3.5,0) ) );
    boardpainter->addActor( new vl::Actor( boards.get(), _Transform.get() ) );
    vl::ref<vl::Image> boardtex = vl::loadImage("textures/boards.tga");
    boardpainter->shader()->textureUnit(0)->setTexture( new vl::Texture(boardtex.get() ) );

//battery
    vl::ref<vl::Painter> batterypainter = new vl::Painter;
    painter->addChild(batterypainter.get());

    vl::ref<vl::Geometry> battery = LoadPLY2::loadPLY("models/i4copter-battery.ply");
    battery->transform( vl::mat4d::translation( vl::vec3d(0,-3,0) ) );
    batterypainter->addActor( new vl::Actor( battery.get(), _Transform.get() ) );
    vl::ref<vl::Image> kokam = vl::loadImage("textures/kokam.tga");
    batterypainter->shader()->textureUnit(0)->setTexture( new vl::Texture(kokam.get() ) );

//floor
    vl::ref<vl::Painter> floorpainter = new vl::Painter;
    painter->addChild(floorpainter.get());


    vl::ref<vl::Geometry> floor = LoadPLY2::loadPLY("models/floor.ply");
    floorpainter->addActor( new vl::Actor( floor.get(), floorTransform.get() ) );
    vl::ref<vl::Image> img1 = vl::loadImage("textures/floor2.tga");
    floorpainter->shader()->textureUnit(0)->setTexture( new vl::Texture(img1.get() ) );





    if (GLEW_ARB_shading_language_100)
    {
      vl::ref<vl::GLSLVertexShader> vert_perpixel = new vl::GLSLVertexShader("file://shaders.glsl","vert_perpixellight");
      vl::ref<vl::GLSLFragmentShader> frag_perpixel = new vl::GLSLFragmentShader("file://shaders.glsl","frag_perpixellight");
      
      vl::ref<vl::GLSLVertexShader> vert_perpixel_texture = new vl::GLSLVertexShader("file://shaders.glsl","vert_perpixellight_texture");
      vl::ref<vl::GLSLFragmentShader> frag_perpixel_texture = new vl::GLSLFragmentShader("file://shaders.glsl","frag_perpixellight_texture");

      vl::ref<vl::GLSLProgram> glsl = painter->shader()->glslProgram();
      glsl->attachShader( vert_perpixel.get() );
      glsl->attachShader( frag_perpixel.get() );

      vl::ref<vl::GLSLProgram> glsl2 = floorpainter->shader()->glslProgram();
      glsl2->attachShader( vert_perpixel_texture.get() );
      glsl2->attachShader( frag_perpixel_texture.get() );

      propellerpainter->shader()->setGLSLProgram(glsl2.get());
      batterypainter->shader()->setGLSLProgram(glsl2.get());
      boardpainter->shader()->setGLSLProgram(glsl2.get());
    }
    else
      vl::Log::error("GLEW_ARB_shading_language_100 not supported.\n");

//text

    vl::ref<vl::Painter> name_painter = new vl::Painter;
    pipeline()->shaderNode()->addChild(name_painter.get());
    name_painter->shader()->disable(vl::EN_LIGHTING);
    name_painter->shader()->enable(vl::EN_BLEND);
    name_painter->shader()->glBlendFunc()->set(vl::BF_SRC_ALPHA, vl::BF_ONE_MINUS_SRC_ALPHA);
    /* to avoid clipping artefacts due to partial character overlapping we either disable depth
         testing, set depth-write mask to false or enable an appropriate alpha testing. */
    name_painter->shader()->disable(vl::EN_DEPTH_TEST);


    vl::ref<vl::Font> font;
    vl::ref<vl::Text> text = new vl::Text;

    font = new vl::Font("fonts/bitstream-vera.ttf", 8);

    text = new vl::Text;
    name_painter->addActor( new vl::Actor( text.get(), engineXpTransform.get() ) );
    text->setFont(font.get());
    text->setMode( vl::Text2D );
    text->setText( L"X+" );
    text->setColor(vlut::white);
    text->setAlignment(vl::AlignBottom | vl::AlignLeft );

    text = new vl::Text;
    name_painter->addActor( new vl::Actor( text.get(), engineZpTransform.get() ) );
    text->setFont(font.get());
    text->setMode( vl::Text2D );
    text->setText( L"Z+" );
    text->setColor(vlut::white);
    text->setAlignment(vl::AlignBottom | vl::AlignLeft );

    info = new vl::Text;
    name_painter->addActor( new vl::Actor( info.get() ) );
    info->setFont(font.get());
    info->setMode( vl::Text2D );
    //info->setText( L"blablabla" );
    info->setColor(vlut::red);
    info->setAlignment(vl::AlignTop | vl::AlignLeft );
  }

protected:
  vl::ref<vl::Transform> _Transform;
  vl::ref<vl::Transform> engineXpTransform;
  vl::ref<vl::Transform> engineXmTransform;
  vl::ref<vl::Transform> engineZpTransform;
  vl::ref<vl::Transform> engineZmTransform;
  vl::ref<vl::Transform> floorTransform;
  vl::ref<vl::Transform> camFollowTransform;
  vl::ref<vl::Transform> camMountTransform;
  double time;
  vl::ref<vl::Text> info;
};

int main ( int argc, char *argv[] )
{
  int pargc = argc;
  glutInit( &pargc, argv );


  vl::visualization_library_init();
  atexit( vlGLUT::atexit_visualization_library_shutdown );

  vl::OpenGLContextInfo info;
  info.setHasDoubleBuffer(true);
  info.setRGBABits( vl::ivec4(8,8,8,8) );
  info.setZBufferBits(24);
  info.setFullscreen(false);

  vl::ref<vl::Object> owner;

  owner=vlGLUT::open_GLUT_Window("simquadcopter", new CopterViewer_Program(), info, 10,10, 800, 600, vlut::white, vl::vec3d(0,500,100), vl::vec3d(0,0,0));

  glutMainLoop();

  /* this point is never reached since glutMainLoop() never returns. */

  return 0;
}
