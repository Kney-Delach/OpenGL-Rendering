/***************************************************************************
 * Filename		: FrameBufferLayer.cpp
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: This layer contains a scene which utilizes frame buffers to
                  perform post processing techniques.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "FrameBufferLayer.h"

#define SUZANNE "Resources/Meshes/Suzanne.obj"
#define CRYTEK_SPONZA "Resources/Meshes/Crytek-Sponza/sponza.obj"
#define BUNNY "Resources/Meshes/bunny.obj"

namespace Sandbox
{
	FrameBufferLayer::FrameBufferLayer()
		: Layer("Frame Buffer Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
	}

	void FrameBufferLayer::OnAttach()
	{
		EX_INFO("Frame Buffer layer attached successfully.");

		// ----- Textures
		Exalted::Ref<Exalted::Texture2D> whiteTexture = Exalted::Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		Exalted::Ref<Exalted::Texture2D> suzanneTexture = Exalted::Texture2D::Create("Resources/Textures/tex_BrickWall.jpg",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		// -------- Shader 
		m_FrameBufferShader = Exalted::Shader::Create("Resources/Shaders/FrameBufferShaders/Vertex.glsl", "Resources/Shaders/FrameBufferShaders/Fragment.glsl");
		m_FrameBufferShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_FrameBufferShader)->SetUniformInt1("u_ScreenTexture", 0);
		m_FrameBufferShader->Unbind();

		Exalted::Ref<Exalted::Shader> modelShader = Exalted::Shader::Create("Resources/Shaders/ModelShaders/ModelLoadVertex.glsl", "Resources/Shaders/ModelShaders/ModelLoadFragment.glsl");

		modelShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(modelShader)->SetUniformInt1("u_DiffuseTexture", 0);
		modelShader->Unbind();

		// ----------- Meshes
		Exalted::Ref<Exalted::Mesh> suzanneMesh = Exalted::Mesh::Create();
		suzanneMesh->SetVertexArray(Exalted::ObjLoader::Load(SUZANNE));
		m_QuadMesh = Exalted::Mesh::Create();
		m_QuadMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateIndexedQuad());

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root (Suzanne)");
		m_SceneRoot->SetMesh(suzanneMesh);
		m_SceneRoot->SetShader(modelShader);
		m_SceneRoot->SetTexture(whiteTexture);
		m_SceneRoot->SetBoundingRadius(FLT_MAX);
		m_SceneManager->SetSceneRoot(m_SceneRoot);

		m_FrameBuffer = Exalted::FrameBuffer::Create(static_cast<uint32_t>(Exalted::Application::Get().GetWindow().GetWindowWidth()), static_cast<uint32_t>(Exalted::Application::Get().GetWindow().GetWindowHeight()), Exalted::FrameBufferFormat::RGBA8);
	}

	void FrameBufferLayer::OnDetach()
	{
		EX_INFO("Framee Buffer Layer detached successfully.");
	}

	void FrameBufferLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);

		// first pass
		m_FrameBuffer->Bind();
		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.3f, 0.9f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::Renderer::BeginScene(*m_EditorCamera);
		m_SceneManager->UpdateScene(deltaTime);
		m_SceneManager->RenderScene();

		// second pass 
		m_FrameBuffer->Unbind();
		Exalted::RenderCommand::SetClearColor({ 0.2f, 0.4f, 1.f, 1.f });
		Exalted::RenderCommand::ClearColorBuffer();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		m_FrameBufferShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_FrameBufferShader)->SetUniformInt1("u_PostProcess", m_PostProcessChoice);
		m_FrameBuffer->BindTexture(0);
		Exalted::Renderer::Submit(m_QuadMesh);
		Exalted::Renderer::EndScene();
	}

	void FrameBufferLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();
		ImGui::Begin("Frame Buffer Post Processing Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Switch between different post processing techniques by dragging the bar below:");
		ImGui::Text("0 -> No Effect");
		ImGui::Text("1 -> Color Inversion");
		ImGui::Text("2 -> Weighted Gray Scaling");
		ImGui::Text("3 -> Sharpening");
		ImGui::Text("4 -> Blurring");
		ImGui::Text("5 -> Edge Detection");
		ImGui::DragInt(" ", &m_PostProcessChoice, 1, 0, 5);
		ImGui::End();
	}

	void FrameBufferLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [11] -> Frame Buffer - Post Processing"))
			m_IsActive = true;
		ImGui::End();
	}

	void FrameBufferLayer::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::WindowResize)
		{
			const auto resizeEvent = dynamic_cast<Exalted::WindowResizeEvent&>(event);
			const auto windowWidth = resizeEvent.GetWidth();
			const auto windowHeight = resizeEvent.GetHeight();
			m_FrameBuffer->Resize(static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight));
			
		}
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}