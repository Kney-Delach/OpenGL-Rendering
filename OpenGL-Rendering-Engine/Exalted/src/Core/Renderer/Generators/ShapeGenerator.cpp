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

	Ref<VertexArray> ShapeGenerator::GenerateCubeIT(float texturedScale) // indexed and textured
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[8 * 5] = {
			// front
			-0.5, -0.5,  0.5, texturedScale * 0.f, texturedScale * 0.f,
			 0.5, -0.5,  0.5, texturedScale * 1.f, texturedScale * 0.f,
			 0.5,  0.5,  0.5, texturedScale * 1.f, texturedScale * 1.f,
			-0.5,  0.5,  0.5, texturedScale * 0.f, texturedScale * 1.f,

			// back
			-0.5, -0.5, -0.5, texturedScale * 0.f, texturedScale * 0.f,
			 0.5, -0.5, -0.5, texturedScale * 1.f, texturedScale * 0.f,
			 0.5,  0.5, -0.5, texturedScale * 1.f, texturedScale * 1.f,
			-0.5,  0.5, -0.5, texturedScale * 0.f, texturedScale * 1.f,
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, 8 * 5);
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		va->AddVertexBuffer(vertexBuffer);
		uint32_t indices[12 * 3] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		va->AddIndexBuffer(indexBuffer);
		return va;
	}

	Ref<VertexArray> ShapeGenerator::GenerateQuad()
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[] = 
		{
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, 24);
		const BufferLayout layout =
		{
			{ShaderDataType::Float2, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		return va;
	}

	Ref<VertexArray> ShapeGenerator::GenerateFlatQuad(float widthScale, float lengthScale)
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[28] = 
		{
			// positions							colormap texture coords                 heightmap texture coords
			 -widthScale ,	0.0f, -lengthScale,		0.0f,				100.0f,				0.0f, 1.0f,
			 -widthScale ,	0.0f,  lengthScale,		0.0f,				0.0f,				0.0f, 0.0f,
			 widthScale  ,	0.0f,  lengthScale,		100.0f,				0.0f,				1.0f, 0.0f,
			 widthScale  ,	0.0f, -lengthScale,		100.0f,				100.0f,				1.0f, 1.0f
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, 28);
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_ColorTexCoord"},
			{ShaderDataType::Float2, "a_HeightmapTexCoord"}
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		uint32_t indices[4] = { 0,1,3,2 };
		Ref<IndexBuffer> ib;
		ib = IndexBuffer::Create(indices,4);
		va->AddIndexBuffer(ib);
		return va;
	}

	Ref<VertexArray> ShapeGenerator::GenerateIndexedQuad()
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[16] = 
		{
			// positions         texture coords
			 1.0f,  1.0f, 1.0f, 1.f,  // top right
			 1.0f, -1.0f, 1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f, 0.0f, // bottom left
			-1.0f,  1.0f, 0.0f, 1.0f  // top left 
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, 16);
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

	Ref<VertexArray> ShapeGenerator::GenerateSkybox() // note this is not indexed
	{
		Ref<VertexArray> va = VertexArray::Create();
		float vertices[36*3] = 
		{
			 // Positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, 36*3);
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		return va;
	}
}