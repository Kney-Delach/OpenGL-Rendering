/***************************************************************************
 * Filename		: OpenGLMesh.cpp
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: Implementation for a mesh object for OpenGL, currently
 *                used to auto generate primitives.
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
#include "OpenGLMesh.h"

namespace Exalted
{
	void OpenGLMesh::CreateTriangle()
	{
		m_VertexArray.reset(VertexArray::Create());
		float vertices[3 * 7] =
		{
			 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLMesh::CreateQuad()
	{
		m_VertexArray.reset(VertexArray::Create());
		float vertices[4 * 7] =
		{
			 -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.75f, 1.0f,
			  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.75f, 1.0f,
			  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		const BufferLayout layout =
		{ 
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}
