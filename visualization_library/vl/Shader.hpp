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

#ifndef Shader_INCLUDE_DEFINE
#define Shader_INCLUDE_DEFINE

#include "vl/Object.hpp"
#include "vl/mat4.hpp"
#include "vl/OpenGL.hpp"
#include "vl/Light.hpp"
#include "vl/Plane.hpp"
#include "vl/Texture.hpp"
#include "vl/GLSL.hpp"
#include <vector>
#include <bitset>

namespace vl
{

  class GLState: public Object
  {
  public:
    GLState() {}
    virtual ~GLState() {}
  };

  class PixelTransfer: public GLState
  {
  public:
    PixelTransfer() 
    {
      mMapColor    = false;
      mMapStencil  = false;
      mIndexShift  = 0;
      mIndexOffset = 0;
      mRedScale    = 1;
      mGreenScale  = 1;
      mBlueScale   = 1;
      mAlphaScale  = 1;
      mDepthScale  = 1;
      mRedBias     = 0;
      mGreenBias   = 0;
      mBlueBias    = 0;
      mAlphaBias   = 0;
      mDepthBias   = 0;
      mPostColorMatrixRedScale   = 1;
      mPostColorMatrixGreenScale = 1;
      mPostColorMatrixBlueScale  = 1;
      mPostColorMatrixAlphaScale = 1;
      mPostColorMatrixRedBias    = 0;
      mPostColorMatrixGreenBias  = 0;
      mPostColorMatrixBlueBias   = 0;
      mPostColorMatrixAlphaBias  = 0;
      mPostConvolutionRedScale   = 1;
      mPostConvolutionGreenScale = 1;
      mPostConvolutionBlueScale  = 1;
      mPostConvolutionAlphaScale = 1;
      mPostConvolutionRedBias    = 0;
      mPostConvolutionGreenBias  = 0;
      mPostConvolutionBlueBias   = 0;
      mPostConvolutionAlphaBias  = 0;
    }

    virtual void applyGLState();

    bool mapColor() const { return mMapColor; }
    bool mapStencil() const { return mMapStencil; }
    int indexShift() const { return mIndexShift; }
    int indexOffset() const { return mIndexOffset; }
    float redScale() const { return mRedScale; }
    float greenScale() const { return mGreenScale; }
    float blueScale() const { return mBlueScale; }
    float alphaScale() const { return mAlphaScale; }
    float depthScale() const { return mDepthScale; }
    float redBias() const { return mRedBias; }
    float greenBias() const { return mGreenBias; }
    float blueBias() const { return mBlueBias; }
    float alphaBias() const { return mAlphaBias; }
    float depthBias() const { return mDepthBias; }
    float postColorMatrixRedScale() const { return mPostColorMatrixRedScale; }
    float postColorMatrixGreenScale() const { return mPostColorMatrixGreenScale; }
    float postColorMatrixBlueScale() const { return mPostColorMatrixBlueScale; }
    float postColorMatrixAlphaScale() const { return mPostColorMatrixAlphaScale; }
    float postColorMatrixRedBias() const { return mPostColorMatrixRedBias; }
    float postColorMatrixGreenBias() const { return mPostColorMatrixGreenBias; }
    float postColorMatrixBlueBias() const { return mPostColorMatrixBlueBias; }
    float postColorMatrixAlphaBias() const { return mPostColorMatrixAlphaBias; }
    float postConvolutionRedScale() const { return mPostConvolutionRedScale; }
    float postConvolutionGreenScale() const { return mPostConvolutionGreenScale; }
    float postConvolutionBlueScale() const { return mPostConvolutionBlueScale; }
    float postConvolutionAlphaScale() const { return mPostConvolutionAlphaScale; }
    float postConvolutionRedBias() const { return mPostConvolutionRedBias; }
    float postConvolutionGreenBias() const { return mPostConvolutionGreenBias; }
    float postConvolutionBlueBias() const { return mPostConvolutionBlueBias; }
    float postConvolutionAlphaBias() const { return mPostConvolutionAlphaBias; }

    void setMapColor(bool map_color) {  mMapColor = map_color; }
    void setMapStencil(bool map_stencil) {  mMapStencil = map_stencil; }
    void setIndexShift(int index_shift) {  mIndexShift = index_shift; }
    void setIndexOffset(int index_offset) {  mIndexOffset = index_offset; }
    void setRedScale(float red_scale) {  mRedScale = red_scale; }
    void setGreenScale(float green_scale) {  mGreenScale = green_scale; }
    void setBlueScale(float blue_scale) {  mBlueScale = blue_scale; }
    void setAlphaScale(float alpha_scale) {  mAlphaScale = alpha_scale; }
    void setDepthScale(float depth_scale) {  mDepthScale = depth_scale; }
    void setRedBias(float red_bias) {  mRedBias = red_bias; }
    void setGreenBias(float green_bias) {  mGreenBias = green_bias; }
    void setBlueBias(float blue_bias) {  mBlueBias = blue_bias; }
    void setAlphaBias(float alpha_bias) {  mAlphaBias = alpha_bias; }
    void setDepthBias(float depth_bias) {  mDepthBias = depth_bias; }
    void setPostColorMatrixRedScale(float scale) {  mPostColorMatrixRedScale = scale; }
    void setPostColorMatrixGreenScale(float scale) {  mPostColorMatrixGreenScale = scale; }
    void setPostColorMatrixBlueScale(float scale) {  mPostColorMatrixBlueScale = scale; }
    void setPostColorMatrixAlphaScale(float scale) {  mPostColorMatrixAlphaScale = scale; }
    void setPostColorMatrixRedBias(float bias) {  mPostColorMatrixRedBias = bias; }
    void setPostColorMatrixGreenBias(float bias) {  mPostColorMatrixGreenBias = bias; }
    void setPostColorMatrixBlueBias(float bias) {  mPostColorMatrixBlueBias = bias; }
    void setPostColorMatrixAlphaBias(float bias) {  mPostColorMatrixAlphaBias = bias; }
    void setPostConvolutionRedScale(float scale) {  mPostConvolutionRedScale = scale; }
    void setPostConvolutionGreenScale(float scale) {  mPostConvolutionGreenScale = scale; }
    void setPostConvolutionBlueScale(float scale) {  mPostConvolutionBlueScale = scale; }
    void setPostConvolutionAlphaScale(float scale) {  mPostConvolutionAlphaScale = scale; }
    void setPostConvolutionRedBias(float bias) {  mPostConvolutionRedBias = bias; }
    void setPostConvolutionGreenBias(float bias) {  mPostConvolutionGreenBias = bias; }
    void setPostConvolutionBlueBias(float bias) {  mPostConvolutionBlueBias = bias; }
    void setPostConvolutionAlphaBias(float bias) {  mPostConvolutionAlphaBias = bias; }

