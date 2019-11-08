/***************************************************************************
 * Filename		: Mesh.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation which decides which rendering api to use.
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
#include "Mesh.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLMesh.h"

namespace Exalted
{
	Ref<Mesh> Mesh::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLMesh>();
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}