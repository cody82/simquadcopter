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

#include "vlut/Program.hpp"
#include "vl/Renderer.hpp"
#include "vl/Time.hpp"
#include "vl/Say.hpp"
#include "vl/Log.hpp"
#include "vlut/RenderPipeline.hpp"

using namespace vlut;

Program::Program()
{
  mLogFPS = false;
  mActivePipeline = 0;
  addPipeline( new RenderPipeline );
  mFly       = new vl::FlyCameraManipulator;
  mTrackball = new vl::TrackballManipulator;
  mTrackball->setEnabled(true);
  mFly->setRank(0);

  mFly->setEnabled(false);

  _framecount = 0;
  _start = vl::Time::timerSeconds();
}

void Program::bindManipulatorsToPipeline(RenderPipeline* pipe)
{
  mFly->setCamera( pipe->camera() );
  mFly->prepareToReconnect();

  mTrackball->setCamera( pipe->camera() );
  mTrackball->setTransform( pipe->transform() );
  mTrackball->setPivot( vl::vec3d(0,0,0) );
}

void Program::boundToOpenGLWidget(vl::OpenGLWidget* openglWidget)
{
  CHECK(openglWidget)
  CHECK(mFly->openglWidget() == NULL);
  mFly->setCamera( pipeline()->camera() );
  openglWidget->addEventListerner( mFly.get() );
  mFly->prepareToReconnect();

  CHECK(mTrackball->openglWidget() == NULL);
  mTrackball->setCamera( pipeline()->camera() );

  mTrackball->setPivot( vl::vec3d(0,0,0) );
  openglWidget->addEventListerner( mTrackball.get() );
}

void Program::keyReleaseEvent(unsigned int, vl::EKey key)
{
  if (key == vl::Key_Escape)
    openglWidget()->quit();
  else
  if (key == vl::Key_U)
    openglWidget()->update();
  else
  if(key == vl::Key_T)
  {
    mFly->setEnabled(false);
    mTrackball->setEnabled(true);
  }
  else
  if(key == vl::Key_F)
  {
    mFly->setEnabled(true);
    mFly->prepareToReconnect();
    mTrackball->setEnabled(false);
  }
  else
  if(key == vl::Key_M)
  {
    mFly->setEnabled(false);
    mTrackball->setEnabled(false);
  }
}

void Program::closeEvent()
{
  shutdown();

  mFly->setCamera(NULL);
  mTrackball->setCamera(NULL);
  mTrackball->setTransform(NULL);
  if (openglWidget())
    openglWidget()->removeAllEventListerners();
}

void Program::resizeEvent(int w, int h)
{

  pipeline()->camera()->renderTarget()->setWidth(w);
  pipeline()->camera()->renderTarget()->setHeight(h);
  pipeline()->camera()->viewport()->setWidth(w);
  pipeline()->camera()->viewport()->setHeight(h);
  pipeline()->camera()->setProjectionMatrixPerspective();
}

void Program::renderEvent()
{

  if (vl::Time::timerSeconds() - _start > 1.000)
  {
    double secs = (vl::Time::timerSeconds() - _start);
    std::string fps = vl::Say("%.2n FPS") << _framecount / secs;
    std::string str = title() + " - " + fps;
    openglWidget()->setWindowTitle(str);
    if (logFPS())
      vl::Log::print(fps + "\n");

    _framecount = 0;
    _start = vl::Time::timerSeconds();
  }
  _framecount++;

  run();

  for(unsigned i=0; i<mPipeline.size(); i++)
    mPipeline[i]->executeRendering();

  mOpenGLWidget->setVSyncEnabled(false);

  mOpenGLWidget->swapBuffers();
}