  protected:
    bool mMapColor;
    bool mMapStencil;
    int mIndexShift;
    int mIndexOffset;
    float mRedScale;
    float mGreenScale;
    float mBlueScale;
    float mAlphaScale;
    float mDepthScale;
    float mRedBias;
    float mGreenBias;
    float mBlueBias;
    float mAlphaBias;
    float mDepthBias;
    float mPostColorMatrixRedScale;
    float mPostColorMatrixGreenScale;
    float mPostColorMatrixBlueScale;
    float mPostColorMatrixAlphaScale;
    float mPostColorMatrixRedBias;
    float mPostColorMatrixGreenBias;
    float mPostColorMatrixBlueBias;
    float mPostColorMatrixAlphaBias;
    float mPostConvolutionRedScale;
    float mPostConvolutionGreenScale;
    float mPostConvolutionBlueScale;
    float mPostConvolutionAlphaScale;
    float mPostConvolutionRedBias;
    float mPostConvolutionGreenBias;
    float mPostConvolutionBlueBias;
    float mPostConvolutionAlphaBias;

  };

  class Hint: public GLState
  {
  public:
    Hint(): mPerspectiveCorrectionHint(HM_DONT_CARE), mPointSmoothHint(HM_DONT_CARE), 
                 mLineSmoothHint(HM_DONT_CARE), mPolygonSmoothHint(HM_DONT_CARE), mFogHint(HM_DONT_CARE) {}
    virtual void applyGLState();

    void setPerspectiveCorrectionHint(EHintMode mode) { mPerspectiveCorrectionHint = mode; }
    void setPolygonSmoohtHint(EHintMode mode) { mPolygonSmoothHint = mode; }
    void setLineSmoothHint(EHintMode mode) { mLineSmoothHint = mode; }
    void setPointSmoothHint(EHintMode mode) { mPointSmoothHint = mode; }
    void setFogHint(EHintMode mode) { mFogHint = mode; }

    EHintMode perspectiveCorrectionHint() const { return mPerspectiveCorrectionHint; }
    EHintMode polygonSmoohtHint() const { return mPolygonSmoothHint; }
    EHintMode lineSmoothHint() const { return mLineSmoothHint; }
    EHintMode pointSmoothHint() const { return mPointSmoothHint; }
    EHintMode fogHint() const { return mFogHint; }

  protected:
    EHintMode mPerspectiveCorrectionHint;
    EHintMode mPointSmoothHint;
    EHintMode mLineSmoothHint;
    EHintMode mPolygonSmoothHint;
    EHintMode mFogHint;
  };

  class CullFace: public GLState
  {
  public:
    CullFace(EPolygonFace cullface=PF_BACK): mFaceMode(cullface) {}
    virtual void applyGLState();
    void set(EPolygonFace facemode) { mFaceMode = facemode; }
    EPolygonFace faceMode() const { return mFaceMode; }
  protected:
    EPolygonFace mFaceMode;
  };

  class FrontFace: public GLState
  {
  public:
    FrontFace(EFrontFace frontface=FF_CCW): mFrontFace(frontface) {}
    virtual void applyGLState();
    void set(EFrontFace frontface) { mFrontFace = frontface; }
    EFrontFace frontFace() const { return mFrontFace; }
  protected:
    EFrontFace mFrontFace;
  };

  class DepthFunc: public GLState
  {
  public:
    DepthFunc(EFunction depthfunc=FU_LESS): mDepthFunc(depthfunc) {}
    virtual void applyGLState();
    void set(EFunction depthfunc) { mDepthFunc = depthfunc; }
    EFunction depthFunc() const { return mDepthFunc; }
  protected:
    EFunction mDepthFunc;
  };

  class DepthMask: public GLState
  {
  public:
    DepthMask(bool depthmask=true): mDepthMask(depthmask) {}
    virtual void applyGLState();
    void set(bool depthmask) { mDepthMask = depthmask; }
    bool depthMask() const { return mDepthMask; }
  protected:
    bool mDepthMask;
  };

  class GLPolygonMode: public GLState
  {
  public:
    GLPolygonMode(EPolygonMode frontface=PM_FILL, EPolygonMode backface=PM_FILL): mFrontFace(frontface), mBackFace(backface) {}
    virtual void applyGLState();
    void set(EPolygonMode frontface, EPolygonMode backface) { mFrontFace = frontface; mBackFace = backface; }
    void setFrontFace(EPolygonMode frontface) { mFrontFace = frontface; }
    void setBackFace(EPolygonMode backface) { mBackFace = backface; }
    EPolygonMode frontFace() const { return mFrontFace; }
    EPolygonMode backFace() const { return mBackFace; }
  protected:
    EPolygonMode mFrontFace;
    EPolygonMode mBackFace;
  };

  class ShadeModel: public GLState
  {
  public:
    ShadeModel(EShadeModel shademodel=SM_SMOOTH): mShadeModel(shademodel) {}
    virtual void applyGLState();
    void set(EShadeModel shademodel) { mShadeModel = shademodel; }
    EShadeModel shadeModel() const { return mShadeModel; }
  protected:
    EShadeModel mShadeModel;
  };

  class BlendFunc: public GLState
  {
  public:
    BlendFunc(EBlendFactor srcrgb=BF_ONE, EBlendFactor dstrgb=BF_ZERO, EBlendFactor srcalpha=BF_ONE, EBlendFactor dstalpha=BF_ZERO): 
      mSrcRGB(srcrgb), mDstRGB(dstrgb), mSrcAlpha(srcalpha), mDstAlpha(dstalpha) {}
    virtual void applyGLState();
    void set(EBlendFactor src_rgb, EBlendFactor dst_rgb, EBlendFactor src_alpha, EBlendFactor dst_alpha) { mSrcRGB = src_rgb; mSrcAlpha = src_alpha; mDstRGB = dst_rgb; mDstAlpha = dst_alpha; }
    void set(EBlendFactor src_rgba, EBlendFactor dst_rgba) { mSrcRGB = src_rgba; mSrcAlpha = src_rgba; mDstRGB = dst_rgba; mDstAlpha = dst_rgba; }
    void setSrcRGB(EBlendFactor factor) { mSrcRGB = factor; }
    void setDstRGB(EBlendFactor factor) { mDstRGB = factor; }
    void setSrcAlpha(EBlendFactor factor) { mSrcAlpha = factor; }
    void setDstAlpha(EBlendFactor factor) { mDstAlpha = factor; }
    EBlendFactor srcRGB() const { return mSrcRGB; }
    EBlendFactor dstRGB() const { return mDstRGB; }
    EBlendFactor srcAlpha() const { return mSrcAlpha; }
    EBlendFactor dstAlpha() const { return mDstAlpha; }
  protected:
    EBlendFactor mSrcRGB;
    EBlendFactor mDstRGB;
    EBlendFactor mSrcAlpha;
    EBlendFactor mDstAlpha;
  };

  class SampleCoverage: public GLState
  {
  public:
    SampleCoverage(GLclampf value=1.0f, bool invert=false): mValue(value), mInvert(invert) {}
    virtual void applyGLState();
    void set(GLclampf value, bool invert) { mValue = value; mInvert = invert; }
    void setValue(GLclampf value) { mValue = value; }
    void setInvert(bool invert) { mInvert = invert; }
    GLclampf value() const { return mValue; }
    bool invert() const { return mInvert; }
  protected:
    GLclampf mValue;
    bool mInvert;
  };

