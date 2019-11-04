/***************************************************************************
 * Filename		: RenderingContext.cpp
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Implements the Rendering context, calls platform specific creation.
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
#include "RenderingContext.h"
#include "Core/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Exalted
{
	Scope<RenderingContext> RenderingContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}