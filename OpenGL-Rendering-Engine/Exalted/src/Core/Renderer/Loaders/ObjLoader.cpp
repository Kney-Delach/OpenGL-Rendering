/***************************************************************************
 * Filename		: ObjLoader.cpp
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: A static class which uses tinyobjloader to load an obj model.
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
#include "ObjLoader.h"

#include <tiny_obj_loader.h>

//todo: Implement material loading
namespace Exalted
{
	Ref<VertexArray> ObjLoader::Load(const std::string& path)
	{
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warning;
		std::string error;

		bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str(), nullptr, true);

		if (!warning.empty()) 
			EX_CORE_WARN("ObjLoader Warning: {0}", warning);

		if (!error.empty()) 
			EX_CORE_ERROR("ObjLoader Error: {0}", error);
		
		if (!success) 
			return nullptr;
		
		std::vector<uint32_t> indexBuffer;
		std::vector<float> meshData;
		Ref<VertexArray> va = VertexArray::Create();
		uint32_t indexCount = 0;

		for (tinyobj::shape_t shape : shapes) 
		{
			size_t offset = 0;
			uint8_t vertexDataLayout = (uint8_t)0;
			BufferLayout bufferLayout;

			for (unsigned int verticesInFace : shape.mesh.num_face_vertices) 
			{
				for (size_t vertexIndex = 0; vertexIndex < verticesInFace; vertexIndex++) 
				{
					tinyobj::index_t index = shape.mesh.indices[offset + vertexIndex];

					if(!attributes.vertices.empty())
					{
						vertexDataLayout |= (uint8_t)1; // mesh has positions

						meshData.push_back(attributes.vertices[0 + 3 * index.vertex_index]);
						meshData.push_back(attributes.vertices[1 + 3 * index.vertex_index]);
						meshData.push_back(attributes.vertices[2 + 3 * index.vertex_index]);
					}

					if (!attributes.normals.empty())
					{
						vertexDataLayout |= (uint8_t)2; // mesh has normals

						meshData.push_back(attributes.normals[0 + 3 * index.normal_index]);
						meshData.push_back(attributes.normals[1 + 3 * index.normal_index]);
						meshData.push_back(attributes.normals[2 + 3 * index.normal_index]);
					}

					if (!attributes.texcoords.empty())
					{
						vertexDataLayout |= (uint8_t)4; // mesh has texture coordinates

						meshData.push_back(attributes.texcoords[0 + 2 * index.texcoord_index]);
						meshData.push_back(attributes.texcoords[1 + 2 * index.texcoord_index]);
					}					
					indexBuffer.push_back(indexCount++);
				}
				offset += verticesInFace;
			}
			if (vertexDataLayout & (uint8_t)1)
				bufferLayout.AddElement(ShaderDataType::Float3, "a_Position");
			if (vertexDataLayout & (uint8_t)2)
				bufferLayout.AddElement(ShaderDataType::Float3, "a_Normal");
			if (vertexDataLayout & (uint8_t)4)
				bufferLayout.AddElement(ShaderDataType::Float2, "a_TexCoord");
			
			bufferLayout.CalculateOffsetsAndStride();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(meshData.data(), meshData.size());
			vertexBuffer->SetLayout(bufferLayout);
			va->AddVertexBuffer(vertexBuffer);
		}
		va->AddIndexBuffer(IndexBuffer::Create(indexBuffer.data(), indexBuffer.size()));
		return va;
	}
}