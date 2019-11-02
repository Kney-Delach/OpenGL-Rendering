/***************************************************************************
 * Filename		: EditorShowcaseLayer.h
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: This layer showcases the Editor FPS camera implementation.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "EditorShowcaseLayer.h"

namespace Sandbox
{
	EditorShowcaseLayer::EditorShowcaseLayer()
		: Layer("Editor Camera Showcase", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void EditorShowcaseLayer::OnAttach()
	{
		EX_INFO("Editor Showcase Layer attached successfully.");

		m_MeshCube.reset(Exalted::Mesh::Create());
		m_MeshCube->CreateCube();

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/VBasicShaderSMOOTH.glsl", "Resources/Shaders/FBasicShaderSMOOTH.glsl"));
		Exalted::OpenGLConfigurations::EnableDepthTesting();
	}

	void EditorShowcaseLayer::OnDetach()
	{
		EX_INFO("Editor Showcase Layer deatched successfully.");
	}

	void EditorShowcaseLayer::OnUpdate(Exalted::Timestep deltaTime)
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

	void EditorShowcaseLayer::OnImGuiRender()
	{
		ImGui::Begin("Editor Camera Transform");
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
		ImGui::InputFloat3("Position", (float*)& m_EditorCamera.GetPosition());
		ImGui::InputFloat("Yaw", (float*)& m_EditorCamera.GetYaw());
		ImGui::InputFloat("Pitch", (float*)& m_EditorCamera.GetPitch());
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
		ImGui::InputFloat("Movement Speed", (float*)& m_EditorCamera.GetMovementSpeed(), 0.01f, 10.f);
		ImGui::InputFloat("Mouse Sensitivity", (float*)& m_EditorCamera.GetSensitivitiy(), 0.01f, 10.f);
		ImGui::End();
		ImGui::Begin("Editor Camera Showcase Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void EditorShowcaseLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [3] -> 3D EditorCamera Showcase"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}

	void EditorShowcaseLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_EditorCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void EditorShowcaseLayer::OnEvent(Exalted::Event& event)
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
