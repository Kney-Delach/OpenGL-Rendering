/***************************************************************************
 * Filename		: GLSLExplorationLayer.h
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: This layer contains a scene I used to explore GLSL.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "GLSLExplorationLayer.h"

#define TEAPOT "Resources/Meshes/Teapot.obj"
#define SUZANNE "Resources/Meshes/Suzanne.obj"
#define NANOSUIT "Resources/Meshes/nanosuit.obj"
#define F16 "Resources/Meshes/f16.obj"
#define BBIRD "Resources/Meshes/BastionBird.obj"
#define BUNNY "Resources/Meshes/bunny.obj"
#define SYMMETRA "Resources/Meshes/Symmetra/symmetra.obj"
#define DABROVIC_SPONZA "Resources/Meshes/Dabrovic-Sponza/sponza.obj"
#define CRYTEK_SPONZA "Resources/Meshes/Crytek-Sponza/sponza.obj"

#define RIGHT	"Resources/Textures/Skyboxes/Crater/Right.tga"
#define LEFT	"Resources/Textures/Skyboxes/Crater/Left.tga"
#define TOP		"Resources/Textures/Skyboxes/Crater/Top.tga"
#define BOTTOM	"Resources/Textures/Skyboxes/Crater/Bottom.tga"
#define FRONT	"Resources/Textures/Skyboxes/Crater/Front.tga"
#define BACK	"Resources/Textures/Skyboxes/Crater/Back.tga"

namespace Sandbox
{
	GLSLExplorationLayer::GLSLExplorationLayer()
		: Layer("GLSL Exploration Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
		m_EditorCamera->SetMouseSpeed(100.f);
	}

	void GLSLExplorationLayer::OnAttach()
	{
		EX_INFO("GLSL Exploration layer attached successfully.");

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
		m_ModelShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/ModelVertex.glsl", "Resources/Shaders/GLSL-Exploration/ModelFragment.glsl");
		m_SkyboxShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/SkyboxVertex.glsl", "Resources/Shaders/GLSL-Exploration/SkyboxFragment.glsl");

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

	void GLSLExplorationLayer::OnDetach()
	{
		EX_INFO("GLSL Exploration Layer detached successfully.");
	}

	void GLSLExplorationLayer::OnUpdate(Exalted::Timestep deltaTime)
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

	void GLSLExplorationLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();
		ImGui::Begin("GLSL Exploration Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Choose an environment mapping technique below:");
		//if (ImGui::Button("Reflection"))
		//{
		//	m_Toggle = 0;
		//}
		//if (ImGui::Button("Refraction"))
		//{
		//	m_Toggle = 1;
		//}
		//if (ImGui::Button("Refraction"))
		//{
		//	m_Toggle = 1;
		//}
		ImGui::End();
	}

	void GLSLExplorationLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [14] -> GLSL Exploration"))
			m_IsActive = true;
		ImGui::End();
	}

	void GLSLExplorationLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}