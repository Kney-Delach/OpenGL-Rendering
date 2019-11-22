/***************************************************************************
 * Filename		: Skybox.h
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Container for a skybox, these should be attached directly 
 *                to the scene management system.
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
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Shader.h"

//todo: Abstract OpenGLConfigurations to a generic configurations setting

// -------------- Texture Macros -------------- //

#define DEFAULT_SKYBOX_TOP		"Resources/Defaults/Skybox/Top.png"
#define DEFAULT_SKYBOX_BOTTOM	"Resources/Defaults/Skybox/Bottom.png"
#define DEFAULT_SKYBOX_FRONT	"Resources/Defaults/Skybox/Front.png"
#define DEFAULT_SKYBOX_BACK		"Resources/Defaults/Skybox/Back.png"
#define DEFAULT_SKYBOX_RIGHT	"Resources/Defaults/Skybox/Right.png"
#define DEFAULT_SKYBOX_LEFT		"Resources/Defaults/Skybox/Left.png"

#define DEFAULT_SKYBOX_SHADER_VERTEX "Resources/Defaults/Skybox/SkyboxVertex.glsl"
#define DEFAULT_SKYBOX_SHADER_FRAGMENT "Resources/Defaults/Skybox/SkyboxFragment.glsl"

namespace Exalted
{
	class Skybox
	{
	public:
		Skybox(
			const std::string& frontTexturePath, const std::string& backTexturePath,
			const std::string& topTexturePath, const std::string& bottomTexturePath, 
			const std::string& rightTexturePath, const std::string& leftTexturePath,
			const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void Draw() const;

		_NODISCARD inline Ref<TextureCube>& GetTexture() { return m_Texture; }
		_NODISCARD inline Ref<Mesh>& GetMesh() { return m_Mesh; }
		_NODISCARD inline Ref<Shader>& GetShader() { return m_Shader; }
	private:
		Ref<TextureCube> m_Texture;
		Ref<Mesh> m_Mesh;
		Ref<Shader> m_Shader;
	public:
		static Ref<Skybox> Create(
			const std::string& frontTexturePath = DEFAULT_SKYBOX_FRONT, const std::string& backTexturePath = DEFAULT_SKYBOX_BACK,
			const std::string& topTexturePath = DEFAULT_SKYBOX_TOP, const std::string& bottomTexturePath = DEFAULT_SKYBOX_BOTTOM,
			const std::string& rightTexturePath = DEFAULT_SKYBOX_RIGHT, const std::string& leftTexturePath = DEFAULT_SKYBOX_LEFT,
			const std::string& vertexShaderPath = DEFAULT_SKYBOX_SHADER_VERTEX, const std::string& fragmentShaderPath = DEFAULT_SKYBOX_SHADER_FRAGMENT);
	};
}