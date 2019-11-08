/***************************************************************************
 * Filename		: ShapeGenerator.cpp
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: A static class which generates shapes vertex arrays.
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
#include "ShapeGenerator.h"

namespace Exalted
{
	Ref<VertexArray> ShapeGenerator::GenerateSphere()
	{
		//todo: Implement this.
		return nullptr;
	}

	Ref<VertexArray> ShapeGenerator::GenerateCube()
	{
		//todo: Implement this.
		return nullptr;
	}

	Ref<VertexArray> ShapeGenerator::GenerateQuad()
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		Ref<VertexBuffer> vb;
		vb = VertexBuffer::Create(vertices, 24);
		const BufferLayout layout =
		{
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		return va;
	}

	Ref<VertexArray> ShapeGenerator::GenerateIndexedQuad()
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[16] = {
			// positions         texture coords
			 1.0f,  1.0f, 1.0f, 1.f,  // top right
			 1.0f, -1.0f, 1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, // bottom left
			-1.0f,  1.0f, 0.0f, 1.0f  // top left 
		};
		Ref<VertexBuffer> vb;
		vb = VertexBuffer::Create(vertices, 16);
		const BufferLayout layout =
		{
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
		Ref<IndexBuffer> ib;
		ib = IndexBuffer::Create(indices,6);
		va->AddIndexBuffer(ib);
		return va;
	}
}