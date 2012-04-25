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

#include "vl/Shader.hpp"
#include "vl/OpenGL.hpp"
#include "vl/GlobalState.hpp"
#include "vl/Log.hpp"
#include "vl/Say.hpp"

using namespace vl;

namespace {
  GLenum EnablesTable[] = 
  {
    GL_ALPHA_TEST,
    GL_BLEND,
    GL_COLOR_LOGIC_OP,
    GL_LIGHTING,
    GL_COLOR_MATERIAL,
    GL_COLOR_SUM,
    GL_CULL_FACE,
    GL_DEPTH_TEST,
    GL_FOG ,
    GL_LINE_SMOOTH,
    GL_LINE_STIPPLE, 
    GL_POLYGON_STIPPLE, 
    GL_MULTISAMPLE,
    GL_NORMALIZE,
    GL_POINT_SMOOTH,
    GL_POINT_SPRITE,
    GL_POLYGON_SMOOTH,
    GL_POLYGON_OFFSET_FILL,
    GL_POLYGON_OFFSET_LINE,
    GL_POLYGON_OFFSET_POINT,
    GL_RESCALE_NORMAL,
    GL_SCISSOR_TEST,
    GL_STENCIL_TEST,
    GL_VERTEX_PROGRAM_POINT_SIZE,
    GL_VERTEX_PROGRAM_TWO_SIDE
  };
}

Shader::Shader(void)
{
  mLastUpdate = -1.0f;
  reset();
}

Shader::~Shader(void)
{
}

void Shader::enable(EEnable enable, EInheritance inher)
{ 
  CHECK(enable>=0 && enable<ENABLE_COUNT)
  mEnable[enable] = 1; 
  mEnableInheritance[enable] = inher; 
}

void Shader::disable(EEnable enable, EInheritance inher)
{ 
  CHECK(enable>=0 && enable<ENABLE_COUNT)
  mEnable[enable] = 0; 
  mEnableInheritance[enable] = inher; 
}

void Shader::resetEnable(EEnable enable)
{
  mEnable[enable] = -1;
  mEnableInheritance[enable] = InheritPublic;
}

bool Shader::isEnabled(EEnable enable) const
{
  return mEnable[enable] == 1;
}

void Shader::getEnableStatus(EEnable enable, int& status, EInheritance& inher) const
{
  status = mEnable[enable];
  inher = mEnableInheritance[enable];
}

template <class T>
void Shader::inherit(StateWrapper<T>& io_child, const StateWrapper<T>& parent)
{

  int who = 0;

  if (io_child.mState == NULL)
  {
    if (parent.mState == NULL || parent.mInheritance == InheritHidden)
      who = 0;
    else
      who = 2;
  }

  else
  if (io_child.mInheritance == InheritHidden)
  {
    if (parent.mState == NULL || parent.mInheritance == InheritHidden)
      who = 0;
    else
      who = 2;
  }

  else
  if (io_child.mInheritance == InheritPublic)
  {
    if (parent.mState == NULL)
      who = 1;
    else
    if (parent.mInheritance == InheritOverride || parent.mInheritance == InheritOverrideProtected)
      who = 2;
    else
      who = 1;
  }

  else
  if (io_child.mInheritance == InheritOverride)
  {
    if (parent.mState == NULL)
      who = 1;
    else
    if (parent.mInheritance == InheritOverride || parent.mInheritance == InheritOverrideProtected)
      who = 2;
    else
      who = 1;
  }

  else
  if (io_child.mInheritance == InheritProtected)
  {
      who = 1;
  }

  else
  if (io_child.mInheritance == InheritOverrideProtected)
  {
      who = 1;
  }
  else
  {
    CHECK(0);
  }

  switch(who)
  {

    case 0: io_child.mState = NULL; break;

    case 1: ; break;

    case 2: io_child = parent; break;
  }
}

void Shader::inherit(int enable, Shader* parent)
{

  int who = 0;

  if (mEnable[enable] == -1)
  {
    if (parent->mEnable[enable] == -1 || parent->mEnableInheritance[enable] == InheritHidden)
      who = 0;
    else
      who = 2;
  }

  else
  if (mEnableInheritance[enable] == InheritHidden)
  {
    if (parent->mEnable[enable] == -1 || parent->mEnableInheritance[enable] == InheritHidden)
      who = 0;
    else
      who = 2;
  }

  else
  if (mEnableInheritance[enable] == InheritPublic)
  {
    if (parent == NULL)
      who = 1;
    else
    if (parent->mEnableInheritance[enable] == InheritOverride || parent->mEnableInheritance[enable] == InheritOverrideProtected)
      who = 2;
    else
      who = 1;
  }

  else
  if (mEnableInheritance[enable] == InheritOverride)
  {
    if (parent == NULL)
      who = 1;
    else
    if (parent->mEnableInheritance[enable] == InheritOverride || parent->mEnableInheritance[enable] == InheritOverrideProtected)
      who = 2;
    else
      who = 1;
  }

  else
  if (mEnableInheritance[enable] == InheritProtected)
  {
      who = 1;
  }

  else
  if (mEnableInheritance[enable] == InheritOverrideProtected)
  {
      who = 1;
  }
  else
  {
    CHECK(0);
  }

  switch(who)
  {

    case 0: mEnable[enable] = -1; break;

    case 1: ; break;

    case 2: mEnable[enable] = parent->mEnable[enable]; mEnableInheritance[enable] = parent->mEnableInheritance[enable]; break;
  }
}

void Shader::combine(Shader* parent)
{

  for (int i=0; i<MAX_TEXTURE_UNITS; i++)
    inherit(mTextureUnit[i], parent->mTextureUnit[i]);

  for (int i=0; i<ENABLE_COUNT; i++)
    inherit(i, parent);

  inherit(mGLSLProgram, parent->mGLSLProgram);
  inherit(mGLPixelTransfer, parent->mGLPixelTransfer);
  inherit(mGLHint, parent->mGLHint);
  inherit(mGLCullFace, parent->mGLCullFace);
  inherit(mGLFrontFace, parent->mGLFrontFace);
  inherit(mGLDepthFunc, parent->mGLDepthFunc);

  inherit(mGLDepthMask, parent->mGLDepthMask);
  inherit(mGLColorMask, parent->mGLColorMask);
  inherit(mGLPolygonMode, parent->mGLPolygonMode);
  inherit(mGLShadeModel, parent->mGLShadeModel);

  inherit(mGLBlendEquation, parent->mGLBlendEquation);

  inherit(mGLAlphaFunc, parent->mGLAlphaFunc);
  inherit(mGLMaterial, parent->mGLMaterial);
  inherit(mGLColorMaterial, parent->mGLColorMaterial);
  inherit(mGLLightModel, parent->mGLLightModel);
  inherit(mGLFog, parent->mGLFog);

  inherit(mGLPolygonOffset, parent->mGLPolygonOffset);
  inherit(mGLLogicOp, parent->mGLLogicOp);
  inherit(mGLDepthRange, parent->mGLDepthRange);
  inherit(mGLLineWidth, parent->mGLLineWidth);
  inherit(mGLPointSize, parent->mGLPointSize);

  inherit(mGLLineStipple, parent->mGLLineStipple);
  inherit(mGLPolygonStipple, parent->mGLPolygonStipple);

  inherit(mGLPointParameter, parent->mGLPointParameter);
  inherit(mGLStencilFunc, parent->mGLStencilFunc);

  inherit(mGLStencilOp, parent->mGLStencilOp);
  inherit(mGLStencilMask, parent->mGLStencilMask);

  inherit(mGLBlendColor, parent->mGLBlendColor);

  inherit(mGLScissor, parent->mGLScissor);

  inherit(mGLBlendFuncSeparate, parent->mGLBlendFuncSeparate);
  inherit(mGLSampleCoverage, parent->mGLSampleCoverage);  
}

