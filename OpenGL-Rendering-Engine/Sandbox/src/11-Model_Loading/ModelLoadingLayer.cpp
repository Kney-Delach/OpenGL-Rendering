/***************************************************************************
 * Filename		: ModelLoadingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: This layer contains a scene with 3D Model loading!
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

#define TEAPOT "Resources/Meshes/Teapot.obj"
#define SUZANNE "Resources/Meshes/Suzanne.obj"
#define NANOSUIT "Resources/Meshes/nanosuit.obj"
#define F16 "Resources/Meshes/f16.obj"
#define BBIRD "Resources/Meshes/BastionBird.obj"
#define SYMMETRA "Resources/Meshes/Symmetra/symmetra.obj"

namespace Sandbox
{
	ModelLoadingLayer::ModelLoadingLayer()
		: Layer("Model Loading Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
	}

	void ModelLoadingLayer::OnAttach()
	{
		EX_INFO("Model Loading layer attached successfully.");
		// ----- Textures
		Exalted::Ref<Exalted::Texture2D> whiteTexture = Exalted::Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		Exalted::Ref<Exalted::Texture2D> transparentTexture = Exalted::Texture2D::Create("Resources/Textures/TexTransparentGlassStained.tga",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		Exalted::Ref<Exalted::Texture2D> symmetraTexture = Exalted::Texture2D::Create("Resources/Meshes/Symmetra/textures/Symmetra_D.tga.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		// -------- Shader 
		Exalted::Ref<Exalted::Shader> modelShader = Exalted::Shader::Create("Resources/Shaders/ModelShaders/ModelLoadVertex.glsl", "Resources/Shaders/ModelShaders/ModelLoadFragment.glsl");

		transparentTexture->Bind();
		modelShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(modelShader)->SetUniformInt1("u_DiffuseTexture", 0);
		modelShader->Unbind();
		transparentTexture->Unbind();

		// ----------- Meshes
		Exalted::Ref<Exalted::Mesh> nanosuitMesh = Exalted::Mesh::Create();
		nanosuitMesh->SetVertexArray(Exalted::ObjLoader::Load(NANOSUIT));
		Exalted::Ref<Exalted::Mesh> teapotMesh = Exalted::Mesh::Create();
		teapotMesh->SetVertexArray(Exalted::ObjLoader::Load(TEAPOT));
		Exalted::Ref<Exalted::Mesh> suzanneMesh = Exalted::Mesh::Create();
		suzanneMesh->SetVertexArray(Exalted::ObjLoader::Load(SUZANNE));
		Exalted::Ref<Exalted::Mesh> f16Mesh = Exalted::Mesh::Create();
		f16Mesh->SetVertexArray(Exalted::ObjLoader::Load(F16));
		Exalted::Ref<Exalted::Mesh> bbirdMesh = Exalted::Mesh::Create();
		bbirdMesh->SetVertexArray(Exalted::ObjLoader::Load(BBIRD));
		Exalted::Ref<Exalted::Mesh> symmetraMesh = Exalted::Mesh::Create();
		symmetraMesh->SetVertexArray(Exalted::ObjLoader::Load(SYMMETRA));

		// -------------- Gameobjects
		Exalted::GameObject* nanosuitGameobject = new Exalted::GameObject("Nanosuit");
		nanosuitGameobject->SetMesh(nanosuitMesh);
		nanosuitGameobject->SetShader(modelShader);
		nanosuitGameobject->SetTexture(whiteTexture);
		nanosuitGameobject->GetTransform()->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
		nanosuitGameobject->GetTransform()->Position = glm::vec3(0.f,0,-3.f);
		nanosuitGameobject->SetBoundingRadius(FLT_MAX);

		Exalted::GameObject* teapotGameobject = new Exalted::GameObject("Teapot");
		teapotGameobject->SetMesh(teapotMesh);
		teapotGameobject->SetShader(modelShader);
		teapotGameobject->SetTexture(transparentTexture);
		teapotGameobject->GetTransform()->Scale = glm::vec3(0.025f, 0.025f, 0.025f);
		teapotGameobject->GetTransform()->Position = glm::vec3(0.f, 0, 3.f);
		teapotGameobject->SetBoundingRadius(FLT_MAX);

		Exalted::GameObject* suzanneGameobject = new Exalted::GameObject("Suzanne");
		suzanneGameobject->SetMesh(suzanneMesh);
		suzanneGameobject->SetShader(modelShader);
		suzanneGameobject->SetTexture(whiteTexture);
		suzanneGameobject->GetTransform()->Scale = glm::vec3(1.f, 1.f, 1.f);
		suzanneGameobject->GetTransform()->Position = glm::vec3(0.f, 0, 6.f);
		suzanneGameobject->SetBoundingRadius(FLT_MAX);

		Exalted::GameObject* f16Gameobject = new Exalted::GameObject("F16");
		f16Gameobject->SetMesh(f16Mesh);
		f16Gameobject->SetShader(modelShader);
		f16Gameobject->SetTexture(transparentTexture);
		f16Gameobject->GetTransform()->Scale = glm::vec3(2.f, 2.f, 2.f);
		f16Gameobject->GetTransform()->Position = glm::vec3(0.f, 0, 12.f);
		f16Gameobject->SetBoundingRadius(FLT_MAX);

		Exalted::GameObject* bbirdGameobject = new Exalted::GameObject("Bastion's Bird");
		bbirdGameobject->SetMesh(bbirdMesh);
		bbirdGameobject->SetShader(modelShader);
		bbirdGameobject->SetTexture(whiteTexture);
		bbirdGameobject->GetTransform()->Scale = glm::vec3(1.f, 1.f, 1.f);
		bbirdGameobject->GetTransform()->Position = glm::vec3(0.f, 0, 15.f);
		bbirdGameobject->SetBoundingRadius(FLT_MAX);

		Exalted::GameObject* symmetra = new Exalted::GameObject("Symmetra");
		symmetra->SetMesh(symmetraMesh);
		symmetra->SetShader(modelShader);
		symmetra->SetTexture(symmetraTexture);
		symmetra->GetTransform()->Scale = glm::vec3(2.f, 2.f, 2.f);
		symmetra->GetTransform()->Position = glm::vec3(6.f, 0, 0.f);
		symmetra->SetBoundingRadius(FLT_MAX);

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root (Suzanne)");
		m_SceneRoot->SetMesh(suzanneMesh);
		m_SceneRoot->SetShader(modelShader);
		m_SceneRoot->SetTexture(whiteTexture);

		m_SceneManager->SetSceneRoot(m_SceneRoot);

		m_SceneRoot->AddChildObject(nanosuitGameobject);
		m_SceneRoot->AddChildObject(suzanneGameobject);
		m_SceneRoot->AddChildObject(teapotGameobject);
		m_SceneRoot->AddChildObject(f16Gameobject);
		m_SceneRoot->AddChildObject(bbirdGameobject);
		m_SceneRoot->AddChildObject(symmetra);
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
		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		m_SceneManager->UpdateScene(deltaTime);
		m_SceneManager->RenderScene();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();
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