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
		m_LightSourceShaderA = Exalted::Shader::Create(LIGHT_ORIGIN_VERTEX, LIGHT_ORIGIN_FRAGMENT);
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));
		m_LightPosition = glm::vec3(3.f, 3.f, 3.f);
		m_LightSourceTransformationA = glm::translate(glm::mat4(1.f), m_LightPosition);
		m_LightSourceColorA = glm::vec3(1.0,1.0,1.0);

		// ---------------- Setup Object A (effected by light) ------------------- //
		m_ObjectShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);
		m_ObjectMesh = Exalted::Mesh::Create();
		m_ObjectMesh->SetVertexArray(Exalted::ObjLoader::Load(LIGHT_SOURCE_MESH));

		// --------------------- Setup object transformations 
		unsigned row = 0;
		int column = 0;
		for (int i = 0; i < m_ObjectCount; ++i)
		{
			if (i % (m_ObjectCount / 5) == 0)
			{
				column += 2;
				row = 0;
			}
			float val = (float)i / (float)m_ObjectCount;
			m_ObjectColors.emplace_back(glm::vec3(val, 0,0));
			m_Transformations.emplace_back(glm::translate(glm::mat4(1.f), glm::vec3(3 * row++, 2* column, 0)));
		}

		// -------------- Scene manager/root 
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true);

		// ----------------------------- Configure Uniform Buffer Objects ----------------------------- //
		// camera data
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShaderA)->SetUniformBlockIndex("Camera_Matrices", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		Exalted::Bytes bufferSize = 4 * sizeof(glm::mat4);
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes bbi = 2;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(bbi, offset, bufferSize);

		// light data 
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Light_Uniforms", 1);
		Exalted::Bytes bufferSize2 = sizeof(glm::vec4);
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(bufferSize2);
		const Exalted::Bytes bbi2 = 1;
		const Exalted::Bytes offset2 = 0;
		m_LightUniformBuffer->BindBufferRange(bbi2, offset2, bufferSize2);

		//todo: uncomment this if using terrain
		//Exalted::OpenGLConfigurations::SetPatchVerticeCount(4);
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

		// --------------------------- Set Light uniform data ------------------------------- //
		m_LightUniformBuffer->Bind();
		Exalted::Bytes offset2 = 0;
		Exalted::Bytes size2 = sizeof(glm::vec4);
		m_LightUniformBuffer->SetBufferSubData(offset2, size2, glm::value_ptr(m_EditorCamera->GetPosition())); // actually a vec3
		//offset2 += sizeof(glm::vec4);
		//m_LightUniformBuffer->SetBufferSubData(offset2, size2, glm::value_ptr(m_EditorCamera->GetPosition())); // actually a vec3
		m_LightUniformBuffer->Unbind();

		// ----------------------------- Render Scene ----------------------------- //
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//m_SceneManager->RenderScene();
		if(m_RotateLight)
		{
			float lightX = 5.f + 10.f * sin(TIME);
			float lightY = m_LightPosition.y;
			float lightZ = 10.f * cos(TIME);
			m_LightPosition= glm::vec3(lightX, lightY, lightZ);
		}
		m_LightSourceTransformationA = glm::translate(glm::mat4(1.f), m_LightPosition); // update light source position


		// render objects 
		m_ObjectShader->Bind();
		for (int i = 0; i < m_ObjectCount; i++)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformMatFloat4("u_Model", m_Transformations[i]);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_ObjectColor", m_ObjectColors[i]);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_LightSourceColor", m_LightSourceColorA);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_LightSourcePosition", m_LightPosition);
			Exalted::Renderer::Submit(m_ObjectMesh);
		}
		m_ObjectShader->Unbind();


		// render light sources 
		m_LightSourceShaderA->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShaderA)->SetUniformMatFloat4("u_Model", m_LightSourceTransformationA);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShaderA)->SetUniformFloat3("u_SourceColor", m_LightSourceColorA);
		Exalted::Renderer::Submit(m_LightSourceMesh);
		m_LightSourceShaderA->Unbind();

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
		ImGui::InputFloat3("Light Source Position", glm::value_ptr(m_LightPosition));
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