template <class T>
void Shader::applyGLState( const StateWrapper<T> state, T* default_state, ref<T>& io_cur_state ) const
{
  CHECK(default_state);
  T* final = NULL;

  if (state.mState == NULL || state.mInheritance == InheritHidden)
    final = default_state;
  else
    final = state.mState.get();

  if (final != io_cur_state) // delta setup :)))
  {
    final->applyGLState();
    io_cur_state = final;
  }
}

void Shader::applyTexture( int texunit, bool enabled, const StateWrapper<TextureUnit> state, TextureUnit* default_state, ref<TextureUnit>& io_cur_state, bool& io_cur_enabled ) const
{
  CHECK(default_state);
  TextureUnit* final = NULL;

  if (state.mState == NULL || state.mInheritance == InheritHidden)
    final = default_state;
  else
    final = state.mState.get();

  if (final != io_cur_state || enabled != io_cur_enabled) // delta setup :)))
  {
    final->applyTexture(texunit, enabled);
    io_cur_state = final;
    io_cur_enabled = enabled;
  }
}

void Shader::applyEnable(int enable, Shader* thread_current, const Shader* thread_default) const
{
  CHECK(enable>=0 && enable<ENABLE_COUNT)

  char on;
  if ( mEnable[enable] == -1 || mEnableInheritance[enable] == InheritHidden )
    on = thread_default->mEnable[enable];
  else
    on = mEnable[enable];

  CHECK(on != -1);

  if ( on != thread_current->mEnable[enable] )
  {
    thread_current->mEnable[enable] = on;
    if ( on )
      glEnable( EnablesTable[enable] );
    else
      glDisable( EnablesTable[enable] );

  }
}

void Shader::applyShader(int render_stream)
{
  GLCHECK4()

  Shader* thread_current = GlobalState::renderStream(render_stream)->currentShader();
  const Shader* thread_default = GlobalState::renderStream(render_stream)->defaultShader();

  int itex = GlobalState::renderStream(render_stream)->maxTextureUnits();
  while(itex--)
    applyTexture( itex, textureUnitEnabled(itex), mTextureUnit[itex], thread_default->mTextureUnit[itex].mState.get(), thread_current->mTextureUnit[itex].mState, thread_current->mTextureUnitEnabled[itex] );

  GLCHECK4()

  applyGLState(mGLSLProgram, thread_default->mGLSLProgram.mState.get(), thread_current->mGLSLProgram.mState);
  applyGLState(mGLPixelTransfer, thread_default->mGLPixelTransfer.mState.get(), thread_current->mGLPixelTransfer.mState);
  applyGLState(mGLHint, thread_default->mGLHint.mState.get(), thread_current->mGLHint.mState);
  applyGLState(mGLCullFace, thread_default->mGLCullFace.mState.get(), thread_current->mGLCullFace.mState);
  applyGLState(mGLFrontFace, thread_default->mGLFrontFace.mState.get(), thread_current->mGLFrontFace.mState);

    applyGLState(mGLDepthFunc, thread_default->mGLDepthFunc.mState.get(), thread_current->mGLDepthFunc.mState);

  applyGLState(mGLDepthMask, thread_default->mGLDepthMask.mState.get(), thread_current->mGLDepthMask.mState);
  applyGLState(mGLColorMask, thread_default->mGLColorMask.mState.get(), thread_current->mGLColorMask.mState);
  applyGLState(mGLPolygonMode, thread_default->mGLPolygonMode.mState.get(), thread_current->mGLPolygonMode.mState);
  applyGLState(mGLShadeModel, thread_default->mGLShadeModel.mState.get(), thread_current->mGLShadeModel.mState);

  applyGLState(mGLBlendEquation, thread_default->mGLBlendEquation.mState.get(), thread_current->mGLBlendEquation.mState);

  applyGLState(mGLAlphaFunc, thread_default->mGLAlphaFunc.mState.get(), thread_current->mGLAlphaFunc.mState);

  {

      applyGLState(mGLColorMaterial, thread_default->mGLColorMaterial.mState.get(), thread_current->mGLColorMaterial.mState);
    applyGLState(mGLLightModel, thread_default->mGLLightModel.mState.get(), thread_current->mGLLightModel.mState);
  }

    applyGLState(mGLFog, thread_default->mGLFog.mState.get(), thread_current->mGLFog.mState);

    applyGLState(mGLPolygonOffset, thread_default->mGLPolygonOffset.mState.get(), thread_current->mGLPolygonOffset.mState);

    applyGLState(mGLLogicOp, thread_default->mGLLogicOp.mState.get(), thread_current->mGLLogicOp.mState);

  applyGLState(mGLDepthRange, thread_default->mGLDepthRange.mState.get(), thread_current->mGLDepthRange.mState);
  applyGLState(mGLLineWidth, thread_default->mGLLineWidth.mState.get(), thread_current->mGLLineWidth.mState);
  applyGLState(mGLPointSize, thread_default->mGLPointSize.mState.get(), thread_current->mGLPointSize.mState);

    applyGLState(mGLLineStipple, thread_default->mGLLineStipple.mState.get(), thread_current->mGLLineStipple.mState);

    applyGLState(mGLPolygonStipple, thread_default->mGLPolygonStipple.mState.get(), thread_current->mGLPolygonStipple.mState);

  applyGLState(mGLPointParameter, thread_default->mGLPointParameter.mState.get(), thread_current->mGLPointParameter.mState);
  applyGLState(mGLStencilFunc, thread_default->mGLStencilFunc.mState.get(), thread_current->mGLStencilFunc.mState);

    applyGLState(mGLStencilOp, thread_default->mGLStencilOp.mState.get(), thread_current->mGLStencilOp.mState);

  applyGLState(mGLStencilMask, thread_default->mGLStencilMask.mState.get(), thread_current->mGLStencilMask.mState);
  applyGLState(mGLBlendColor, thread_default->mGLBlendColor.mState.get(), thread_current->mGLBlendColor.mState);

  applyGLState(mGLBlendFuncSeparate, thread_default->mGLBlendFuncSeparate.mState.get(), thread_current->mGLBlendFuncSeparate.mState);

  applyGLState(mGLScissor, thread_default->mGLScissor.mState.get(), thread_current->mGLScissor.mState);
  applyGLState(mGLSampleCoverage, thread_default->mGLSampleCoverage.mState.get(), thread_current->mGLSampleCoverage.mState);

  bool update_material = glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE;

  for (int i=0; i<ENABLE_COUNT; i++)
    applyEnable(i, thread_current, thread_default);
	
  CLEAR_GL_ERROR()

  if (update_material)
    thread_current->setMaterial(NULL);

  applyGLState(mGLMaterial, thread_default->mGLMaterial.mState.get(), thread_current->mGLMaterial.mState);
}

