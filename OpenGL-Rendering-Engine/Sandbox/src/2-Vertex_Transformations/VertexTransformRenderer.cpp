/***************************************************************************
 * Filename		: VertexTransformRenderer.cpp
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: Contains the code for rendering vertex transformations to the screen.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "VertexTransformRenderer.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Sandbox
{
	VertexTransformLayer::VertexTransformLayer()
		: Layer("[2]: Vertex Transformation Layer", false), m_SceneCamera(-1.f, 1.f, -1.f, 1.f)
	{
		// ------------------------ Mesh Setup ------------------------ // 

		m_MeshTriangle.reset(Exalted::Mesh::Create());
		m_MeshTriangle->CreateTriangle();
		m_MeshQuad.reset(Exalted::Mesh::Create());
		m_MeshQuad->CreateQuad();

		// ------------------------ Shader setup ------------------------ // 

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderSMOOTH.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderSMOOTH.glsl"));

		// ------------------------ Transform setup ------------------------ // 

		m_TriangleTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
		m_QuadTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	}

	void VertexTransformLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		Exalted::OpenGLConfigurations::SetPointSize(m_PointSize);

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_SceneCamera);
		Exalted::Renderer::Submit(m_Shader, m_MeshTriangle, m_TriangleTransform);
		Exalted::Renderer::Submit(m_Shader, m_MeshQuad, m_QuadTransform);

		Exalted::Renderer::EndScene();
	}

	void VertexTransformLayer::OnImGuiRender()
	{
		ImGui::Begin("Vertex Transformation Scene Settings");
		if (ImGui::Button("Disable Scene"))
		{
			m_IsActive = false;
		}
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text(m_DrawMode.c_str());
		ImGui::SliderInt("Point Size", &m_PointSize, 1, 240);

		ImGui::End();
	}


	void VertexTransformLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [2] -> Vertex Transformation"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}
	void VertexTransformLayer::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_1)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);
				m_DrawMode = "Current Draw Mode: POINT";
			}
			if (e.GetKeyCode() == EX_KEY_2)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);
				m_DrawMode = "Current Draw Mode: LINE";
			}
			if (e.GetKeyCode() == EX_KEY_3)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
				m_DrawMode = "Current Draw Mode: FILL";
			}
		}
	}

	void VertexTransformLayer::OnAttach()
	{
		EX_INFO("Tutorial 2 - Layer Attached");
	}

	void VertexTransformLayer::OnDetach()
	{
		EX_INFO("Tutorial 2 - Layer Detached");
	}
}