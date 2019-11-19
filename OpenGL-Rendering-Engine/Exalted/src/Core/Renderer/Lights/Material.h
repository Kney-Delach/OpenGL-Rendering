/***************************************************************************
 * Filename		: Material.h
 * Name			: Ori Lazar
 * Date			: 13/11/2019
 * Description	: Material structures used to represent object colors.
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

#include "Core/Renderer/Texture.h"

//todo: Remove ambient and diffuse color components. 
//todo: Incorporate vectors to be able to introduce multiple diffuse / specular textures.

namespace Exalted
{
	enum class TextureType { DIFFUSE = 0, SPECULAR = 1, EMISSION = 2 };

	struct Material
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse; //todo: Remove?
		glm::vec3 Specular; //todo: Remove?
		float Shininess;
		Ref<Texture2D> DiffuseTexture;
		Ref<Texture2D> SpecularTexture;
		Ref<Texture2D> EmissionTexture;

		Material()
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0)
		{
			std::string path = "";
			SetupTexture(TextureType::DIFFUSE,  path);
			SetupTexture(TextureType::SPECULAR, path);
			SetupTexture(TextureType::EMISSION, path);
		}

		Material(std::string& diffusePath, std::string& specularPath)
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0)
		{
			SetupTexture(TextureType::DIFFUSE, diffusePath);
			SetupTexture(TextureType::SPECULAR, specularPath);
			std::string path = "";
			SetupTexture(TextureType::EMISSION, path);
		}

		Material(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess, std::string& diffusePath, std::string& specularPath, std::string& emissionPath)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess)
		{
			SetupTexture(TextureType::DIFFUSE, diffusePath);
			SetupTexture(TextureType::SPECULAR, specularPath);
			SetupTexture(TextureType::EMISSION, emissionPath);
		}

		static Ref<Material> Create(std::string& diffusePath, std::string& specularPath)
		{
			return Exalted::CreateRef<Material>(diffusePath, specularPath);
		}

		static Ref<Material> Create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess, std::string& diffusePath, std::string& specularPath, std::string& emissionPath)
		{
			return Exalted::CreateRef<Material>(ambient, diffuse, specular, shininess, diffusePath, specularPath, emissionPath);
		}

	private:
		void SetupTexture(TextureType textureIndex, std::string& path)
		{
			uint32_t whiteTextureData = 0xffffffff;

			switch(textureIndex)
			{
				case TextureType::DIFFUSE: // diffuse 
				{
					if(!path.empty())
					{
						DiffuseTexture = Texture2D::Create(path, TextureFormat::RGB, TextureWrap::REPEAT,
							TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
						return;
					}
					DiffuseTexture = Texture2D::Create(1, 1);
					DiffuseTexture->SetData(&whiteTextureData, sizeof(uint32_t));
					return;
				}
				case TextureType::SPECULAR: // specular
				{
					if (!path.empty())
					{
						SpecularTexture = Texture2D::Create(path, TextureFormat::RGB, TextureWrap::REPEAT,
							TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
						return;
					}
					SpecularTexture = Texture2D::Create(1, 1);
					SpecularTexture->SetData(&whiteTextureData, sizeof(uint32_t));
					return;
				}
				case TextureType::EMISSION: // Emission
				{
					if (!path.empty())
					{
						EmissionTexture = Texture2D::Create(path, TextureFormat::RGB, TextureWrap::REPEAT,
							TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
						return;
					}
					EmissionTexture = Texture2D::Create(1, 1);
					EmissionTexture->SetData(&whiteTextureData, sizeof(uint32_t));
					return;
				}
			}
		}
	};
}