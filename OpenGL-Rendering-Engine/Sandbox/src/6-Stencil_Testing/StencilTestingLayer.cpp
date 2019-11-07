/***************************************************************************
 * Filename		: StencilTestingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This layer contains a scene showcasing stencil testing capabilities.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "StencilTestingLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Sandbox
{
	StencilTestingLayer::StencilTestingLayer()
		: Layer("Stencil Testing Layer", false)
	{
		const float windowWidth = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth());
		const float windowHeight = static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight());
		m_EditorCamera.SetAspectRatio(windowWidth / windowHeight);
	}

	void StencilTestingLayer::OnAttach()
	{
		EX_INFO("Stencil Testing layer attached successfully.");

		// ------------------------- Initialize Meshes ------------------------- //

		m_MeshFloor = Exalted::Mesh::Create();
		m_MeshFloor->CreateTexturedQuad(1);

		m_MeshCube = Exalted::Mesh::Create();
		m_MeshCube->CreateTexturedCube(1);

		// ------------------------- Initialize Textures ------------------------- //

		m_FloorTexture = Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_CubeTexture = Exalted::Texture2D::Create("Resources/Textures/TexContainer.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_CubeTexture2 = Exalted::Texture2D::Create("Resources/Textures/TexGridBW.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_ChessboardTexture = Exalted::Texture2D::Create("Resources/Textures/TexChessboard.tga",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		// ------------------------- Initialize Transformations ------------------------- //

		// --------- Cube transforms --------- //
		glm::mat4 cubeTransform = glm::mat4(1.0f);
		
		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.5f, 5.f));
		m_CubeTransform1 = cubeTransform; 
		m_CubeTransformOutline1 = glm::scale(m_CubeTransform1, glm::vec3(1.1, 1.1, 1.1));

		cubeTransform = glm::translate(cubeTransform, glm::vec3(2.f, 0.0f, 5.f));
		m_CubeTransform2 = cubeTransform;
		m_CubeTransformOutline2 = glm::scale(m_CubeTransform2, glm::vec3(1.1, 1.1, 1.1));

		// ------- Floor tile transforms ------ //
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
			meshTransform = glm::translate(meshTransform, glm::vec3(column++ * 1.0f, 0.f, rowLength));
			meshTransform = glm::rotate(meshTransform, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
			m_FloorTransforms.push_back(meshTransform);
		}

		// ------------------------- Initialize Shader ------------------------- //

		m_ChessboardShader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/Blending/GrassFragmentShader.glsl");
		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		m_OutlineShader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FRedShader.glsl");

		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();

	}

	void StencilTestingLayer::OnDetach()
	{
		EX_INFO("Stencil Testing Layer deatched successfully.");
	}

	void StencilTestingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera.UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::Renderer::BeginScene(m_EditorCamera);

		// ------------ Draw the floor ------------ //

		Exalted::OpenGLConfigurations::SetStencilMaskReadOnly();
		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_Shader, m_MeshFloor, m_FloorTransforms[i]);
		m_FloorTexture->Unbind();

		// ------------ Initialize Stencil Buffer Processing ------------ //

		if (m_EnableStencilTesting)
			Exalted::OpenGLConfigurations::EnableStencilTesting();
		if (m_EnableScissorTesting)
			Exalted::OpenGLConfigurations::EnableScissorTesting();

		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::SetStencilActions(Exalted::StencilAction::KEEP, Exalted::StencilAction::KEEP, Exalted::StencilAction::REPLACE);


		// --------------------- Draw the regular cubes --------------------- //
		Exalted::OpenGLConfigurations::SetStencilFunction(Exalted::StencilAction::ALWAYS, 1, 0xFF);
		Exalted::OpenGLConfigurations::SetStencilMaskWriteALL();
		m_CubeTexture->Bind();
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, m_CubeTransform1);
		Exalted::Renderer::Submit(m_Shader, m_MeshCube, 6 * 6 * 9, m_CubeTransform2);
		//m_CubeTexture->Unbind();

		// --------------------- Draw Stencil outlines --------------------- //
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::SetStencilFunction(Exalted::StencilAction::NOT_EQUAL, 1, 0xFF);
		Exalted::OpenGLConfigurations::SetStencilMaskReadOnly();
		m_CubeTexture2->Bind();
		Exalted::Renderer::Submit(m_OutlineShader, m_MeshCube, 6 * 6 * 9, m_CubeTransformOutline1);
		Exalted::Renderer::Submit(m_OutlineShader, m_MeshCube, 6 * 6 * 9, m_CubeTransformOutline2);
		//m_CubeTexture2->Unbind();
		Exalted::OpenGLConfigurations::SetStencilMaskWriteALL();

		// ------------------------------ Draw Chess Board ------------------------------ //
		Exalted::OpenGLConfigurations::SetColorMask(false, false, false, false);
		Exalted::OpenGLConfigurations::SetStencilFunction(Exalted::StencilAction::ALWAYS, 2, 0xFF);
		Exalted::OpenGLConfigurations::SetStencilActions(Exalted::StencilAction::REPLACE, Exalted::StencilAction::REPLACE, Exalted::StencilAction::REPLACE);
		const glm::mat4 chessboardPosition = glm::translate(m_CubeTransform1, glm::vec3(0.f, 0.f, -2.f));
		m_ChessboardTexture->Bind();
		Exalted::Renderer::Submit(m_ChessboardShader, m_MeshFloor, chessboardPosition);
		Exalted::OpenGLConfigurations::SetColorMask(true, true, true, true);
		Exalted::OpenGLConfigurations::SetStencilFunction(Exalted::StencilAction::EQUAL, 2, 0xFF);
		Exalted::OpenGLConfigurations::SetStencilActions(Exalted::StencilAction::KEEP, Exalted::StencilAction::KEEP, Exalted::StencilAction::KEEP);
		Exalted::Renderer::Submit(m_ChessboardShader, m_MeshFloor, chessboardPosition);
		m_ChessboardTexture->Unbind();

		
		// ------------ cleanup ------------ //
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableStencilTesting();
		Exalted::OpenGLConfigurations::DisableScissorTesting();

		Exalted::Renderer::EndScene();
	}

	void StencilTestingLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();

		ImGui::Begin("Stencil Testing Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Toggle Stencil Testing:");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Enable Stencil"))
			m_EnableStencilTesting = true;
		if (ImGui::Button("Disable Stencil"))
			m_EnableStencilTesting = false;
		ImGui::Text("----------------------------");
		ImGui::Text("Toggle Scissor Testing:");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Enable Scissor Testing"))
			m_EnableScissorTesting = true;
		if (ImGui::Button("Disable Scissor Testing"))
			m_EnableScissorTesting = false;
		if (ImGui::Button("Integrate new Scissor Region Values"))
			Exalted::OpenGLConfigurations::SetScissorBox(static_cast<int>(m_ScissorTestRegion.x), static_cast<int>(m_ScissorTestRegion.y), static_cast<int>(m_ScissorTestRegion.z), static_cast<int>(m_ScissorTestRegion.w));
		ImGui::DragFloat4("Scissor Region Values (x position,y position,width,height)", (float*)& m_ScissorTestRegion,1,0,1440);
		ImGui::End();
	}

	void StencilTestingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [6] -> Stencil Testing"))
			m_IsActive = true;
		ImGui::End();
	}

	void StencilTestingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera.OnEvent(event);

		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = dynamic_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_1)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);

			if (e.GetKeyCode() == EX_KEY_2)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);

			if (e.GetKeyCode() == EX_KEY_3)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
		}
	}
}