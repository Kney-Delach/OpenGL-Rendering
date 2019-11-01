/***************************************************************************
 * Filename		: TextureMappingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: This layer contains a scene showcasing texture mapping capabilities 
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "TextureMappingLayer.h"
//#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
//#include "imgui/imgui_internal.h"

namespace Sandbox
{
	TextureMappingLayer::TextureMappingLayer()
		: Layer("Texture Mapping Layer", true)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void TextureMappingLayer::OnAttach()
	{
		EX_INFO("Texture MappingLayer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //
		m_Meshes.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			m_Meshes.emplace_back(Exalted::Mesh::Create());
			m_Meshes[i]->CreateTexturedQuad(static_cast<float>(i+1));
		}

		m_Mesh3D.reset(Exalted::Mesh::Create());
		m_Mesh3D->CreateTexturedCube(1);

		// ------------------------- Initialize Textures ------------------------- //

		m_Textures.reserve(24);
		for (int textureWrap = 0; textureWrap < 2; textureWrap++)
		{
			for (int textureMagFilter = 0; textureMagFilter < 2; textureMagFilter++)
			{
				for (int textureMinFilter = 0; textureMinFilter < 6; textureMinFilter++)
				{
					m_Textures.emplace_back(Exalted::Texture2D::Create("Resources/Textures/TexGrid.png",
						Exalted::TextureFormat::RGBA, 
						static_cast<Exalted::TextureWrap>(textureWrap), 
						static_cast<Exalted::TextureMagFilter>(textureMagFilter), 
						static_cast<Exalted::TextureMinFilter>(textureMinFilter), 
						false, 
						0));
				}
			}
		}

		m_Texture3D.reset(Exalted::Texture2D::Create("Resources/Textures/TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR,
			false,
			0));

		// ------------------------- Initialize Block Transformations ------------------------- //

		for (unsigned i = 0; i < m_Meshes.size(); i++)
		{
			glm::mat4 meshTransform = glm::mat4(1.0f);
			meshTransform = glm::translate(meshTransform, glm::vec3(0.f, i * 1.0f, 0.f));
			for (int j = 0; j < m_Textures.size(); j++)
			{
				meshTransform = glm::translate(meshTransform, glm::vec3(1.f, 0.0f, 0.f));
				meshTransforms.push_back(meshTransform);
			}
		}

		// ------------------------- Initialize Shader ------------------------- //

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl"));
		Exalted::OpenGLConfigurations::EnableDepthTesting();

		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();
	}

	void TextureMappingLayer::OnDetach()
	{
		EX_INFO("Texture Mapping Layer deatched successfully.");
	}

	void TextureMappingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		if (m_ProcessingCameraMovement)
		{
			m_EditorCamera.UpdateCamera(deltaTime);
		}

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_EditorCamera);

		glm::mat4 cubeTransform = glm::mat4(1.0f);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(0.f, 0.0f, 5.f));
		m_Texture3D->Bind();
		Exalted::Renderer::Submit(m_Shader, m_Mesh3D, 6*6*9, cubeTransform);
		m_Texture3D->Unbind();

		unsigned transformCount = 0;
		for (unsigned i = 0; i < m_Meshes.size(); i++)
		{
			for (unsigned j = 0; j < m_Textures.size(); j++)
			{
				m_Textures[j]->Bind();
				Exalted::Renderer::Submit(m_Shader, m_Meshes[i], meshTransforms[transformCount++]);
			}
		}
		Exalted::Renderer::EndScene();
		
		m_Textures[0]->Unbind();
	}

	void TextureMappingLayer::OnImGuiRender()
	{
		ImGui::Begin("Camera Transform");
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
		ImGui::InputFloat3("Position", (float*) &m_EditorCamera.GetPosition());
		ImGui::InputFloat("Yaw", (float*) &m_EditorCamera.GetYaw());
		ImGui::InputFloat("Pitch", (float*) &m_EditorCamera.GetPitch());
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
		ImGui::InputFloat("Movement Speed", (float*) &m_EditorCamera.GetMovementSpeed(), 0.01f, 10.f);
		ImGui::InputFloat("Mouse Sensitivity", (float*)& m_EditorCamera.GetSensitivitiy(), 0.01f, 10.f);
		ImGui::End();

		ImGui::Begin("Texture Mapping Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void TextureMappingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [4] -> Texture Mapping"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}

	void TextureMappingLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_EditorCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void TextureMappingLayer::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::WindowResize)
		{
			OnWindowResize(static_cast<Exalted::WindowResizeEvent&>(event));
		}
		if ((event.GetEventType() == Exalted::EventType::MouseButtonPressed) && !m_MouseMoving)
		{
			auto& e = static_cast<Exalted::MouseButtonPressedEvent&>(event);
			if (e.GetMouseButton() == EX_MOUSE_BUTTON_2)
			{
				m_FirstMouseMovement = true;
				m_ProcessingMouseMovement = true;
				m_MouseMoving = true;
			}
		}
		if (event.GetEventType() == Exalted::EventType::MouseButtonReleased)
		{
			auto& e = static_cast<Exalted::MouseButtonReleasedEvent&>(event);
			if (e.GetMouseButton() == EX_MOUSE_BUTTON_2)
			{
				m_ProcessingMouseMovement = false;
				m_MouseMoving = false;
			}
		}
		if (event.GetEventType() == Exalted::EventType::MouseScrolled)
		{
			auto& e = static_cast<Exalted::MouseScrolledEvent&>(event);
			m_EditorCamera.ProcessMouseScrollEvent(e.GetYOffset());
		}
		if (m_ProcessingMouseMovement && (event.GetEventType() == Exalted::EventType::MouseMoved))
		{
			auto& e = static_cast<Exalted::MouseMovedEvent&>(event);
			if (m_FirstMouseMovement)
			{
				m_LastMouseX = e.GetX();
				m_LastMouseY = e.GetY();
				m_FirstMouseMovement = false;
			}
			float xOffset = e.GetX() - m_LastMouseX;
			float yOffset = m_LastMouseY - e.GetY();

			m_LastMouseX = e.GetX();
			m_LastMouseY = e.GetY();
			m_EditorCamera.ProcessRotationEvent(xOffset, yOffset);
		}

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