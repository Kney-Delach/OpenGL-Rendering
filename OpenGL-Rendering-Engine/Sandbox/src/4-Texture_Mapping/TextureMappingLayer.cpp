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
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

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

		m_Mesh.reset(Exalted::Mesh::Create());
		m_Mesh->CreateTexturedQuad();

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/TexturedVertex.glsl", "Resources/Shaders/TexturedFragment.glsl"));
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		m_BrickTexture2D.reset(Exalted::Texture2D::Create("Resources/Textures/brick.tga",
			Exalted::TextureFormat::RGBA, Exalted::TextureWrap::REPEAT, Exalted::TextureMagFilter::NEAREST, Exalted::TextureMinFilter::NEAREST, false, 0));
		m_ChessBoardTexture2D.reset(Exalted::Texture2D::Create("Resources/Textures/BarrenReds.JPG", 
			Exalted::TextureFormat::RGBA, Exalted::TextureWrap::REPEAT, Exalted::TextureMagFilter::NEAREST, Exalted::TextureMinFilter::NEAREST, false, 0));

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

		m_BrickTexture2D->Bind(0);
		Exalted::Renderer::Submit(m_Shader, m_Mesh, glm::mat4(1.0f));
		m_ChessBoardTexture2D->Bind(0);
		const glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(1.0f, 0.0f, 0.0f));
		Exalted::Renderer::Submit(m_Shader, m_Mesh, transform);

		Exalted::Renderer::EndScene();

		m_ChessBoardTexture2D->Unbind();
		m_BrickTexture2D->Unbind();
	
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
