/***************************************************************************
 * Filename		: MultipleMultipleShadowsLayer.cpp
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: This layer contains a scene using omnidirectional Shadow mapping techniques.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "MultipleShadowsLayer.h"

#define LIGHTING_SHADER_VERTEX		"Resources/Shaders/MultipleShadows/LightingVertex.glsl"
#define LIGHTING_SHADER_FRAGMENT	"Resources/Shaders/MultipleShadows/LightingFragment.glsl"

#define DEPTH_RENDER_VERTEX "Resources/Shaders/MultipleShadows/ShadowDepthVertex.glsl"
#define DEPTH_RENDER_FRAGMENT "Resources/Shaders/MultipleShadows/ShadowDepthFragment.glsl"
#define DEPTH_RENDER_GEOMETRY "Resources/Shaders/MultipleShadows/ShadowDepthGeometry.glsl"

#define LIGHT_ORIGIN_VERTEX "Resources/Shaders/Lighting/LightOriginVertex.glsl"
#define LIGHT_ORIGIN_FRAGMENT "Resources/Shaders/Lighting/LightOriginFragment.glsl"

#define DEPTH_RENDER_QUAD_VERTEX "Resources/Shaders/MultipleShadows/ShadowDepthQUADVertex.glsl" //todo: render debug quad into this
#define DEPTH_RENDER_QUAD_FRAGMENT "Resources/Shaders/MultipleShadows/ShadowDepthQUADFragment.glsl" //todo: render debug quad into this

#define LIGHT_SOURCE_MESH "Resources/Meshes/cube.obj"

#define TEXTURE_DIFFUSE_CUBE	"Resources/Textures/Lights/CubeDiffuse.png";
#define TEXTURE_SPECULAR_CUBE	"Resources/Textures/Lights/CubeSpecular.png";
#define TEXTURE_EMISSION_CUBE	"Resources/Textures/Lights/CubeEmission.png";

#define SURFACE_MESH				"Resources/Meshes/Surfaces/Rugs/Rug.obj"
#define TEXTURE_DIFFUSE_SURFACE		"Resources/Meshes/Surfaces/Rugs/Textures/1.jpg";
#define TEXTURE_SPECULAR_SURFACE	"Resources/Meshes/Surfaces/Rugs/Textures/2.jpg";
#define TEXTURE_EMISSION_SURFACE	"";

namespace Sandbox
{
	MultipleShadowsLayer::MultipleShadowsLayer()
		: Layer("Multiple Shadow Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000.f);
		m_EditorCamera->SetMouseSpeed(100.f);
	}

	void MultipleShadowsLayer::OnAttach()
	{
		EX_INFO("Multiple Shadow layer attached successfully. {0}", TIME);

		// ---------------- Setup Light A Data ------------------- //
		m_LightSourceShader = Exalted::Shader::Create(LIGHT_ORIGIN_VERTEX, LIGHT_ORIGIN_FRAGMENT);
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		// --------- Point Light ------------ //
		m_PointLightA = Exalted::PointLight::Create();
		m_PointLightA->Ambient = glm::vec3(0.2);
		m_PointLightA->Diffuse = glm::vec3(0.5);
		m_PointLightA->Specular = glm::vec3(1.0);
		m_PointLightA->SetAttenuationDistance(200);
		m_PointLightTransform = Exalted::GameTransform::Create();
		m_PointLightTransform->Position = glm::vec3(2.5f, 4.5f, 7.f);

		// ---------------- Setup Object A (effected by light) ------------------- //
		m_ObjectShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);
		m_ObjectMesh = Exalted::Mesh::Create();
		m_ObjectMesh->SetVertexArray(Exalted::ObjLoader::Load(LIGHT_SOURCE_MESH));

		// --------------------- Setup Scene objects --------------------- //
		glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
		const float shininess = 32.f;
		std::string diffusePath = TEXTURE_DIFFUSE_SURFACE;
		std::string specularPath = TEXTURE_SPECULAR_SURFACE;
		std::string emissionPath = TEXTURE_EMISSION_CUBE;

		//unsigned row = 0;
		//int column = 0;
		//for (int i = 0; i < m_ObjectCount; ++i)
		//{
		//	if (i % 5 == 0)
		//	{
		//		column += 2;
		//		row = 0;
		//	}
		//	m_ObjectMaterials.emplace_back(Exalted::Material::Create(color, color, color, shininess, diffusePath, specularPath, emissionPath));
		//	m_ObjectTransformations.emplace_back(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(row++, column, 0)), glm::vec3(2.f + column * 2.f)));
		//}
		//
		
		// object 1 
		m_ObjectMaterials.emplace_back(Exalted::Material::Create(color, color, color, shininess, diffusePath, specularPath, emissionPath));
		m_ObjectTransformations.emplace_back(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(-18, 3, 0)), glm::vec3(4.f)));

		// object 2
		m_ObjectMaterials.emplace_back(Exalted::Material::Create(color, color, color, shininess, diffusePath, specularPath, emissionPath));
		m_ObjectTransformations.emplace_back(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(14, 3, 0)), glm::vec3(4.f)));

		// ---------------- Floor Setup ----------------
		std::string floorDiffusePath = TEXTURE_DIFFUSE_SURFACE;
		std::string floorSpecularPath = TEXTURE_SPECULAR_SURFACE;
		std::string floorEmissionPath = TEXTURE_EMISSION_SURFACE;
		m_FloorMesh = Exalted::Mesh::Create();
		m_FloorMesh->SetVertexArray(Exalted::ObjLoader::Load(SURFACE_MESH));
		m_FloorMaterial = Exalted::Material::Create(color, color, color, shininess, floorDiffusePath, floorSpecularPath, floorEmissionPath);
		m_FloorTransformation = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), glm::vec3(2.f, 1.f, 2.f));

		// -------------- Scene manager/root ------------------ //
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true);

		// ----------------------------- Configure Uniform Buffer Objects ----------------------------- //
		// camera UBO data
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Camera_Matrices", 2);
		Exalted::Bytes bufferSize = 4 * sizeof(glm::mat4) + sizeof(glm::vec4);;
		m_MatUniformBuffer = Exalted::UniformBuffer::Create(bufferSize);
		const Exalted::Bytes bbi = 2;
		const Exalted::Bytes offset = 0;
		m_MatUniformBuffer->BindBufferRange(bbi, offset, bufferSize);

		// lighting UBO setup 
		const Exalted::Bytes noPointLights = 1;
		const Exalted::Bytes noDirectionalLights = 0;
		const Exalted::Bytes noSpotLights = 0;

		const Exalted::Bytes lightsBBI = 1;
		const Exalted::Bytes lightsOffset = 0;
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Light_Uniforms", lightsBBI);
		Exalted::Bytes lightsBufferSize = noPointLights * Exalted::PointLight::UBSize() + noDirectionalLights * Exalted::DirectionalLight::UBSize() + noSpotLights * Exalted::SpotLight::UBSize();
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);

		///////////////////////////////////////////////
		//// Shadow Data Setup ////////////////////////
		///////////////////////////////////////////////
		m_DepthFrameBuffer = Exalted::FrameBuffer::Create(1024, 1024);
		m_ObjectDepthShader = Exalted::Shader::Create(DEPTH_RENDER_VERTEX, DEPTH_RENDER_FRAGMENT, DEPTH_RENDER_GEOMETRY);
		m_QuadDepthShader = Exalted::Shader::Create(DEPTH_RENDER_QUAD_VERTEX, DEPTH_RENDER_QUAD_FRAGMENT);
		m_QuadMesh = Exalted::Mesh::Create();
		m_QuadMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateIndexedQuad());
	}

	void MultipleShadowsLayer::OnDetach()
	{
		EX_INFO("Multiple Shadow Layer detached successfully.");
	}

	void MultipleShadowsLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_EditorCamera->UpdateUBO(m_MatUniformBuffer);
		m_SceneManager->UpdateScene(deltaTime);

		// --------------------------- light transformations ------------------------------------- //	
		if (m_RotateLight)
		{
			float lightX = 5.f + 10.f * sin(TIME);
			float lightY = m_PointLightA->Position.y;
			float lightZ = 10.f * cos(TIME);
			m_PointLightA->Position = glm::vec3(lightX, lightY, lightZ);
		}
		m_PointLightTransform->Position = m_PointLightA->Position;

		// --------------------------- Set Light uniform data ------------------------------- // 
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		m_PointLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_LightUniformBuffer->Unbind();

		// ------------------------------------------------------------------------------- //	
		Exalted::Renderer::BeginScene(*m_EditorCamera); //todo: remove this

		///////////////////////////////////////////////////////////////////////////////////////
		//// Prepare lighting matrix data for shadow depth rendering (do this for every light) 
		///////////////////////////////////////////////////////////////////////////////////////
		 // 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		float near_plane = 1.0f;
		float far_plane = 50.f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(m_PointLightA->Position, m_PointLightA->Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		/////////////////////////////////////////////////////////////////
		//// Initial render to depth map for shadow mapping ///////////// 
		/////////////////////////////////////////////////////////////////
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::FRONT);

		m_DepthFrameBuffer->Bind();
		m_ObjectDepthShader->Bind();
		Exalted::RenderCommand::ClearDepthBuffer();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		
		// set face transformation uniforms
		for (unsigned int i = 0; i < 6; ++i)
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformFloat1("u_FarPlane", far_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformFloat3("lightPos", m_PointLightA->Position);

		// render scene to depth buffer
		for (int i = 0; i < m_ObjectCount; i++)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("u_Model", m_ObjectTransformations[i]);
			Exalted::Renderer::Submit(m_ObjectMesh);
		}

		// draw floor
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("u_Model", m_FloorTransformation);
		Exalted::Renderer::Submit(m_FloorMesh);

		//Exalted::OpenGLConfigurations::DisableDepthTesting();
		m_DepthFrameBuffer->UnbindMiniFrame();
		m_ObjectDepthShader->Unbind();

		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::OpenGLConfigurations::DisableFaceCulling();
		
		/////////////////////////////////////////////////////////////////////////////
		//// Render scene output //////////////////////////////////////////////////// 
		/////////////////////////////////////////////////////////////////////////////
		Exalted::RenderCommand::ClearColorDepthBuffers();

		m_ObjectShader->Bind();
		// vertex shader bindings
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBool1("shadows", m_DisplayShadows);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat1("u_FarPlane", far_plane);

		//fragment shader bindings
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureDiffuse", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureSpecular", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureEmission", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_ShadowMap", 3);
		m_DepthFrameBuffer->BindTextureCube(3);
		for (int i = 0; i < m_ObjectCount; i++) //todo: Group by material / shader ID for instancing
		{
			m_ObjectMaterials[i]->DiffuseTexture->Bind(0);
			m_ObjectMaterials[i]->SpecularTexture->Bind(1);
			m_ObjectMaterials[i]->EmissionTexture->Bind(2);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformMatFloat4("u_Model", m_ObjectTransformations[i]);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat1("u_Material.Shininess", m_ObjectMaterials[i]->Shininess);
			Exalted::Renderer::Submit(m_ObjectMesh);
		}
		// draw floor

		m_FloorMaterial->DiffuseTexture->Bind(0);
		m_FloorMaterial->SpecularTexture->Bind(1);
		m_FloorMaterial->EmissionTexture->Bind(2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformMatFloat4("u_Model", m_FloorTransformation);
		Exalted::Renderer::Submit(m_FloorMesh);
		m_DepthFrameBuffer->Unbind();
		m_ObjectShader->Unbind();

		// draw the sun 
		m_LightSourceShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_PointLightTransform->SetAndGetWorldTransform());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_PointLightA->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);
		m_LightSourceShader->Unbind();
		// --------------------------- Render Skybox --------------------------- //
		m_SceneManager->RenderSkybox();

		/////////////////////////////////////////////////////////////////////////////////////
		//// Render scene as normal with shadow mapping using depth map (DEBUG) ///////////// 
		/////////////////////////////////////////////////////////////////////////////////////

		//Exalted::OpenGLConfigurations::SetViewport(0, 0, 1024, 512);
		////Exalted::RenderCommand::ClearColorDepthBuffers();

		// //prepare to render to draw framebuffer

		//m_QuadDepthShader->Bind();
		//m_DepthFrameBuffer->BindTexture(0);
		//std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("near_plane", near_plane);
		//std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("far_plane", far_plane);
		//std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformInt1("depthMap", 0);
		//Exalted::Renderer::Submit(m_QuadMesh);
		//m_QuadDepthShader->Unbind();
		//Exalted::OpenGLConfigurations::SetViewport(0, 0, Exalted::Application::Get().GetWindow().GetWindowWidth(), Exalted::Application::Get().GetWindow().GetWindowHeight());

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void MultipleShadowsLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI(); //todo: Change this to scene manager 

		ImGui::Begin("Multiple Shadow Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Point Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Position", glm::value_ptr(m_PointLightA->Position));
		if(ImGui::Button("Toggle Shadow Display"))
		{
			m_DisplayShadows = !m_DisplayShadows;
		}
		if (ImGui::Button("Toggle Light Rotation Display"))
		{
			m_RotateLight = !m_RotateLight;
		}
		ImGui::End();
	}

	void MultipleShadowsLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [19] -> Multiple Shadow Mapping"))
			m_IsActive = true;
		ImGui::End();
	}

	void MultipleShadowsLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}
