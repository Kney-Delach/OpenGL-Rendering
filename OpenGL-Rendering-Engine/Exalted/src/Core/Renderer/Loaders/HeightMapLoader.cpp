/***************************************************************************
 * Filename		: HeightMapLoader.h
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: A static class which loads a heightmap to a vertex array.
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
#include "HeightMapLoader.h"
#include "Core/Renderer/VertexArray.h"

#include <string>
#include <fstream>
#include "glm/vec3.hpp"

namespace Exalted
{
#define RAW_WIDTH 257
#define RAW_HEIGHT 257
#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

	Ref<VertexArray> HeightMapLoader::Load(const std::string& path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);
		if (!file)
		{
			EX_CORE_ERROR("Attempting to read heightmap from invalid file: {0}", path.c_str());
			return nullptr;
		}
		const float vertCount = RAW_WIDTH * RAW_HEIGHT;

		std::vector<float> allVertices;
		std::vector<uint32_t> indices;

		const unsigned char* data = new unsigned char[static_cast<int>(vertCount)];
		file.read((char*)(data), static_cast<float>(vertCount) * sizeof(unsigned char));
		file.close();

		for (int x = 0; x < RAW_WIDTH; ++x)
		{
			for (int z = 0; z < RAW_HEIGHT; ++z)
			{
				// vertex position
				allVertices.push_back(static_cast<float>(x) * HEIGHTMAP_X);
				allVertices.push_back(static_cast<float>(data[x * RAW_WIDTH + z]) * HEIGHTMAP_Y);
				allVertices.push_back(static_cast<float>(z) * HEIGHTMAP_Z);

				// normal
				allVertices.push_back(0);
				allVertices.push_back(1);
				allVertices.push_back(0);

				// texture co-ordinate
				allVertices.push_back(static_cast<float>(x) * HEIGHTMAP_TEX_X);
				allVertices.push_back(static_cast<float>(z) * HEIGHTMAP_TEX_Z);
			}
		}

		for (int x = 0; x < RAW_WIDTH - 1; ++x)
		{
			for (int z = 0; z < RAW_HEIGHT - 1; ++z)
			{
				const auto a = (x * (RAW_WIDTH)) + z;
				const auto b = ((x + 1) * (RAW_WIDTH)) + z;
				const auto c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
				const auto d = (x * (RAW_WIDTH)) + (z + 1);

				indices.push_back(c);
				indices.push_back(b);
				indices.push_back(a);
				indices.push_back(a);
				indices.push_back(d);
				indices.push_back(c);
			}		}
		Ref<VertexArray> va = VertexArray::Create();
		Ref<VertexBuffer> vb = VertexBuffer::Create(allVertices.data(), allVertices.size());
		const BufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position" },
			{ShaderDataType::Float3, "a_Normal" },
			{ShaderDataType::Float2, "a_TexCoord" }
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);
		va->AddIndexBuffer(IndexBuffer::Create(indices.data(), indices.size()));
		delete[] data;
		file.close();

		return va;
	}
}
