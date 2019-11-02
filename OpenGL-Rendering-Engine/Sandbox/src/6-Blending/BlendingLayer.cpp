/***************************************************************************
 * Filename		: BlendingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 01/11/2019
 * Description	: This layer contains a scene showcasing blending capabilities.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "BlendingLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Sandbox
{
	BlendingLayer::BlendingLayer()
		: Layer("Blending Layer", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void BlendingLayer::OnAttach()
	{
		EX_INFO("Blending layer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_Meshes.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			m_Meshes.emplace_back(Exalted::Mesh::Create());
			m_Meshes[i]->CreateTexturedQuad(static_cast<float>(i + 1));
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
					m_Textures.emplace_back(Exalted::Texture2D::Create("Resources/Textures/tex_BrickWall.jpg",
						Exalted::TextureFormat::RGBA,
						static_cast<Exalted::TextureWrap>(textureWrap), //Exalted::TextureWrap::REPEAT,
						static_cast<Exalted::TextureMagFilter>(textureMagFilter),
						static_cast<Exalted::TextureMinFilter>(textureMinFilter), //Exalted::TextureMinFilter::LINEAR_LINEAR,
						false,
						0));
				}
			}
		}

		m_Texture3D.reset(Exalted::Texture2D::Create("Resources/Textures/TexLava.jpg",//TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::NEAR_LINEAR,
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

	void BlendingLayer::OnDetach()
	{
		EX_INFO("Blending Layer deatched successfully.");
	}

	void BlendingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		if (m_ProcessingCameraMovement)
		{
			m_EditorCamera.UpdateCamera(deltaTime);
		}

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_EditorCamera);

		// ----------------------- Quads ----------------------- //

		unsigned transformCount = 0;
		for (unsigned i = 0; i < m_Meshes.size(); i++)
		{
			for (unsigned j = 0; j < m_Textures.size(); j++)
			{
				m_Textures[j]->Bind();
				Exalted::Renderer::Submit(m_Shader, m_Meshes[i], meshTransforms[transformCount++]);
			}
		}
		// --------------------- Cube Animation ----------------------- //

		if (m_AnimateCube)
		{
			m_Mesh3D->GetVertexArray()->GetVertexBuffers()[0]->Bind();
			void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); // getting the buffer pointer from opengl
			float* currentBufferVertices = static_cast<float*>(ptr);
			for (int i = 0; i < 36; i++)
			{
				//EX_ERROR("texture co-ordinate 1: {0}   -  value: {1} ", 7 + i*9, currentBufferVertices[7 + i * 9]);
				//EX_ERROR("texture co-ordinate 2: {0}   -  value: {1} ", 8 + i * 9, currentBufferVertices[8 + i * 9]);
				if (m_AnimatePositiveDirection)
				{
					currentBufferVertices[7 + i * 9] = currentBufferVertices[7 + i * 9] + deltaTime * 0.1f;
					currentBufferVertices[8 + i * 9] = currentBufferVertices[8 + i * 9] + deltaTime * 0.1f;
				}
				else
				{
					currentBufferVertices[7 + i * 9] = currentBufferVertices[7 + i * 9] - deltaTime * 0.1f;
					currentBufferVertices[8 + i * 9] = currentBufferVertices[8 + i * 9] - deltaTime * 0.1f;
				}
			}
			memcpy(ptr, currentBufferVertices, sizeof(currentBufferVertices));
			glUnmapBuffer(GL_ARRAY_BUFFER); // informing opengl that done with pointer
			m_Mesh3D->GetVertexArray()->GetVertexBuffers()[0]->Unbind();
		}

		// --------------------- Cube ----------------------- //
		glm::mat4 cubeTransform = glm::mat4(1.0f);
		cubeTransform = glm::translate(cubeTransform, glm::vec3(0.f, 0.0f, 5.f));
		m_Texture3D->Bind();
		Exalted::Renderer::Submit(m_Shader, m_Mesh3D, 6 * 6 * 9, cubeTransform);
		m_Texture3D->Unbind();


		// ------------ cleanup 
		m_Textures[0]->Unbind();
		Exalted::Renderer::EndScene();
	}

	void BlendingLayer::OnImGuiRender()
	{
		ImGui::Begin("Blending Camera Transform");
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

		ImGui::Begin("Blending Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		if (ImGui::Button("Animate Lava Cube!"))
		{
			m_AnimateCube = !m_AnimateCube;
		}
		if (ImGui::Button("Toggle Animation Direction!"))
		{
			m_AnimatePositiveDirection = !m_AnimatePositiveDirection;
		}
		ImGui::End();

	}

	void BlendingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [6] -> Blending"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}

	void BlendingLayer::OnWindowResize(Exalted::WindowResizeEvent& resizeEvent)
	{
		const auto windowWidth = resizeEvent.GetWidth();
		const auto windowHeight = resizeEvent.GetHeight();
		m_EditorCamera.OnWindowResize(windowWidth, windowHeight);
	}

	void BlendingLayer::OnEvent(Exalted::Event& event)
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