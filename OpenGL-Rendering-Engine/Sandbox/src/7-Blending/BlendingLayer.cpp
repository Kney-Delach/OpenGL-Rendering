/***************************************************************************
 * Filename		: BlendingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This layer contains a scene showcasing Blending  testing capabilities.
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "BlendingLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <map>

namespace Sandbox
{
	BlendingLayer::BlendingLayer()
		: Layer("Blending Layer", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void BlendingLayer::OnAttach()
	{
		EX_INFO("Blending layer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_MeshQuad = Exalted::Mesh::Create();
		m_MeshQuad->CreateTexturedQuad(1);

		m_MeshCube = Exalted::Mesh::Create();
		m_MeshCube->CreateTexturedCube(1);

		// ------------------------- Initialize Textures ------------------------- //


		m_CubeTexture = Exalted::Texture2D::Create("Resources/Textures/TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_FloorTexture = Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_WindowTexture = Exalted::Texture2D::Create("Resources/Textures/TexTransparentWindow.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_GrassTexture = Exalted::Texture2D::Create("Resources/Textures/TexGrass.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP, // note, transparent textures should be clamped, as when repeating, the border values get interpolated. 
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_StainedWindowTexture = Exalted::Texture2D::Create("Resources/Textures/TexTransparentGlassStained.tga",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		
		// ------------------------- Initialize Transformations ------------------------- //

		glm::mat4 cubeTransform = glm::mat4(1.0f);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.5f, 5.f));
		m_CubeTransform1 = cubeTransform;
		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.0f, 5.f));
		m_CubeTransform2 = cubeTransform;
		m_WindowTransforms.emplace_back(glm::vec3(2.f, 0.5f, 2.0f));
		m_WindowTransforms.emplace_back(glm::vec3(4.f, 0.5f, 2.0f));
		m_WindowTransforms.emplace_back(glm::vec3(0.f, 0.5f,7.0f));
		m_GrassTransform = glm::translate(m_CubeTransform1, glm::vec3(-2.f, 0.0f, 0.0f));
		m_FloorTransforms.reserve(m_FloorTileCount * m_FloorTileCount);
		unsigned rowLength = 0;
		unsigned column = 0;
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
		{
			if (i % m_FloorTileCount == 0)
			{
				rowLength++;
				column = 0;
			}
			glm::mat4 meshTransform = glm::mat4(1.0f);
			meshTransform = glm::translate(meshTransform, glm::vec3(column++ * 1.0f, 0.f, rowLength));
			meshTransform = glm::rotate(meshTransform, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
			m_FloorTransforms.push_back(meshTransform);
		}

		// ------------------------- Initialize Shader ------------------------- //

		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		m_WindowShader = Exalted::Shader::Create("Resources/Shaders/Blending/WindowVertexShader.glsl", "Resources/Shaders/Blending/WindowFragmentShader.glsl");
		m_GrassShader = Exalted::Shader::Create("Resources/Shaders/Blending/GrassVertexShader.glsl", "Resources/Shaders/Blending/GrassFragmentShader.glsl");

		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();
		m_WindowShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_WindowShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_WindowShader->Unbind();
		m_GrassShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_GrassShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_GrassShader->Unbind();
	}

	void BlendingLayer::OnDetach()
	{
		EX_INFO("Blending Layer detached successfully.");
	}

	void BlendingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera.UpdateCamera(deltaTime);

		// ------------ Initialize Blending  Buffer Processing ------------ //

		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);

		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		
		Exalted::Renderer::BeginScene(m_EditorCamera);

		// ------------ Draw the floor ------------ //

		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_Shader, m_MeshQuad, m_FloorTransforms[i]);

		// --------------------- Draw the cubes --------------------- //

		m_CubeTexture->Bind();
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, m_CubeTransform1);
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, m_CubeTransform2);

		// --------------------- Draw the transparent grass --------------------- //

		m_GrassTexture->Bind();
		Exalted::Renderer::Submit(m_GrassShader, m_MeshQuad, m_GrassTransform);

		// --------------------- Draw the transparent windows --------------------- //

		// calculate sorted container object that stores each window positions based on their distance from camera from lowest to highest
		std::map<float, glm::vec3> sortedWindows;
		for (unsigned int i = 0; i < m_WindowTransforms.size(); i++)
		{
			float distance = glm::length(m_EditorCamera.GetPosition() - m_WindowTransforms[i]);
			sortedWindows[distance] = m_WindowTransforms[i];
		}

		// Render from farthest to nearest. 
		for(std::map<float, glm::vec3>::reverse_iterator  it = sortedWindows.rbegin(); it != sortedWindows.rend(); ++it)
		{
			glm::mat4 windowModelTransform = glm::mat4(1.f);
			windowModelTransform = glm::translate(windowModelTransform, it->second);
			if (it->second == m_WindowTransforms[0])
				m_StainedWindowTexture->Bind();
			else
				m_WindowTexture->Bind();
			Exalted::Renderer::Submit(m_WindowShader, m_MeshQuad, windowModelTransform);
		}		
		m_WindowTexture->Unbind();

		// ------------ cleanup ------------ //
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();
		Exalted::Renderer::EndScene();
	}

	void BlendingLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();

		ImGui::Begin("Blending Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Note:");
		ImGui::Text("----------------------------");
		ImGui::Text("-> When sampling textures at their borders OpenGL interpolates the border values with \nthe next repeated value of the texture when you set its wrapping to REPEAT.\nAs a direct result opposing border sides interpolate their values, thus may result in coloured borders.\nThe Solution is to set texture wrapping to be CLAMPED.");
		ImGui::Text("-> When drawing a scene with non-transparent objects and transparent objects,\nthe following is a good general outline which should be followed:\n1: Draw all opaque objects first.\n2: Sort all transparent objects.\n3: Draw all transparent objects in sorted order.");
		ImGui::End();
	}

	void BlendingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [7] -> Blending"))
			m_IsActive = true;
		ImGui::End();
	}

	void BlendingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera.OnEvent(event);
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_1)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);

			if (e.GetKeyCode() == EX_KEY_2)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);

			if (e.GetKeyCode() == EX_KEY_3)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
		}
	}
}