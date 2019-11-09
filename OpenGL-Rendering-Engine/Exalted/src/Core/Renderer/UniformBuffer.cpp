/***************************************************************************
 * Filename		: UniformBuffer.cpp
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: A wrapper for uniform buffers for the exalted engine.
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
#include "UniformBuffer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "RendererAPI.h"

namespace Exalted
{
	Ref<UniformBuffer> UniformBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLUniformBuffer>();
		}
		return nullptr;
	}
}