  class BlendEquation: public GLState
  {
  public:
    BlendEquation(EBlendEquation blendequation=BE_FUNC_ADD): mBlendEquation(blendequation) {}
    virtual void applyGLState();
    void set(EBlendEquation blendequation) { mBlendEquation = blendequation; }
    EBlendEquation blendEquation() const { return mBlendEquation; }
  protected:
    EBlendEquation mBlendEquation;
  };

  class AlphaFunc: public GLState
  {
  public:
    AlphaFunc(EFunction alphafunc=FU_ALWAYS, GLfloat refvalue=0): mRefValue(refvalue), mAlphaFunc(alphafunc) {}
    virtual void applyGLState();
    void set(EFunction alphafunc, GLfloat ref_value) { mAlphaFunc = alphafunc; mRefValue = ref_value; }
    EFunction alphaFunc() const { return mAlphaFunc; }
    GLfloat refValue() const { return mRefValue; }
  protected:
    GLfloat mRefValue;
    EFunction mAlphaFunc;  
  };

  class ColorMaterial: public GLState
  {
  public:
    ColorMaterial(EPolygonFace face=PF_FRONT_AND_BACK, EColorMaterial color=CM_AMBIENT_AND_DIFFUSE): mFace(face), mColor(color) {}
    virtual void applyGLState();
    void set(EPolygonFace face, EColorMaterial color){ mFace = face; mColor = color; }
    void setFace(EPolygonFace face) { mFace = face; }
    void setColor(EColorMaterial color) { mColor = color; }
    EPolygonFace face() const { return mFace; }
    EColorMaterial color() const { return mColor; }
  protected:
    EPolygonFace mFace;
    EColorMaterial mColor;
  };

  class Material: public GLState
  {
  public:
    Material();
    virtual void applyGLState();

    void setTransparency(float alpha);
    void setFrontTransparency(float alpha);
    void setBackTransparency(float alpha);
    void setFrontFlatColor(const vec4& color);
    void setBackFlatColor(const vec4& color);
    void setFlatColor(const vec4& color);

    void setAmbient(vec4 color)          { mFrontAmbient   = mBackAmbient   = color; }
    void setDiffuse(vec4 color)          { mFrontDiffuse   = mBackDiffuse   = color; }
    void setSpecular(vec4 color)         { mFrontSpecular  = mBackSpecular  = color; }
    void setEmission(vec4 color)         { mFrontEmission  = mBackEmission  = color; }
    void setShininess(GLfloat shininess) { mFrontShininess = mBackShininess = shininess; }

    void setFrontAmbient(vec4 color) { mFrontAmbient = color; }
    void setFrontDiffuse(vec4 color) { mFrontDiffuse = color; }
    void setFrontSpecular(vec4 color) { mFrontSpecular = color; }
    void setFrontEmission(vec4 color) { mFrontEmission = color; }
    void setFrontShininess(GLfloat shininess) { mFrontShininess=shininess; }
    vec4 frontAmbient() const { return mFrontAmbient; }
    vec4 frontDiffuse() const { return mFrontDiffuse; }
    vec4 frontSpecular() const { return mFrontSpecular; }
    vec4 frontEmission() const { return mFrontEmission; }
    GLfloat frontShininess() const { return mFrontShininess; }

    void setBackAmbient(vec4 color) { mBackAmbient = color; }
    void setBackDiffuse(vec4 color) { mBackDiffuse = color; }
    void setBackSpecular(vec4 color) { mBackSpecular = color; }
    void setBackEmission(vec4 color) { mBackEmission = color; }
    void setBackShininess(GLfloat shininess) { mBackShininess=shininess; }
    vec4 backAmbient() const { return mBackAmbient; }
    vec4 backDiffuse() const { return mBackDiffuse; }
    vec4 backSpecular() const { return mBackSpecular; }
    vec4 backEmission() const { return mBackEmission; }
    GLfloat backShininess() const { return mBackShininess; }

  protected:
    vec4 mFrontAmbient;
    vec4 mFrontDiffuse;
    vec4 mFrontSpecular;
    vec4 mFrontEmission;
    vec4 mBackAmbient;
    vec4 mBackDiffuse;
    vec4 mBackSpecular;
    vec4 mBackEmission;
    GLfloat mBackShininess;
    GLfloat mFrontShininess;
  };

  class LightModel: public GLState
  {
  public:
    LightModel(): mAmbientColor(0.2f,0.2f,0.2f,1.0f), mColorControl(CC_SINGLE_COLOR), mLocalViewer(false), mTwoSide(false) {}
    virtual void applyGLState();
    void setLocalViewer(bool localviewer) { mLocalViewer = localviewer; }
    void setTwoSide(bool twoside) { mTwoSide = twoside; }
    void setColorControl(EColorControl colorcontrol) { mColorControl = colorcontrol; }
    void setAmbientColor(vec4 ambientcolor) { mAmbientColor = ambientcolor; }
    bool localViewer() const { return mLocalViewer; }
    bool twoSide() const { return mTwoSide; }
    EColorControl colorControl() const { return mColorControl; }
    vec4 ambientColor() const { return mAmbientColor; }
  protected:
    vec4 mAmbientColor;
    EColorControl mColorControl;
    bool mLocalViewer;
    bool mTwoSide;
  };

  class Fog: public GLState
  {
  public:
    Fog(EFogMode mode=FM_LINEAR, vec4 color=vec4(0,0,0,0), GLfloat density=1, GLfloat start=0, GLfloat end=1): 
      mColor(color), mMode(mode), mDensity(density), mStart(start), mEnd(end) {}
    virtual void applyGLState();
    void set(EFogMode mode, vec4 color, GLfloat density, GLfloat start, GLfloat end) { mColor = color; mMode = mode; mDensity = density; mStart = start; mEnd = end; }
    void setColor(vec4 color) { mColor = color; }
    void setMode(EFogMode mode) { mMode = mode; }
    void setDensity(GLfloat density) { mDensity = density; }
    void setStart(GLfloat start) { mStart = start; }
    void setEnd(GLfloat end) { mEnd = end; }
    vec4 color() const { return mColor; }
    EFogMode mode() const { return mMode; }
    GLfloat density() const { return mDensity; }
    GLfloat start() const { return mStart; }
    GLfloat end() const { return mEnd; }
  protected:
    vec4 mColor;
    EFogMode mMode;
    GLfloat mDensity;
    GLfloat mStart;
    GLfloat mEnd;
  };

  class PolygonOffset: public GLState
  {
  public:
    PolygonOffset(): mFactor(0.0f), mUnits(0.0f) {}
    PolygonOffset(GLfloat factor, GLfloat units): mFactor(factor), mUnits(units) {}
    virtual void applyGLState();
    void set(GLfloat factor, GLfloat units) { mFactor = factor; mUnits = units; }
    void setFactor(GLfloat factor) { mFactor = factor; }
    void setUnits(GLfloat units) { mUnits = units; }
    GLfloat factor() const { return mFactor; }
    GLfloat units() const { return mUnits; }
  protected:
    GLfloat mFactor;
    GLfloat mUnits;
  };

