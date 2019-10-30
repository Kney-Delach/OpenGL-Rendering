/***************************************************************************
 * Filename		: OpenGLContext.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declares the OpenGL Context handling interface.
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
#include "Core/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Exalted
{
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		OpenGLContext() = delete;
		~OpenGLContext() = default;
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		virtual void GetInfo() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
