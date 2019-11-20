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
		m_EditorCamera->SetMouseSpeed(400.f);
	}
	
	void EnvironmentSceneLayer::OnAttach()
	{
		EX_INFO("Environment Scene Layer attached successfully.");

		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera, true, false);

		//// point light A
		//m_PointLights.emplace_back(Exalted::PointLight::Create());
		//m_PointLights[0]->Ambient = glm::vec3(0.2);
		//m_PointLights[0]->Diffuse = glm::vec3(0.5);
		//m_PointLights[0]->Specular = glm::vec3(0.8);
		//m_PointLights[0]->Position = glm::vec3(0.1,0.2,0.3);
		//m_PointLights[0]->SetAttenuationDistance(200);
		//m_PointLightTransforms.emplace_back(Exalted::GameTransform::Create());
		//m_PointLightTransforms[0]->Position = m_PointLights[0]->Position;
		//
		//// point light B
		//m_PointLights.emplace_back(Exalted::PointLight::Create());
		//m_PointLights[1]->Ambient = glm::vec3(0.2);
		//m_PointLights[1]->Diffuse = glm::vec3(0.5);
		//m_PointLights[1]->Specular = glm::vec3(0.8);
		//m_PointLights[1]->Position = glm::vec3(0.1, 0, 0.3);
		//m_PointLights[1]->SetAttenuationDistance(200);
		//m_PointLightTransforms.emplace_back(Exalted::GameTransform::Create());
		//m_PointLightTransforms[0]->Position = m_PointLights[1]->Position;

		// directional light
		m_DirectionalLight = Exalted::DirectionalLight::Create();
		m_DirectionalLight->Ambient = glm::vec3(0.2);
		m_DirectionalLight->Diffuse = glm::vec3(0.5);
		m_DirectionalLight->Specular = glm::vec3(1.0);
		m_DirectionalLight->Direction = glm::vec3(-0.1f, -0.1f, -0.1f);
		m_DirectionalLightTransform = Exalted::GameTransform::Create();
		m_DirectionalLightTransform->Position = m_DirectionalLight->Direction;
		
		// 3. Create Meshes
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateCubeIT(1.f));
		
		// 4. Create Shaders
		Exalted::Ref<Exalted::Shader> terrainShaderNNNS = Exalted::Shader::Create(LIGHTING_NNNS_VERTEX, LIGHTING_NNNS_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(terrainShaderNNNS)->SetUniformBlockIndex("Light_Uniforms", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(terrainShaderNNNS)->SetUniformBlockIndex("Camera_Uniforms", 2);

		Exalted::Ref<Exalted::Shader> basicLightingShader = Exalted::Shader::Create(BASIC_LIGHTING_SHADER_VERTEX, BASIC_LIGHTING_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(basicLightingShader)->SetUniformBlockIndex("Light_Uniforms", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(basicLightingShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		m_LightSourceShader = Exalted::Shader::Create(LIGHT_SOURCE_SHADER_VERTEX, LIGHT_SOURCE_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		// 5. Create Materials
		Exalted::Ref<Exalted::Material> brickMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_BRICKS, TEXTURE_SPECULAR_BRICKS, "", TEXTURE_NORMAL_BRICKS, 32.f);
		Exalted::Ref<Exalted::Material> islandMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_ISLAND, TEXTURE_SPECULAR_ISLAND, "", TEXTURE_NORMAL_ISLAND, 16.f);
		Exalted::Ref<Exalted::Material> leafMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_LEAF, "", "", "", 2.f);
		Exalted::Ref<Exalted::Material> treeBarkMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_ISLAND, TEXTURE_SPECULAR_ISLAND, "", TEXTURE_NORMAL_ISLAND, 16.f); //todo: change this

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
		//Exalted::Ref<Exalted::Material> treeMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_BRICKS, TEXTURE_SPECULAR_BRICKS, "", TEXTURE_NORMAL_BRICKS, 32.f);
		
		// 4. Create Gameobjects

		////////////////////////////////////////////////////////////////////////
		//// ALL THE TREES 
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject = new Exalted::GameObject("Tree-A");
		Exalted::Ref<Exalted::Mesh> treeMesh = Exalted::Mesh::Create();
		treeMesh->SetVertexArray(Exalted::ObjLoader::Load(TREE_MESH, true));
		treeObject->SetMesh(treeMesh);
		treeObject->SetShader(terrainShaderNNNS);
		treeObject->SetMaterial(treeBarkMaterial);
		treeObject->SetTransparency(true);
		treeObject->SetBoundingRadius(200.f);
		treeObject->GetTransform()->Scale = glm::vec3(10);
		treeObject->GetTransform()->Rotation = glm::vec3(0,90,0);
		treeObject->GetTransform()->Position = glm::vec3(570.f, 186.f, -400);
		
		// tree leaves
		Exalted::GameObject* leafObject = new Exalted::GameObject("Tree-Leaves-A");
		Exalted::Ref<Exalted::Mesh> leafMesh = Exalted::Mesh::Create();
		leafMesh->SetVertexArray(Exalted::ObjLoader::Load(LEAF_MESH, true));
		leafObject->SetMesh(leafMesh);
		leafObject->SetShader(terrainShaderNNNS);
		leafObject->SetMaterial(leafMaterial);
		leafObject->SetTransparency(true);
		leafObject->SetBoundingRadius(200.f);

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
		terrainObject->SetShader(basicLightingShader);
		terrainObject->SetMaterial(islandMaterial);
		terrainObject->SetTransparency(false);
		terrainObject->SetBoundingRadius(FLT_MAX);

		////////////////////////////////////////////////////////////////////////
		//// The Hut
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* hutMain = new Exalted::GameObject("Hut-Main");
		Exalted::Ref<Exalted::Mesh> hutMainMesh = Exalted::Mesh::Create();
		hutMainMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_MAIN));
		hutMain->SetMesh(hutMainMesh);
		hutMain->SetShader(terrainShaderNNNS);
		hutMain->SetMaterial(brickMaterial);
		hutMain->SetTransparency(false); 
		hutMain->SetBoundingRadius(500.f);
		hutMain->GetTransform()->Scale = glm::vec3(10);
		hutMain->GetTransform()->Position = glm::vec3(0, 200, 400);
		hutMain->GetTransform()->Rotation = glm::vec3(0, -45.f, 0);

		Exalted::GameObject* hutRoof = new Exalted::GameObject("Hut-Roof");
		Exalted::Ref<Exalted::Mesh> hutRoofMesh = Exalted::Mesh::Create();
		hutRoofMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_ROOF));
		hutRoof->SetMesh(hutRoofMesh);
		hutRoof->SetShader(terrainShaderNNNS);
		hutRoof->SetMaterial(brickMaterial);
		hutRoof->SetTransparency(false); //todo: make the roof slightly transparent? or maybe the doorway
		hutRoof->SetBoundingRadius(500.f);

		Exalted::GameObject* hutFloor = new Exalted::GameObject("Hut-Floor");
		Exalted::Ref<Exalted::Mesh> hutFloorMesh = Exalted::Mesh::Create();
		hutFloorMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_FLOOR));
		hutFloor->SetMesh(hutFloorMesh);
		hutFloor->SetShader(terrainShaderNNNS);
		hutFloor->SetMaterial(brickMaterial);
		hutFloor->SetTransparency(false);
		hutFloor->SetBoundingRadius(500.f);

		Exalted::GameObject* hutBottom = new Exalted::GameObject("Hut-Bottom");
		Exalted::Ref<Exalted::Mesh> hutBottomMesh = Exalted::Mesh::Create();
		hutBottomMesh->SetVertexArray(Exalted::ObjLoader::Load(HUT_MESH_BOTTOM));
		hutBottom->SetMesh(hutBottomMesh);
		hutBottom->SetShader(terrainShaderNNNS);
		hutBottom->SetMaterial(brickMaterial);
		hutBottom->SetTransparency(false);
		hutBottom->SetBoundingRadius(500.f);

		// parent hut objects
		hutMain->AddChildObject(hutRoof);
		hutMain->AddChildObject(hutFloor);
		hutMain->AddChildObject(hutBottom);


		Exalted::GameObject* sculpture = new Exalted::GameObject("Sculpture");
		Exalted::Ref<Exalted::Mesh> sculptureMesh = Exalted::Mesh::Create();
		sculptureMesh->SetVertexArray(Exalted::ObjLoader::Load(SCULPTURE_MESH));
		sculpture->SetMesh(sculptureMesh);
		sculpture->SetShader(terrainShaderNNNS);
		sculpture->SetMaterial(islandMaterial);
		sculpture->SetTransparency(false);
		sculpture->SetBoundingRadius(500.f);
		sculpture->GetTransform()->Scale = glm::vec3(40);
		sculpture->GetTransform()->Position = glm::vec3(0.f, 445.f, 400.f);		
		
		//sculpture->GetTransform()->Rotation = glm::vec3(0, -45.f, 0);		
		
		// 5. Add all gameobjects to scene manager
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject);
		m_SceneManager->GetSceneRoot()->AddChildObject(terrainObject);
		m_SceneManager->GetSceneRoot()->AddChildObject(hutMain);
		m_SceneManager->GetSceneRoot()->AddChildObject(sculpture);
		
		// lighting UBO setup 
		const Exalted::Bytes noPointLights = 0;
		const Exalted::Bytes noDirectionalLights = 1;
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
		m_SceneManager->UpdateScene(deltaTime);

		// --------------------------- Set Light uniform data ------------------------------- //
		m_LightUniformBuffer->Bind();
		Exalted::Bytes lightBufferOffset = 0;
		//for (int i = 0; i < m_PointLights.size(); ++i)
		//{
		//	m_PointLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		//	m_PointLightTransforms[i]->Position = m_PointLights[i]->Position; 
		//}
		m_DirectionalLight->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		m_DirectionalLightTransform->Position = m_DirectionalLight->Direction; //todo: move this from here if it won't move
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

		m_LightSourceShader->Bind();
		//// point lights
		//for (int i = 0; i < m_PointLights.size(); ++i) //todo: instance this
		//{
		//	std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_PointLightTransforms[i]->SetAndGetWorldTransform());
		//	std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_PointLights[i]->Diffuse);
		//	Exalted::Renderer::Submit(m_LightSourceMesh);
		//}
		// directional light
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformMatFloat4("u_Model", m_DirectionalLightTransform->SetAndGetWorldTransform());
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformFloat3("u_SourceDiffuse", m_DirectionalLight->Diffuse);
		Exalted::Renderer::Submit(m_LightSourceMesh);
		m_LightSourceShader->Unbind();
		m_SceneManager->RenderSkybox();

		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::EnableBlending();
		m_SceneManager->DrawTransparentObjects();
		m_SceneManager->ClearObjectLists();
		
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();
	}

	void EnvironmentSceneLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();
		m_SceneManager->GetSceneRoot()->RenderHierarchyGUI();
		ImGui::Begin("Environment Scene Settings");
		ImGui::Text("----------------------------");
		ImGui::Text("Directional Light Settings");
		ImGui::Text("----------------------------");
		ImGui::InputFloat3("Direction", glm::value_ptr(m_DirectionalLight->Direction));
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
