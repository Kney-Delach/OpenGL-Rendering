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

		// ---------------- Setup Light A Data ------------------- //
		m_LightSourceShader = Exalted::Shader::Create(LIGHT_ORIGIN_VERTEX, LIGHT_ORIGIN_FRAGMENT);
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));
		m_LightA = Exalted::Light::Create(); 
		m_LightA->Transform->Position = glm::vec3(2.5f, 12.5f, 7.f);
		m_LightA->Ambient = glm::vec3(0.2); 
		m_LightA->Diffuse = glm::vec3(0.5);
		m_LightA->Specular = glm::vec3(1.0);

		// ---------------- Setup Object A (effected by light) ------------------- //
		m_ObjectShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);
		m_ObjectMesh = Exalted::Mesh::Create();
		m_ObjectMesh->SetVertexArray(Exalted::ObjLoader::Load(LIGHT_SOURCE_MESH));

		// --------------------- Setup Scene objects --------------------- //
		glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
		const float shininess = 32.f;

		unsigned row = 0;
		int column = 0;
		for (int i = 0; i < m_ObjectCount; ++i)
		{
			if (i % (m_ObjectCount / 5) == 0)
			{
				column += 2;
				row = 0;
			}
			m_ObjectMaterialS.emplace_back(Exalted::Material::Create(color, color, color, shininess));
			m_ObjectTransformations.emplace_back(glm::translate(glm::mat4(1.f), glm::vec3(3 * row++, 2* column, 0)));
		}

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true);

		// ----------------------------- Configure Uniform Buffer Objects ----------------------------- //
		// camera UBO data
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		Exalted::Bytes bufferSize = 4 * sizeof(glm::mat4);
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes bbi = 2;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(bbi, offset, bufferSize);

		// lighting UBO setup 
		const Exalted::Bytes numberOfLights = 1;
		const Exalted::Bytes lightsBBI = 1;
		const Exalted::Bytes lightsOffset = 0;
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Light_Uniforms", lightsBBI);
		Exalted::Bytes lightsBufferSize = numberOfLights * Exalted::Light::UBSize();
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);
	}

	void LightingLayer::OnDetach()
	{
		EX_INFO("Lighting Layer detached successfully.");
	}

	void LightingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		// ---------------------------- Update Light Positions ---------------------------- //
		if (m_RotateLight)
		{
			float lightX = 5.f + 10.f * sin(TIME);
			float lightY = m_LightA->Transform->Position.y;
			float lightZ = 10.f * cos(TIME);
			m_LightA->Transform->Position = glm::vec3(lightX, lightY, lightZ);
		}

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		// ----------------------------- Set camera matrices data ----------------------------- // todo: Abstract this into the camera object (Camera->UpdateUniformBuffer)
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

		// --------------------------- Set Light uniform data ------------------------------- // Iterate over this for each light source 
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		m_LightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_LightUniformBuffer->Unbind();

		// ----------------------------- Render Scene ----------------------------- //
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//m_SceneManager->RenderScene();

		// render objects 
		m_ObjectShader->Bind();
		for (int i = 0; i < m_ObjectCount; i++)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformMatFloat4("u_Model", m_ObjectTransformations[i]);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_Material.Ambient", m_ObjectMaterialS[i]->Ambient);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_Material.Diffuse", m_ObjectMaterialS[i]->Diffuse);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_Material.Specular", m_ObjectMaterialS[i]->Specular);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat1("u_Material.Shininess", m_ObjectMaterialS[i]->Shininess);
			Exalted::Renderer::Submit(m_ObjectMesh);
		}
		m_ObjectShader->Unbind();

		// render light sources 
		m_LightSourceShader->Bind();
		//todo: verify this works correctly
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_LightA->Transform->SetAndGetWorldTransform());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_LightA->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);
		m_LightSourceShader->Unbind();

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
		ImGui::InputFloat3("Light Source Position", glm::value_ptr(m_LightA->Transform->Position));
		if(ImGui::Button("Toggle Light Rotation"))
		{
			m_RotateLight = !m_RotateLight;
		}
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