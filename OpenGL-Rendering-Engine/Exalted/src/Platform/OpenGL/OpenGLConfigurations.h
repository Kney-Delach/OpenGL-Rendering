/***************************************************************************
 * Filename		: OpenGLConfigurations.h
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: This class contains some helpful config functionality, 
 *                it should be made into an interface and inherited at some point.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once
#include <glad/glad.h>

//todo: Implement an abstract interface in the renderer for these configurations.
//todo: Document polygon mode. 
//todo: Document Depth mode.

namespace Exalted
{
#define GL_ENUM(x) static_cast<GLenum>(x)

	enum PolygonMode
	{
		POINT = 0x1B00, LINE = 0x1B01, FILL = 0x1B02
	};
	enum class DepthMode 
	{
		NEVER = 0x0200, LESS = 0x0201, EQUAL = 0x0202, 
		LESS_EQUAL = 0x0203, GREATER = 0x0204, NOT_EQUAL = 0x0205, 
		GREATER_EQUAL = 0x0206, ALWAYS = 0x0207
	};
	enum class StencilAction
	{
		ALWAYS			= GL_ALWAYS,	// Stencil value is always replaced.
		EQUAL			= GL_EQUAL,		// Stencil only kept when equal.
		NOT_EQUAL		= GL_NOTEQUAL,	// Stencil value is only replaced when not equal.
		KEEP			= GL_KEEP,		// The currently stored stencil value is kept.
		ZERO			= GL_ZERO,		// The stencil value is set to 0.
		REPLACE			= GL_REPLACE,	// The stencil value is replaced with the reference value set with glStencilFunc.
		INVERT			= GL_INVERT,	// Bitwise inverts the current stencil buffer value.
		INCREMENT		= GL_INCR,		// The stencil value is increased by 1 if it is lower than the maximum value.
		INCREMENT_WRAP	= GL_INCR_WRAP,	// Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
		DECREMENT		= GL_DECR,		// The stencil value is decreased by 1 if it is higher than the minimum value.
		DECREMENT_WRAP	= GL_DECR_WRAP	// Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
	};
	enum class BlendFactors
	{
		ZERO						= GL_ZERO,						// Factor is equal to 0. 
		ONE							= GL_ONE,						// Factor is equal to 1.

		SOURCE						= GL_SRC_COLOR,					// Factor is equal to source color.
		SOURCE_MINUS				= GL_ONE_MINUS_SRC_COLOR,		// Factor is equal to 1 - source color.
		SOURCE_ALPHA				= GL_SRC_ALPHA,					// Factor is equal to alpha component of source color.
		SOURCE_ALPHA_MINUS			= GL_ONE_MINUS_SRC_ALPHA,		// Factor is equal to 1 - alpha component of source color.

		DESTINATION					= GL_DST_COLOR,					// Factor is equal to destination color.
		DESTINATION_MINUS			= GL_ONE_MINUS_DST_COLOR,		// Factor is equal to 1 - destination color.
		DESTINATION_ALPHA			= GL_DST_ALPHA,					// Factor is equal to alpha component of destination color.
		DESTINATION_ALPHA_MINUS		= GL_ONE_MINUS_DST_ALPHA,		// Factor is equal to 1 - alpha component of destination color.

		CONSTANT					= GL_CONSTANT_COLOR,			// Factor is equal to the constant color vector assigned through glBlendColor (SetBlendConstantColor below).
		CONSTANT_MINUS				= GL_ONE_MINUS_CONSTANT_COLOR,	// Factor is equal to 1 - the costant color.
		CONSTANT_ALPHA				= GL_CONSTANT_ALPHA,			// Factor is equal to the alpha component of the constant color.
		CONSTANT_ALPHA_MINUS		= GL_ONE_MINUS_CONSTANT_ALPHA	// Factor is equal to 1 - the alpha component of the constant color.
	};
	enum class BlendMode
	{
		ADD					= GL_FUNC_ADD,				// Adds both components together.
		SUBTRACT			= GL_FUNC_SUBTRACT,			// Subtracts both components from each other.
		REVERSE_SUBTRACT	= GL_FUNC_REVERSE_SUBTRACT	// Subtracts both components but reverses the order.
	};

	enum class FaceCullMode
	{
		BACK = GL_BACK,
		FRONT = GL_FRONT
	};
	class OpenGLConfigurations
	{
	public:
		OpenGLConfigurations() = default;
		~OpenGLConfigurations() = default;

		static inline void SetViewport(float offsetX, float offsetY, float width, float height) { glViewport(static_cast<GLint>(offsetX), static_cast<GLint>(offsetY), static_cast<GLsizei>(width), static_cast<GLsizei>(height)); }

		static inline void SetPolygonMode(PolygonMode polygonMode) { glPolygonMode(GL_FRONT_AND_BACK, polygonMode); }
		static inline void SetPointSize(unsigned int pointSize) { if(pointSize > 0) glPointSize(static_cast<GLfloat>(pointSize)); }

		static inline void EnableDepthTesting() { glEnable(GL_DEPTH_TEST); }
		static inline void DisableDepthTesting() { glDisable(GL_DEPTH_TEST); }
		static inline void SetDepthFunction(DepthMode depthMode) { glDepthFunc(GL_ENUM(depthMode)); }
		static inline void SetDepthMask(bool value) { glDepthMask(value); }

		static inline void EnableStencilTesting() { glEnable(GL_STENCIL_TEST); }
		static inline void DisableStencilTesting() { glDisable(GL_STENCIL_TEST); }
		static inline void SetStencilFunction(StencilAction successAction, unsigned refValue, unsigned mask) { glStencilFunc(GL_ENUM(successAction), refValue, mask); }
		static inline void SetStencilActions(StencilAction stencilFail, StencilAction depthFail, StencilAction bothFail) { glStencilOp(GL_ENUM(stencilFail), GL_ENUM(depthFail), GL_ENUM(bothFail)); }
		static inline void SetStencilMaskReadOnly() { glStencilMask(0x00); }
		static inline void SetStencilMaskWriteALL() { glStencilMask(0xFF); }
		static inline void SetStencilMaskCustom(unsigned value) { glStencilMask(value); }

		static inline void SetColorMask(bool red, bool green, bool blue, bool alpha) { glColorMask(red, green, blue, alpha);  }

		static inline void EnableScissorTesting() { glEnable(GL_SCISSOR_TEST); }
		static inline void DisableScissorTesting() { glDisable(GL_SCISSOR_TEST); }
		static inline void SetScissorBox(int x, int y, int width, int height) { glScissor(x, y, width, height); }

		static inline void EnableBlending() { glEnable(GL_BLEND); }
		static inline void DisableBlending() { glDisable(GL_BLEND); }
		static inline void SetBlendFunction(BlendFactors sourceFactor, BlendFactors destinationFactor) { glBlendFunc(GL_ENUM(sourceFactor), GL_ENUM(destinationFactor)); }
		static inline void SetBlendFunctionSeparate(BlendFactors sourceFactorRGB, BlendFactors destinationFactorRGB, BlendFactors sourceFactorAlpha, BlendFactors destinationFactorAlpha) { glBlendFuncSeparate(GL_ENUM(sourceFactorRGB), GL_ENUM(destinationFactorRGB), GL_ENUM(sourceFactorAlpha), GL_ENUM(destinationFactorAlpha)); }
		static inline void SetBlendConstantColor(float red, float green, float blue, float alpha = 1.0f) { glBlendColor(red, green, blue, alpha); }
		static inline void SetBlendEquation(BlendMode blendMode) { glBlendEquation(GL_ENUM(blendMode)); }

		static inline void EnableFaceCulling() { glEnable(GL_CULL_FACE); }
		static inline void DisableFaceCulling() { glDisable(GL_CULL_FACE); }
		static inline void SetFaceCullingMode(FaceCullMode cullMode) { glCullFace(GL_ENUM(cullMode)); }

		static inline void SetPatchVerticeCount(unsigned count = 4) { glPatchParameteri(GL_PATCH_VERTICES, count); }
	};
}