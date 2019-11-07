/***************************************************************************
 * Filename		: HeigtMapLayer.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene which loads an external Model!
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "ModelLoadingLayer.h"

namespace Sandbox
{
	ModelLoadingLayer::ModelLoadingLayer()
		: Layer("Model Loading Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
	}

	void ModelLoadingLayer::OnAttach()
	{
		EX_INFO("Model Loading layer attached successfully.");
		Exalted::Ref<Exalted::Mesh> rootMesh = Exalted::Mesh::Create();
		rootMesh->CreateCube();

		// ----------------- Model Setup ----------------- //
		m_ModelShader = Exalted::Shader::Create("Resources/Shaders/VTerrain.glsl", "Resources/Shaders/FTerrain.glsl");
		m_ModelShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_ModelShader->Unbind();

		m_ModelMesh = Exalted::Mesh::Create();
		//m_ModelMesh.CreateModel("INSERT_PATH_HERE");

		Exalted::GameObject* modelGameObject = new Exalted::GameObject("Model");
		//modelGameObject->SetMesh(m_ModelMesh);
		modelGameObject->SetShader(m_ModelShader);
		//modelGameObject->SetTexture(m_TerrainTexture);
		modelGameObject->SetBoundingRadius(FLT_MAX);

		// -------------- Scene manager/root Setup ------------ //
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root");
		m_SceneRoot->SetMesh(rootMesh);
		m_SceneManager->SetSceneRoot(m_SceneRoot);
		//m_SceneRoot->AddChildObject(modelGameObject);
	}

	void ModelLoadingLayer::OnDetach()
	{
		EX_INFO("Model Loading Layer detached successfully.");
	}

	void ModelLoadingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//Exalted::OpenGLConfigurations::EnableFaceCulling();
		//Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		m_SceneManager->UpdateScene(deltaTime);
		m_SceneManager->RenderScene();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		//Exalted::OpenGLConfigurations::DisableFaceCulling();
	}

	void ModelLoadingLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();

		ImGui::Begin("Model Loading Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::End();
	}

	void ModelLoadingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [10] -> Model Loading"))
			m_IsActive = true;
		ImGui::End();
	}

	void ModelLoadingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}