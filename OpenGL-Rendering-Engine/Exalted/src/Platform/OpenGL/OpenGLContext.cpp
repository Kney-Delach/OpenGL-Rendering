/***************************************************************************
 * Filename		: OpenGLContext.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implements the OpenGL Context handling interface.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Exalted
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EX_CORE_ASSERT(m_WindowHandle, "Window handle is null!"); 
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		const int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); 
		EX_CORE_ASSERT(status, "Failed to initialize Glad!");
		OpenGLContext::GetInfo();
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLContext::GetInfo()
	{
		EX_CORE_INFO("\n-----------------------------------------\nRendering Information:\n GPU Vendor	: {0}\n GPU Renderer	: {1}\n GPU Driver	: Version {2}\n-----------------------------------------", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}
}
