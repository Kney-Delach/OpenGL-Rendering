/***************************************************************************
 * Filename		: FrameBuffer.cpp
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: Decalres the creation of an insatnce of a Framebuffer 
 *                wrapper for the exalted engine.
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
#include "FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Exalted
{
	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(width, height);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height, FrameBufferFormat format)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(width, height, format);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height, bool generateBorderColor)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(width, height, generateBorderColor);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}