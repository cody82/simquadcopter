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

#ifndef Program_INCLUDE_DEFINE
#define Program_INCLUDE_DEFINE

#include "vl/EventListener.hpp"
#include "vl/TrackballManipulator.hpp"
#include "vl/FlyCameraManipulator.hpp"
#include "vlut/RenderPipeline.hpp"

namespace vlut
{

  class Program: public vl::EventListener
  {
  public:
    Program();

    ~Program() {}

    void bindManipulatorsToPipeline(RenderPipeline* pipe);

    virtual void boundToOpenGLWidget(vl::OpenGLWidget* openglWidget);

    void keyReleaseEvent(unsigned int, vl::EKey key);

    virtual void closeEvent();

    void resizeEvent(int w, int h);

    void renderEvent();

    vl::FlyCameraManipulator* fly() { return mFly.get(); }
    vl::TrackballManipulator* trackball() { return mTrackball.get(); }

    int activePipeline() const { return mActivePipeline; }
    void setActivePipeline(int index) { mActivePipeline = index; }

    void addPipeline(RenderPipeline* pipeline) { mPipeline.push_back(pipeline); }
    RenderPipeline* pipeline() { return mPipeline[mActivePipeline].get(); }
    RenderPipeline* pipeline(int index) { return mPipeline[index].get(); }
    int pipelineCount() const { return (int)mPipeline.size(); }
    void removePipeline(int index) { mPipeline.erase( mPipeline.begin() + index); }
    void removeAllPipelines() { mPipeline.clear(); }

    const std::string& title() const { return mTitle; }
    void setTitle(const std::string& title) { mTitle = title; }

    bool logFPS() const { return mLogFPS; }
    void setLogFPS(bool log_fps) { mLogFPS = log_fps; }

    virtual void run() = 0;
    virtual void init() = 0;
    virtual void shutdown() = 0;

  protected:
    vl::ref<vl::FlyCameraManipulator> mFly;
    vl::ref<vl::TrackballManipulator> mTrackball;
    std::vector< vl::ref<RenderPipeline> > mPipeline;
    int mActivePipeline;
    std::string mTitle;
    int _framecount;
    double _start;
    bool mLogFPS;
  };
}

#endif