void Shader::reset()
{
  
  mInheritParentLights = true;

  mInheritParentPlanes = true;

  memset( mEnable, 0xFF/*-1*/, ENABLE_COUNT * sizeof(char) );

  memset( mEnableInheritance, 0, ENABLE_COUNT * sizeof(EInheritance) );

  for(int i=0; i<MAX_TEXTURE_UNITS; i++)
  {
    mTextureUnit[i].clear();
    mTextureUnitEnabled[i] = true;
  }

  mGLSLProgram.clear();
  mGLPixelTransfer.clear();
  mGLHint.clear();
  mGLCullFace.clear();
  mGLFrontFace.clear();
  mGLDepthFunc.clear();

  mGLDepthMask.clear();
  mGLColorMask.clear();
  mGLPolygonMode.clear();
  mGLShadeModel.clear();

  mGLBlendEquation.clear();

  mGLAlphaFunc.clear();
  mGLMaterial.clear();
  mGLColorMaterial.clear();
  mGLLightModel.clear();
  mGLFog.clear();

  mGLPolygonOffset.clear();
  mGLLogicOp.clear();
  mGLDepthRange.clear();
  mGLLineWidth.clear();
  mGLPointSize.clear();

  mGLLineStipple.clear();
  mGLPolygonStipple.clear();

  mGLPointParameter.clear();
  mGLStencilFunc.clear();

  mGLStencilOp.clear();
  mGLStencilMask.clear();

  mGLBlendColor.clear();

  mGLScissor.clear();

  mGLBlendFuncSeparate.clear();
  mGLSampleCoverage.clear();
}

void Shader::setToDefaults()
{
  mLights.clear();

  memset( mEnable, 0, ENABLE_COUNT * sizeof(char) );

  memset( mEnableInheritance, 0, ENABLE_COUNT * sizeof(EInheritance) );

  for(int i=0; i<MAX_TEXTURE_UNITS; i++)
  {
    mTextureUnit[i].setToDefaults();
    mTextureUnitEnabled[i] = true;
  }

  mGLSLProgram.setToDefaults();
  mGLPixelTransfer.setToDefaults();
  mGLHint.setToDefaults();
  mGLCullFace.setToDefaults();
  mGLFrontFace.setToDefaults();
  mGLDepthFunc.setToDefaults();

  mGLDepthMask.setToDefaults();
  mGLColorMask.setToDefaults();
  mGLPolygonMode.setToDefaults();
  mGLShadeModel.setToDefaults();

  mGLBlendEquation.setToDefaults();

  mGLAlphaFunc.setToDefaults();
  mGLMaterial.setToDefaults();
  mGLColorMaterial.setToDefaults();
  mGLLightModel.setToDefaults();
  mGLFog.setToDefaults();

  mGLPolygonOffset.setToDefaults();
  mGLLogicOp.setToDefaults();
  mGLDepthRange.setToDefaults();
  mGLLineWidth.setToDefaults();
  mGLPointSize.setToDefaults();

  mGLLineStipple.setToDefaults();
  mGLPolygonStipple.setToDefaults();

  mGLPointParameter.setToDefaults();
  mGLStencilFunc.setToDefaults();

  mGLStencilOp.setToDefaults();
  mGLStencilMask.setToDefaults();

  mGLBlendColor.setToDefaults();

  mGLScissor.setToDefaults();

  mGLBlendFuncSeparate.setToDefaults();
  mGLSampleCoverage.setToDefaults();
}

EInheritance Shader::inheritanceGLSLProgram() { return mGLSLProgram.mInheritance; }

EInheritance Shader::inheritancePixelTransfer() { return mGLPixelTransfer.mInheritance; }

EInheritance Shader::inheritanceHint() { return mGLHint.mInheritance; }

EInheritance Shader::inheritanceCullFace() { return mGLCullFace.mInheritance; }

EInheritance Shader::inheritanceFrontFace() { return mGLFrontFace.mInheritance; }

EInheritance Shader::inheritanceDepthFunc() { return mGLDepthFunc.mInheritance; }

EInheritance Shader::inheritanceDepthMask() { return mGLDepthMask.mInheritance; }

EInheritance Shader::inheritanceColorMask() { return mGLColorMask.mInheritance; }

EInheritance Shader::inheritancePolygonMode() { return mGLPolygonMode.mInheritance; }

EInheritance Shader::inheritanceShadeModel() { return mGLShadeModel.mInheritance; }

EInheritance Shader::inheritanceBlendEquation() { return mGLBlendEquation.mInheritance; }

EInheritance Shader::inheritanceAlphaFunc() { return mGLAlphaFunc.mInheritance; }

EInheritance Shader::inheritanceMaterial() { return mGLMaterial.mInheritance; }

EInheritance Shader::inheritanceColorMaterial() { return mGLColorMaterial.mInheritance; }

EInheritance Shader::inheritanceLightModel() { return mGLLightModel.mInheritance; }

EInheritance Shader::inheritanceFog() { return mGLFog.mInheritance; }

EInheritance Shader::inheritancePolygonOffset() { return mGLPolygonOffset.mInheritance; }

EInheritance Shader::inheritanceLogicOp() { return mGLLogicOp.mInheritance; }

EInheritance Shader::inheritanceDepthRange() { return mGLDepthRange.mInheritance; }

EInheritance Shader::inheritanceLineWidth() { return mGLLineWidth.mInheritance; }

EInheritance Shader::inheritancePointSize() { return mGLPointSize.mInheritance; }

EInheritance Shader::inheritanceLineStipple() { return mGLLineStipple.mInheritance; }

EInheritance Shader::inheritancePolygonStipple() { return mGLPolygonStipple.mInheritance; }

EInheritance Shader::inheritancePointParameter() { return mGLPointParameter.mInheritance; }

EInheritance Shader::inheritanceStencilFunc() { return mGLStencilFunc.mInheritance; }

EInheritance Shader::inheritanceStencilOp() { return mGLStencilOp.mInheritance; }

EInheritance Shader::inheritanceStencilMask() { return mGLStencilMask.mInheritance; }

EInheritance Shader::inheritanceBlendColor() { return mGLBlendColor.mInheritance; }

EInheritance Shader::inheritanceScissor() { return mGLScissor.mInheritance; }

EInheritance Shader::inheritanceBlendFuncSeparate() { return mGLBlendFuncSeparate.mInheritance; }

EInheritance Shader::inheritanceSampleCoverage() { return mGLSampleCoverage.mInheritance; }

EInheritance Shader::inheritance_TextureUnit(int unit) { return mTextureUnit[unit].mInheritance; }

void Shader::setInheritanceGLSLProgram(EInheritance inher) { mGLSLProgram.mInheritance = inher; }

