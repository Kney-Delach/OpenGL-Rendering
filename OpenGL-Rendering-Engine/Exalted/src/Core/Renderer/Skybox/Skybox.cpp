/***************************************************************************
 * Filename		: Skybox.cpp
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
#include "expch.h"
#include "Skybox.h"
#include "Core/Renderer/Generators/ShapeGenerator.h"
#include "Platform/OpenGL/OpenGLConfigurations.h"
#include "Core/Renderer/Renderer.h"

namespace Exalted
{
	Skybox::Skybox(
		const std::string& frontTexturePath, const std::string& backTexturePath,
		const std::string& topTexturePath, const std::string& bottomTexturePath,
		const std::string& rightTexturePath, const std::string& leftTexturePath,
		const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		const std::vector<std::string> faces { frontTexturePath, backTexturePath, topTexturePath, bottomTexturePath, rightTexturePath, leftTexturePath };
		m_Texture = TextureCube::Create(faces);
		m_Mesh = Mesh::Create();
		m_Mesh->SetVertexArray(ShapeGenerator::GenerateSkybox());
		m_Shader = Shader::Create(vertexShaderPath, fragmentShaderPath);
		m_Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformInt1("u_SkyboxTexture", 0);
		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->SetUniformBlockIndex("Camera_Matrices", 2);
		m_Shader->Unbind();
	}

	void Skybox::Draw() const
	{
		OpenGLConfigurations::EnableDepthTesting();
		OpenGLConfigurations::SetDepthFunction(DepthMode::LESS_EQUAL);
		m_Shader->Bind();
		m_Texture->Bind();
		Renderer::Submit(m_Mesh, 36);
		m_Texture->Unbind();
		m_Shader->Unbind();
		OpenGLConfigurations::SetDepthFunction(DepthMode::LESS);
		OpenGLConfigurations::DisableDepthTesting();
	}

	Ref<Skybox> Skybox::Create(
		const std::string& frontTexturePath, const std::string& backTexturePath,
		const std::string& topTexturePath, const std::string& bottomTexturePath,
		const std::string& rightTexturePath, const std::string& leftTexturePath,
		const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		return Exalted::CreateRef<Skybox>(frontTexturePath, backTexturePath, topTexturePath, bottomTexturePath, rightTexturePath, leftTexturePath, vertexShaderPath, fragmentShaderPath);
	}
}
