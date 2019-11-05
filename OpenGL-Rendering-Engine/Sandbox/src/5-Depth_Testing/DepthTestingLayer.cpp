/***************************************************************************
 * Filename		: DepthTestingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 01/11/2019
 * Description	: This layer contains a scene showcasing depth testing capabilities
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "DepthTestingLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Sandbox
{
	DepthTestingLayer::DepthTestingLayer()
		: Layer("Depth Testing Layer", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void DepthTestingLayer::OnAttach()
	{
		EX_INFO("Depth testing layer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_MeshFloor = Exalted::Mesh::Create();
		m_MeshFloor->CreateTexturedQuad(1);
		
		m_MeshCube = Exalted::Mesh::Create();
		m_MeshCube->CreateTexturedCube(1);

		// ------------------------- Initialize Textures ------------------------- //

		m_FloorTexture = Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_CubeTexture = Exalted::Texture2D::Create("Resources/Textures/TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_CubeTexture2 = Exalted::Texture2D::Create("Resources/Textures/TexGridBW.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		// ------------------------- Initialize Block Transformations ------------------------- //
		
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
			meshTransform = glm::translate(meshTransform, glm::vec3(column++*1.0f, 0.f, rowLength));
			meshTransform = glm::rotate(meshTransform, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
			m_FloorTransforms.push_back(meshTransform);
		}

		m_FloorTransformsDepth.reserve(m_FloorTileCount * m_FloorTileCount);
		rowLength = 0;
		column = 0;
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
		{
			if (i % m_FloorTileCount == 0)
			{
				rowLength++;
				column = 0;
			}
			glm::mat4 meshTransform = glm::mat4(1.0f);
			meshTransform = glm::translate(meshTransform, glm::vec3(-10.0 + column++* 1.0f, 0.f, rowLength));
			meshTransform = glm::rotate(meshTransform, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
			m_FloorTransformsDepth.push_back(meshTransform);
		}

		// ------------------------- Initialize Shader ------------------------- //

		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		m_DepthShader = Exalted::Shader::Create("Resources/Shaders/VDepthShader.glsl", "Resources/Shaders/FDepthShader.glsl");

		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();

		m_DepthShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_DepthShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_DepthShader->Unbind();
	}

	void DepthTestingLayer::OnDetach()
	{
		EX_INFO("Depth Testing Layer deatched successfully.");
	}

	void DepthTestingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera.UpdateCamera(deltaTime);

		if(m_EnableDepthTest)
			Exalted::OpenGLConfigurations::EnableDepthTesting();
		
		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_EditorCamera);

		// --------------------- Regular Colour display --------------------- //

		glm::mat4 cubeTransform = glm::mat4(1.0f);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.5f, 5.f));
		m_CubeTexture->Bind();
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, cubeTransform);
		m_CubeTexture->Unbind();

		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount* m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_Shader, m_MeshFloor, m_FloorTransforms[i]);
		m_FloorTexture->Unbind();

		m_CubeTexture2->Bind();
		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.0f, 5.f));
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, cubeTransform);
		m_CubeTexture2->Unbind();

		// --------------------- Depth display --------------------- //

		cubeTransform = glm::mat4(1.0f);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(-2.f, 0.5f, 5.f));
		m_CubeTexture->Bind();
		Exalted::Renderer::Submit(m_DepthShader, m_MeshCube, 6 * 6 * 9, cubeTransform);
		m_CubeTexture->Unbind();

		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_DepthShader, m_MeshFloor, m_FloorTransformsDepth[i]);
		m_FloorTexture->Unbind();

		m_CubeTexture2->Bind();
		cubeTransform = glm::translate(cubeTransform, glm::vec3(-2.f, 0.0f, 5.f));
		Exalted::Renderer::Submit(m_DepthShader, m_MeshCube, 6 * 6 * 9, cubeTransform);
		m_CubeTexture2->Unbind();

		// ------------ cleanup ------------ //
		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void DepthTestingLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();

		ImGui::Begin("Depth Testing Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Toggle Depth Testing:");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Enable"))
			m_EnableDepthTest = true;
		if (ImGui::Button("Disable"))
			m_EnableDepthTest = false;
		ImGui::Text("----------------------------");
		ImGui::Text("Set Depth Testing Function:");
		ImGui::Text("----------------------------");
		if(ImGui::Button("ALWAYS"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::ALWAYS);
		ImGui::Text("-> The latest vertex pushed in that location will get rendered. \n-> This is the same as disabling depth testing.");
		if(ImGui::Button("NEVER"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::NEVER);
		ImGui::Text("-> Whilst this flag is active nothing will get rendered.");
		if(ImGui::Button("LESS THAN"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS);
		ImGui::Text("-> The vertex only gets rendered if its depth value is less than the stored depth value.");
		if (ImGui::Button("GREATER THAN"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::GREATER);
		ImGui::Text("-> The vertex only gets rendered if its depth value is greater than the stored depth value.");
		if(ImGui::Button("EQUAL"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::EQUAL);
		ImGui::Text("-> The vertex only gets rendered if its depth value is equal to the stored depth value.");
		if (ImGui::Button("NOT EQUAL"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::NOT_EQUAL);
		ImGui::Text("-> The vertex only gets rendered if its depth value is not equal to the stored depth value.");
		if(ImGui::Button("LESS THAN OR EQUAL"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS_EQUAL);
		ImGui::Text("-> The vertex only gets rendered if its depth value is less than or equal to the stored depth value.");
		if(ImGui::Button("GREAT THAN OR EQUAL"))
			Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::GREATER_EQUAL);
		ImGui::Text("-> The vertex only gets rendered if its depth value is greater than or equal to the stored depth value.");
		ImGui::Text("----------------------------");
		ImGui::End();
	}

	void DepthTestingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [5] -> Depth Testing"))
			m_IsActive = true;
		ImGui::End();
	}

	void DepthTestingLayer::OnEvent(Exalted::Event& event)
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