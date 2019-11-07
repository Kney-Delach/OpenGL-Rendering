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
#include "Platform/OpenGL/OpenGLShader.h"

namespace Sandbox
{
	TextureMappingLayer::TextureMappingLayer()
		: Layer("Texture Mapping Layer", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void TextureMappingLayer::OnAttach()
	{
		EX_INFO("Texture MappingLayer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_Meshes.reserve(10);
		for (int i = 0; i < 10; i++)
		{
			m_Meshes.emplace_back(Exalted::Mesh::Create());
			m_Meshes[i]->CreateTexturedQuad(static_cast<float>(i+1));
		}

		m_Mesh3D = Exalted::Mesh::Create();
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

		m_Texture3D = Exalted::Texture2D::Create("Resources/Textures/TexLava.jpg",//TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::NEAR_LINEAR,
			false,
			0);

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

		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		Exalted::OpenGLConfigurations::EnableDepthTesting();

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
		m_EditorCamera.UpdateCamera(deltaTime);
		
		Exalted::OpenGLConfigurations::EnableDepthTesting();
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
				if(m_AnimatePositiveDirection)
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
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::Renderer::EndScene();
	}

	void TextureMappingLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();

		ImGui::Begin("Texture Mapping Scene Settings");
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

	void TextureMappingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [4] -> Texture Mapping"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}

	void TextureMappingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera.OnEvent(event);
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