void Shader::setInheritancePixelTransfer(EInheritance inher) { mGLPixelTransfer.mInheritance = inher; }

void Shader::setInheritanceHint(EInheritance inher) { mGLHint.mInheritance = inher; }

void Shader::setInheritanceCullFace(EInheritance inher) { mGLCullFace.mInheritance = inher; }

void Shader::setInheritanceFrontFace(EInheritance inher) { mGLFrontFace.mInheritance = inher; }

void Shader::setInheritanceDepthFunc(EInheritance inher) { mGLDepthFunc.mInheritance = inher; }

void Shader::setInheritanceDepthMask(EInheritance inher) { mGLDepthMask.mInheritance = inher; }

void Shader::setInheritanceColorMask(EInheritance inher) { mGLColorMask.mInheritance = inher; }

void Shader::setInheritancePolygonMode(EInheritance inher) { mGLPolygonMode.mInheritance = inher; }

void Shader::setInheritanceShadeModel(EInheritance inher) { mGLShadeModel.mInheritance = inher; }

void Shader::setInheritanceBlendEquation(EInheritance inher) { mGLBlendEquation.mInheritance = inher; }

void Shader::setInheritanceAlphaFunc(EInheritance inher) { mGLAlphaFunc.mInheritance = inher; }

void Shader::setInheritanceMaterial(EInheritance inher) { mGLMaterial.mInheritance = inher; }

void Shader::setInheritanceColorMaterial(EInheritance inher) { mGLColorMaterial.mInheritance = inher; }

void Shader::setInheritanceLightModel(EInheritance inher) { mGLLightModel.mInheritance = inher; }

void Shader::setInheritanceFog(EInheritance inher) { mGLFog.mInheritance = inher; }

void Shader::setInheritancePolygonOffset(EInheritance inher) { mGLPolygonOffset.mInheritance = inher; }

void Shader::setInheritanceLogicOp(EInheritance inher) { mGLLogicOp.mInheritance = inher; }

void Shader::setInheritanceDepthRange(EInheritance inher) { mGLDepthRange.mInheritance = inher; }

void Shader::setInheritanceLineWidth(EInheritance inher) { mGLLineWidth.mInheritance = inher; }

void Shader::setInheritancePointSize(EInheritance inher) { mGLPointSize.mInheritance = inher; }

void Shader::setInheritanceLineStipple(EInheritance inher) { mGLLineStipple.mInheritance = inher; }

void Shader::setInheritancePolygonStipple(EInheritance inher) { mGLPolygonStipple.mInheritance = inher; }

void Shader::setInheritancePointParameter(EInheritance inher) { mGLPointParameter.mInheritance = inher; }

void Shader::setInheritanceStencilFunc(EInheritance inher) { mGLStencilFunc.mInheritance = inher; }

void Shader::setInheritanceStencilOp(EInheritance inher) { mGLStencilOp.mInheritance = inher; }

void Shader::setInheritanceStencilMask(EInheritance inher) { mGLStencilMask.mInheritance = inher; }

void Shader::setInheritanceBlendColor(EInheritance inher) { mGLBlendColor.mInheritance = inher; }

void Shader::setInheritanceScissor(EInheritance inher) { mGLScissor.mInheritance = inher; }

void Shader::setInheritanceBlendFuncSeparate(EInheritance inher) { mGLBlendFuncSeparate.mInheritance = inher; }

void Shader::setInheritanceSampleCoverage(EInheritance inher) { mGLSampleCoverage.mInheritance = inher; }

void Shader::setInheritance_TextureUnit(int unit, EInheritance inher) { mTextureUnit[unit].mInheritance = inher; }

GLSLProgram* Shader::glslProgram(bool create) { return mGLSLProgram.getOrCreateState(create); }
bool Shader::hasGLSLProgram() const { return mGLSLProgram.mState != NULL; }

PixelTransfer* Shader::glPixelTransfer(bool create) { return mGLPixelTransfer.getOrCreateState(create); }

Hint* Shader::glHint(bool create) { return mGLHint.getOrCreateState(create); }

CullFace* Shader::glCullFace(bool create) { return mGLCullFace.getOrCreateState(create); }

FrontFace* Shader::glFrontFace(bool create) { return mGLFrontFace.getOrCreateState(create); }

DepthFunc* Shader::glDepthFunc(bool create) { return mGLDepthFunc.getOrCreateState(create); }

DepthMask* Shader::glDepthMask(bool create) { return mGLDepthMask.getOrCreateState(create); }

ColorMask* Shader::glColorMask(bool create) { return mGLColorMask.getOrCreateState(create); }

GLPolygonMode* Shader::glPolygonMode(bool create) { return mGLPolygonMode.getOrCreateState(create); }

ShadeModel* Shader::glShadeModel(bool create) { return mGLShadeModel.getOrCreateState(create); }

BlendEquation* Shader::glBlendEquation(bool create) { return mGLBlendEquation.getOrCreateState(create); }

AlphaFunc* Shader::glAlphaFunc(bool create) { return mGLAlphaFunc.getOrCreateState(create); }

Material* Shader::glMaterial(bool create) { return mGLMaterial.getOrCreateState(create); }

ColorMaterial* Shader::glColorMaterial(bool create) { return mGLColorMaterial.getOrCreateState(create); }

LightModel* Shader::glLightModel(bool create) { return mGLLightModel.getOrCreateState(create); }

Fog* Shader::glFog(bool create) { return mGLFog.getOrCreateState(create); }

PolygonOffset* Shader::glPolygonOffset(bool create) { return mGLPolygonOffset.getOrCreateState(create); }

LogicOp* Shader::glLogicOp(bool create) { return mGLLogicOp.getOrCreateState(create); }

DepthRange* Shader::glDepthRange(bool create) { return mGLDepthRange.getOrCreateState(create); }

LineWidth* Shader::glLineWidth(bool create) { return mGLLineWidth.getOrCreateState(create); }

PointSize* Shader::glPointSize(bool create) { return mGLPointSize.getOrCreateState(create); }

LineStipple* Shader::glLineStipple(bool create) { return mGLLineStipple.getOrCreateState(create); }

PolygonStipple* Shader::glPolygonStipple(bool create) { return mGLPolygonStipple.getOrCreateState(create); }

PointParameter* Shader::glPointParameter(bool create) { return mGLPointParameter.getOrCreateState(create); }

StencilFunc* Shader::glStencilFunc(bool create) { return mGLStencilFunc.getOrCreateState(create); }

StencilOp* Shader::glStencilOp(bool create) { return mGLStencilOp.getOrCreateState(create); }

StencilMask* Shader::glStencilMask(bool create) { return mGLStencilMask.getOrCreateState(create); }

BlendColor* Shader::glBlendColor(bool create) { return mGLBlendColor.getOrCreateState(create); }

Scissor* Shader::glScissor(bool create) { return mGLScissor.getOrCreateState(create); }

BlendFunc* Shader::glBlendFunc(bool create) { return mGLBlendFuncSeparate.getOrCreateState(create); }

SampleCoverage* Shader::glSampleCoverage(bool create) { return mGLSampleCoverage.getOrCreateState(create); }

