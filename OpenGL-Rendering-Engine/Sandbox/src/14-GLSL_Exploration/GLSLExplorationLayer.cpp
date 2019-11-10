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

#include "GLFW/include/GLFW/glfw3.h" //todo: Remove this and add a static application call to get the current time.

namespace Sandbox
{
	GLSLExplorationLayer::GLSLExplorationLayer()
		: Layer("GLSL Exploration Layer", false)
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
		m_ExplosionShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/ExplosionVertex.glsl", "Resources/Shaders/GLSL-Exploration/ExplosionFragment.glsl", "Resources/Shaders/GLSL-Exploration/ExplosionGeometry.glsl");
		m_NormalsShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/NormalsVertex.glsl", "Resources/Shaders/GLSL-Exploration/NormalsFragment.glsl", "Resources/Shaders/GLSL-Exploration/NormalsGeometry.glsl");
		m_ModelShader = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/ModelVertex.glsl", "Resources/Shaders/GLSL-Exploration/ModelFragment.glsl");
		m_ModelShader2 = Exalted::Shader::Create("Resources/Shaders/GLSL-Exploration/ModelVertex.glsl", "Resources/Shaders/GLSL-Exploration/ModelFragment2.glsl");
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

		// ----------------------------- Configure Uniform Buffer Object ----------------------------- //
		// configure shader bindings
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformBlockIndex("Matrices", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader2)->SetUniformBlockIndex("Matrices", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformBlockIndex("Matrices", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformBlockIndex("Matrices", 0);

		// create uniform buffer
		Exalted::Bytes bufferSize = sizeof(glm::mat4); //only 1 as uploading a single view projection matrix
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);

		const Exalted::Bytes blockBindingIndex = 0;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(blockBindingIndex, offset, bufferSize); 		// define range of buffer that links to a uniform binding point
	}

	void GLSLExplorationLayer::OnDetach()
	{
		EX_INFO("GLSL Exploration Layer detached successfully.");
	}

	void GLSLExplorationLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);
		Exalted::OpenGLConfigurations::EnableDepthTesting();

		// Set uniform buffer object once per loop
		m_MatUniformBuffer->Bind();
		Exalted::Bytes offset = 0;
		Exalted::Bytes size = sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewProjectionMatrix()));
		m_MatUniformBuffer->Unbind();

		// Render models

		m_SkyboxTexture->Bind();
		glm::mat4 transform = glm::mat4(1.f);
		glm::mat4 projection = m_EditorCamera->GetProjectionMatrix();
		glm::mat4 view = glm::mat4(glm::mat3(m_EditorCamera->GetViewMatrix()));
		
		if (m_ExplosionToggle) // draw explosions for object 1
		{
			m_ExplosionShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformInt1("u_Skybox", 0);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_View", m_EditorCamera->GetViewMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Projection", m_EditorCamera->GetProjectionMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformFloat1("u_Time", glfwGetTime()); //todo: abstract this get time call
			Exalted::Renderer::Submit(m_ModelMesh);
		}
		else
		{ // draw object 1 regularly
			m_ModelShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformInt1("u_Skybox", 0);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
			Exalted::Renderer::Submit(m_ModelMesh);
		}

		// draw normals for object 1
		if (m_NormalToggle)
		{
			m_NormalsShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_View", m_EditorCamera->GetViewMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Projection", m_EditorCamera->GetProjectionMatrix());
			Exalted::Renderer::Submit(m_ModelMesh);
		}

		transform = glm::translate(glm::mat4(1.f), glm::vec3(100.0, 0.0, 0.0));
		if (m_ExplosionToggle) // draw explosions for object 2
		{
			m_ExplosionShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformInt1("u_Skybox", 0);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_View", m_EditorCamera->GetViewMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Projection", m_EditorCamera->GetProjectionMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ExplosionShader)->SetUniformFloat1("u_Time", glfwGetTime()); //todo: abstract this get time call
			Exalted::Renderer::Submit(m_ModelMesh);
		}
		else
		{ // draw object 2 regularly
			m_ModelShader2->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader2)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader2)->SetUniformInt1("u_Skybox", 0);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ModelShader2)->SetUniformFloat3("u_CameraPosition", m_EditorCamera->GetPosition());
			Exalted::Renderer::Submit(m_ModelMesh);
		}

		// draw normals  for object 2
		if(m_NormalToggle)
		{
			m_NormalsShader->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Model", transform);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_View", m_EditorCamera->GetViewMatrix());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_NormalsShader)->SetUniformMatFloat4("u_Projection", m_EditorCamera->GetProjectionMatrix());
			Exalted::Renderer::Submit(m_ModelMesh);
		}

		// Skybox rendering
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
		ImGui::Begin("GLSL Exploration (uniform buffer objects) Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		if(ImGui::Button("Toggle Normal Display"))
		{
			m_NormalToggle = !m_NormalToggle;
		}
		if (ImGui::Button("Toggle Object Explosion"))
		{
			m_ExplosionToggle = !m_ExplosionToggle;
		}
		ImGui::End();
	}

	void GLSLExplorationLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [14] -> GLSL Exploration (UBO / Geometry)"))
			m_IsActive = true;
		ImGui::End();
	}

	void GLSLExplorationLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}