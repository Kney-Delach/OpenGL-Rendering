/***************************************************************************
 * Filename		: OpenGLFrameBuffer.h
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: Implements the OpenGL Framebuffer wrapper
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "Core/Renderer/FrameBuffer.h"

#pragma once

namespace Exalted
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height); // framebuffer for point light shadows
		OpenGLFrameBuffer(uint32_t width, uint32_t height, FrameBufferFormat format);
		OpenGLFrameBuffer(uint32_t width, uint32_t height, bool generateBorderColor); // framebuffer for directional light shadows
		virtual ~OpenGLFrameBuffer();
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UnbindMiniFrame() const override;
		virtual void BindTexture(uint32_t slot = 0) const override;
		virtual void BindTextureCube(uint32_t slot = 0) const override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual uint32_t GetRenderBufferAttachmentRendererID() const override { return m_RenderBufferID; }
		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual FrameBufferFormat GetFormat() const { return m_Format; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width;
		uint32_t m_Height;
		FrameBufferFormat m_Format;
		uint32_t m_ColorAttachment;
		uint32_t m_RenderBufferID;
	};
}