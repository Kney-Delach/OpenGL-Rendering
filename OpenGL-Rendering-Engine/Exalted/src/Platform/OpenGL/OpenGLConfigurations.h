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

namespace Exalted
{
	enum PolygonMode {POINT = 0x1B00, LINE = 0x1B01, FILL = 0x1B02};
	enum class DepthMode 
	{
		NEVER = 0x0200, LESS = 0x0201, EQUAL = 0x0202, 
		LESS_EQUAL = 0x0203, GREATER = 0x0204, NOT_EQUAL = 0x0205, 
		GREATER_EQUAL = 0x0206, ALWAYS = 0x0207
	};
	enum class StencilAction
	{
		ALWAYS = GL_ALWAYS,				// Stencil value is always replaced.
		NOT_EQUAL = GL_NOTEQUAL,		// Stencil value is only replaced when not equal.
		KEEP = GL_KEEP,					// The currently stored stencil value is kept.
		ZERO = GL_ZERO,					// The stencil value is set to 0.
		REPLACE = GL_REPLACE,			// The stencil value is replaced with the reference value set with glStencilFunc.
		INVERT = GL_INVERT,				// Bitwise inverts the current stencil buffer value.
		INCREMENT = GL_INCR,			// The stencil value is increased by 1 if it is lower than the maximum value.
		INCREMENT_WRAP = GL_INCR_WRAP,	// Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
		DECREMENT = GL_DECR,			// The stencil value is decreased by 1 if it is higher than the minimum value.
		DECREMENT_WRAP = GL_DECR_WRAP	// Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
	};
	class OpenGLConfigurations
	{
	public:
		OpenGLConfigurations() = default;
		~OpenGLConfigurations() = default;

		static inline void SetPolygonMode(PolygonMode polygonMode) { glPolygonMode(GL_FRONT_AND_BACK, polygonMode); }
		static inline void SetPointSize(unsigned int pointSize) { if(pointSize > 0) glPointSize(static_cast<GLfloat>(pointSize)); }

		static inline void EnableDepthTesting() { glEnable(GL_DEPTH_TEST); }
		static inline void DisableDepthTesting() { glDisable(GL_DEPTH_TEST); }
		static inline void SetDepthFunction(DepthMode depthMode) { glDepthFunc(static_cast<GLenum>(depthMode)); }

		static inline void EnableStencilTesting() { glEnable(GL_STENCIL_TEST); }
		static inline void DisableStencilTesting() { glDisable(GL_STENCIL_TEST); }
		static inline void SetStencilFunction(StencilAction successAction, unsigned refValue, unsigned mask) { glStencilFunc(static_cast<GLenum>(successAction), refValue, mask); }
		static inline void SetStencilActions(StencilAction stencilFail, StencilAction depthFail, StencilAction bothFail) { glStencilOp(static_cast<GLenum>(stencilFail), static_cast<GLenum>(depthFail), static_cast<GLenum>(bothFail)); }
		static inline void SetStencilMaskReadOnly() { glStencilMask(0x00); }
		static inline void SetStencilMaskWriteALL() { glStencilMask(0xFF); }
		static inline void SetStencilMaskCustom(unsigned value) { glStencilMask(value); }
	};
}