  class LogicOp: public GLState
  {
  public:
    LogicOp(ELogicOp logicop=LO_COPY): mLogicOp(logicop) {}
    virtual void applyGLState();
    void set(ELogicOp logicop) { mLogicOp = logicop; }
    ELogicOp logicOp() const { return mLogicOp; }
  protected:
    ELogicOp mLogicOp;
  };

  class DepthRange: public GLState
  {
  public:
    DepthRange(): mZNear(0), mZFar(1.0f) {}
    DepthRange(GLfloat znear, GLfloat zfar): mZNear(znear), mZFar(zfar) {}
    virtual void applyGLState();
    void set(GLfloat znear, GLfloat zfar) { mZNear = znear; mZFar = zfar; }
    void setZNear(GLfloat znear) { mZNear = znear; }
    void setZFar(GLfloat zfar) { mZFar = zfar; }
    GLfloat zNear() const { return mZNear; }
    GLfloat zFar() const { return mZFar; }
  protected:
    GLfloat mZNear;
    GLfloat mZFar;
  };

  class LineWidth: public GLState
  {
  public:
    LineWidth(GLfloat linewidth=1.0f): mLineWidth(linewidth) {}
    virtual void applyGLState();
    void set(GLfloat linewidth) { mLineWidth = linewidth; }
    GLfloat lineWidth() const { return mLineWidth; }
  protected:
    GLfloat mLineWidth;
  };

  class PointSize: public GLState
  {
  public:
    PointSize(GLfloat pointsize=1.0f): mPointSize(pointsize) {}
    virtual void applyGLState();
    void set(GLfloat pointsize) { mPointSize = pointsize; }
    GLfloat pointSize() const { return mPointSize; }
  protected:
    GLfloat mPointSize;
  };

  class PolygonStipple: public GLState
  {
  public:
    PolygonStipple();
    PolygonStipple(const GLubyte* mask);
    virtual void applyGLState();
    void set(const GLubyte* mask);
    const GLubyte* mask() { return mMask; }
  protected:
    GLubyte mMask[32*32/8];
  };

  class LineStipple: public GLState
  {
  public:
    LineStipple(GLint factor=1, GLushort pattern=~(GLushort)0): mFactor(factor), mPattern(pattern) {}
    virtual void applyGLState();
    void set(GLint factor, GLushort pattern) { mFactor = factor; mPattern = pattern; }
    void setFactor(GLint factor) { mFactor = factor; }
    void setPattern(GLushort pattern) { mPattern = pattern; }
    GLint factor() const { return mFactor; }
    GLushort pattern() const { return mPattern; }
  protected:
    GLint mFactor;
    GLushort mPattern;
  };

  class PointParameter: public GLState
  {
  public: 
    PointParameter(GLfloat sizemin=0, GLfloat sizemax=1024.0f, GLfloat fadethresholdsize=1.0f, vec3 distanceattenuation=vec3(1,0,0)):
      mDistanceAttenuation(distanceattenuation), mSizeMin(sizemin), mSizeMax(sizemax), mFadeThresholdSize(fadethresholdsize) {}
    virtual void applyGLState();
    void set(GLfloat sizemin, GLfloat sizemax, GLfloat fadethresholdsize, vec3 distanceattenuation) { mDistanceAttenuation = distanceattenuation; mSizeMin = sizemin; mSizeMax = sizemax; mFadeThresholdSize = fadethresholdsize; }
    void setDistanceAttenuation(vec3 attenuation) { mDistanceAttenuation = attenuation; }
    void setSizeMin(GLfloat sizemin) { mSizeMin = sizemin; }
    void setSizeMax(GLfloat sizemax) { mSizeMax = sizemax; }
    void setFadeThresholdSize(GLfloat threshold) { mFadeThresholdSize = threshold; }
    vec3 distanceAttenuation() const { return mDistanceAttenuation; }
    GLfloat sizeMin() const { return mSizeMin; }
    GLfloat sizeMax() const { return mSizeMax; }
    GLfloat fadeThresholdSize() const { return mFadeThresholdSize; }
  protected:
    vec3 mDistanceAttenuation;
    GLfloat mSizeMin;
    GLfloat mSizeMax;
    GLfloat mFadeThresholdSize;
  };

  class StencilFunc: public GLState
  {
  public:
    StencilFunc(EFunction function=FU_ALWAYS, GLint refvalue=0, GLuint mask=~(GLuint)0): mFunction(function), mRefValue(refvalue), mMask(mask) {}
    virtual void applyGLState();
    void set(EFunction function, GLint refvalue, GLuint mask) { mFunction = function; mRefValue = refvalue; mMask = mask; }
    void setFunction(EFunction function) { mFunction = function; }
    void setRefValue(GLint refvalue) { mRefValue = refvalue; }
    void setMask(GLuint mask) { mMask = mask; }
    EFunction function() const { return mFunction; }
    GLint refValue() const { return mRefValue; }
    GLuint mask() const { return mMask; }
  protected:
    EFunction mFunction;
    GLint mRefValue;
    GLuint mMask;
  };

  class StencilOp: public GLState
  {
  public:
    StencilOp(EStencilFail sfail=SF_KEEP, EStencilFail dpfail=SF_KEEP, EStencilFail dppass=SF_KEEP): mSFail(sfail), mDpFail(dpfail), mDpPass(dppass) {}
    virtual void applyGLState();
    void set(EStencilFail sfail, EStencilFail dpfail, EStencilFail dppass) { mSFail = sfail; mDpFail = dpfail; mDpPass = dppass; }
    void setSFail(EStencilFail sfail) { mSFail = sfail; }
    void dpFail(EStencilFail dpfail) { mDpFail = dpfail; }
    void dpPass(EStencilFail dppass) { mDpPass = dppass; }
    EStencilFail sFail() const { return mSFail; }
    EStencilFail dpFail() const { return mDpFail; }
    EStencilFail dpPass() const { return mDpPass; }
  protected:
    EStencilFail mSFail;
    EStencilFail mDpFail;
    EStencilFail mDpPass;
  };

  class StencilMask: public GLState
  {
  public:
    StencilMask(GLuint mask=~(GLuint)0): mMask(mask) {}
    virtual void applyGLState();
    void set(GLuint mask) { mMask = mask; }
    GLuint mask() const { return mMask; }
  protected:
    GLuint mMask;
  };

  class BlendColor: public GLState
  {
  public:
    BlendColor(vec4 blendcolor=vec4(0,0,0,0)): mBlendColor(blendcolor) {}
    virtual void applyGLState();
    void set(vec4 blendcolor) { mBlendColor = blendcolor; }
    vec4 blendColor() const { return mBlendColor; }
  protected:
    vec4 mBlendColor;
  };

