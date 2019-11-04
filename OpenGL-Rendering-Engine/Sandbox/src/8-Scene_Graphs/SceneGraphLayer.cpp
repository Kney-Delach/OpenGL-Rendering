/***************************************************************************
 * Filename		: SceneGraphLayer.cpp
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: This layer contains a scene which utilizes this engines 
                  scene graph storage structure.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "SceneGraphLayer.h"

namespace Sandbox
{
	SceneGraphLayer::SceneGraphLayer()
		: Layer("Scene Graph Layer", true)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void SceneGraphLayer::OnAttach()
	{
		EX_INFO("Scene Graph layer attached successfully.");
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Root object");
		Exalted::Ref<Exalted::Mesh> rootMesh = Exalted::Mesh::Create();
		rootMesh->CreateQuad();
		m_SceneRoot->SetMesh(rootMesh);

		m_pBoxObject = new Exalted::GameObject("Box GameObject");
		Exalted::Ref<Exalted::Mesh> boxMesh = Exalted::Mesh::Create();
		boxMesh->CreateCube();
		m_pBoxObject->SetMesh(boxMesh);
		glm::mat4 localTransformBox = glm::translate(glm::mat4(1.f), glm::vec3(5.0f, 2.0f, 2.0f));
		m_pBoxObject->GetTransform()->SetLocalTransform(localTransformBox);
		m_pBoxObject->GetTransform()->SetScaleTransform(glm::vec3(2.f));

		m_SceneRoot->AddChildObject(m_pBoxObject);
	}

	void SceneGraphLayer::OnDetach()
	{
		EX_INFO("Scene Graph Layer detached successfully.");
	}

	void SceneGraphLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		if (m_ProcessingCameraMovement)
			m_EditorCamera.UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::Renderer::BeginScene(m_EditorCamera);

		m_SceneRoot->Update(deltaTime);
		m_SceneRoot->Draw();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void SceneGraphLayer::OnImGuiRender()
	{
		ImGui::Begin("Scene Graph Camera Transform");
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

		ImGui::Begin("Scene Graph Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		if (ImGui::Button("Remove Cube From Root"))
			m_SceneRoot->RemoveChildObject(m_pBoxObject);
		ImGui::End();
	}

	void SceneGraphLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [8] -> Scene Graph"))
			m_IsActive = true;
		ImGui::End();
	}

	void SceneGraphLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_EditorCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void SceneGraphLayer::OnEvent(Exalted::Event& event)
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