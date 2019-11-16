/***************************************************************************
 * Filename		: FrameBuffer.h
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: Decalres the Framebuffer wrapper for the exalted engine.
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
#include "RendererAPI.h"

//todo: Implement an OnEvent function for resizing.
namespace Exalted
{
	enum class FrameBufferFormat
	{
		NONE = 0,
		RGBA8 = 1,
		RGBA16F = 2
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void UnbindMiniFrame() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void BindTexture(uint32_t slot = 0) const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetRenderBufferAttachmentRendererID() const = 0;
	public:
		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height, FrameBufferFormat format);
		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height, bool generateBorderColor);
	};
}