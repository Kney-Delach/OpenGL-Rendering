/***************************************************************************
 * Filename		: OpenGLUniformBuffer.cpp
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
#include "expch.h"
#include "OpenGLUniformBuffer.h"

#include "glad/glad.h"

namespace Exalted
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(signed long long int& bufferSize)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW); //todo: Make this configurable (static/dynamic)
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::SetBufferSubData(Bytes& offset, Bytes& size, const void* data) const
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
	void OpenGLUniformBuffer::BindBufferRange(const uint32_t blockBindingIndex, const Bytes& offset, const Bytes& size) const
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, blockBindingIndex, m_RendererID, offset, size);
	}
}