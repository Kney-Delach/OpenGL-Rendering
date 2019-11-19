/***************************************************************************
 * Filename		: BumpMapping.h
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: This layer contains a scene which displays bump mapping techniques
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "BumpMapping.h"

#define LIGHTING_SHADER_VERTEX		"Resources/Shaders/BumpMapping/LightingVertex.glsl"
#define LIGHTING_SHADER_FRAGMENT	"Resources/Shaders/BumpMapping/LightingFragment.glsl"

#define LIGHT_ORIGIN_VERTEX "Resources/Shaders/Lighting/LightOriginVertex.glsl"
#define LIGHT_ORIGIN_FRAGMENT "Resources/Shaders/Lighting/LightOriginFragment.glsl"

#define LIGHT_SOURCE_MESH "Resources/Meshes/cube.obj"

#define TEXTURE_DIFFUSE_CUBE	"Resources/Textures/HD/ForestGround/GroundForest003_COL_VAR1_6K.jpg" //Lights/CubeDiffuse.png";
#define TEXTURE_SPECULAR_CUBE	"Resources/Textures/HD/ForestGround/GroundForest003_GLOSS_6K.jpg" //Lights/CubeSpecular.png";
#define TEXTURE_EMISSION_CUBE	"Resources/Textures/Lights/CubeEmission.png";

#define SURFACE_MESH				"Resources/Meshes/Surfaces/Rugs/Rug.obj"
#define TEXTURE_DIFFUSE_OTHER	"Resources/Textures/wood.png";
#define TEXTURE_SPECULAR_OTHER	"Resources/Textures/wood.png";
#define TEXTURE_EMISSION_OTHER	"Resources/Textures/Lights/CubeEmission.png";

namespace Sandbox
{
	BumpMappingLayer::BumpMappingLayer()
		: Layer("Bump Mapping Layer", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			10000.f);
		m_EditorCamera->SetMouseSpeed(10.f);
	}

	void BumpMappingLayer::OnAttach()
	{
		EX_INFO("Bump Mapping layer attached successfully. {0}", TIME);

		// ---------------- Setup Light A Data ------------------- //
		m_LightSourceShader = Exalted::Shader::Create(LIGHT_ORIGIN_VERTEX, LIGHT_ORIGIN_FRAGMENT);
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		// --------- Point Light ------------ //
		m_PointLightA = Exalted::PointLight::Create(); //todo: change to a spot light
		m_PointLightA->Ambient = glm::vec3(0.2);
		m_PointLightA->Diffuse = glm::vec3(0.5);
		m_PointLightA->Specular = glm::vec3(1.0);
		m_PointLightA->SetAttenuationDistance(200);
		m_PointLightTransform = Exalted::GameTransform::Create();
		m_PointLightTransform->Position = glm::vec3(2.5f, 12.5f, 7.f);

		// --------- Directional Light ------------ //
		m_DirectionalLightA = Exalted::DirectionalLight::Create(); //todo: change to a spot light
		m_DirectionalLightA->Ambient = glm::vec3(0.2);
		m_DirectionalLightA->Diffuse = glm::vec3(0.5);
		m_DirectionalLightA->Specular = glm::vec3(1.0);
		m_DirectionalLightA->Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

		// --------- Spot Light ------------ //
		m_SpotLightA = Exalted::SpotLight::Create();
		m_SpotLightA->Ambient = glm::vec3(0);
		m_SpotLightA->Diffuse = glm::vec3(1.0);
		m_SpotLightA->Specular = glm::vec3(1.0);
		m_SpotLightA->CutoffInner = glm::cos(glm::radians(12.5f));
		m_SpotLightA->CutoffOuter = glm::cos(glm::radians(15.f));
		m_SpotLightA->SetAttenuationDistance(200);
		m_SpotLightA->Position = m_EditorCamera->GetPosition();
		m_SpotLightA->Direction = m_EditorCamera->GetFront();
		m_SpotLightTransform = Exalted::GameTransform::Create();
		m_SpotLightTransform->Position = m_EditorCamera->GetPosition();

		// ---------------- Setup Object A (effected by light) ------------------- //
		m_ObjectShader = Exalted::Shader::Create(LIGHTING_SHADER_VERTEX, LIGHTING_SHADER_FRAGMENT);
		m_ObjectMesh = Exalted::Mesh::Create();
		m_ObjectMesh->SetVertexArray(Exalted::ObjLoader::Load(LIGHT_SOURCE_MESH, true));

		// --------------------- Setup Scene objects --------------------- //
		glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.31f);
		const float shininess = 32.f;
		std::string diffusePath = TEXTURE_DIFFUSE_CUBE;
		std::string specularPath = TEXTURE_SPECULAR_CUBE;
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
			m_ObjectTransformations.emplace_back(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(3 * row++, 20.f + 2 * column, 0)), glm::vec3(10.f)));
		}

		// ---------------- Floor Setup ----------------
		std::string floorDiffusePath = TEXTURE_DIFFUSE_OTHER;
		std::string floorSpecularPath = TEXTURE_SPECULAR_CUBE;
		std::string floorEmissionPath = TEXTURE_EMISSION_OTHER;
		m_FloorMesh = Exalted::Mesh::Create();
		m_FloorMesh->SetVertexArray(Exalted::ObjLoader::Load(SURFACE_MESH));
		m_FloorMaterial = Exalted::Material::Create(color, color, color, shininess, floorDiffusePath, floorSpecularPath, floorEmissionPath);
		m_FloorTransformation = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)), glm::vec3(2.f,1.f,2.f));

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
		const Exalted::Bytes noDirectionalLights = 1;
		const Exalted::Bytes noSpotLights = 1;

		const Exalted::Bytes lightsBBI = 1;
		const Exalted::Bytes lightsOffset = 0;
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBlockIndex("Light_Uniforms", lightsBBI);
		Exalted::Bytes lightsBufferSize = noPointLights * Exalted::PointLight::UBSize() + noDirectionalLights * Exalted::DirectionalLight::UBSize() + noSpotLights * Exalted::SpotLight::UBSize();
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);



		//todo: move to material
		// normal map
		m_NormalMap = Exalted::Texture2D::Create("Resources/Textures/HD/ForestGround/GroundForest003_NRM_6K.jpg",
			Exalted::TextureFormat::RGB,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			true,
			0);
	}

	void BumpMappingLayer::OnDetach()
	{
		EX_INFO("Bump Mapping Layer detached successfully.");
	}

	void BumpMappingLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		// ---------------------------- Point Light Data Update ---------------------------- //
		if (m_RotateLight)
		{
			float lightX = 5.f + 10.f * sin(TIME);
			float lightY = m_PointLightA->Position.y;
			float lightZ = 10.f * cos(TIME);
			m_PointLightA->Position = glm::vec3(lightX, lightY, lightZ);
		}
		m_PointLightTransform->Position = m_PointLightA->Position;
		if (m_ChangeLightColor)
		{
			glm::vec3 lightColor;
			lightColor.x = sin(TIME * 2.0f);
			lightColor.y = sin(TIME * 0.7f);
			lightColor.z = sin(TIME * 1.3f);

			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
			glm::vec3 specularColor = lightColor;

			m_PointLightA->Ambient = ambientColor;
			m_PointLightA->Diffuse = diffuseColor;
			m_PointLightA->Specular = specularColor;
		}
		// ---------------------- Spot Light Data Update ------------------------- //
		if (m_FlashlightMode)
		{
			m_SpotLightA->Position = m_EditorCamera->GetPosition();
			m_SpotLightA->Direction = m_EditorCamera->GetFront();
			m_SpotLightTransform->Position = m_SpotLightA->Position;
		}

		// ------------------------------------------------------------------------------- //

		Exalted::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::Renderer::BeginScene(*m_EditorCamera);

		// ----------------------------- Set camera matrices data ----------------------------- // todo: Abstract this into the camera object (Camera->UpdateUniformBuffer)
		m_MatUniformBuffer->Bind();
		Exalted::Bytes offset = 0;
		Exalted::Bytes size = sizeof(glm::mat4);
		Exalted::Bytes sizeofVec4 = sizeof(glm::vec4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewMatrix()));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(glm::mat4(glm::mat3(m_EditorCamera->GetViewMatrix()))));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetProjectionMatrix()));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(m_EditorCamera->GetViewProjectionMatrix()));
		offset += sizeof(glm::mat4);
		m_MatUniformBuffer->SetBufferSubData(offset, sizeofVec4, glm::value_ptr(m_EditorCamera->GetPosition()));
		offset += sizeof(glm::vec4);
		m_MatUniformBuffer->Unbind();

		// --------------------------- Set Light uniform data ------------------------------- // Iterate over this for each light source THAT CAN CHANGE during runtime
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		m_PointLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_DirectionalLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_SpotLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_LightUniformBuffer->Unbind();

		// ----------------------------- Render Scene ----------------------------- //
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//m_SceneManager->RenderScene();

		// render objects 
		m_ObjectShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBool1("u_ActivateEmission", m_ActivateEmission);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBool1("u_EmissionTransform", m_EmissionTransform);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformBool1("u_BlinnPhong", m_BlinnPhong);
		if (m_EmissionTransform)
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat1("u_Time", TIME);

		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureDiffuse", 0);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureSpecular", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_Material.TextureEmission", 2);

		//----------------normap mapping texture binding
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformInt1("u_NormalMap", 4);
		m_NormalMap->Bind(4);
		//-----------------
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
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectShader)->SetUniformFloat1("u_Material.Shininess", m_FloorMaterial->Shininess);
		Exalted::Renderer::Submit(m_FloorMesh);
		m_FloorMaterial->DiffuseTexture->Unbind(); //todo: make unbind a static function?
		m_ObjectShader->Unbind();

		// ---------------------------------- render light sources --------------------------------- //
		m_LightSourceShader->Bind();

		// ---------------- Render Point Light ----------------------- //
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_PointLightTransform->SetAndGetWorldTransform());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_PointLightA->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);

		// ---------------- Render Directional Light ----------------------- //
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", glm::mat4(1.0f)); // draw it at the origin
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_DirectionalLightA->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);

		// ---------------- Render Spot Light ----------------------- //
		if (!m_FlashlightMode)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_SpotLightTransform->SetAndGetWorldTransform()); // draw it at the origin
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_SpotLightA->Diffuse);
			Exalted::Renderer::Submit(m_LightSourceMesh);
		}

		m_LightSourceShader->Unbind();

		// --------------------------- Render Skybox --------------------------- //
		m_SceneManager->RenderSkybox();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
	}

	void BumpMappingLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI(); //todo: Change this to scene manager 

		ImGui::Begin("Bump Mapping Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Directional Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Direction", glm::value_ptr(m_DirectionalLightA->Direction));
		ImGui::Text("----------------------------");
		ImGui::Text("Point Light Settings");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Toggle Light Rotation"))
		{
			m_RotateLight = !m_RotateLight;
		}
		if (ImGui::Button("Toggle Light Color"))
		{
			m_ChangeLightColor = !m_ChangeLightColor;
		}
		ImGui::InputFloat3("Position", glm::value_ptr(m_PointLightA->Position));
		ImGui::InputFloat3("Ambient", glm::value_ptr(m_PointLightA->Ambient));
		ImGui::InputFloat3("Diffuse", glm::value_ptr(m_PointLightA->Diffuse));
		ImGui::InputFloat3("Specular", glm::value_ptr(m_PointLightA->Specular));
		ImGui::Text("----------------------------");
		ImGui::Text("Spot Light Settings");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Toggle Emission Display"))
		{
			m_ActivateEmission = !m_ActivateEmission;
		}
		if (ImGui::Button("Toggle Emission Translation"))
		{
			m_EmissionTransform = !m_EmissionTransform;
		}
		if (ImGui::Button("Toggle Flashlight Mode"))
		{
			m_FlashlightMode = !m_FlashlightMode;
		}
		ImGui::Text("----------------------------");
		ImGui::Text("Blinn-Phong Settings");
		ImGui::Text("----------------------------");
		if (ImGui::Button("Blinn-Phong Specular ON"))
		{
			m_BlinnPhong = true;
		}
		if (ImGui::Button("Blinn-Phong Specular OFF"))
		{
			m_BlinnPhong = false;
		}
		ImGui::End();
	}

	void BumpMappingLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [20] -> Bump Mapping"))
			m_IsActive = true;
		ImGui::End();
	}

	void BumpMappingLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}