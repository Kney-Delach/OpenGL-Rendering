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
	};
}