TextureUnit* Shader::textureUnit(int i, bool create) { return mTextureUnit[i].getOrCreateState(create); }

void Shader::setGLSLProgram(GLSLProgram* state, EInheritance inher) { mGLSLProgram.mState = state; mGLSLProgram.mInheritance= inher; }

void Shader::setPixelTransfer(PixelTransfer* state, EInheritance inher) { mGLPixelTransfer.mState = state; mGLPixelTransfer.mInheritance= inher; }

void Shader::setHint(Hint* state, EInheritance inher) { mGLHint.mState = state; mGLHint.mInheritance= inher; }

void Shader::setCullFace(CullFace* state, EInheritance inher) { mGLCullFace.mState = state; mGLCullFace.mInheritance= inher; }

void Shader::setFrontFace(FrontFace* state, EInheritance inher) { mGLFrontFace.mState = state; mGLFrontFace.mInheritance= inher; }

void Shader::setDepthFunc(DepthFunc* state, EInheritance inher) { mGLDepthFunc.mState = state; mGLDepthFunc.mInheritance= inher; }

void Shader::setDepthMask(DepthMask* state, EInheritance inher) { mGLDepthMask.mState = state; mGLDepthMask.mInheritance= inher; }

void Shader::setColorMask(ColorMask* state, EInheritance inher) { mGLColorMask.mState = state; mGLColorMask.mInheritance= inher; }

void Shader::setPolygonMode(GLPolygonMode* state, EInheritance inher) { mGLPolygonMode.mState = state; mGLPolygonMode.mInheritance= inher; }

void Shader::setShadeModel(ShadeModel* state, EInheritance inher) { mGLShadeModel.mState = state; mGLShadeModel.mInheritance= inher; }

void Shader::setBlendEquation(BlendEquation* state, EInheritance inher) { mGLBlendEquation.mState = state; mGLBlendEquation.mInheritance= inher; }

void Shader::setAlphaFunc(AlphaFunc* state, EInheritance inher) { mGLAlphaFunc.mState = state; mGLAlphaFunc.mInheritance= inher; }

void Shader::setMaterial(Material* state, EInheritance inher) { mGLMaterial.mState = state; mGLMaterial.mInheritance= inher; }

void Shader::setColorMaterial(ColorMaterial* state, EInheritance inher) { mGLColorMaterial.mState = state; mGLColorMaterial.mInheritance= inher; }

void Shader::setLightModel(LightModel* state, EInheritance inher) { mGLLightModel.mState = state; mGLLightModel.mInheritance= inher; }

void Shader::setFog(Fog* state, EInheritance inher) { mGLFog.mState = state; mGLFog.mInheritance= inher; }

void Shader::setPolygonOffset(PolygonOffset* state, EInheritance inher) { mGLPolygonOffset.mState = state; mGLPolygonOffset.mInheritance= inher; }

void Shader::setLogicOp(LogicOp* state, EInheritance inher) { mGLLogicOp.mState = state; mGLLogicOp.mInheritance= inher; }

void Shader::setDepthRange(DepthRange* state, EInheritance inher) { mGLDepthRange.mState = state; mGLDepthRange.mInheritance= inher; }

void Shader::setLineWidth(LineWidth* state, EInheritance inher) { mGLLineWidth.mState = state; mGLLineWidth.mInheritance= inher; }

void Shader::setPointSize(PointSize* state, EInheritance inher) { mGLPointSize.mState = state; mGLPointSize.mInheritance= inher; }

void Shader::setLineStipple(LineStipple* state, EInheritance inher) { mGLLineStipple.mState = state; mGLLineStipple.mInheritance= inher; }

void Shader::setPolygonStipple(PolygonStipple* state, EInheritance inher) { mGLPolygonStipple.mState = state; mGLPolygonStipple.mInheritance= inher; }

void Shader::setPointParameter(PointParameter* state, EInheritance inher) { mGLPointParameter.mState = state; mGLPointParameter.mInheritance= inher; }

void Shader::setStencilFunc(StencilFunc* state, EInheritance inher) { mGLStencilFunc.mState = state; mGLStencilFunc.mInheritance= inher; }

void Shader::setStencilOp(StencilOp* state, EInheritance inher) { mGLStencilOp.mState = state; mGLStencilOp.mInheritance= inher; }

void Shader::setStencilMask(StencilMask* state, EInheritance inher) { mGLStencilMask.mState = state; mGLStencilMask.mInheritance= inher; }

void Shader::setBlendColor(BlendColor* state, EInheritance inher) { mGLBlendColor.mState = state; mGLBlendColor.mInheritance= inher; }

void Shader::setScissor(Scissor* state, EInheritance inher) { mGLScissor.mState = state; mGLScissor.mInheritance= inher; }

void Shader::setBlendFuncSeparate(BlendFunc* state, EInheritance inher) { mGLBlendFuncSeparate.mState = state; mGLBlendFuncSeparate.mInheritance= inher; }

void Shader::setSampleCoverage(SampleCoverage* state, EInheritance inher) { mGLSampleCoverage.mState = state; mGLSampleCoverage.mInheritance= inher; }

void Shader::setTextureUnit(int i, TextureUnit* texunit, EInheritance inher) { mTextureUnit[i].mState = texunit; mTextureUnit[i].mInheritance= inher; }

void PixelTransfer::applyGLState()
{
  glPixelTransferi(GL_MAP_COLOR, mapColor() ? GL_TRUE : GL_FALSE);
  glPixelTransferi(GL_MAP_STENCIL, mapStencil() ? GL_TRUE : GL_FALSE);
  glPixelTransferi(GL_INDEX_SHIFT, indexShift() );
  glPixelTransferi(GL_INDEX_OFFSET, indexOffset() );
  glPixelTransferf(GL_RED_SCALE, redScale() );  
  glPixelTransferf(GL_GREEN_SCALE, greenScale() ); 
  glPixelTransferf(GL_BLUE_SCALE, blueScale() ); 
  glPixelTransferf(GL_ALPHA_SCALE, alphaScale() );
  glPixelTransferf(GL_DEPTH_SCALE, depthScale() );
  glPixelTransferf(GL_RED_BIAS, redBias() );
  glPixelTransferf(GL_GREEN_BIAS, greenBias() );  
  glPixelTransferf(GL_BLUE_BIAS, blueBias() );
  glPixelTransferf(GL_ALPHA_BIAS, alphaBias() ); 
  glPixelTransferf(GL_DEPTH_BIAS, depthBias() );
  GLCHECK4()
  if (GLEW_ARB_imaging)
  {
    glPixelTransferf(GL_POST_COLOR_MATRIX_RED_SCALE, postColorMatrixRedScale() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_GREEN_SCALE, postColorMatrixGreenScale() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_BLUE_SCALE, postColorMatrixBlueScale() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_ALPHA_SCALE, postColorMatrixAlphaScale() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_RED_BIAS, postColorMatrixRedBias() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_GREEN_BIAS, postColorMatrixGreenBias() ); 
    glPixelTransferf(GL_POST_COLOR_MATRIX_BLUE_BIAS, postColorMatrixBlueBias() );
    glPixelTransferf(GL_POST_COLOR_MATRIX_ALPHA_BIAS, postColorMatrixAlphaBias() );
    glPixelTransferf(GL_POST_CONVOLUTION_RED_SCALE, postConvolutionRedScale() );
    glPixelTransferf(GL_POST_CONVOLUTION_GREEN_SCALE, postConvolutionGreenScale() ); 
    glPixelTransferf(GL_POST_CONVOLUTION_BLUE_SCALE, postConvolutionBlueScale() );
    glPixelTransferf(GL_POST_CONVOLUTION_ALPHA_SCALE, postConvolutionAlphaScale() );
    glPixelTransferf(GL_POST_CONVOLUTION_RED_BIAS, postConvolutionRedBias() );
    glPixelTransferf(GL_POST_CONVOLUTION_GREEN_BIAS, postConvolutionGreenBias() );  
    glPixelTransferf(GL_POST_CONVOLUTION_BLUE_BIAS, postConvolutionBlueBias() );
    glPixelTransferf(GL_POST_CONVOLUTION_ALPHA_BIAS, postConvolutionAlphaBias() ); 
    GLCHECK4()
  }
}