  class ColorMask: public GLState
  {
  public:
    ColorMask(bool red=true, bool green=true, bool blue=true, bool alpha=true): mRed(red), mGreen(green), mBlue(blue), mAlpha(alpha) {}
    virtual void applyGLState();
    void set(bool red, bool green, bool blue, bool alpha) { mRed = red; mGreen = green; mBlue = blue; mAlpha = alpha; }
    void setRed(bool red) { mRed = red; }
    void setGreen(bool green) { mGreen = green; }
    void setBlue(bool blue) { mBlue = blue; }
    void setAlpha(bool alpha) { mAlpha = alpha; }
    bool red() const { return mRed; }
    bool green() const { return mGreen; }
    bool blue() const { return mBlue; }
    bool alpha() const { return mAlpha; }
  protected:
    bool mRed;
    bool mGreen;
    bool mBlue;
    bool mAlpha;
  };

  class Scissor: public GLState
  {
  public:
    Scissor();
    Scissor(GLint x, GLint y, GLsizei width, GLsizei height): mX(x), mY(y), mWidth(width), mHeight(height) {}
    virtual void applyGLState();
    void set(GLint x, GLint y, GLsizei width, GLsizei height) { mX = x; mY = y; mWidth = width; mHeight = height; }
    void setX(GLint x) { mX = x; }
    void setY(GLint y) { mY = y; }
    void setWidth(GLsizei width) { mWidth = width; }
    void setHeight(GLsizei height) { mHeight = height; }
    GLint x() const { return mX; }
    GLint y() const { return mY; }
    GLsizei width() const { return mWidth; }
    GLsizei height() const { return mHeight; }
  protected:
    GLint mX;
    GLint mY;
    GLsizei mWidth;
    GLsizei mHeight;
  };

  class TexParameter: public GLState
  {
  public:
    TexParameter();
    void applyGLState(ETextureDimension dimension);

    ETexParamFilter minFilter() const { return mMinFilter; }
    ETexParamFilter magFilter() const { return mmagfilter; }
    ETexParamWrap wrapS() const { return mWrapS; }
    ETexParamWrap wrapT() const { return mWrapT; }
    ETexParamWrap wrapR() const { return mWrapR; }
    vec4 borderColor() const { return mBorderColor; }
    GLfloat anisotropy() const { return mAnisotropy; }

    void setMinFilter(ETexParamFilter minfilter) { mMinFilter = minfilter; }
    void setMagFilter(ETexParamFilter magfilter);
    void setWrapS(ETexParamWrap texturewrap) { mWrapS = texturewrap; }
    void setWrapT(ETexParamWrap texturewrap) { mWrapT = texturewrap; }
    void setWrapR(ETexParamWrap texturewrap) { mWrapR = texturewrap; }
    void setBorderColor(vec4 bordercolor) { mBorderColor = bordercolor; }
    void setAnisotropy(GLfloat anisotropy) { mAnisotropy = anisotropy; }

  protected:
    ETexParamFilter mMinFilter;
    ETexParamFilter mmagfilter;
    ETexParamWrap mWrapS;
    ETexParamWrap mWrapT;
    ETexParamWrap mWrapR;
    vec4 mBorderColor;
    GLfloat mAnisotropy;
  };

  class TexEnv: public GLState
  {
  public:
    TexEnv();
    void applyGLState();

    void setMode(ETexEnvMode mode) { mMode = mode; }
    ETexEnvMode mode() const { return mMode; }
    void setCombineRGB(ETexEnvMode combineRGB) { mCombineRGB = combineRGB; }
    ETexEnvMode combineRGB() const { return mCombineRGB; }
    void setCombineAlpha(ETexEnvMode combineAlpha) { mCombineAlpha = combineAlpha; }
    ETexEnvMode combineAlpha() const { return mCombineAlpha; }
    void setColor(vec4 color) { mColor = color; }
    vec4 color() const { return mColor; }
    void setRGBScale(GLfloat rgbscale) { mRGBScale = rgbscale; }
    GLfloat rgbScale() const { return mRGBScale; }
    void setAlphaScale(GLfloat alphascale) { mAlphaScale = alphascale; }
    GLfloat alphaScale() const { return mAlphaScale; }

    void setSource0RGB(ETexEnvSource source) { mSource0RGB = source; }
    void setSource1RGB(ETexEnvSource source) { mSource1RGB = source; }
    void setSource2RGB(ETexEnvSource source) { mSource2RGB = source; }
    ETexEnvSource source0RGB() const { return mSource0RGB; }
    ETexEnvSource source1RGB() const { return mSource1RGB; }
    ETexEnvSource source2RGB() const { return mSource2RGB; }
    void setSource0Alpha(ETexEnvSource source) { mSource0Alpha = source; }
    void setSource1Alpha(ETexEnvSource source) { mSource1Alpha = source; }
    void setSource2Alpha(ETexEnvSource source) { mSource2Alpha = source; }
    ETexEnvSource source0Alpha() const { return mSource0Alpha; }
    ETexEnvSource source1Alpha() const { return mSource1Alpha; }
    ETexEnvSource source2Alpha() const { return mSource2Alpha; }

    void setOperand0RGB(ETexEnvOperand operand) { mOperand0RGB = operand; }
    void setOperand1RGB(ETexEnvOperand operand) { mOperand1RGB = operand; }
    void setOperand2RGB(ETexEnvOperand operand) { mOperand2RGB = operand; }
    ETexEnvOperand operand0RGB() const { return mOperand0RGB; }
    ETexEnvOperand operand1RGB() const { return mOperand1RGB; }
    ETexEnvOperand operand2RGB() const { return mOperand2RGB; }
    void setOperand0Alpha(ETexEnvOperand operand) { mOperand0Alpha = operand; }
    void setOperand1Alpha(ETexEnvOperand operand) { mOperand1Alpha = operand; }
    void setOperand2Alpha(ETexEnvOperand operand) { mOperand2Alpha = operand; }
    ETexEnvOperand operand0Alpha() const { return mOperand0Alpha; }
    ETexEnvOperand operand1Alpha() const { return mOperand1Alpha; }
    ETexEnvOperand operand2Alpha() const { return mOperand2Alpha; }

    void setPointSpriteCoordReplace(bool replace) { mPointSpriteCoordReplace = replace; }
    bool pointSpriteCoordReplace() const { return mPointSpriteCoordReplace; }

    void setLodBias(float lodbias) { mLodBias = lodbias; }
    GLfloat lodBias() const { return mLodBias; }

  public:
    vec4 mColor;
    GLfloat mRGBScale;
    GLfloat mAlphaScale;
    ETexEnvMode mMode;
    ETexEnvMode mCombineRGB;
    ETexEnvMode mCombineAlpha;
    ETexEnvSource mSource0RGB;
    ETexEnvSource mSource1RGB;
    ETexEnvSource mSource2RGB;
    ETexEnvSource mSource0Alpha;
    ETexEnvSource mSource1Alpha;
    ETexEnvSource mSource2Alpha;
    ETexEnvOperand mOperand0RGB;
    ETexEnvOperand mOperand1RGB;
    ETexEnvOperand mOperand2RGB;
    ETexEnvOperand mOperand0Alpha;
    ETexEnvOperand mOperand1Alpha;
    ETexEnvOperand mOperand2Alpha;
    GLfloat mLodBias;
    bool mPointSpriteCoordReplace;
  };

