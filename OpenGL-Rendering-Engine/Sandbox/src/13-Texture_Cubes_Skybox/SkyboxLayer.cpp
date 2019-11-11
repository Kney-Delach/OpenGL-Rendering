/***************************************************************************
 * Filename		: SkyboxLayer.h
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: This layer contains a scene which displays a skybox using a cubemap.
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "SkyboxLayer.h"

#define SUZANNE "Resources/Meshes/Suzanne.obj"
#define NANOSUIT "Resources/Meshes/nanosuit.obj"
#define TEAPOT "Resources/Meshes/Teapot.obj"

#define RIGHT	"Resources/Textures/Skyboxes/Crater/Right.tga"
#define LEFT	"Resources/Textures/Skyboxes/Crater/Left.tga"
#define TOP		"Resources/Textures/Skyboxes/Crater/Top.tga"
#define BOTTOM	"Resources/Textures/Skyboxes/Crater/Bottom.tga"
#define FRONT	"Resources/Textures/Skyboxes/Crater/Front.tga"
#define BACK	"Resources/Textures/Skyboxes/Crater/Back.tga"

namespace Sandbox
{
	SkyboxLayer::SkyboxLayer()
		: Layer("Texture Cubemaps Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
			m_EditorCamera->SetMouseSpeed(100.f);
	}

	void SkyboxLayer::OnAttach()
	{
		EX_INFO("Texture Cubemaps layer attached successfully.");

		// ----- Textures
		Exalted::Ref<Exalted::Texture2D> whiteTexture = Exalted::Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		const std::vector<std::string> faces
		{
			FRONT, BACK, TOP, BOTTOM, RIGHT, LEFT,
		};
		m_SkyboxTexture = Exalted::TextureCube::Create(faces);

		// -------- Shader 
		m_ModelShader = Exalted::Shader::Create("Resources/Shaders/SkyboxShaders/ModelSkymapVertex.glsl", "Resources/Shaders/SkyboxShaders/ModelSkymapFragment.glsl");
		m_SkyboxShader = Exalted::Shader::Create("Resources/Shaders/SkyboxShaders/SkyboxVertex.glsl", "Resources/Shaders/SkyboxShaders/SkyboxFragment.glsl");

		// ----------- Meshes
		m_ModelMesh = Exalted::Mesh::Create();
		m_ModelMesh->SetVertexArray(Exalted::ObjLoader::Load(TEAPOT));
		m_SkyboxMesh = Exalted::Mesh::Create();
		m_SkyboxMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateSkybox());

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root (Suzanne)");
		m_SceneRoot->SetTexture(whiteTexture);
		m_SceneRoot->SetBoundingRadius(FLT_MAX);
		m_SceneManager->SetSceneRoot(m_SceneRoot);
	}

	void SkyboxLayer::OnDetach()
	{
		EX_INFO("Texture Cubemaps Layer detached successfully.");
	}

	void SkyboxLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		//m_SceneManager->UpdateScene(deltaTime);

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//m_SceneManager->RenderScene();// render rest of scene

		// model rendering
		m_ModelShader->Bind();
		m_SkyboxTexture->Bind();
		glm::mat4 projection = m_EditorCamera->GetProjectionMatrix();
		glm::mat4 view = glm::mat4(glm::mat3(m_EditorCamera->GetViewMatrix()));
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformMatFloat4("u_ViewProjection", m_EditorCamera->GetViewProjectionMatrix());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformMatFloat4("u_Model", glm::mat4(1.f));
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformInt1("u_Skybox", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformInt1("u_Toggle", m_Toggle);

		Exalted::Renderer::Submit(m_ModelMesh);

		// skybox rendering
		Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS_EQUAL);
		m_SkyboxShader->Bind();
		m_SkyboxTexture->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformMatFloat4("u_ViewMatrix", view);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformMatFloat4("u_ProjectionMatrix", projection);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformInt1("u_Skybox", 0);
		Exalted::Renderer::Submit(m_SkyboxMesh, 36);
		m_SkyboxTexture->Unbind();
		m_SkyboxShader->Unbind();

		Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS);
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::Renderer::EndScene();
	}

	void SkyboxLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();
		ImGui::Begin("Texture Cubemaps (skybox) Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Choose an environment mapping technique below:");
		if(ImGui::Button("Reflection"))
		{
			m_Toggle = 0;
		}
		if (ImGui::Button("Refraction"))
		{
			m_Toggle = 1;
		}
		ImGui::End();
	}

	void SkyboxLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [13] -> Texture Cubemaps - Skybox - Environment Mapping"))
			m_IsActive = true;
		ImGui::End();
	}

	void SkyboxLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}