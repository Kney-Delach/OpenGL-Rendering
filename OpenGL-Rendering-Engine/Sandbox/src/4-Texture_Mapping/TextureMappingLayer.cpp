/***************************************************************************
 * Filename		: EditorShowcaseLayer.cpp
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
#include "imgui/imgui.h"

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

		m_MeshCube.reset(Exalted::Mesh::Create());
		m_MeshCube->CreateCube();

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderSMOOTH.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderSMOOTH.glsl"));
		Exalted::OpenGLConfigurations::EnableDepthTesting();
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
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, glm::mat4(1.0f));
		Exalted::Renderer::EndScene();
	}

	void TextureMappingLayer::OnImGuiRender()
	{
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
	}
}