void Hint::applyGLState()
{
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, mPerspectiveCorrectionHint);  GLCHECK4()
  glHint(GL_POLYGON_SMOOTH_HINT, mPolygonSmoothHint);  GLCHECK4()
  glHint(GL_LINE_SMOOTH_HINT, mLineSmoothHint);  GLCHECK4()
  glHint(GL_POINT_SMOOTH_HINT, mPointSmoothHint);  GLCHECK4()
  glHint(GL_FOG_HINT, mFogHint); GLCHECK4()
}

void CullFace::applyGLState()
{
  glCullFace(mFaceMode); GLCHECK4()
}

void FrontFace::applyGLState()
{
  glFrontFace(mFrontFace); GLCHECK4()
}

void DepthFunc::applyGLState()
{
  glDepthFunc(mDepthFunc); GLCHECK4()
}

void DepthMask::applyGLState()
{
  glDepthMask(mDepthMask?GL_TRUE:GL_FALSE); GLCHECK4()
}

void GLPolygonMode::applyGLState()
{
  glPolygonMode(GL_FRONT, mFrontFace); GLCHECK4()
  glPolygonMode(GL_BACK, mBackFace); GLCHECK4()
}

void ShadeModel::applyGLState()
{
  glShadeModel(mShadeModel); GLCHECK4()
}

void BlendFunc::applyGLState()
{
  if (GLEW_EXT_blend_func_separate)
  {
    glBlendFuncSeparate(mSrcRGB, mDstRGB, mSrcAlpha, mDstAlpha); GLCHECK4()
  }
  else
  {

    glBlendFunc(mSrcRGB, mDstRGB); GLCHECK4()
  }
}

void BlendEquation::applyGLState()
{
  if (GLEW_EXT_blend_minmax)
  {
    glBlendEquation(mBlendEquation); GLCHECK4()
  }
}

void AlphaFunc::applyGLState()
{
  glAlphaFunc(mAlphaFunc, mRefValue); GLCHECK4()
}

void ColorMaterial::applyGLState()
{
  glColorMaterial(mFace, mColor); GLCHECK4()
}

Material::Material()
{
  mFrontAmbient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
  mFrontDiffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
  mFrontSpecular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  mFrontEmission = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  mFrontShininess = 0;

  mBackAmbient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
  mBackDiffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
  mBackSpecular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  mBackEmission = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  mBackShininess = 0;
}

void Material::setTransparency(float alpha)
{
  mFrontAmbient.a()   = mBackAmbient.a()   = alpha;
  mFrontDiffuse.a()   = mBackDiffuse.a()   = alpha;
  mFrontSpecular.a()  = mBackSpecular.a()  = alpha;
  mFrontEmission.a()  = mBackEmission.a()  = alpha;
}

void Material::setFrontTransparency(float alpha)
{
  mFrontAmbient.a()  = alpha;
  mFrontDiffuse.a()  = alpha;
  mFrontSpecular.a() = alpha;
  mFrontEmission.a() = alpha;
}

void Material::setBackTransparency(float alpha)
{
  mBackAmbient.a()  = alpha;
  mBackDiffuse.a()  = alpha;
  mBackSpecular.a() = alpha;
  mBackEmission.a() = alpha;
}

void Material::setFrontFlatColor(const vec4& color)
{
  mFrontAmbient  = 0;
  mFrontDiffuse  = 0;
  mFrontSpecular = 0;
  mFrontEmission = color;
  mFrontShininess = 0;
  setFrontTransparency( color.a() );
}

void Material::setBackFlatColor(const vec4& color)
{
  mBackAmbient  = 0;
  mBackDiffuse  = 0;
  mBackSpecular = 0;
  mBackEmission = color;
  mBackShininess = 0;
  setBackTransparency( color.a() );
}

void Material::setFlatColor(const vec4& color)
{
  setFrontFlatColor(color);
  setBackFlatColor(color);
}

void Material::applyGLState()
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, mFrontAmbient.ptr()); GLCHECK4()
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mFrontDiffuse.ptr()); GLCHECK4()
  glMaterialfv(GL_FRONT, GL_SPECULAR, mFrontSpecular.ptr()); GLCHECK4()
  glMaterialfv(GL_FRONT, GL_EMISSION, mFrontEmission.ptr()); GLCHECK4()
  glMaterialf(GL_FRONT, GL_SHININESS, mFrontShininess); GLCHECK4()

  glMaterialfv(GL_BACK, GL_AMBIENT, mBackAmbient.ptr()); GLCHECK4()
  glMaterialfv(GL_BACK, GL_DIFFUSE, mBackDiffuse.ptr()); GLCHECK4()
  glMaterialfv(GL_BACK, GL_SPECULAR, mBackSpecular.ptr()); GLCHECK4()
  glMaterialfv(GL_BACK, GL_EMISSION, mBackEmission.ptr()); GLCHECK4()
  glMaterialf(GL_BACK, GL_SHININESS, mBackShininess); GLCHECK4()
}

void LightModel::applyGLState()
{
  if (GLEW_EXT_separate_specular_color)
  {
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, mColorControl); GLCHECK4()
  }
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mAmbientColor.ptr()); GLCHECK4()
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, mLocalViewer?1:0); GLCHECK4()
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, mTwoSide?1:0); GLCHECK4()
}

void Fog::applyGLState()
{
  glFogi(GL_FOG_MODE, mMode); GLCHECK4()
  glFogfv(GL_FOG_COLOR, mColor.ptr()); GLCHECK4()
  glFogf(GL_FOG_DENSITY, mDensity); GLCHECK4()
  glFogf(GL_FOG_START, mStart); GLCHECK4()
  glFogf(GL_FOG_END, mEnd); GLCHECK4()
}

void PolygonOffset::applyGLState()
{
  glPolygonOffset(mFactor, mUnits); GLCHECK4()
}

void LogicOp::applyGLState()
{
  glLogicOp(mLogicOp); GLCHECK4()
}

