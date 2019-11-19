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
#include "glm/vec3.hpp"
#include "glm/gtx/normal.hpp"

//todo: Implement material loading
namespace Exalted
{
	Ref<VertexArray> ObjLoader::Load(const std::string& path, bool calculateTangents)
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

			for (unsigned int verticesInFace : shape.mesh.num_face_vertices)   // += 3 for each index 
			{
				glm::vec3 tangent;
				glm::vec3 bitangent;

				if (calculateTangents && !attributes.texcoords.empty()) // calculate tangents and bitangents
				{
					// vertex 0
					// positions
					float vx0 = attributes.vertices[0 + 3 * (offset + 0)];
					float vy0 = attributes.vertices[1 + 3 * (offset + 0)];
					float vz0 = attributes.vertices[2 + 3 * (offset + 0)];
					glm::vec3 pos0(vx0, vy0, vz0);
					// texture co-ordinates
					float tx0 = attributes.texcoords[0 + 2 * (offset + 0)];
					float ty0 = attributes.texcoords[1 + 2 * (offset + 0)];

					// vertex 1
					// positions
					float vx1 = attributes.vertices[0 + 3 * (offset + 1)];
					float vy1 = attributes.vertices[1 + 3 * (offset + 1)];
					float vz1 = attributes.vertices[2 + 3 * (offset + 1)];
					glm::vec3 pos1(vx1, vy1, vz1);
					// texture co-ordinates
					float tx1 = attributes.texcoords[0 + 2 * (offset + 1)];
					float ty1 = attributes.texcoords[1 + 2 * (offset + 1)];

					//vertex 2
					// positions
					float vx2 = attributes.vertices[0 + 3 * (offset + 2)];
					float vy2 = attributes.vertices[1 + 3 * (offset + 2)];
					float vz2 = attributes.vertices[2 + 3 * (offset + 2)];
					glm::vec3 pos2(vx2, vy2, vz2);
					// texture co-ordinates
					float tx2 = attributes.texcoords[0 + 2 * (offset + 2)];
					float ty2 = attributes.texcoords[1 + 2 * (offset + 2)];

					glm::vec3 edge1 = pos1 - pos0;
					glm::vec3 edge2 = pos2 - pos0;

					float deltaU1 = tx1 - tx0;
					float deltaV1 = ty1 - ty0;
					float deltaU2 = tx2 - tx0;
					float deltaV2 = ty2 - ty0;

					float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

					tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
					tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
					tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

					bitangent.x = f * (-deltaU2 * edge1.x - deltaU1 * edge2.x);
					bitangent.y = f * (-deltaU2 * edge1.y - deltaU1 * edge2.y);
					bitangent.z = f * (-deltaU2 * edge1.z - deltaU1 * edge2.z);

					tangent = glm::normalize(tangent); //todo: verify this is functional
				}

				
				for (size_t vertexIndex = 0; vertexIndex < verticesInFace; vertexIndex++)  // iterate for each vertex in face (3 each time)
				{
					tinyobj::index_t index = shape.mesh.indices[offset + vertexIndex]; // use this to calculate offsets above

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

					if(calculateTangents && !attributes.texcoords.empty())
					{
						meshData.push_back(tangent.x);
						meshData.push_back(tangent.y);
						meshData.push_back(tangent.z);

						meshData.push_back(bitangent.x);
						meshData.push_back(bitangent.y);
						meshData.push_back(bitangent.z);
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

			if (calculateTangents)
			{
				bufferLayout.AddElement(ShaderDataType::Float3, "a_Tangent");
				bufferLayout.AddElement(ShaderDataType::Float3, "a_Bitangent");
			}
			
			bufferLayout.CalculateOffsetsAndStride();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(meshData.data(), meshData.size());
			vertexBuffer->SetLayout(bufferLayout);
			va->AddVertexBuffer(vertexBuffer);
		}
		va->AddIndexBuffer(IndexBuffer::Create(indexBuffer.data(), indexBuffer.size()));
		return va;
	}
}