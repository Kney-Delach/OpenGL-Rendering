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
#include "imgui/imgui.h"

Sandbox::EditorShowcaseLayer::EditorShowcaseLayer()
	: Layer("3D EditorCamera Showcase", true)
{
}

void Sandbox::EditorShowcaseLayer::OnAttach()
{
	m_MeshCube.reset(Exalted::Mesh::Create());
	m_MeshCube->CreateCube();

	m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderSMOOTH.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderSMOOTH.glsl"));
	Exalted::OpenGLConfigurations::EnableDepthTesting();
}

void Sandbox::EditorShowcaseLayer::OnDetach()
{
}

void Sandbox::EditorShowcaseLayer::OnUpdate(Exalted::Timestep deltaTime)
{
	if(m_ProcessingCameraMovement)
	{
		m_EditorCamera.UpdateCamera(deltaTime);
	}

	Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
	Exalted::RenderCommand::Clear();

	Exalted::Renderer::BeginScene(m_EditorCamera);
	Exalted::Renderer::Submit(m_Shader, m_MeshCube, glm::mat4(1.0f));
	Exalted::Renderer::EndScene();
	
}

void Sandbox::EditorShowcaseLayer::OnImGuiRender()
{
	ImGui::Begin("EditorCamera Showcase Scene Settings");
	if (ImGui::Button("Disable Scene"))
		m_IsActive = false;
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::End();
}

void Sandbox::EditorShowcaseLayer::OnInactiveImGuiRender()
{
	ImGui::Begin("Disabled Scenes Settings");
	if (ImGui::Button("Enable Scene [3] -> 3D EditorCamera Showcase"))
	{
		m_IsActive = true;
	}
	ImGui::End();
}

void Sandbox::EditorShowcaseLayer::OnEvent(Exalted::Event& event)
{
	if((event.GetEventType() == Exalted::EventType::MouseButtonPressed) && !m_MouseMoving)
	{
		auto& e = static_cast<Exalted::MouseButtonPressedEvent&>(event);
		if (e.GetMouseButton() == EX_MOUSE_BUTTON_2)
		{
			m_FirstMouseMovement = true;
			m_ProcessingMouseMovement = true;
			m_MouseMoving = true;
		}
	}
	if(event.GetEventType() == Exalted::EventType::MouseButtonReleased)
	{
		auto& e = static_cast<Exalted::MouseButtonReleasedEvent&>(event);
		if(e.GetMouseButton() == EX_MOUSE_BUTTON_2)
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
		if(m_FirstMouseMovement)
		{
			m_LastMouseX = e.GetX();
			m_LastMouseY = e.GetY();
			m_FirstMouseMovement = false;
		}
		float xOffset = e.GetX() - m_LastMouseX;
		float yOffset = m_LastMouseY - e.GetY();

		m_LastMouseX = e.GetX();
		m_LastMouseY = e.GetY();
		m_EditorCamera.ProcessMouseMoveEvent(xOffset, yOffset);
	}
}