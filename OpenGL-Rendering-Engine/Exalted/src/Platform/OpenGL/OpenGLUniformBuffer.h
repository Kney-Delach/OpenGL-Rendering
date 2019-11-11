/***************************************************************************
 * Filename		: OpenGLUniformBuffer.h
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Uniform buffer implementation using OpenGL.
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
#include "Core/Renderer/UniformBuffer.h"

namespace Exalted
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(Bytes& bufferSize);
		virtual ~OpenGLUniformBuffer();
		uint32_t GetRendererID() const override { return m_RendererID; }
		void Bind() const override; //todo: Possible bug? Intellisense states not being declared. 
		void Unbind() const override;
		void SetBufferSubData(Bytes& offset, Bytes& size, const void* data) const override;
		void BindBufferRange(const uint32_t blockBindingIndex, const Bytes& offset, const Bytes& size) const override;
	private:
		uint32_t m_RendererID = 0;
	};
}