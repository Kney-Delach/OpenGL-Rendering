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
	struct Material
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse; //todo: Remove?
		glm::vec3 Specular; //todo: Remove?
		float Shininess;
		Ref<Texture2D> DiffuseTexture;
		Ref<Texture2D> SpecularTexture;

		Material()
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0)
		{
			uint32_t whiteTextureData = 0xffffffff;
			DiffuseTexture = Texture2D::Create(1, 1);
			SpecularTexture = Texture2D::Create(1, 1);	
			DiffuseTexture->SetData(&whiteTextureData, sizeof(uint32_t));
			SpecularTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		}

		Material(std::string& diffusePath, std::string& specularPath)
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0)
		{
			DiffuseTexture = Texture2D::Create(diffusePath, TextureFormat::RGB,TextureWrap::REPEAT
				,TextureMagFilter::LINEAR,TextureMinFilter::NEAR_LINEAR,true,0);

			SpecularTexture = Texture2D::Create(specularPath, TextureFormat::RGB, TextureWrap::REPEAT
				, TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
		}

		Material(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess, std::string& diffusePath, std::string& specularPath)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess)
		{
			if(!diffusePath.empty())
			{
				DiffuseTexture = Texture2D::Create(diffusePath, TextureFormat::RGB, TextureWrap::REPEAT,
					TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
			}
			else
			{
				uint32_t whiteTextureData = 0xffffffff;
				SpecularTexture = Texture2D::Create(1, 1);
				SpecularTexture->SetData(&whiteTextureData, sizeof(uint32_t));
			}
			if (!specularPath.empty())
			{
				SpecularTexture = Texture2D::Create(specularPath, TextureFormat::RGB, TextureWrap::REPEAT,
					TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
			}
			else
			{
				uint32_t whiteTextureData = 0xffffffff;
				SpecularTexture = Texture2D::Create(1, 1);
				SpecularTexture->SetData(&whiteTextureData, sizeof(uint32_t));
			}
		}

		static Ref<Material> Create(std::string& diffusePath, std::string& specularPath)
		{
			return Exalted::CreateRef<Material>(diffusePath, specularPath);
		}

		static Ref<Material> Create(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess, std::string& diffusePath, std::string& specularPath)
		{
			return Exalted::CreateRef<Material>(ambient, diffuse, specular, shininess, diffusePath, specularPath);
		}
	};
}