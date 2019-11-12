/***************************************************************************
 * Filename		: InstancingLayer.h
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: This layer contains a scene which utilizes instanced drawing.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "InstancingLayer.h"

namespace Sandbox
{
	InstancingLayer::InstancingLayer()
		: Layer("Instancing Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000000.f);
		m_EditorCamera->SetMouseSpeed(100.f);
	}

	void InstancingLayer::OnAttach()
	{
		EX_INFO("Instancing layer attached successfully. {0}", TIME);

		// ----- Textures
		Exalted::Ref<Exalted::Texture2D> whiteTexture = Exalted::Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		const std::vector<std::string> faces
		{
			SKYBOX_FRONT, SKYBOX_BACK, SKYBOX_TOP, SKYBOX_BOTTOM, SKYBOX_RIGHT, SKYBOX_LEFT,
		};
		m_SkyboxTexture = Exalted::TextureCube::Create(faces);

		// -------- Shader 
		m_InstancedModelShader = Exalted::Shader::Create("Resources/Shaders/InstancingShaders/InstancedModelVertex.glsl", "Resources/Shaders/InstancingShaders/InstancedModelFragment.glsl");
		m_ModelShader = Exalted::Shader::Create("Resources/Shaders/InstancingShaders/ModelVertex.glsl", "Resources/Shaders/InstancingShaders/ModelFragment.glsl");
		m_SkyboxShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/SkyboxVertex.glsl", "Resources/Shaders/GLSL-Exploration/SkyboxFragment.glsl");

		// ----------- Meshes
		m_SkyboxMesh = Exalted::Mesh::Create();
		m_SkyboxMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateSkybox());

		// ------------ Instanced Model Transformations and vertex binding

		m_ModelMesh = Exalted::Mesh::Create();
		m_ModelMesh->SetVertexArray(Exalted::ObjLoader::Load(TEAPOT));

		unsigned row = 0;
		int column = 0;
		for (int i = 0; i < m_InstanceCount; ++i)
		{
			if (i % (m_InstanceCount/100) == 0)
			{
				column += 2;
				row = 0;
			}
			m_Transformations.emplace_back(glm::translate(glm::mat4(1.f), glm::vec3(300 * row++, 100* column, 0)));
		}
		vb = Exalted::VertexBuffer::Create(glm::value_ptr(m_Transformations[0]), m_InstanceCount * 16);
		const Exalted::BufferLayout layout =
		{
			{Exalted::ShaderDataType::Float4, "a_Model"},
			{Exalted::ShaderDataType::Float4, "a_Model"},
			{Exalted::ShaderDataType::Float4, "a_Model"},
			{Exalted::ShaderDataType::Float4, "a_Model"}
		};
		vb->SetLayout(layout);
		m_ModelMesh->GetVertexArray()->AddVertexBufferDivisor(vb, 3, 1);
		
		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root [NOT ACTIVE IN SCENE]");
		m_SceneRoot->SetTexture(whiteTexture);
		m_SceneRoot->SetBoundingRadius(FLT_MAX);
		m_SceneManager->SetSceneRoot(m_SceneRoot);

		// ----------------------------- Configure Uniform Buffer Object ----------------------------- //
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformBlockIndex("Matrices2", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_InstancedModelShader)->SetUniformBlockIndex("Matrices2", 1);
		Exalted::Bytes bufferSize = sizeof(glm::mat4);
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes blockBindingIndex = 1;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(blockBindingIndex, offset, bufferSize);
	}

	void InstancingLayer::OnDetach()
	{
		EX_INFO("Instancing Layer detached successfully.");
	}

	void InstancingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);
		Exalted::OpenGLConfigurations::EnableDepthTesting();

		// Set uniform buffer object
		m_MatUniformBuffer->Bind();
		Exalted::Bytes offset = 0;
		Exalted::Bytes size = sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewProjectionMatrix()));
		m_MatUniformBuffer->Unbind();

		// Render models
		m_SkyboxTexture->Bind();
		if(!m_ToggleInstancing) // not instanced rendering (single draw calls)
		{
			m_ModelShader->Bind();
			for (int i = 0; i < m_InstanceCount; i++)
			{
				std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformMatFloat4("u_Model", m_Transformations[i]);
				std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformInt1("u_Skybox", 0);
				std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
				Exalted::Renderer::Submit(m_ModelMesh);
			}
		}
		else
		{
			m_InstancedModelShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_InstancedModelShader)->SetUniformInt1("u_Skybox", 0);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_InstancedModelShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
			unsigned long long quantity = m_InstanceCount;
			Exalted::Renderer::SubmitInstanced(m_ModelMesh, quantity);
		}

		// Skybox rendering
		Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS_EQUAL);
		m_SkyboxShader->Bind();
		m_SkyboxTexture->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformMatFloat4("u_ViewMatrix", glm::mat4(glm::mat3(m_EditorCamera->GetViewMatrix())));
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformMatFloat4("u_ProjectionMatrix", m_EditorCamera->GetProjectionMatrix());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_SkyboxShader)->SetUniformInt1("u_Skybox", 0);
		Exalted::Renderer::Submit(m_SkyboxMesh, 36);
		m_SkyboxTexture->Unbind();
		m_SkyboxShader->Unbind();

		Exalted::OpenGLConfigurations::SetDepthFunction(Exalted::DepthMode::LESS);
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::Renderer::EndScene();
	}

	void InstancingLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneRoot->RenderHierarchyGUI();
		ImGui::Begin("Instancing Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Total Object Instances: %i", m_InstanceCount);

		if (ImGui::Button("Toggle Instancing Display"))
		{
			m_ToggleInstancing = !m_ToggleInstancing;
		}
		ImGui::End();
	}

	void InstancingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [15] -> Instancing"))
			m_IsActive = true;
		ImGui::End();
	}

	void InstancingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}