/***************************************************************************
 * Filename		: HeigtMapLayer.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene with a height map!
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "HeightMapLayer.h"

namespace Sandbox
{
	HeightMapLayer::HeightMapLayer()
		: Layer("Height Map Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
		m_EditorCamera->SetMouseSpeed(2000.f);
	}

	void HeightMapLayer::OnAttach()
	{
		EX_INFO("Height Map layer attached successfully.");

		// ----------------- Terrain ----------------- //
		m_TerrainShader = Exalted::Shader::Create("Resources/Shaders/VTerrain.glsl", "Resources/Shaders/FTerrain.glsl");
		m_TerrainShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TerrainShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_TerrainShader->Unbind();

		m_TerrainTexture = Exalted::Texture2D::Create("Resources/Textures/tex_BrickWall.jpg",
			Exalted::TextureFormat::RGB,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			true,
			0);

		m_HeightMap = Exalted::Mesh::Create();
		m_HeightMap->CreateHeightMap("Resources/Textures/terrain.raw");

		Exalted::GameObject* terrainGameObject = new Exalted::GameObject("Terrain");
		terrainGameObject->SetMesh(m_HeightMap);
		terrainGameObject->SetShader(m_TerrainShader);
		terrainGameObject->SetTexture(m_TerrainTexture);
		terrainGameObject->GetTransform()->Scale = glm::vec3(40.f, .5f, 40.f);
		terrainGameObject->SetBoundingRadius(FLT_MAX);

		// -------------- Scene manager/root Setup ------------ //
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root");
		m_SceneManager->SetSceneRoot(m_SceneRoot);
		m_SceneRoot->AddChildObject(terrainGameObject);
	}

	void HeightMapLayer::OnDetach()
	{
		EX_INFO("Height Map Layer detached successfully.");
	}

	void HeightMapLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		m_SceneManager->RenderScene();
		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableFaceCulling();
	}

	void HeightMapLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();

		ImGui::Begin("Height Map Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::End();
	}

	void HeightMapLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [9] -> Height Map"))
			m_IsActive = true;
		ImGui::End();
	}

	void HeightMapLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}