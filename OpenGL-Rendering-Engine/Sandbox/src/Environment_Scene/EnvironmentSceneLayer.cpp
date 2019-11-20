#include "EnvironmentSceneLayer.h"
#include "EnvironmentSceneResources.h"

namespace Sandbox
{
	EnvironmentSceneLayer::EnvironmentSceneLayer()
		: Layer("Example Environment Scene", true)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			1000.f, 2); // sets ubo of camera to index 2
		m_EditorCamera->SetMouseSpeed(100.f);
	}
	
	void EnvironmentSceneLayer::OnAttach()
	{
		EX_INFO("Environment Scene Layer attached successfully.");

		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true, false);

		// point light A
		m_PointLights.emplace_back(Exalted::PointLight::Create());
		m_PointLights[0]->Ambient = glm::vec3(0.2);
		m_PointLights[0]->Diffuse = glm::vec3(0.5);
		m_PointLights[0]->Specular = glm::vec3(1.0);
		m_PointLights[0]->Position = glm::vec3(0.1,0.2,0.3);
		m_PointLights[0]->SetAttenuationDistance(200);
		m_PointLightTransforms.emplace_back(Exalted::GameTransform::Create());
		m_PointLightTransforms[0]->Position = m_PointLights[0]->Position;
		
		// point light B
		m_PointLights.emplace_back(Exalted::PointLight::Create());
		m_PointLights[1]->Ambient = glm::vec3(0.2);
		m_PointLights[1]->Diffuse = glm::vec3(0.5);
		m_PointLights[1]->Specular = glm::vec3(1.0);
		m_PointLights[1]->Position = glm::vec3(0.1, 0, 0.3);
		m_PointLights[1]->SetAttenuationDistance(200);
		m_PointLightTransforms.emplace_back(Exalted::GameTransform::Create());
		m_PointLightTransforms[0]->Position = m_PointLights[1]->Position;
		
		// 3. Create Meshes
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		Exalted::Ref<Exalted::Mesh> ObjectA = Exalted::Mesh::Create();
		ObjectA->SetVertexArray(Exalted::ObjLoader::Load(CUBE_MESH, true));
		
		// 4. Create Shaders
		Exalted::Ref<Exalted::Shader> basicLightingShader = Exalted::Shader::Create(BASIC_LIGHTING_SHADER_VERTEX, BASIC_LIGHTING_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(basicLightingShader)->SetUniformBlockIndex("Light_Uniforms", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(basicLightingShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		m_LightSourceShader = Exalted::Shader::Create(LIGHT_SOURCE_SHADER_VERTEX, LIGHT_SOURCE_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		// 5. Create Materials
		Exalted::Ref<Exalted::Material> brickMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_BRICKS, TEXTURE_SPECULAR_BRICKS, "", TEXTURE_NORMAL_BRICKS, 32.f);
		
		// 4. Create Gameobjects
		
		// 4.B Cube A
		Exalted::GameObject* CubeA = new Exalted::GameObject("Cube A");
		CubeA->GetTransform()->Position = glm::vec3(0, 4.f, 0);
		CubeA->SetMesh(ObjectA);
		CubeA->SetShader(basicLightingShader);
		CubeA->SetMaterial(brickMaterial);
		CubeA->SetTransparency(false);
		CubeA->SetBoundingRadius(200.f);
		
		// 4.C Cube B
		Exalted::GameObject* CubeB = new Exalted::GameObject("Cube B");
		CubeB->GetTransform()->Position = glm::vec3(12, 12.f, 0);
		CubeB->GetTransform()->Scale = glm::vec3(4, 4.f, 4);
		CubeB->SetMesh(ObjectA);
		CubeB->SetShader(basicLightingShader);
		CubeB->SetMaterial(brickMaterial);
		CubeB->SetTransparency(false);
		CubeB->SetBoundingRadius(200.f);

		// 5. Add all gameobjects to scene manager
		m_SceneManager->GetSceneRoot()->AddChildObject(CubeA);
		m_SceneManager->GetSceneRoot()->AddChildObject(CubeB);

		// lighting UBO setup 
		const Exalted::Bytes noPointLights = 2;
		const Exalted::Bytes noDirectionalLights = 0;
		const Exalted::Bytes noSpotLights = 0;

		const Exalted::Bytes lightsBBI = 1;
		const Exalted::Bytes lightsOffset = 0;
		Exalted::Bytes lightsBufferSize = noPointLights * Exalted::PointLight::UBSize() + noDirectionalLights * Exalted::DirectionalLight::UBSize() + noSpotLights * Exalted::SpotLight::UBSize();
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);		
	}
	
	void EnvironmentSceneLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		// 1. Update the camera transformation and buffer objects, either dependent on lerping motion or manual movement
		m_EditorCamera->UpdateCamera(deltaTime);

		float lightX = 5.f + 10.f * sin(TIME);
		float lightY = m_PointLights[1]->Position.y;
		float lightZ = 10.f * cos(TIME);
		m_PointLights[1]->Position = glm::vec3(lightX, lightY, lightZ); // see if this updates the object's location through the gameobjects
	
		// 2. Update transformations of all objects and (3) non-static light uniform buffers
		m_SceneManager->UpdateScene(deltaTime);

		// --------------------------- Set Light uniform data ------------------------------- // Iterate over this for each light source THAT CAN CHANGE during runtime
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		for (int i = 0; i < m_PointLights.size(); ++i)
		{
			m_PointLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			m_PointLightTransforms[i]->Position = m_PointLights[i]->Position; 
		}
		//m_PointLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		//m_DirectionalLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		//m_SpotLightA->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_LightUniformBuffer->Unbind();
		
		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);
		// 4. Perform a render pass for each light in order to see shadows

				// render scene
		m_SceneManager->RenderScene();
		m_SceneManager->DrawOpaqueObjects();

		// draw the sun 
		m_LightSourceShader->Bind();
		for (int i = 0; i < m_PointLights.size(); ++i) //todo: instance this
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_PointLightTransforms[i]->SetAndGetWorldTransform());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_PointLights[i]->Diffuse);
			Exalted::Renderer::Submit(m_LightSourceMesh);
		}
		m_LightSourceShader->Unbind();

		m_SceneManager->DrawTransparentObjects();
		//m_SceneManager->RenderSkybox();
		m_SceneManager->ClearObjectLists();
		
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();
	}

	void EnvironmentSceneLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI();
		ImGui::Begin("Environment Scene Settings");
		
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void EnvironmentSceneLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene] -> Environment Showcase"))
			m_IsActive = true;
		ImGui::End();
	}

	void EnvironmentSceneLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}

	void EnvironmentSceneLayer::OnDetach()
	{
		EX_INFO("Environment Scene Layer detatched successfully.");
	}
}
