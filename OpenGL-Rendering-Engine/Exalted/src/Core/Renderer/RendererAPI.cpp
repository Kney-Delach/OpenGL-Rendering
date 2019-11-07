/***************************************************************************
 * Filename		: RendererAPI.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Assigns current rendering api to OpenGL.
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
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Exalted 
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