void DepthRange::applyGLState()
{
  glDepthRange(mZNear, mZFar); GLCHECK4()
}

void LineWidth::applyGLState()
{
  glLineWidth(mLineWidth); GLCHECK4()
}

void PointSize::applyGLState()
{
  glPointSize(mPointSize); GLCHECK4()
}

PolygonStipple::PolygonStipple()
{
  memset(mMask, 0xFF, sizeof(GLubyte)*32*32/8);
}

PolygonStipple::PolygonStipple(const GLubyte* mask)
{
  set(mask);
}

void PolygonStipple::set(const GLubyte* mask)
{
  memcpy(mMask, mask, sizeof(GLubyte)*32*32/8);
}

void PolygonStipple::applyGLState()
{
  glPolygonStipple(mask()); GLCHECK4()
}

void LineStipple::applyGLState()
{
  glLineStipple(mFactor, mPattern); GLCHECK4()
}

void PointParameter::applyGLState()
{
  if (GLEW_ARB_point_parameters)
  {
    glPointParameterf(GL_POINT_SIZE_MIN, mSizeMin); GLCHECK4()
    glPointParameterf(GL_POINT_SIZE_MAX, mSizeMax); GLCHECK4()
    glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, mFadeThresholdSize); GLCHECK4()
    glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, mDistanceAttenuation.ptr()); GLCHECK4()
  }
}

void StencilFunc::applyGLState()
{
  glStencilFunc(mFunction, mRefValue, mMask); GLCHECK4()
}

void StencilOp::applyGLState()
{
  glStencilOp(mSFail, mDpFail, mDpPass); GLCHECK4()
}

void StencilMask::applyGLState()
{
  glStencilMask(mMask); GLCHECK4()
}

void BlendColor::applyGLState()
{
  if (GLEW_EXT_blend_color)
  {
    glBlendColor(mBlendColor.r(), mBlendColor.g(), mBlendColor.b(), mBlendColor.a()); GLCHECK4()
  }
}

void ColorMask::applyGLState()
{
  glColorMask(mRed?GL_TRUE:GL_FALSE, mGreen?GL_TRUE:GL_FALSE, mBlue?GL_TRUE:GL_FALSE, mAlpha?GL_TRUE:GL_FALSE); GLCHECK4()
}

Scissor::Scissor()
{
  mX = 0;
  mY = 0;
  mWidth = 0;
  mHeight = 0;
}

void Scissor::applyGLState()
{
  if (mWidth && mHeight)
  {
    glScissor(mX,mY,mWidth,mHeight); GLCHECK4()
  }
}

void SampleCoverage::applyGLState()
{
  if (GLEW_ARB_multisample)
  {
    glSampleCoverage(mValue, mInvert?GL_TRUE:GL_FALSE); GLCHECK4()
  }
}

TexParameter::TexParameter()
{
  setMinFilter(TPF_LINEAR);
  setMagFilter(TPF_LINEAR);
  setWrapS(TPW_REPEAT);
  setWrapT(TPW_REPEAT);
  setWrapR(TPW_REPEAT);
  setBorderColor(vec4(0,0,0,0));
  setAnisotropy(1.0f);
}

void TexParameter::setMagFilter(ETexParamFilter magfilter) 
{ 
  #ifndef NDEBUG
  switch(magfilter)
  {
    case TPF_LINEAR:
    case TPF_NEAREST:
    {
      break;
    }
    default:
    {
      Log::bug("TexParameter::setMagFilter() accepts only the following values: OGL_LINEAR, OGL_NEAREST.\n");
      break;
    }
  }
  #endif
  CHECK(magfilter == TPF_LINEAR || magfilter == TPF_NEAREST); 
  mmagfilter = magfilter; 
}

void TexParameter::applyGLState(ETextureDimension dimension)
{
  glTexParameteri(dimension, GL_TEXTURE_MIN_FILTER, minFilter()); GLCHECK4()
  glTexParameteri(dimension, GL_TEXTURE_MAG_FILTER, magFilter()); GLCHECK4()
  glTexParameteri(dimension, GL_TEXTURE_WRAP_S, wrapS()); GLCHECK4()
  glTexParameteri(dimension, GL_TEXTURE_WRAP_T, wrapT()); GLCHECK4()
  if (GLEW_EXT_texture3D)
  {
    glTexParameteri(dimension, GL_TEXTURE_WRAP_R, wrapR()); GLCHECK4()
  }
  glTexParameterfv(dimension, GL_TEXTURE_BORDER_COLOR, borderColor().ptr()); GLCHECK4()
  if (GLEW_EXT_texture_filter_anisotropic)
    glTexParameterf( dimension, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy() ); GLCHECK4()

}

TexEnv::TexEnv()
{
  mMode = TEM_MODULATE;
  mColor = vec4(0,0,0,0);

  mRGBScale = 1.0f;
  mCombineRGB = TEM_REPLACE;
  mSource0RGB = TES_TEXTURE;
  mSource1RGB = TES_TEXTURE;
  mSource2RGB = TES_TEXTURE;
  mOperand0RGB = TEO_SRC_COLOR;
  mOperand1RGB = TEO_SRC_COLOR;
  mOperand2RGB = TEO_SRC_COLOR;

  mAlphaScale  = 1.0f;
  mCombineAlpha = TEM_REPLACE;
  mSource0Alpha = TES_TEXTURE;
  mSource1Alpha = TES_TEXTURE;
  mSource2Alpha = TES_TEXTURE;
  mOperand0Alpha = TEO_SRC_ALPHA;
  mOperand1Alpha = TEO_SRC_ALPHA;
  mOperand2Alpha = TEO_SRC_ALPHA;

  mLodBias = 0.0;
  mPointSpriteCoordReplace = false;
}

void TexEnv::applyGLState()
{
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode()); GLCHECK4()

  if (mode() == TEM_BLEND)
  {
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color().ptr()); GLCHECK4()
  }

  if (mode() == TEM_COMBINE && GLEW_EXT_texture_env_combine)
  {
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, rgbScale()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, combineRGB()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, source0RGB()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, source1RGB()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, operand0RGB()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, operand1RGB()); GLCHECK4()
    if (combineRGB() == TEM_INTERPOLATE)
    {
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, source2RGB()); GLCHECK4()
      glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, operand2RGB()); GLCHECK4()
    }

    glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE, alphaScale()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, combineAlpha()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, source0Alpha()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, source1Alpha()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, operand0Alpha()); GLCHECK4()
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, operand1Alpha()); GLCHECK4()
    if (combineAlpha() == TEM_INTERPOLATE)
    {
      glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, source2Alpha()); GLCHECK4()
      glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, operand2Alpha()); GLCHECK4()
    }
  }

  if (GLEW_ARB_point_sprite)
  {
    glTexEnvi( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, mPointSpriteCoordReplace ? GL_TRUE : GL_FALSE ); GLCHECK4()
  }

  if (GLEW_EXT_texture_lod_bias)
  {
    glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, mLodBias); GLCHECK4()
  }
}