  class TexGen: public GLState
  {
  public:
    TexGen();
    void applyGLState(bool s, bool t, bool r, bool q);

    void setEyePlaneS(vec4 plane) { mEyePlaneS = plane; }
    void setObjectPlaneS(vec4 plane) { mObjectPlaneS = plane; }
    void setGenModeS(ETexGenMode mode) { mGenModeS = mode; }
    vec4 eyePlaneS() const { return mEyePlaneS; }
    vec4 objectPlaneS() const { return mObjectPlaneS; }
    ETexGenMode genModeS() const { return mGenModeS; }

    void setEyePlaneT(vec4 plane) { mEyePlaneT = plane; }
    void setObjectPlaneT(vec4 plane) { mObjectPlaneT = plane; }
    void setGenModeT(ETexGenMode mode) { mGenModeT = mode; }
    vec4 eyePlaneT() const { return mEyePlaneT; }
    vec4 objectPlaneT() const { return mObjectPlaneT; }
    ETexGenMode genModeT() const { return mGenModeT; }

    void setEyePlaneR(vec4 plane) { mEyePlaneR = plane; }
    void setObjectPlaneR(vec4 plane) { mObjectPlaneR = plane; }
    void setGenModeR(ETexGenMode mode) { mGenModeR = mode; }
    vec4 eyePlaneR() const { return mEyePlaneR; }
    vec4 objectPlaneR() const { return mObjectPlaneR; }
    ETexGenMode genModeR() const { return mGenModeR; }

    void setEyePlaneQ(vec4 plane) { mEyePlaneQ = plane; }
    void setObjectPlaneQ(vec4 plane) { mObjectPlaneQ = plane; }
    void setGenModeQ(ETexGenMode mode) { mGenModeQ = mode; }
    vec4 eyePlaneQ() const { return mEyePlaneQ; }
    vec4 objectPlaneQ() const { return mObjectPlaneQ; }
    ETexGenMode genModeQ() const { return mGenModeQ; }

  public:
    vec4 mEyePlaneS;
    vec4 mObjectPlaneS;
    vec4 mEyePlaneT;
    vec4 mObjectPlaneT;
    vec4 mEyePlaneR;
    vec4 mObjectPlaneR;
    vec4 mEyePlaneQ;
    vec4 mObjectPlaneQ;
    ETexGenMode mGenModeS;
    ETexGenMode mGenModeT;
    ETexGenMode mGenModeR;
    ETexGenMode mGenModeQ;
  };

  class TextureUnit: public Object
  {
  public:
    TextureUnit(): mMatrix(NULL), mEnableTexGenS(false), mEnableTexGenT(false), mEnableTexGenR(false), mEnableTexGenQ(false) 
    {
    }
    void applyTexture(GLint texunit, bool enabled);
    void setTexture(Texture* texture) { mTexture = texture; }
    void set_glTexParameter(TexParameter* texpar) { mGLTexParameter = texpar; }
    void set_glTexEnv(TexEnv* texenv) { mGLTexEnv = texenv; }
    void set_glTexGen(TexGen* texgen) { mGLTexGen = texgen; }

    void setMatrix(MatrixObject* matrix) { mMatrix = matrix; }
    MatrixObject* matrix() { return mMatrix.get(); }
    const MatrixObject* matrix() const { return mMatrix.get(); }

    Texture* texture() { return mTexture.get(); }
    const Texture* texture() const { return mTexture.get(); }
    TexParameter* glTexParameter();
    TexEnv* glTexEnv();
    TexGen* glTexGen();

    bool isEnabledTextureGenS() { return mEnableTexGenS ; }
    bool isEnabledTextureGenT() { return mEnableTexGenT ; }
    bool isEnabledTextureGenR() { return mEnableTexGenR ; }
    bool isEnabledTextureGenQ() { return mEnableTexGenQ ; }

    void enableTextureGenS() { mEnableTexGenS = true; }
    void enableTextureGenT() { mEnableTexGenT = true; }
    void enableTextureGenR() { mEnableTexGenR = true; }
    void enableTextureGenQ() { mEnableTexGenQ = true; }

    void disableTextureGenS() { mEnableTexGenS = false; }
    void disableTextureGenT() { mEnableTexGenT = false; }
    void disableTextureGenR() { mEnableTexGenR = false; }
    void disableTextureGenQ() { mEnableTexGenQ = false; }

    bool hasTexture() const { return mTexture && mTexture->handle(); }

  protected:
    ref<Texture> mTexture;
    ref<TexParameter> mGLTexParameter;
    ref<TexEnv> mGLTexEnv;
    ref<TexGen> mGLTexGen;
    ref<MatrixObject> mMatrix;
    bool mEnableTexGenS;
    bool mEnableTexGenT;
    bool mEnableTexGenR;
    bool mEnableTexGenQ;
  };

  class Shader: public Object
  {
  public:
    Shader();
    virtual ~Shader();

    void combine(Shader* parent);

    void applyShader(int thread);

    void reset();

    void setToDefaults();

    void shallowCopy(Shader* state) { *this = *state; }

    void enable(EEnable enable, EInheritance inher = InheritPublic);
    void disable(EEnable enable, EInheritance inher = InheritPublic);
    void resetEnable(EEnable enable);
    void getEnableStatus(EEnable enable, int& status, EInheritance& inher) const;

    bool isEnabled(EEnable enable) const;

    EInheritance inheritanceGLSLProgram();
    EInheritance inheritancePixelTransfer();
    EInheritance inheritanceHint();
    EInheritance inheritanceCullFace();
    EInheritance inheritanceFrontFace();
    EInheritance inheritanceDepthFunc();

    EInheritance inheritanceDepthMask();
    EInheritance inheritanceColorMask();
    EInheritance inheritancePolygonMode();
    EInheritance inheritanceShadeModel();

    EInheritance inheritanceBlendEquation();
    EInheritance inheritanceAlphaFunc();
    EInheritance inheritanceMaterial();
    EInheritance inheritanceColorMaterial();
    EInheritance inheritanceLightModel();

    EInheritance inheritanceFog();
    EInheritance inheritancePolygonOffset();
    EInheritance inheritanceLogicOp();
    EInheritance inheritanceDepthRange();
    EInheritance inheritanceLineWidth();

    EInheritance inheritancePointSize();
    EInheritance inheritanceLineStipple();
    EInheritance inheritancePolygonStipple();

    EInheritance inheritancePointParameter();

    EInheritance inheritanceStencilFunc();
    EInheritance inheritanceStencilOp();
    EInheritance inheritanceStencilMask();

    EInheritance inheritanceBlendColor();

    EInheritance inheritanceClipPlane();
    EInheritance inheritanceScissor();

    EInheritance inheritanceBlendFuncSeparate();
    EInheritance inheritanceSampleCoverage();
    EInheritance inheritance_TextureUnit(int unit);

