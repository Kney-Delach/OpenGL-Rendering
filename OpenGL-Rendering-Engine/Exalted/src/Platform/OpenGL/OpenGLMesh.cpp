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

#include <string>
#include <iostream>
#include <fstream>
//todo: Assimp is currently linked under a single threaded implementation, in the future it will be wise to swap this with the multi-threaded one. 
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

namespace Exalted
{
	void OpenGLMesh::CreateTriangle()
	{
		m_VertexArray = VertexArray::Create();
		float vertices[3 * 7] =
		{
			 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLMesh::CreateQuad()
	{
		m_VertexArray = VertexArray::Create();
		float vertices[4 * 7] =
		{
			 -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.75f, 1.0f,
			  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.75f, 1.0f,
			  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.25f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.25f, 1.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{ 
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLMesh::CreateCube()
	{
		m_VertexArray = VertexArray::Create();
		float vertices[8*7] = {
			// front
			-.5, -.5,  .5,		1.0, 0.0, 0.0f, 1.0f,
			 0.5, -0.5,  0.5,	0.0, 1.0, 0.0f, 1.0f,
			 0.5,  0.5,  0.5,	0.0, 0.0, 1.0f, 1.0f,
			-0.5,  0.5,  0.5,	1.0, 0.0, 1.0f, 1.0f,
			// back
			-0.5, -0.5, -0.5,	1.0, 0.0, 0.0f, 1.0f,
			 0.5, -0.5, -0.5,	0.0, 1.0, 0.0f, 1.0f,
			 0.5,  0.5, -0.5,	0.0, 0.0, 1.0f, 1.0f,
			-0.5,  0.5, -0.5,	1.0, 0.0, 1.0f , 1.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[12*3] = {
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
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLMesh::CreateTexturedQuad(float textureScale)
	{
		m_VertexArray = VertexArray::Create();
		float vertices[4*9] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   textureScale*1.0f, textureScale*1.f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   textureScale*1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, textureScale*1.0f  // top left 
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Colour" },
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OpenGLMesh::CreateTexturedCube(float textureScale)
	{
		m_VertexArray = VertexArray::Create();
		float vertices[6*6*9] = 
		{
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
											 	
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
													
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
												
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
										
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
							 				 
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 1.0f, textureScale * 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, textureScale * 0.0f, textureScale * 1.0f
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Colour" },
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
	}

	void OpenGLMesh::CreateHeightMap(const std::string path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);
		if (!file)
		{
			EX_CORE_ERROR("Attempting to read heightmap from invalid file: {0}", path.c_str());
			return;
		}
		float vertCount = RAW_WIDTH * RAW_HEIGHT;
		float indCount = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;

		float* allVertices = new float[(int)vertCount * 5];
		uint32_t* indices = new uint32_t[(int)indCount];

		unsigned char* data = new unsigned char[(int)vertCount];
		file.read((char*)data, (float)vertCount * sizeof(unsigned char));
		file.close();

		for (int x = 0; x < RAW_WIDTH; ++x)
		{
			for (int z = 0; z < RAW_HEIGHT; ++z)
			{
				int offset = (x * RAW_WIDTH) + z;
				allVertices[0 + offset * 5] = (float)x * HEIGHTMAP_X;
				allVertices[1 + offset * 5] = (float)data[offset] * HEIGHTMAP_Y;
				allVertices[2 + offset * 5] = (float)z * HEIGHTMAP_Z;
				allVertices[3 + offset * 5] = (float)x * HEIGHTMAP_TEX_X * 10;
				allVertices[4 + offset * 5] = (float)z * HEIGHTMAP_TEX_Z * 10;
			}
		}

		int numIndices = 0;
		for (int x = 0; x < RAW_WIDTH - 1; ++x)
		{
			for (int z = 0; z < RAW_HEIGHT - 1; ++z)
			{
				int a = (x * (RAW_WIDTH)) + z;
				int b = ((x + 1) * (RAW_WIDTH)) + z;
				int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
				int  d = (x * (RAW_WIDTH)) + (z + 1);

				indices[numIndices++] = c;
				indices[numIndices++] = b;
				indices[numIndices++] = a;
				indices[numIndices++] = a;
				indices[numIndices++] = d;
				indices[numIndices++] = c;

			}		}
		m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> allVertexBuffer = VertexBuffer::Create(allVertices, sizeof(float) * vertCount * 5);
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float2, "a_TexCoord" }
		};
		allVertexBuffer->SetLayout(layout);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indCount / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->AddVertexBuffer(allVertexBuffer);
		delete[] indices;
		delete[] allVertices;
		delete[] data;
		file.close();
	}
}
