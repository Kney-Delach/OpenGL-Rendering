/***************************************************************************
 * Filename		: Light.h
 * Name			: Ori Lazar
 * Date			: 13/11/2019
 * Description	: Light structures used to represent a 3D light. 
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once

#include "Core/Core.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "Core/SceneGraph/GameTransform.h"
#include "Core/Renderer/UniformBuffer.h"

//todo: Abstract this into just a color component, then inherit as needed into point, directional and spot lights.
namespace Exalted
{
	struct Light
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;

		Ref<GameTransform> Transform;

		Light()
			: Ambient(0), Diffuse(0), Specular(0)
		{
			Transform = GameTransform::Create();
		}

		Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular)
		{
			Transform = GameTransform::Create();
		}

		void UpdateUniformBuffer(Bytes& offset, Ref<UniformBuffer>& ub)
		{
			Bytes sizeOfVec4 = sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Ambient));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Diffuse));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Specular));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Transform->Position));
			offset += sizeof(glm::vec4);
		}

		static Bytes UBSize() //todo: Refactor this
		{
			return 4 * sizeof(glm::vec4); // 3 for the colors, 1 for the transform position
		}

		static Ref<Light> Create()
		{
			return Exalted::CreateRef<Light>();
		}

		static Ref<Light> Create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular)
		{
			return Exalted::CreateRef<Light>(ambient, diffuse, specular);
		}
	};
}