    void setInheritanceGLSLProgram(EInheritance inher);
    void setInheritancePixelTransfer(EInheritance inher);
    void setInheritanceHint(EInheritance inher);
    void setInheritanceCullFace(EInheritance inher);
    void setInheritanceFrontFace(EInheritance inher);
    void setInheritanceDepthFunc(EInheritance inher);
    void setInheritanceDepthMask(EInheritance inher);
    void setInheritanceColorMask(EInheritance inher);
    void setInheritancePolygonMode(EInheritance inher);
    void setInheritanceShadeModel(EInheritance inher);

    void setInheritanceBlendEquation(EInheritance inher);
    void setInheritanceAlphaFunc(EInheritance inher);
    void setInheritanceMaterial(EInheritance inher);
    void setInheritanceColorMaterial(EInheritance inher);
    void setInheritanceLightModel(EInheritance inher);
    void setInheritanceFog(EInheritance inher);
    void setInheritancePolygonOffset(EInheritance inher);
    void setInheritanceLogicOp(EInheritance inher);
    void setInheritanceDepthRange(EInheritance inher);
    void setInheritanceLineWidth(EInheritance inher);
    void setInheritancePointSize(EInheritance inher);
    void setInheritanceLineStipple(EInheritance inher);
    void setInheritancePolygonStipple(EInheritance inher);

    void setInheritancePointParameter(EInheritance inher);
    void setInheritanceStencilFunc(EInheritance inher);
    void setInheritanceStencilOp(EInheritance inher);
    void setInheritanceStencilMask(EInheritance inher);

    void setInheritanceBlendColor(EInheritance inher);

    void setInheritanceScissor(EInheritance inher);

    void setInheritanceBlendFuncSeparate(EInheritance inher);
    void setInheritanceSampleCoverage(EInheritance inher);
    void setInheritance_TextureUnit(int unit, EInheritance inher);

    GLSLProgram* glslProgram(bool create=true);
    bool hasGLSLProgram() const;
    PixelTransfer* glPixelTransfer(bool create=true);
    Hint* glHint(bool create=true);
    CullFace* glCullFace(bool create=true);
    FrontFace* glFrontFace(bool create=true);
    DepthFunc* glDepthFunc(bool create=true);
    DepthMask* glDepthMask(bool create=true);
    ColorMask* glColorMask(bool create=true);
    GLPolygonMode* glPolygonMode(bool create=true);
    ShadeModel* glShadeModel(bool create=true);

    BlendEquation* glBlendEquation(bool create=true);
    AlphaFunc* glAlphaFunc(bool create=true);
    Material* glMaterial(bool create=true);
    ColorMaterial* glColorMaterial(bool create=true);
    LightModel* glLightModel(bool create=true);
    Fog* glFog(bool create=true);
    PolygonOffset* glPolygonOffset(bool create=true);
    LogicOp* glLogicOp(bool create=true);
    DepthRange* glDepthRange(bool create=true);
    LineWidth* glLineWidth(bool create=true);
    PointSize* glPointSize(bool create=true);
    LineStipple* glLineStipple(bool create=true);
    PolygonStipple* glPolygonStipple(bool create=true);

    PointParameter* glPointParameter(bool create=true);
    StencilFunc* glStencilFunc(bool create=true);
    StencilOp* glStencilOp(bool create=true);
    StencilMask* glStencilMask(bool create=true);

    BlendColor* glBlendColor(bool create=true);

    Scissor* glScissor(bool create=true);

    BlendFunc* glBlendFunc(bool create=true);
    SampleCoverage* glSampleCoverage(bool create=true);
    TextureUnit* textureUnit(int texunit, bool create=true);

    void setGLSLProgram(GLSLProgram* gl, EInheritance inher=InheritPublic);
    void setPixelTransfer(PixelTransfer* gl, EInheritance inher=InheritPublic);
    void setHint(Hint* gl, EInheritance inher=InheritPublic);
    void setCullFace(CullFace* gl, EInheritance inher=InheritPublic);
    void setFrontFace(FrontFace* gl, EInheritance inher=InheritPublic);
    void setDepthFunc(DepthFunc* gl, EInheritance inher=InheritPublic);
    void setDepthMask(DepthMask* gl, EInheritance inher=InheritPublic);
    void setColorMask(ColorMask* gl, EInheritance inher=InheritPublic);
    void setPolygonMode(GLPolygonMode* gl, EInheritance inher=InheritPublic);
    void setShadeModel(ShadeModel* gl, EInheritance inher=InheritPublic);

    void setBlendEquation(BlendEquation* gl, EInheritance inher=InheritPublic);
    void setAlphaFunc(AlphaFunc* gl, EInheritance inher=InheritPublic);
    void setMaterial(Material* gl, EInheritance inher=InheritPublic);
    void setColorMaterial(ColorMaterial* gl, EInheritance inher=InheritPublic);
    void setLightModel(LightModel* gl, EInheritance inher=InheritPublic);
    void setFog(Fog* gl, EInheritance inher=InheritPublic);
    void setPolygonOffset(PolygonOffset* gl, EInheritance inher=InheritPublic);
    void setLogicOp(LogicOp* gl, EInheritance inher=InheritPublic);
    void setDepthRange(DepthRange* gl, EInheritance inher=InheritPublic);
    void setLineWidth(LineWidth* gl, EInheritance inher=InheritPublic);
    void setPointSize(PointSize* gl, EInheritance inher=InheritPublic);
    void setLineStipple(LineStipple* gl, EInheritance inher=InheritPublic);
    void setPolygonStipple(PolygonStipple* gl, EInheritance inher=InheritPublic);

    void setPointParameter(PointParameter* gl, EInheritance inher=InheritPublic);
    void setStencilFunc(StencilFunc* gl, EInheritance inher=InheritPublic);
    void setStencilOp(StencilOp* gl, EInheritance inher=InheritPublic);
    void setStencilMask(StencilMask* gl, EInheritance inher=InheritPublic);

    void setBlendColor(BlendColor* gl, EInheritance inher=InheritPublic);

    void setScissor(Scissor* gl, EInheritance inher=InheritPublic);

    void setBlendFuncSeparate(BlendFunc* gl, EInheritance inher=InheritPublic);
    void setSampleCoverage(SampleCoverage* gl, EInheritance inher=InheritPublic);
    void setTextureUnit(int i, TextureUnit* texunit, EInheritance inher=InheritPublic);

    bool textureUnitEnabled(int tex_unit) const { return mTextureUnitEnabled[tex_unit]; }
    void setTextureUnitEnabled(int tex_unit, bool enabled) { mTextureUnitEnabled[tex_unit] = enabled; }
    
    void setInheritParentLights(bool inherit) { mInheritParentLights = inherit; }
    bool inheritParentLights() const { return mInheritParentLights; }

    void addLight(Light* light);
    void setLight(int index, Light* light) { mLights[index] = light; }
    Light* light(int index) { return mLights[index].get(); }
    Light* lastLight() { return mLights.back().get(); }
    int lightCount() const { return (int)mLights.size(); }
    const std::vector< ref<Light> >& lights() const;
    void removeLight(Light* light);
    void removeAllLights();
    
