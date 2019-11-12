/***************************************************************************
 * Filename		: LightingLayer.cpp
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: This layer contains a scene which displays different lighting techniques.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "LightingLayer.h"

#define LIGHTING_SHADER_VERTEX		"Resources/Shaders/Lighting/LightingVertex.glsl"
#define LIGHTING_SHADER_FRAGMENT	"Resources/Shaders/Lighting/LightingFragment.glsl"

#define LIGHT_ORIGIN_VERTEX "Resources/Shaders/Lighting/LightOriginVertex.glsl"
#define LIGHT_ORIGIN_FRAGMENT "Resources/Shaders/Lighting/LightOriginFragment.glsl"

#define LIGHT_SOURCE_MESH "Resources/Meshes/cube.obj"
namespace Sandbox
{
	LightingLayer::LightingLayer()
		: Layer("Lighting Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000.f);
		m_EditorCamera->SetMouseSpeed(10.f);
	}

	void LightingLayer::OnAttach()
	{
		EX_INFO("Lighting layer attached successfully. {0}", TIME);

		// ---------------- Setup Lighting data ------------------- //
		m_LightingShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);

		Exalted::Ref<Exalted::Mesh> lightMesh = Exalted::Mesh::Create();
		lightMesh->SetVertexArray(Exalted::ObjLoader::Load(LIGHT_SOURCE_MESH));

		Exalted::GameObject* lightObjectA = new Exalted::GameObject("Light A");
		lightObjectA->SetMesh(lightMesh);
		lightObjectA->SetShader(m_LightingShader);
		lightObjectA->GetTransform()->Position = glm::vec3(0.f, 1.f, 0.f);
		lightObjectA->SetBoundingRadius(FLT_MAX);

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true);
		m_SceneManager->GetSceneRoot()->AddChildObject(lightObjectA);
		m_SceneManager->GetSceneRoot()->SetActive(true);

		// ----------------------------- Configure Uniform Buffer Object ----------------------------- //
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightingShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		Exalted::Bytes bufferSize = 4 * sizeof(glm::mat4);
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes blockBindingIndex = 2;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(blockBindingIndex, offset, bufferSize);


		Exalted::OpenGLConfigurations::SetPatchVerticeCount(4);
	}

	void LightingLayer::OnDetach()
	{
		EX_INFO("Lighting Layer detached successfully.");
	}

	void LightingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		// ----------------------------- Set camera matrices data ----------------------------- //
		m_MatUniformBuffer->Bind();
		Exalted::Bytes offset = 0;
		Exalted::Bytes size = sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewMatrix()));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(glm::mat4(glm::mat3(m_EditorCamera->GetViewMatrix()))));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetProjectionMatrix()));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewProjectionMatrix()));
		m_MatUniformBuffer->Unbind();

		// ----------------------------- Render Scene ----------------------------- //
		Exalted::OpenGLConfigurations::EnableDepthTesting();

		m_SceneManager->RenderScene();

		// --------------------------- Render Skybox --------------------------- //

		m_SceneManager->RenderSkybox();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void LightingLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI(); //todo: Change this to scene manager 

		ImGui::Begin("Lighting Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");

		ImGui::End();
	}

	void LightingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [17] -> Lighting"))
			m_IsActive = true;
		ImGui::End();
	}

	void LightingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}