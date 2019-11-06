/***************************************************************************
 * Filename		: HeightMap.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: An abstract interface declaration for a mesh height map
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
#include "HeightMap.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLHeightMap.h"

namespace Exalted
{
	Ref<HeightMap> HeightMap::Create(const std::string path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLHeightMap>(path);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}