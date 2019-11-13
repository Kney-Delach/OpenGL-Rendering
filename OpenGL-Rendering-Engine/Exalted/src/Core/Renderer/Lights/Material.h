#pragma once

#include "Core/Core.h"
#include "glm/vec3.hpp"

namespace Exalted
{
	struct Material
	{
		glm::vec3 Ambient;  
		glm::vec3 Diffuse; // todo: replace with reference to diffuse texture
		glm::vec3 Specular; // todo: replace with reference to specular texture

		float Shininess;

		Material() 
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0) {}
		Material(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess) 
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess) {}

		static Ref<Material> Create()
		{
			return Exalted::CreateRef<Material>();
		}
		static Ref<Material> Create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess)
		{
			return Exalted::CreateRef<Material>(ambient, diffuse, specular, shininess);
		}

		static Bytes UBSize()
		{
			//todo: Refactor this
			return 3 * sizeof(glm::vec4) + sizeof(float); // 3 for the data in the light, 1 for the transform position //Transform->UBSize();
		}
	};
}