    void setInheritParentPlanes(bool inherit) { mInheritParentPlanes = inherit; }
    bool inheritParentPlanes() const { return mInheritParentPlanes; }

    void addPlane(Plane* plane);
    void setPlane(int index, Plane* plane) { mPlanes[index] = plane; }
    Plane* plane(int index) { return mPlanes[index].get(); }
    Plane* lastPlane() { return mPlanes.back().get(); }
    int planeCount() const { return (int)mPlanes.size(); }
    const std::vector< ref<Plane> >& planes() const;
    void removePlane(Plane* plane);
    void removeAllPlanes();

    double lastUpdateTime() const { return mLastUpdate; }
    void setLastUpdateTime(double time) { mLastUpdate = time; }
    virtual void update(Camera*, double /*delta_t*/) {}

    bool getDepthMask() const
    {
      if (!mGLDepthMask.mState)
        return true;
      else
        return mGLDepthMask.mState->depthMask();
    }

    bool sortByGLSLProgram(Shader* shader) const
    {
      return mGLSLProgram.mState.get() < shader->mGLSLProgram.mState.get();
    }

    bool sortByMaterial(Shader* shader) const
    {

      if ( !mGLMaterial.mState || !shader->mGLMaterial.mState )
        return mGLMaterial.mState != 0; // render this first if it has a material
      else // both have a material
        return mGLMaterial.mState < shader->mGLMaterial.mState;
    }

    bool sortByTextUnits(Shader* shader) const
    {

      if ( (mTextureUnit[0].mState->texture() == 0) || (shader->mTextureUnit[0].mState->texture() == 0) )
        return mTextureUnit[0].mState->texture() != 0; // if one of the two doesn't have a texture draw "this" first if it has one
      else // both have a texture
      if ( mTextureUnit[0].mState->texture()->handle() != shader->mTextureUnit[0].mState->texture()->handle() )
        return mTextureUnit[0].mState->texture()->handle() < shader->mTextureUnit[0].mState->texture()->handle();
      else 
      if ( mTextureUnit[0].mState != shader->mTextureUnit[0].mState || MAX_TEXTURE_UNITS < 2)
        return mTextureUnit[0].mState.get() < shader->mTextureUnit[0].mState.get();
      else

      if ( (mTextureUnit[1].mState->texture() == 0) || (shader->mTextureUnit[1].mState->texture() == 0) )
        return mTextureUnit[1].mState->texture() != 0; // if one of the two doesn't have a texture draw "this" first if it has one
      else // both have a texture
      if ( mTextureUnit[1].mState->texture()->handle() != shader->mTextureUnit[1].mState->texture()->handle() )
        return mTextureUnit[1].mState->texture()->handle() < shader->mTextureUnit[1].mState->texture()->handle();
      else 
      if ( mTextureUnit[1].mState != shader->mTextureUnit[1].mState || MAX_TEXTURE_UNITS < 3)
        return mTextureUnit[1].mState.get() < shader->mTextureUnit[1].mState.get();
      else

      if ( (mTextureUnit[2].mState->texture() == 0) || (shader->mTextureUnit[2].mState->texture() == 0) )
        return mTextureUnit[2].mState->texture() != 0; // if one of the two doesn't have a texture draw "this" first if it has one
      else // both have a texture
      if ( mTextureUnit[2].mState->texture()->handle() != shader->mTextureUnit[2].mState->texture()->handle() )
        return mTextureUnit[2].mState->texture()->handle() < shader->mTextureUnit[2].mState->texture()->handle();
      else

        return mTextureUnit[2].mState.get() < shader->mTextureUnit[2].mState.get();
    }

  protected:
    template <class T>
    class StateWrapper
    {
    public:
      void clear() { mInheritance = InheritPublic; mState = NULL; }
      void setToDefaults() { mInheritance = InheritPublic; mState = new T; }
      T* getOrCreateState(bool create) { if (!mState && create) mState = new T; return mState.get(); }
      StateWrapper() { mInheritance = InheritPublic; }
      ref<T> mState;
      EInheritance mInheritance;
    };

  protected:

    template <class T> void inherit(StateWrapper<T>& io_child, const StateWrapper<T>& parent);

    template <class T>
    void applyGLState( const StateWrapper<T>, T* default_state, ref<T>& io_cur_state ) const;
    void applyTexture( int texunit, bool enabled, const StateWrapper<TextureUnit>, TextureUnit* default_state, ref<TextureUnit>& io_cur_state, bool& io_cur_enabled ) const;
    void applyEnable(int enable, Shader* thread_current, const Shader* thread_default) const;
    void inherit(int enable, Shader* parent);

  protected:
    double mLastUpdate;

    StateWrapper<GLSLProgram> mGLSLProgram;
    StateWrapper<PixelTransfer> mGLPixelTransfer;
    StateWrapper<Hint> mGLHint;
    StateWrapper<CullFace> mGLCullFace;
    StateWrapper<FrontFace> mGLFrontFace;
    StateWrapper<DepthFunc> mGLDepthFunc;
    StateWrapper<DepthMask> mGLDepthMask;
    StateWrapper<ColorMask> mGLColorMask;
    StateWrapper<GLPolygonMode> mGLPolygonMode;
    StateWrapper<ShadeModel> mGLShadeModel;

    StateWrapper<BlendEquation> mGLBlendEquation;
    StateWrapper<AlphaFunc> mGLAlphaFunc;
    StateWrapper<Material> mGLMaterial;
    StateWrapper<ColorMaterial> mGLColorMaterial;
    StateWrapper<LightModel> mGLLightModel;
    StateWrapper<Fog> mGLFog;
    StateWrapper<PolygonOffset> mGLPolygonOffset;
    StateWrapper<LogicOp> mGLLogicOp;
    StateWrapper<DepthRange> mGLDepthRange;
    StateWrapper<LineWidth> mGLLineWidth;
    StateWrapper<PointSize> mGLPointSize;
    StateWrapper<LineStipple> mGLLineStipple;
    StateWrapper<PolygonStipple> mGLPolygonStipple;

    StateWrapper<PointParameter> mGLPointParameter;
    StateWrapper<StencilFunc> mGLStencilFunc;
    StateWrapper<StencilOp> mGLStencilOp;
    StateWrapper<StencilMask> mGLStencilMask;

    StateWrapper<BlendColor> mGLBlendColor;

    StateWrapper<Scissor> mGLScissor;

    StateWrapper<BlendFunc> mGLBlendFuncSeparate;
    StateWrapper<SampleCoverage> mGLSampleCoverage;

    StateWrapper<TextureUnit> mTextureUnit[MAX_TEXTURE_UNITS];
    bool mTextureUnitEnabled[MAX_TEXTURE_UNITS];

    char mEnable[ENABLE_COUNT];

    EInheritance mEnableInheritance[ENABLE_COUNT];

    std::vector< ref<Light> > mLights;
    bool mInheritParentLights;

    std::vector< ref<Plane> > mPlanes;
    bool mInheritParentPlanes;
  };
}

#endif
