/***************************************************************************
 * Filename		: OpenGLVertexArray.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: The implementation of vertex arrays for OpenGL.
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
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

//todo: @AddVertexBuffer: Add vertex array reference to vertex buffer for potential dynamic updating of vertex buffer (note: remove const)

namespace Exalted 
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Exalted::ShaderDataType::Float:    return GL_FLOAT;
		case Exalted::ShaderDataType::Float2:   return GL_FLOAT;
		case Exalted::ShaderDataType::Float3:   return GL_FLOAT;
		case Exalted::ShaderDataType::Float4:   return GL_FLOAT;
		case Exalted::ShaderDataType::Mat3:     return GL_FLOAT;
		case Exalted::ShaderDataType::Mat4:     return GL_FLOAT;
		case Exalted::ShaderDataType::Int:      return GL_INT;
		case Exalted::ShaderDataType::Int2:     return GL_INT;
		case Exalted::ShaderDataType::Int3:     return GL_INT;
		case Exalted::ShaderDataType::Int4:     return GL_INT;
		case Exalted::ShaderDataType::Bool:     return GL_BOOL;
		}
		EX_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) //todo: Find a more dynamic method of implementing this.
	{
		EX_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*) element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddVertexBufferDivisor(const Ref<VertexBuffer>& vertexBuffer, unsigned attributeIndex, unsigned advancementRate) //todo: Make advancement rate a member of vertex buffer.
	{
		EX_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_RendererID);

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(attributeIndex);
			vertexBuffer->Bind();
			glVertexAttribPointer(attributeIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			vertexBuffer->Unbind();
			glVertexAttribDivisor(attributeIndex, advancementRate);
			attributeIndex++;
		}
		m_VertexBuffers.emplace_back(vertexBuffer);
		glBindVertexArray(0);
	}
}
