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
#include <glm/gtc/matrix_transform.hpp>

namespace Sandbox
{
	VertexTransformLayer::VertexTransformLayer()
		: Layer("Vertex Transformation Layer", false),
		m_OrthoCamera(-640.f, 640.f, -360.f, 360.f, -1.0f, 10000.0f),
		m_PerspCamera(45.f, 1280.f/720.f, 1.0f, 10000.0f)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_PerspCamera.SetAspectRatio(windowWidth / windowHeight);

		// ------------------------ Mesh Setup ------------------------ // 

		m_MeshTriangle = Exalted::Mesh::Create();
		m_MeshTriangle->CreateTriangle();
		m_MeshTriangle2 = Exalted::Mesh::Create();
		m_MeshTriangle2->CreateTriangle();
		m_MeshTriangle3 = Exalted::Mesh::Create();
		m_MeshTriangle3->CreateTriangle();
		m_MeshQuad = Exalted::Mesh::Create();
		m_MeshQuad->CreateQuad();

		// ------------------------ Shader setup ------------------------ // 

		m_Shader = Exalted::Shader::Create("Resources/Shaders/VBasicShaderSMOOTH.glsl", "Resources/Shaders/FBasicShaderSMOOTH.glsl");
	}

	void VertexTransformLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		Exalted::OpenGLConfigurations::SetPointSize(m_PointSize);

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		m_TriangleTransform = glm::translate(glm::mat4(1.f), m_TrianglePosition);
		m_TriangleTransform = glm::scale(m_TriangleTransform, m_TriangleScale);

		m_TriangleTransform2 = glm::translate(glm::mat4(1.f), m_TrianglePosition2);
		m_TriangleTransform2 = glm::scale(m_TriangleTransform2, m_TriangleScale2);

		m_TriangleTransform3 = glm::translate(glm::mat4(1.f), m_TrianglePosition3);
		m_TriangleTransform3 = glm::scale(m_TriangleTransform3, m_TriangleScale3);

		m_QuadTransform = glm::translate(glm::mat4(1.f), m_QuadPosition);
		m_QuadTransform = glm::scale(m_QuadTransform, m_QuadScale);

		if(m_CameraType)
		{
			m_OrthoCamera.SetPosition(m_OrthoCameraPosition);
			m_OrthoCamera.SetRotation(m_OrthoCameraRotation);

			m_CameraString = "Current Camera: Orthographic";
			Exalted::Renderer::BeginScene(m_OrthoCamera);
		}
		else
		{
			m_PerspCamera.SetFOV(m_PerspectiveFOV);
			m_PerspCamera.SetPosition(m_PerspCameraPosition);

			m_CameraString = "Current Camera: Perspective";
			Exalted::Renderer::BeginScene(m_PerspCamera);
		}

		Exalted::Renderer::Submit(m_Shader, m_MeshTriangle, m_TriangleTransform);
		Exalted::Renderer::Submit(m_Shader, m_MeshTriangle2, m_TriangleTransform2);
		Exalted::Renderer::Submit(m_Shader, m_MeshTriangle3, m_TriangleTransform3);

		Exalted::Renderer::Submit(m_Shader, m_MeshQuad, m_QuadTransform);

		Exalted::Renderer::EndScene();
	}

	void VertexTransformLayer::OnImGuiRender()
	{
		ImGui::Begin("Vertex Transformation Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("----------------------------------------------------");
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------------------------------");
		ImGui::Text(m_DrawMode.c_str());
		ImGui::SliderInt("Point Size", &m_PointSize, 1, 240);
		ImGui::Text("----------------------------------------------------");
		ImGui::Text(m_CameraString.c_str());
		if (ImGui::Button("Switch Camera Type"))
			m_CameraType = !m_CameraType;
		ImGui::Text("----------------------------------------------------");
		ImGui::Text(" Orthographic Camera Settings");
		ImGui::Text("----------------------------------------------------");
		ImGui::InputFloat3("Orthographic Position", (float*)&m_OrthoCameraPosition);
		ImGui::SliderFloat("Orthographic Rotation", (float*)& m_OrthoCameraRotation, 1.0f, 360.0f);
		ImGui::Text("----------------------------------------------------");
		ImGui::Text(" Perspective Camera Settings");
		ImGui::Text("----------------------------------------------------");
		ImGui::InputFloat3("Perspective Position", (float*)& m_PerspCameraPosition);
		ImGui::SliderFloat("Perspective FOV", (float*)&m_PerspectiveFOV, 45.f, 100.f);

		ImGui::Text("----------------------------------------------------");
		ImGui::Text(" Mesh Settings");
		ImGui::Text("----------------------------------------------------");
		ImGui::InputFloat3("Triangle Position", (float*)& m_TrianglePosition);
		ImGui::InputFloat3("Triangle Scale", (float*)& m_TriangleScale);

		ImGui::InputFloat3("Triangle 2 Position", (float*)& m_TrianglePosition2);
		ImGui::InputFloat3("Triangle 2 Scale", (float*)& m_TriangleScale2);

		ImGui::InputFloat3("Triangle 3 Position", (float*)& m_TrianglePosition3);
		ImGui::InputFloat3("Triangle 3 Scale", (float*)& m_TriangleScale3);

		ImGui::InputFloat3("Quad Position", (float*)& m_QuadPosition);
		ImGui::InputFloat3("Quad Scale", (float*)& m_QuadScale);

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

	void Sandbox::VertexTransformLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_PerspCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void VertexTransformLayer::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::WindowResize)
		{
			OnWindowResize(static_cast<Exalted::WindowResizeEvent&>(event));
		}

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
		EX_INFO("Vertex Transformation layer attached successfully. ");
	}

	void VertexTransformLayer::OnDetach()
	{
		EX_INFO("Vertex Transformation layer detached successfully. ");
	}
}