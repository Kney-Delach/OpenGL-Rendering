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
	enum class TextureType { DIFFUSE = 0, SPECULAR = 1, EMISSION = 2, NORMAL = 3 };

	struct Material
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
		Ref<Texture2D> DiffuseTexture;
		Ref<Texture2D> SpecularTexture;
		Ref<Texture2D> EmissionTexture;
		Ref<Texture2D> NormalTexture;

		Material()
			: Ambient(0), Diffuse(0), Specular(0), Shininess(0)
		{
			std::string path = "";
			SetupTexture(TextureType::DIFFUSE,  path);
			SetupTexture(TextureType::SPECULAR, path);
			SetupTexture(TextureType::EMISSION, path);
			SetupTexture(TextureType::NORMAL, path);
		}

		Material(const std::string& diffusePath, const std::string& specularPath, const std::string& emissionPath = "", const std::string& normalPath = "", const float shininess = 0)
			: Ambient(0), Diffuse(0), Specular(0), Shininess(shininess)
		{
			SetupTexture(TextureType::DIFFUSE, diffusePath);
			SetupTexture(TextureType::SPECULAR, specularPath);
			SetupTexture(TextureType::EMISSION, emissionPath);
			SetupTexture(TextureType::NORMAL, normalPath);
		}

		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess, const std::string& diffusePath, const std::string& specularPath, const std::string& emissionPath, const std::string& normalPath = "")
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess)
		{
			SetupTexture(TextureType::DIFFUSE, diffusePath);
			SetupTexture(TextureType::SPECULAR, specularPath);
			SetupTexture(TextureType::EMISSION, emissionPath);
			SetupTexture(TextureType::NORMAL, normalPath);
		}

		static Ref<Material> Create(const std::string& diffusePath, const std::string& specularPath)
		{
			return Exalted::CreateRef<Material>(diffusePath, specularPath);
		}

		static Ref<Material> Create(const std::string& diffusePath, const std::string& specularPath, const std::string& emissionPath, const std::string& normalPath = "", const float shininess = 0)
		{
		
			return Exalted::CreateRef<Material>(diffusePath, specularPath, emissionPath, normalPath, shininess);
		}
		
		static Ref<Material> Create(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess, const std::string& diffusePath, const std::string& specularPath, const std::string& emissionPath, const std::string& normalPath = "")
		{
			return Exalted::CreateRef<Material>(ambient, diffuse, specular, shininess, diffusePath, specularPath, emissionPath, normalPath);
		}

		void Bind() const
		{
			DiffuseTexture->Bind(0);
			SpecularTexture->Bind(1);
			EmissionTexture->Bind(2);
			NormalTexture->Bind(3);
		}

		void Unbind() const
		{
			DiffuseTexture->Unbind();
		}
	private:
		void SetupTexture(TextureType textureIndex, const std::string& path)
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
				case TextureType::NORMAL: // Normal
				{
					if (!path.empty())
					{
						NormalTexture = Texture2D::Create(path, TextureFormat::RGB, TextureWrap::REPEAT,
							TextureMagFilter::LINEAR, TextureMinFilter::NEAR_LINEAR, true, 0);
						return;
					}
					NormalTexture = Texture2D::Create(1, 1);
					NormalTexture->SetData(&whiteTextureData, sizeof(uint32_t));
					return;
				}
			}
		}
	};
}