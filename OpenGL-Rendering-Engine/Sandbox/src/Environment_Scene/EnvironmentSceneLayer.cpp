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
			10000.f, 2); // sets ubo of camera to index 2
		m_EditorCamera->SetMouseSpeed(350.f);
	}
	
	void EnvironmentSceneLayer::OnAttach()
	{
		EX_INFO("Environment Scene Layer attached successfully.");

		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true, false);

		////////////////////////////////////////////////////////////////////////
		//// Light Setup 
		////////////////////////////////////////////////////////////////////////
		// 1. point lights
		m_PointLights.emplace_back(Exalted::PointLight::Create());
		m_PointLights[0]->Ambient = glm::vec3(0.2);
		m_PointLights[0]->Diffuse = glm::vec3(0.5);
		m_PointLights[0]->Specular = glm::vec3(1.0);
		m_PointLights[0]->Position = glm::vec3(-30,370,400);
		m_PointLights[0]->SetAttenuationDistance(325);
		m_PointLightTransforms.emplace_back(Exalted::GameTransform::Create());
		m_PointLightTransforms[0]->Scale = glm::vec3(5.0);
		m_PointLightTransforms[0]->Position = m_PointLights[0]->Position;

		// 2. spot lights
		m_SpotLights.emplace_back(Exalted::SpotLight::Create());
		m_SpotLights[0]->Ambient = glm::vec3(1,1,0);
		m_SpotLights[0]->Diffuse = glm::vec3(1,1,0);
		m_SpotLights[0]->Specular = glm::vec3(1,1,0);
		m_SpotLights[0]->Position = glm::vec3(180, 10, 200);
		m_SpotLights[0]->SetAttenuationDistance(325);
		m_SpotLights[0]->CutoffInner = glm::cos(glm::radians(12.5f));
		m_SpotLights[0]->CutoffOuter = glm::cos(glm::radians(28.f));
		m_SpotLightTransforms.emplace_back(Exalted::GameTransform::Create());
		m_SpotLightTransforms[0]->Scale = glm::vec3(1.0);
		m_SpotLightTransforms[0]->Position = m_SpotLights[0]->Position;

		m_SpotLights.emplace_back(Exalted::SpotLight::Create());
		m_SpotLights[1]->Ambient = glm::vec3(1, 1, 0);
		m_SpotLights[1]->Diffuse = glm::vec3(1, 1, 0);
		m_SpotLights[1]->Specular = glm::vec3(1, 1, 0);
		m_SpotLights[1]->Position = glm::vec3(100, 20, 100);
		m_SpotLights[1]->Direction = glm::vec3(10, -360, 0);
		m_SpotLights[1]->SetAttenuationDistance(200);
		m_SpotLights[1]->CutoffInner = glm::cos(glm::radians(12.5f));
		m_SpotLights[1]->CutoffOuter = glm::cos(glm::radians(30.f));
		m_SpotLightTransforms.emplace_back(Exalted::GameTransform::Create());
		m_SpotLightTransforms[1]->Scale = glm::vec3(1);
		m_SpotLightTransforms[1]->Position = m_SpotLights[1]->Position;
		
		
		// 3. directional light
		m_DirectionalLight = Exalted::DirectionalLight::Create();
		m_DirectionalLight->Ambient = glm::vec3(0.2);
		m_DirectionalLight->Diffuse = glm::vec3(0.5);
		m_DirectionalLight->Specular = glm::vec3(0.0);
		m_DirectionalLight->Direction = glm::vec3(0.1f, 0.f, 0.f);
		
		// 4. Create Shaders
		Exalted::Ref<Exalted::Shader> multipleLightsShader = Exalted::Shader::Create(MULTIPLE_LIGHTS_VERTEX, MULTIPLE_LIGHTS_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Light_Uniforms", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		// 5. Create Materials
		Exalted::Ref<Exalted::Material> islandMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_ISLAND, TEXTURE_SPECULAR_ISLAND, "", TEXTURE_NORMAL_ISLAND, 33.f);
		Exalted::Ref<Exalted::Material> debugMaterial = Exalted::Material::Create(DEBUG_TEXTURE_GRID_D, DEBUG_TEXTURE_GRID_S, "", "", 33.f); //todo: change this

		//todo: Allow for this to be manually configured in the engine 
		// custom creation of leaf material due to engine back-end specifications
		Exalted::Ref<Exalted::Material> leafMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_LEAF, "", "", "", 2.f);
		Exalted::Ref<Exalted::Texture2D> leafTexture = Exalted::Texture2D::Create(TEXTURE_DIFFUSE_LEAF,
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		leafMaterial->DiffuseTexture = leafTexture;
		leafMaterial->SpecularTexture = leafTexture;
		leafMaterial->NormalTexture = leafTexture;
		
		// 4. Create Gameobjects

		////////////////////////////////////////////////////////////////////////
		//// ALL THE TREES 
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject = new Exalted::GameObject("Tree-A");
		Exalted::Ref<Exalted::Mesh> treeMesh = Exalted::Mesh::Create();
		treeMesh->SetVertexArray(Exalted::ObjLoader::Load(TREE_MESH, true));
		treeObject->SetMesh(treeMesh);
		treeObject->SetShader(multipleLightsShader);
		treeObject->SetMaterial(debugMaterial);
		treeObject->SetTransparency(true);
		treeObject->SetBoundingRadius(200.f);
		treeObject->GetTransform()->Scale = glm::vec3(10);
		treeObject->GetTransform()->Position = glm::vec3(1000.f, 2.1f, 1000.f);

		//todo: Make a transparent shader for these (manually tweaking their transparency)
		// tree leaves
		Exalted::GameObject* leafObject = new Exalted::GameObject("Tree-Leaves-A");
		Exalted::Ref<Exalted::Mesh> leafMesh = Exalted::Mesh::Create();
		leafMesh->SetVertexArray(Exalted::ObjLoader::Load(LEAF_MESH, true));
		leafObject->SetMesh(leafMesh);
		leafObject->SetShader(multipleLightsShader);
		leafObject->SetMaterial(leafMaterial);
		leafObject->SetTransparency(true);
		leafObject->SetBoundingRadius(200.f);
		leafObject->SetActive(false); //todo: remove this
		
		// add leaves to tree
		treeObject->AddChildObject(leafObject);
		
		////////////////////////////////////////////////////////////////////////
		//// Terrain A and its children 
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* terrainObject = new Exalted::GameObject("Terrain");
		Exalted::Ref<Exalted::Mesh> terrainMesh = Exalted::Mesh::Create();
		terrainMesh->SetVertexArray(Exalted::ObjLoader::Load(ISLAND_MAIN_MESH));
		terrainObject->GetTransform()->Scale = glm::vec3(10);
		terrainObject->GetTransform()->Position = glm::vec3(0,0,0);
		terrainObject->SetMesh(terrainMesh);
		terrainObject->SetShader(multipleLightsShader);
		terrainObject->SetMaterial(islandMaterial);
		terrainObject->SetTransparency(false);
		terrainObject->SetBoundingRadius(FLT_MAX);

		////////////////////////////////////////////////////////////////////////
		//// The Hut
		////////////////////////////////////////////////////////////////////////
		//Exalted::GameObject* hutMain = new Exalted::GameObject("Hut-Main");
		//Exalted::Ref<Exalted::Mesh> hutMainMesh = Exalted::Mesh::Create();
		//hutMainMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_MAIN));
		//hutMain->SetMesh(hutMainMesh);
		//hutMain->SetShader(multipleLightsShader);
		//hutMain->SetMaterial(debugMaterial);
		//hutMain->SetTransparency(false); 
		//hutMain->SetBoundingRadius(500.f);
		//hutMain->GetTransform()->Scale = glm::vec3(10);
		//hutMain->GetTransform()->Position = glm::vec3(0, 200, 400);
		//hutMain->GetTransform()->Rotation = glm::vec3(0, -45.f, 0);

		//Exalted::GameObject* hutRoof = new Exalted::GameObject("Hut-Roof");
		//Exalted::Ref<Exalted::Mesh> hutRoofMesh = Exalted::Mesh::Create();
		//hutRoofMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_ROOF));
		//hutRoof->SetMesh(hutRoofMesh);
		//hutRoof->SetShader(multipleLightsShader);
		//hutRoof->SetMaterial(debugMaterial);
		//hutRoof->SetTransparency(false); //todo: make the roof slightly transparent? or maybe the doorway
		//hutRoof->SetBoundingRadius(500.f);

		Exalted::GameObject* hutFloor = new Exalted::GameObject("Hut-Floor");
		Exalted::Ref<Exalted::Mesh> hutFloorMesh = Exalted::Mesh::Create();
		hutFloorMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_FLOOR));
		hutFloor->SetMesh(hutFloorMesh);
		hutFloor->SetShader(multipleLightsShader);
		hutFloor->SetMaterial(debugMaterial);
		hutFloor->SetTransparency(false);
		hutFloor->SetBoundingRadius(500.f);

		Exalted::GameObject* hutBottom = new Exalted::GameObject("Hut-Bottom");
		Exalted::Ref<Exalted::Mesh> hutBottomMesh = Exalted::Mesh::Create();
		hutBottomMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_BOTTOM));
		hutBottom->SetMesh(hutBottomMesh);
		hutBottom->SetShader(multipleLightsShader);
		hutBottom->SetMaterial(debugMaterial);
		hutBottom->SetTransparency(false);
		hutBottom->SetBoundingRadius(500.f);
		hutBottom->GetTransform()->Scale = glm::vec3(10);
		hutBottom->GetTransform()->Position = glm::vec3(0, 200, 400);
		hutBottom->GetTransform()->Rotation = glm::vec3(0, -45.f, 0);
		
		// parent hut objects
		hutBottom->AddChildObject(hutFloor);
		//hutMain->AddChildObject(hutRoof);
		//hutMain->AddChildObject(hutFloor);
		//hutMain->AddChildObject(hutBottom);
		//hutMain->SetActive(false); //todo; reactivate this
		
		Exalted::GameObject* symmetra = new Exalted::GameObject("Sculpture");
		Exalted::Ref<Exalted::Mesh> sculptureMesh = Exalted::Mesh::Create();
		sculptureMesh->SetVertexArray(Exalted::ObjLoader::Load(SYMMETRA));
		symmetra->SetMesh(sculptureMesh);
		symmetra->SetShader(multipleLightsShader);
		symmetra->SetMaterial(debugMaterial);
		symmetra->SetTransparency(false);
		symmetra->SetBoundingRadius(500.f);
		symmetra->GetTransform()->Scale = glm::vec3(40);
		symmetra->GetTransform()->Position = glm::vec3(-30.f, 364.f, 290.f);
		//sculpture->SetActive(false);
		
		// 5. Add all gameobjects to scene manager
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject);
		m_SceneManager->GetSceneRoot()->AddChildObject(terrainObject);
		m_SceneManager->GetSceneRoot()->AddChildObject(hutBottom);
		m_SceneManager->GetSceneRoot()->AddChildObject(symmetra);
		m_SceneManager->GetSceneRoot()->GetTransform()->Scale = glm::vec3(.1f); //todo: !
		
		// lighting UBO setup 
		const Exalted::Bytes noPointLights = 1;
		const Exalted::Bytes noDirectionalLights = 1;
		const Exalted::Bytes noSpotLights = 2;
		const Exalted::Bytes lightsBBI = 1;
		const Exalted::Bytes lightsOffset = 0;
		Exalted::Bytes lightsBufferSize = noPointLights * Exalted::PointLight::UBSize() + noDirectionalLights * Exalted::DirectionalLight::UBSize() + noSpotLights * Exalted::SpotLight::UBSize();
		m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);

		/////////////////////////////////////////////////
		////// Shadow Data Setup ////////////////////////
		/////////////////////////////////////////////////
		m_DepthFrameBuffer = Exalted::FrameBuffer::Create(4096, 4096, true);
		m_QuadDepthShader = Exalted::Shader::Create(SHADOW_QUAD_TEST_SHADER_VERTEX, SHADOW_QUAD_TEST_SHADER_FRAGMENT); //todo: render models into this
		m_ObjectDepthShader = Exalted::Shader::Create(DIRECTIONAL_SHADOW_SHADER_VERTEX_DEPTH, DIRECTIONAL_SHADOW_SHADER_FRAGMENT_DEPTH); //todo: render models into this
		m_QuadMesh = Exalted::Mesh::Create();
		m_QuadMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateIndexedQuad());


		//todo: instance this? 
		/////////////////////////////////////
		//// DEBUG DATA /////////////////////
		/////////////////////////////////////
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		m_SpotLightMesh = Exalted::Mesh::Create();
		m_SpotLightMesh->SetVertexArray(Exalted::ObjLoader::Load(SPOT_LIGHTHOUSE_MESH));

		m_LightSourceShader = Exalted::Shader::Create(LIGHT_SOURCE_SHADER_VERTEX, LIGHT_SOURCE_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Uniforms", 2);
	}
	
	void EnvironmentSceneLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		// 1. Update the camera transformation and buffer objects, either dependent on lerping motion or manual movement
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		if (DEBUG_FlashlightMode)
		{
			m_SpotLights[0]->Position = m_EditorCamera->GetPosition();
			m_SpotLights[0]->Direction = m_EditorCamera->GetFront();
		}

		if(DEBUG_ColorChange)
		{
			glm::vec3 lightColor;
			lightColor.x = sin(TIME);
			lightColor.y = sin(TIME * 0.5f);
			lightColor.z = sin(TIME * 1.5f);

			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
			glm::vec3 specularColor = lightColor;

			m_SpotLights[0]->Diffuse = diffuseColor;
			m_SpotLights[0]->Ambient = ambientColor;
			m_SpotLights[0]->Specular = specularColor;
		}
		
		/////////////////////////////////////////////////////////////////////////////
		//// LIGHT UNIFORM DATA /////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		for (int i = 0; i < m_PointLights.size(); ++i)
		{
			m_PointLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			m_PointLightTransforms[i]->Position = m_PointLights[i]->Position; 
		}
		m_DirectionalLight->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		for (int i = 0; i < m_SpotLights.size(); ++i)
		{
			m_SpotLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			m_SpotLightTransforms[i]->Position = m_SpotLights[i]->Position;
		}
		m_LightUniformBuffer->Unbind();

		/////////////////////////////////////////////////////////////////////////////
		//// Sort the scene objects for rendering ///////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		m_SceneManager->RenderScene();

		///////////////////////////////////////////////////////////////////////////////////////
		//// Directional Light Shadows
		/////////////////////////////////////////////////////////////////////////////////////
		float near_plane = 1.f;
		float far_plane = 200.f;
		glm::mat4 lightProjection = glm::perspective<float>(glm::radians(45.0f), 1.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(m_SpotLights[1]->Position, m_SpotLights[1]->Position + m_SpotLights[1]->Direction, glm::vec3(0, 1, 0)); //glm::normalize(
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		Exalted::GameObject::LightSpaceMatrix = lightSpaceMatrix;

		/////////////////////////////////////////////////////////////////
		//// Initial render to depth map for shadow mapping ///////////// 
		/////////////////////////////////////////////////////////////////
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		//Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::FRONT);
		m_DepthFrameBuffer->Bind();
		m_ObjectDepthShader->Bind();
		Exalted::RenderCommand::ClearColorDepthBuffers();//ClearDepthBuffer();
		Exalted::OpenGLConfigurations::EnableDepthTesting();

		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformMatFloat4("lightSpaceMatrix", lightSpaceMatrix);
		
		m_SceneManager->DrawOpaqueBindless(m_ObjectDepthShader);
		m_SceneManager->DrawTransparentBindless(m_ObjectDepthShader);
		
		m_DepthFrameBuffer->UnbindMiniFrame();
		m_ObjectDepthShader->Unbind();
		//Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::OpenGLConfigurations::DisableFaceCulling();

		
		/////////////////////////////////////////////////////////////////////////////
		//// Render scene output //////////////////////////////////////////////////// 
		/////////////////////////////////////////////////////////////////////////////
		Exalted::RenderCommand::ClearColorDepthBuffers();
		m_DepthFrameBuffer->BindTexture(4); // assign depth texture

		// render scene
		m_SceneManager->DrawOpaqueObjects();

		// Render scene lights
		m_LightSourceShader->Bind();

		for (int i = 0; i < m_PointLights.size(); ++i)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_PointLightTransforms[i]->SetAndGetWorldTransform());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_PointLights[i]->Diffuse);
			Exalted::Renderer::Submit(m_LightSourceMesh);
		}

		// render all spot lights except the flashlight
		for (int i = 1; i < m_SpotLights.size(); ++i)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_SpotLightTransforms[i]->SetAndGetWorldTransform());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_SpotLights[i]->Diffuse);
			Exalted::Renderer::Submit(m_LightSourceMesh);
		}
		// render the flashlight spotlight
		if (!DEBUG_FlashlightMode)
		{
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_SpotLightTransforms[0]->SetAndGetWorldTransform());
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_SpotLights[0]->Diffuse);
			Exalted::Renderer::Submit(m_LightSourceMesh);
		}
		m_LightSourceShader->Unbind();

		// render the rest of the scene, skyboxes and transparent objects
		m_SceneManager->RenderSkybox();

		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);
		m_SceneManager->DrawTransparentObjects();
		Exalted::OpenGLConfigurations::DisableBlending();
		
		m_SceneManager->ClearObjectLists();
		Exalted::OpenGLConfigurations::DisableDepthTesting();

		/////////////////////////////////////////////////////////////////////////////
		//// Directional Light Shadow Debugging ///////////////////////////////////// 
		/////////////////////////////////////////////////////////////////////////////

		Exalted::OpenGLConfigurations::SetViewport(0, 0, 1024, 512);
		m_QuadDepthShader->Bind();
		m_DepthFrameBuffer->BindTexture(4);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("near_plane", near_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("far_plane", far_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformInt1("depthMap", 4);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_QuadDepthShader->Unbind();
		m_DepthFrameBuffer->Unbind();
		Exalted::OpenGLConfigurations::SetViewport(0, 0, Exalted::Application::Get().GetWindow().GetWindowWidth(), Exalted::Application::Get().GetWindow().GetWindowHeight());

	}

	void EnvironmentSceneLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI();
		ImGui::Begin("Environment Scene Settings");
		ImGui::Text("----------------------------");
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		ImGui::Text("Directional Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Direction", glm::value_ptr(m_DirectionalLight->Direction));
		ImGui::Text("----------------------------");
		ImGui::Text("Point Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Position", glm::value_ptr(m_PointLights[0]->Position));
		ImGui::InputFloat3("Ambient", glm::value_ptr(m_PointLights[0]->Ambient));
		ImGui::InputFloat3("Diffuse", glm::value_ptr(m_PointLights[0]->Diffuse));
		ImGui::InputFloat3("Specular", glm::value_ptr(m_PointLights[0]->Specular));
		ImGui::Text("----------------------------");
		ImGui::Text("Spot Light Settings");
		ImGui::Text("----------------------------");
		ImGui::Text("Lighthouse Settings");
		ImGui::InputFloat3("LH - Position", glm::value_ptr(m_SpotLights[1]->Position));
		ImGui::InputFloat3("LH - Direction", glm::value_ptr(m_SpotLights[1]->Direction));
		ImGui::InputFloat3("LH - Ambient", glm::value_ptr(m_SpotLights[1]->Ambient));
		ImGui::InputFloat3("LH - Diffuse", glm::value_ptr(m_SpotLights[1]->Diffuse));
		ImGui::InputFloat3("LH - Specular", glm::value_ptr(m_SpotLights[1]->Specular));
		ImGui::InputFloat("LH - RADIUS INNER", &m_SpotLights[1]->CutoffInner);
		ImGui::InputFloat("LH - RADIUS OUTER", &m_SpotLights[1]->CutoffOuter);

		if (ImGui::Button("Toggle Flashlight Mode"))
		{
			DEBUG_FlashlightMode = !DEBUG_FlashlightMode;
		}
		if (ImGui::Button("Toggle Flashlight Color"))
		{
			DEBUG_ColorChange = !DEBUG_ColorChange;
		}
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
