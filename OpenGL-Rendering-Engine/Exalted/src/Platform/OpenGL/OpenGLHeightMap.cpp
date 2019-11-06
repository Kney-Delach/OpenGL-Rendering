/***************************************************************************
 * Filename		: OpenGLHeightMap.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: An abstract interface declaration for a mesh height map
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
#include "Platform/OpenGL/OpenGLHeightmap.h"

#include <string>
#include <iostream>
#include <fstream>
namespace Exalted
{
	OpenGLHeightMap::OpenGLHeightMap(const std::string path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);
		if(!file)
		{
			EX_CORE_ERROR("Attempting to read heightmap from invalid file: {0}", path.c_str());
			return;
		}
		const float numVertices = RAW_WIDTH * RAW_HEIGHT;
		float vertices[RAW_WIDTH * RAW_HEIGHT * 5];
		uint32_t indices[(RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6];

		unsigned char* data = new unsigned char[numVertices];
		file.read((char*)data, numVertices * sizeof(unsigned char));
		file.close();

		for (int x = 0; x < RAW_WIDTH; ++x)
		{
			for (int z = 0; z < RAW_HEIGHT; ++z)
			{
				int offset = (x * RAW_WIDTH) + z;

				vertices[0 + offset * 5] = x * HEIGHTMAP_TEX_X;
				vertices[1 + offset * 5] = data[offset] * HEIGHTMAP_Y;
				vertices[2 + offset * 5] = z * HEIGHTMAP_Z;
				vertices[3 + offset * 5] = x * HEIGHTMAP_TEX_X;
				vertices[4 + offset * 5] = z * HEIGHTMAP_TEX_Z;
			}
		}

		delete data; 

		int numIndices = 0;
		for (int x = 0; x < RAW_WIDTH - 1; ++x) 
		{
			for (int z = 0; z < RAW_HEIGHT - 1; ++z) 
			{
				int a = (x * (RAW_WIDTH)) + z;
				int b = ((x + 1) * (RAW_WIDTH)) + z;
				int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
				int d = (x * (RAW_WIDTH)) + (z + 1);
				
				indices[numIndices++] = c;
				indices[numIndices++] = b;
				indices[numIndices++] = a;
				
				indices[numIndices++] = a;
				indices[numIndices++] = d;
				indices[numIndices++] = c;
				
			}		}

		m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float2, "a_TexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}
