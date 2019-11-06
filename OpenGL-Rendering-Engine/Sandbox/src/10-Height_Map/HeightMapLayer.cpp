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
			10000.f);
		m_EditorCamera->SetPosition(glm::vec3(-2000,3000,3000));
	}

	void HeightMapLayer::OnAttach()
	{
		EX_INFO("Height Map layer attached successfully.");

		Exalted::Ref<Exalted::Mesh> cubeMesh = Exalted::Mesh::Create();
		cubeMesh->CreateCube();

		// ----------------- Terrain ----------------- //
		m_TerrainShader = Exalted::Shader::Create("Resources/Shaders/VTerrain.glsl", "Resources/Shaders/FTerrain.glsl");
		m_TerrainShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TerrainShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_TerrainShader->Unbind();
		m_TerrainTexture = Exalted::Texture2D::Create("Resources/Textures/BarrenReds.JPG",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		// -------------- Scene manager Setup ------------ //
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root");
		m_SceneRoot->SetMesh(cubeMesh); //todo: Automatically implement this as a sphere
		m_SceneManager->SetSceneRoot(m_SceneRoot);

		heightMap = Exalted::HeightMap::Create("Resources/Textures/terrain.raw");

		Exalted::GameObject* terrainGameObject = new Exalted::GameObject("Terrain");
		terrainGameObject->SetMesh(heightMap);
		terrainGameObject->SetShader(m_TerrainShader);
		terrainGameObject->SetTexture(m_TerrainTexture);
		terrainGameObject->GetTransform()->Scale = glm::vec3(10.f, 1.5f, 10.f);
		terrainGameObject->SetBoundingRadius(1000000.f);
		m_SceneRoot->AddChildObject(terrainGameObject);
	}

	void HeightMapLayer::OnDetach()
	{
		EX_INFO("Height Map Layer detached successfully.");
	}

	void HeightMapLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		m_SceneManager->UpdateScene(deltaTime);
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
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = dynamic_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_I)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);

			if (e.GetKeyCode() == EX_KEY_O)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);

			if (e.GetKeyCode() == EX_KEY_P)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
		}
	}
}