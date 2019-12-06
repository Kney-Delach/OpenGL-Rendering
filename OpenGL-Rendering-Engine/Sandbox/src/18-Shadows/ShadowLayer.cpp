/***************************************************************************
 * Filename		: ShadowLayer.cpp
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: This layer contains a scene which displays different Shadow techniques.
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "ShadowLayer.h"

#define LIGHTING_SHADER_VERTEX		"Resources/Shaders/Shadows/LightingVertex.glsl"
#define LIGHTING_SHADER_FRAGMENT	"Resources/Shaders/Shadows/LightingFragment.glsl"

#define LIGHT_ORIGIN_VERTEX "Resources/Shaders/Lighting/LightOriginVertex.glsl"
#define LIGHT_ORIGIN_FRAGMENT "Resources/Shaders/Lighting/LightOriginFragment.glsl"

#define DEPTH_RENDER_VERTEX "Resources/Shaders/Shadows/ShadowDepthVertex.glsl" //todo: render models into this
#define DEPTH_RENDER_FRAGMENT "Resources/Shaders/Shadows/ShadowDepthFragment.glsl" //todo: render models into this

#define DEPTH_RENDER_QUAD_VERTEX "Resources/Shaders/Shadows/ShadowDepthQUADVertex.glsl" //todo: render debug quad into this
#define DEPTH_RENDER_QUAD_FRAGMENT "Resources/Shaders/Shadows/ShadowDepthQUADFragment.glsl" //todo: render debug quad into this

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
	ShadowLayer::ShadowLayer()
		: Layer("Shadow Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000.f);
		m_EditorCamera->SetMouseSpeed(100.f);
	}

	void ShadowLayer::OnAttach()
	{
		EX_INFO("Shadow layer attached successfully. {0}", TIME);

		// ---------------- Setup Light A Data ------------------- //
		m_LightSourceShader = Exalted::Shader::Create(LIGHT_ORIGIN_VERTEX, LIGHT_ORIGIN_FRAGMENT);
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		// --------- Point Light ------------ //
		//m_PointLightA = Exalted::PointLight::Create(); //todo: change to a spot light
		//m_PointLightA->Ambient = glm::vec3(0.2);
		//m_PointLightA->Diffuse = glm::vec3(0.5);
		//m_PointLightA->Specular = glm::vec3(1.0);
		//m_PointLightA->SetAttenuationDistance(200);
		//m_PointLightTransform = Exalted::GameTransform::Create();
		//m_PointLightTransform->Position = glm::vec3(2.5f, 4.5f, 7.f);

		// --------- Directional Light ------------ //
		m_DirectionalLightA = Exalted::DirectionalLight::Create(); //todo: change to a spot light
		m_DirectionalLightA->Ambient = glm::vec3(0.2);
		m_DirectionalLightA->Diffuse = glm::vec3(0.5);
		m_DirectionalLightA->Specular = glm::vec3(1.f);
		m_DirectionalLightA->Direction = glm::vec3(0.1f, -30.f, -550.f);
		m_DirectionalLightTransform = Exalted::GameTransform::Create();
		m_DirectionalLightTransform->Position = m_DirectionalLightA->Direction;

		// --------- Spot Light ------------ //
		//m_SpotLightA = Exalted::SpotLight::Create();
		//m_SpotLightA->Ambient = glm::vec3(0);
		//m_SpotLightA->Diffuse = glm::vec3(1.0);
		//m_SpotLightA->Specular = glm::vec3(1.0);
		//m_SpotLightA->CutoffInner = glm::cos(glm::radians(12.5f));
		//m_SpotLightA->CutoffOuter = glm::cos(glm::radians(15.f));
		//m_SpotLightA->SetAttenuationDistance(200);
		//m_SpotLightA->Position = m_EditorCamera->GetPosition();
		//m_SpotLightA->Direction = m_EditorCamera->GetFront();
		//m_SpotLightTransform = Exalted::GameTransform::Create();
		//m_SpotLightTransform->Position = m_EditorCamera->GetPosition();

		// ---------------- Setup Object A (effected by light) ------------------- //
		m_ObjectShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);
		m_ObjectMesh = Exalted::Mesh::Create();
		m_ObjectMesh->SetVertexArray(Exalted::ObjLoader::Load(SUZANNE));

		// --------------------- Setup Scene objects --------------------- //
		glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
		const float shininess = 32.f;
		std::string diffusePath = TEXTURE_DIFFUSE_SURFACE;//TEXTURE_DIFFUSE_CUBE;
		std::string specularPath = TEXTURE_SPECULAR_SURFACE;//TEXTURE_SPECULAR_CUBE;
		std::string emissionPath = TEXTURE_EMISSION_CUBE;

		unsigned row = 0;
		int column = 0;
		for (int i = 0; i < m_ObjectCount; ++i)
		{
			if (i % 5 == 0)
			{
				column += 2;
				row = 0;
			}
			m_ObjectMaterials.emplace_back(Exalted::Material::Create(color, color, color, shininess, diffusePath, specularPath, emissionPath));
			m_ObjectTransformations.emplace_back(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(2.f + i * i * row++ , 4 + 2 * i + i * column, 2.f + i * i * row++)), glm::vec3( 2.f + column * 2.f)));
		}

		// ---------------- Floor Setup ----------------
		std::string floorDiffusePath = TEXTURE_DIFFUSE_SURFACE;
		std::string floorSpecularPath = TEXTURE_SPECULAR_SURFACE;
		std::string floorEmissionPath = TEXTURE_EMISSION_SURFACE;
		m_FloorMesh = Exalted::Mesh::Create();
		m_FloorMesh->SetVertexArray(Exalted::ObjLoader::Load(SURFACE_MESH));
		m_FloorMaterial = Exalted::Material::Create(color, color, color, shininess, floorDiffusePath, floorSpecularPath, floorEmissionPath);
		m_FloorTransformation = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), glm::vec3(2.f, 0.1f, 2.f));

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
		const Exalted::Bytes noPointLights = 0;
		const Exalted::Bytes noDirectionalLights = 1;
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
		m_DepthFrameBuffer = Exalted::FrameBuffer::Create(4096, 4096, true);
		m_ObjectDepthShader = Exalted::Shader::Create(DEPTH_RENDER_VERTEX, DEPTH_RENDER_FRAGMENT); //todo: render models into this
		m_QuadDepthShader = Exalted::Shader::Create(DEPTH_RENDER_QUAD_VERTEX, DEPTH_RENDER_QUAD_FRAGMENT); //todo: render quad into this
		m_QuadMesh = Exalted::Mesh::Create();
		m_QuadMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateIndexedQuad());
	}

	void ShadowLayer::OnDetach()
	{
		EX_INFO("Shadow Layer detached successfully.");
	}

	void ShadowLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_EditorCamera->UpdateUBO(m_MatUniformBuffer);
		m_SceneManager->UpdateScene(deltaTime);

		if(m_IncreaseSunHeight)
		{
			if(m_DirectionalLightA->Direction.z < 0)
				m_DirectionalLightA->Direction = m_DirectionalLightA->Direction + glm::vec3(0, 150*deltaTime, 150 * deltaTime);
			else
				m_DirectionalLightA->Direction = m_DirectionalLightA->Direction + glm::vec3(0, -150 * deltaTime, 150 * deltaTime);

			if(m_DirectionalLightA->Direction.z > 550)
			{
				m_DirectionalLightA->Direction = glm::vec3(0.1f, -30.f, -550.f);

				m_IncreaseSunHeight = false;
			}
			m_DirectionalLightTransform->Position = m_DirectionalLightA->Direction;
		}
		// ------------------------------------------------------------------------------- //	
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		// --------------------------- Set Light uniform data ------------------------------- // 
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		//m_PointLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_DirectionalLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		//m_SpotLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_LightUniformBuffer->Unbind();

		///////////////////////////////////////////////////////////////////////////////////////
		//// Prepare lighting matrix data for shadow depth rendering (do this for every light) 
		///////////////////////////////////////////////////////////////////////////////////////
		//glm::ortho(-600.f, 600.f, -200.0f, 200.0f, near_plane, far_plane);//
		float near_plane = 0.1f;
		float far_plane = 1000.f;
		glm::mat4 lightProjection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 1000.f); // no perspective deform for directional light 
		glm::mat4 lightView = glm::lookAt(m_DirectionalLightA->Direction, glm::vec3(0,0,0), glm::vec3(0, 1, 0)); // glm::lookAt(m_DirectionalLightA->Direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // direction assigned as position, looking towards center.
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;


		
		/////////////////////////////////////////////////////////////////
		//// Initial render to depth map for shadow mapping ///////////// 
		/////////////////////////////////////////////////////////////////
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::FRONT);

		m_DepthFrameBuffer->Bind();
		m_ObjectDepthShader->Bind();
		Exalted::RenderCommand::ClearDepthBuffer();
		Exalted::OpenGLConfigurations::EnableDepthTesting(); //todo: remove?
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("lightSpaceMatrix", lightSpaceMatrix);
		for (int i = 0; i < m_ObjectCount; i++)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("u_Model", m_ObjectTransformations[i]);
			Exalted::Renderer::Submit(m_ObjectMesh);
		}

		// draw floor
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("u_Model", m_FloorTransformation);
		Exalted::Renderer::Submit(m_FloorMesh);
		
		//Exalted::OpenGLConfigurations::DisableDepthTesting(); //todo: remove?
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
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat3("u_LightPosition", m_DirectionalLightA->Direction); //todo: change this, temporarily using direction as position then going towards origin
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformMatFloat4("u_LightSpaceMatrix", lightSpaceMatrix);

		//fragment shader bindings
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureDiffuse", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureSpecular", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureEmission", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_ShadowMap", 3);
		m_DepthFrameBuffer->BindTexture(3);
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
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_DirectionalLightTransform->SetAndGetWorldTransform());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse",m_DirectionalLightA->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);
		m_LightSourceShader->Unbind();
		// --------------------------- Render Skybox --------------------------- //
		//m_SceneManager->RenderSkybox();

		/////////////////////////////////////////////////////////////////////////////////////
		//// Render scene as normal with shadow mapping using depth map (DEBUG) ///////////// 
		/////////////////////////////////////////////////////////////////////////////////////

		Exalted::OpenGLConfigurations::SetViewport(0, 0, 1024, 512);
		//Exalted::RenderCommand::ClearColorDepthBuffers();

		 //prepare to render to draw framebuffer

		m_QuadDepthShader->Bind();
		m_DepthFrameBuffer->BindTexture(0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("near_plane", near_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("far_plane", far_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformInt1("depthMap", 0);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_QuadDepthShader->Unbind();
		Exalted::OpenGLConfigurations::SetViewport(0, 0, Exalted::Application::Get().GetWindow().GetWindowWidth(), Exalted::Application::Get().GetWindow().GetWindowHeight());

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void ShadowLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI(); //todo: Change this to scene manager 

		ImGui::Begin("Shadow Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Directional Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Direction", glm::value_ptr(m_DirectionalLightA->Direction));
		//ImGui::Text("----------------------------");
		//ImGui::Text("Point Light Settings");
		//ImGui::Text("----------------------------");
		//if (ImGui::Button("Toggle Light Rotation"))
		//{
		//	m_RotateLight = !m_RotateLight;
		//}
		//if (ImGui::Button("Toggle Light Color"))
		//{
		//	m_ChangeLightColor = !m_ChangeLightColor;
		//}
		//ImGui::InputFloat3("Position", glm::value_ptr(m_PointLightA->Position));
		//ImGui::InputFloat3("Ambient", glm::value_ptr(m_PointLightA->Ambient));
		//ImGui::InputFloat3("Diffuse", glm::value_ptr(m_PointLightA->Diffuse));
		//ImGui::InputFloat3("Specular", glm::value_ptr(m_PointLightA->Specular));
		//ImGui::Text("----------------------------");
		//ImGui::Text("Spot Light Settings");
		//ImGui::Text("----------------------------");
		//if (ImGui::Button("Toggle Emission Display"))
		//{
		//	m_ActivateEmission = !m_ActivateEmission;
		//}
		//if (ImGui::Button("Toggle Emission Translation"))
		//{
		//	m_EmissionTransform = !m_EmissionTransform;
		//}
		//if (ImGui::Button("Toggle Flashlight Mode"))
		//{
		//	m_FlashlightMode = !m_FlashlightMode;
		//}
		//ImGui::Text("----------------------------");
		//ImGui::Text("Blinn-Phong Settings");
		//ImGui::Text("----------------------------");
		//if (ImGui::Button("Blinn-Phong Specular ON"))
		//{
		//	m_BlinnPhong = true;
		//}
		if (ImGui::Button("Toggle Sun Movement"))
		{
			m_IncreaseSunHeight = !m_IncreaseSunHeight;
		}
		ImGui::End();
	}

	void ShadowLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [18] -> Shadow Mapping"))
			m_IsActive = true;
		ImGui::End();
	}

	void ShadowLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}