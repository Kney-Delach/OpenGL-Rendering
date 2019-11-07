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

		m_MeshCube = Exalted::Mesh::Create();
		m_MeshCube->CreateCube();

		m_Shader = Exalted::Shader::Create("Resources/Shaders/VBasicShaderSMOOTH.glsl", "Resources/Shaders/FBasicShaderSMOOTH.glsl");
	}

	void EditorShowcaseLayer::OnDetach()
	{
		EX_INFO("Editor Showcase Layer deatched successfully.");
	}

	void EditorShowcaseLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera.UpdateCamera(deltaTime);
		
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_EditorCamera);
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); // hard constructing the identity matrix in glm for analysis purposes.
		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void EditorShowcaseLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();
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

	void EditorShowcaseLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera.OnEvent(event);
	}
}