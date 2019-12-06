/***************************************************************************
 * Filename		: TessellationLayer.cpp
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: This layer contains a scene which displays tesselated terrain.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "TessellationLayer.h"

#define TESSELLATION_VERTEX		"Resources/Shaders/Tessellation/Vertex.glsl"
#define TESSELLATION_FRAGMENT	"Resources/Shaders/Tessellation/Fragment.glsl"
#define TESSELLATION_GEOMETRY	"Resources/Shaders/Tessellation/Geometry.glsl"
#define TESSELLATION_EVALUATION	"Resources/Shaders/Tessellation/TessEvaluation.glsl"
#define TESSELLATION_CONTROL	"Resources/Shaders/Tessellation/TessControl.glsl"

namespace Sandbox
{
	TessellationLayer::TessellationLayer()
		: Layer("Tessellation Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000.f);
		m_EditorCamera->SetMouseSpeed(10.f);
	}

	void TessellationLayer::OnAttach()
	{
		EX_INFO("Tessellation layer attached successfully. {0}", TIME);

		// ---------------- Setup tesselation data ------------------- //
		m_TessellationShader = Exalted::Shader::Create(TESSELLATION_VERTEX, TESSELLATION_FRAGMENT, "", TESSELLATION_EVALUATION, TESSELLATION_CONTROL); //todo: insert geometry shader
		
		m_TessellationMesh = Exalted::Mesh::Create();
		m_TessellationMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateFlatQuad(100, 100));
		u_TerrainColorMap = Exalted::Texture2D::Create("Resources/Textures/Terrain/Sand.jpg",
			Exalted::TextureFormat::RGB,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			true,
			0,
			true);

		u_TerrainHeightMap = Exalted::Texture2D::Create("Resources/Textures/Terrain/Lake.png",//"HeightMap_GrandCanyon.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0,
			false);
		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true);

		// ----------------------------- Configure Uniform Buffer Object ----------------------------- //
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TessellationShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		Exalted::Bytes bufferSize = 4 * sizeof(glm::mat4);
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes blockBindingIndex = 2;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(blockBindingIndex, offset, bufferSize);


		Exalted::OpenGLConfigurations::SetPatchVerticeCount(4);
	}

	void TessellationLayer::OnDetach()
	{
		EX_INFO("Tessellation Layer detached successfully.");
	}

	void TessellationLayer::OnUpdate(Exalted::Timestep deltaTime)
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

		// ----------------------------- Render Tessellation Terrain ----------------------------- //
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::FRONT);

		u_TerrainHeightMap->Bind(0);
		u_TerrainColorMap->Bind(1);
		m_TessellationShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TessellationShader)->SetUniformFloat1("u_TerrainScale", m_TerrainScale);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TessellationShader)->SetUniformInt1("u_HeightMap", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_TessellationShader)->SetUniformInt1("u_ColorMap", 1);
		Exalted::Renderer::SubmitTessellated(m_TessellationMesh);
		m_TessellationShader->Unbind();
		u_TerrainHeightMap->Unbind();
		u_TerrainColorMap->Unbind();

		Exalted::OpenGLConfigurations::DisableFaceCulling();

		// --------------------------- Render Skybox --------------------------- //
		
		m_SceneManager->RenderSkybox();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void TessellationLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI(); //todo: Change this to scene manager 

		ImGui::Begin("Tessellation Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::InputFloat("Terrain Height Scale", &m_TerrainScale);
		ImGui::End();
	}

	void TessellationLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [16] -> Tessellation"))
			m_IsActive = true;
		ImGui::End();
	}

	void TessellationLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}