TexGen::TexGen()
{
  mEyePlaneS = vec4(1,0,0,0);
  mObjectPlaneS = vec4(1,0,0,0);
  mEyePlaneT = vec4(0,1,0,0);
  mObjectPlaneT = vec4(0,1,0,0);
  mEyePlaneR = vec4(0,0,0,0);
  mObjectPlaneR = vec4(0,0,0,0);
  mEyePlaneQ = vec4(0,0,0,0);
  mObjectPlaneQ = vec4(0,0,0,0);
  mGenModeS = TGM_EYE_LINEAR;
  mGenModeT = TGM_EYE_LINEAR;
  mGenModeR = TGM_EYE_LINEAR;
  mGenModeQ = TGM_EYE_LINEAR;
}

void TexGen::applyGLState(bool s, bool t, bool r, bool q)
{
  if (s || t || r || q)
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); GLCHECK4();
    glLoadIdentity(); GLCHECK4();
  }

  if (s)
  {
    glEnable(GL_TEXTURE_GEN_S);
    glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, genModeS()); GLCHECK4()
    if (genModeS() == TGM_OBJECT_LINEAR) glTexGenfv(GL_S, GL_OBJECT_PLANE, objectPlaneS().ptr());
    if (genModeS() == TGM_EYE_LINEAR)    glTexGenfv(GL_S, GL_EYE_PLANE,       eyePlaneS().ptr());
    GLCHECK4()
  }
  else
    glDisable(GL_TEXTURE_GEN_S);

  if (t)
  {
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, genModeT()); GLCHECK4()
    if (genModeT() == TGM_OBJECT_LINEAR) glTexGenfv(GL_T, GL_OBJECT_PLANE, objectPlaneT().ptr());
    if (genModeT() == TGM_EYE_LINEAR)    glTexGenfv(GL_T, GL_EYE_PLANE,       eyePlaneT().ptr());
    GLCHECK4()
  }
  else
    glDisable(GL_TEXTURE_GEN_T);

  if (r)
  {
    glEnable(GL_TEXTURE_GEN_R);
    glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, genModeR()); GLCHECK4()
    if (genModeR() == TGM_OBJECT_LINEAR) glTexGenfv(GL_R, GL_OBJECT_PLANE, objectPlaneR().ptr());
    if (genModeR() == TGM_EYE_LINEAR)    glTexGenfv(GL_R, GL_EYE_PLANE,       eyePlaneR().ptr());
    GLCHECK4()
  }
  else
    glDisable(GL_TEXTURE_GEN_R);

  if (q)
  {
    glEnable(GL_TEXTURE_GEN_Q);
    glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, genModeQ()); GLCHECK4()
    if (genModeQ() == TGM_OBJECT_LINEAR) glTexGenfv(GL_Q, GL_OBJECT_PLANE, objectPlaneQ().ptr());
    if (genModeQ() == TGM_EYE_LINEAR)    glTexGenfv(GL_Q, GL_EYE_PLANE,       eyePlaneQ().ptr());
    GLCHECK4()
  }
  else
    glDisable(GL_TEXTURE_GEN_Q);

  if (s || t || r || q)
  {
    glPopMatrix(); GLCHECK4();
  }
}

TexParameter* TextureUnit::glTexParameter() 
{ 
  if (!mGLTexParameter) 
    mGLTexParameter = new TexParameter; 
  return mGLTexParameter.get(); 
}

TexEnv* TextureUnit::glTexEnv() 
{ 
  if (!mGLTexEnv) 
    mGLTexEnv=new TexEnv; 
  return mGLTexEnv.get(); 
}

TexGen* TextureUnit::glTexGen() 
{ 
  if (!mGLTexGen) 
    mGLTexGen=new TexGen; 
  return mGLTexGen.get(); 
}

void TextureUnit::applyTexture(GLint texunit, bool enabled)
{
  GLCHECK4()

  if (GLEW_ARB_multitexture)
  {
    glActiveTexture(GL_TEXTURE0 + texunit); GLCHECK4()
  }
  else

  if ( texunit > 0 )
    return;

  glDisable( GL_TEXTURE_1D ); GLCHECK4() 
  glDisable( GL_TEXTURE_2D ); GLCHECK4()
  if (GLEW_EXT_texture3D)
  {
    glDisable( GL_TEXTURE_3D ); GLCHECK4()
  }
  if (GLEW_EXT_texture_cube_map)
  {
    glDisable( GL_TEXTURE_CUBE_MAP ); GLCHECK4()
  }

  glBindTexture( GL_TEXTURE_1D, 0 ); GLCHECK4()
  glBindTexture( GL_TEXTURE_2D, 0 ); GLCHECK4()
  if (GLEW_EXT_texture3D)
  {
    glBindTexture( GL_TEXTURE_3D, 0 ); GLCHECK4()
  }
  if (GLEW_EXT_texture_cube_map)
  {
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ); GLCHECK4()
  }

  if( enabled && mTexture && mTexture->handle() )
  {
    CHECK(mTexture)
    CHECK(mTexture->handle())

    #ifndef NDEBUG

    if ( !texture()->hasMipMaps() )
    {
      switch(glTexParameter()->minFilter())
      {
        default:
        break;

        case TPF_LINEAR_MIPMAP_LINEAR:
        case TPF_LINEAR_MIPMAP_NEAREST:
        case TPF_NEAREST_MIPMAP_LINEAR:
        case TPF_NEAREST_MIPMAP_NEAREST:
        {
          Log::bug("Requested mipmapping texture filtering on a Texture object without mipmaps.\n");
          CHECK(0);
          break;
        }
      }
    }

    #endif

    glEnable( texture()->dimension() ); GLCHECK4()

    glBindTexture( texture()->dimension(), texture()->handle() ); GLCHECK4()

    glTexParameter()->applyGLState( texture()->dimension() );

    glTexEnv()->applyGLState();

    glTexGen()->applyGLState( isEnabledTextureGenS(), isEnabledTextureGenT(), isEnabledTextureGenR(), isEnabledTextureGenQ() );

    glMatrixMode(GL_TEXTURE); GLCHECK4()
    if (matrix())
      glLoadMatrixd( matrix()->mMatrix.ptr() );
    else
      glLoadIdentity();
    GLCHECK4()
  }

}

void Shader::addLight(Light* light) { mLights.push_back(light); }

const std::vector< ref<Light> >& Shader::lights() const { return mLights; }

void Shader::removeAllLights() { mLights.clear(); }

void Shader::removeLight(Light* light)
{
  for(int i=(int)mLights.size() ; i-- ; )
  {
    if (mLights[i] == light)
    {
      mLights.erase(mLights.begin() + i);
      return;
    }
  }
}

void Shader::addPlane(Plane* plane) { mPlanes.push_back(plane); }

const std::vector< ref<Plane> >& Shader::planes() const { return mPlanes; }

void Shader::removeAllPlanes() { mPlanes.clear(); }

void Shader::removePlane(Plane* plane)
{
  for(int i=(int)mPlanes.size() ; i-- ; )
  {
    if (mPlanes[i] == plane)
    {
      mPlanes.erase(mPlanes.begin() + i);
      return;
    }
  }
}
