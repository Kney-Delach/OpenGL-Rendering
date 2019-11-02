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
		: Layer("Depth Testing Layer", true)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void DepthTestingLayer::OnAttach()
	{
		EX_INFO("Depth testing layer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_MeshFloor.reset(Exalted::Mesh::Create());
		m_MeshFloor->CreateTexturedQuad(1);
		
		m_MeshCube.reset(Exalted::Mesh::Create());
		m_MeshCube->CreateTexturedCube(1);

		// ------------------------- Initialize Textures ------------------------- //

		m_FloorTexture.reset(Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0));

		m_CubeTexture.reset(Exalted::Texture2D::Create("Resources/Textures/TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0));

		m_CubeTexture2.reset(Exalted::Texture2D::Create("Resources/Textures/TexGridBW.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0));

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

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl"));
		m_DepthShader.reset(Exalted::Shader::Create("Resources/Shaders/VDepthShader.glsl", "Resources/Shaders/FDepthShader.glsl"));

		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();

		m_DepthShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_DepthShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_DepthShader->Unbind();

		// ------------- Enable depth testing ------------- //

		Exalted::OpenGLConfigurations::EnableDepthTesting();
	}

	void DepthTestingLayer::OnDetach()
	{
		EX_INFO("Depth Testing Layer deatched successfully.");
	}

	void DepthTestingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		if (m_ProcessingCameraMovement)	
			m_EditorCamera.UpdateCamera(deltaTime);
		
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

		// ------------ cleanup 
		Exalted::Renderer::EndScene();
	}

	void DepthTestingLayer::OnImGuiRender()
	{
		ImGui::Begin("Depth Testing Camera Transform");
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
		ImGui::InputFloat3("Position", (float*) &m_EditorCamera.GetPosition());
		ImGui::InputFloat("Yaw", (float*) &m_EditorCamera.GetYaw());
		ImGui::InputFloat("Pitch", (float*) &m_EditorCamera.GetPitch());
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
		ImGui::InputFloat("Movement Speed", (float*) &m_EditorCamera.GetMovementSpeed(), 0.01f, 10.f);
		ImGui::InputFloat("Mouse Sensitivity", (float*) &m_EditorCamera.GetSensitivitiy(), 0.01f, 10.f);
		ImGui::End();

		ImGui::Begin("Depth Testing Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Toggle Depth Testing:");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Enable"))
			Exalted::OpenGLConfigurations::EnableDepthTesting();
		if (ImGui::Button("Disable"))
			Exalted::OpenGLConfigurations::DisableDepthTesting();
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
		ImGui::Begin("Disabled Scene Settings");
		if (ImGui::Button("Enable Scene [5] -> Depth Testing"))
			m_IsActive = true;
		ImGui::End();
	}

	void DepthTestingLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_EditorCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void DepthTestingLayer::OnEvent(Exalted::Event& event)
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