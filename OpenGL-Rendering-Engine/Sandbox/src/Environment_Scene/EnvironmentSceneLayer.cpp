/***************************************************************************
 * Filename		: EnvironmentSceneLayer.cpp
 * Name			: Ori Lazar
 * Date			: 20/11/2019
 * Description	: Contains the implementation for the environment layer used to
                  demonstrate the capabilities of this engine.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
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
		for (int i = 0; i < 4; i++)
		{
			m_SpotLights.emplace_back(Exalted::SpotLight::Create());
			m_SpotLights[i]->Ambient = glm::vec3(0.f, 1.f, 0.4f);
			m_SpotLights[i]->Diffuse = glm::vec3(0.f, 0.5f, 0.f);
			m_SpotLights[i]->Specular = glm::vec3(0.f, 1.f, 0.f);
			m_SpotLights[i]->Direction = glm::vec3(10, -360, 0.f);
			m_SpotLights[i]->SetAttenuationDistance(200);
			m_SpotLights[i]->CutoffInner = glm::cos(glm::radians(12.5f));
			m_SpotLights[i]->CutoffOuter = 0.8f;
			// m_SpotLightTransforms.emplace_back(Exalted::GameTransform::Create());
			// m_SpotLightTransforms[i]->Position = m_SpotLights[i]->Position;
		}
		
		// 3. directional light
		m_DirectionalLight = Exalted::DirectionalLight::Create();
		m_DirectionalLight->Ambient = glm::vec3(0.15, 0.1, 0.3);
		m_DirectionalLight->Diffuse = glm::vec3(0.5);
		m_DirectionalLight->Specular = glm::vec3(0.1,0.3,0.2);
		m_DirectionalLight->Direction = glm::vec3(0.1f, 20.f, -6.f);
		
		// 4. Create Shaders
		Exalted::Ref<Exalted::Shader> multipleLightsShader = Exalted::Shader::Create(MULTIPLE_LIGHTS_VERTEX, MULTIPLE_LIGHTS_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Light_Uniforms", 1);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Camera_Uniforms", 2);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Light_Space_Uniforms", 3);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(multipleLightsShader)->SetUniformBlockIndex("Directional_Light_Space_Uniforms", 4);
		
		Exalted::Ref<Exalted::Shader> skyboxReflectiveShader = Exalted::Shader::Create(SKYBOX_MAP_VERTEX, SKYBOX_MAP_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(skyboxReflectiveShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

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
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// UFOs
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Exalted::Ref<Exalted::Mesh> ufoMesh = Exalted::Mesh::Create();
		ufoMesh->SetVertexArray(Exalted::ObjLoader::Load(UFO_MESH, true));
		Exalted::Ref<Exalted::Material> ufoMaterial = Exalted::Material::Create(TEXTURE_DIFFUSE_UFO, TEXTURE_SPECULAR_UFO, TEXTURE_EMISSION_UFO, TEXTURE_NORMAL_UFO, 33.f);
		Exalted::Ref<Exalted::Texture2D> ufoDiffuseRGBA = Exalted::Texture2D::Create(TEXTURE_DIFFUSE_UFO,
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);
		ufoMaterial->DiffuseTexture = ufoDiffuseRGBA; 
		
		Exalted::GameObject* UFOA = new Exalted::GameObject("UFO-A");

		UFOA->SetMesh(ufoMesh);
		UFOA->SetShader(multipleLightsShader); //todo: make sure near plane is far enough not to take effect from these objects
		UFOA->SetMaterial(ufoMaterial);
		UFOA->SetTransparency(true);
		UFOA->SetBoundingRadius(30.f);
		UFOA->GetTransform()->Scale = glm::vec3(3);
		UFOA->GetTransform()->Position = glm::vec3(1000.f, 200.f, 1000.f);
		UFOA->SetSpotLight(m_SpotLights[0]);
		UFOA->AddGameComponent(new Exalted::RotateComponent(UFOA->GetTransform(), glm::vec3(0,20,0)));
		
		Exalted::GameObject* UFOB = new Exalted::GameObject("UFO-B");

		UFOB->SetMesh(ufoMesh);
		UFOB->SetShader(multipleLightsShader); //todo: make sure near plane is far enough not to take effect from these objects
		UFOB->SetMaterial(ufoMaterial);
		UFOB->SetTransparency(true);
		UFOB->SetBoundingRadius(30.f);
		UFOB->GetTransform()->Scale = glm::vec3(3);
		UFOB->GetTransform()->Position = glm::vec3(-1000.f, 200.f, 1000.f);
		UFOB->SetSpotLight(m_SpotLights[1]);
		UFOB->AddGameComponent(new Exalted::RotateComponent(UFOB->GetTransform(), glm::vec3(0, -20, 0)));

		Exalted::GameObject* UFOC = new Exalted::GameObject("UFO-C");
		UFOC->SetMesh(ufoMesh);
		UFOC->SetShader(multipleLightsShader); //todo: make sure near plane is far enough not to take effect from these objects
		UFOC->SetMaterial(ufoMaterial);
		UFOC->SetTransparency(true); 
		UFOC->SetBoundingRadius(30.f);
		UFOC->GetTransform()->Scale = glm::vec3(3);
		UFOC->GetTransform()->Position = glm::vec3(-1000.f, 200.f, -1000.f);
		UFOC->SetSpotLight(m_SpotLights[2]);
		UFOC->AddGameComponent(new Exalted::RotateComponent(UFOC->GetTransform(), glm::vec3(0, 20, 0)));

		Exalted::GameObject* UFOD = new Exalted::GameObject("UFO-D");
		UFOD->SetMesh(ufoMesh);
		UFOD->SetShader(multipleLightsShader); //todo: make sure near plane is far enough not to take effect from these objects
		UFOD->SetMaterial(ufoMaterial);
		UFOD->SetTransparency(true); 
		UFOD->SetBoundingRadius(30.f);
		UFOD->GetTransform()->Scale = glm::vec3(3);
		UFOD->GetTransform()->Position = glm::vec3(1000.f, 200.f, -1000.f);
		UFOD->SetSpotLight(m_SpotLights[3]);
		UFOD->AddGameComponent(new Exalted::RotateComponent(UFOD->GetTransform(), glm::vec3(0, -20, 0)));

		//todo: This UFO maps the skybox onto it
		Exalted::GameObject* reflectiveUFO = new Exalted::GameObject("UFO-Reflective");

		reflectiveUFO->SetMesh(ufoMesh);
		reflectiveUFO->SetShader(skyboxReflectiveShader); 
		reflectiveUFO->SetTransparency(true);
		reflectiveUFO->SetBoundingRadius(100.f);
		reflectiveUFO->GetTransform()->Scale = glm::vec3(10);
		reflectiveUFO->GetTransform()->Position = glm::vec3(-50, 600.f, -500);
		reflectiveUFO->AddGameComponent(new Exalted::RotateComponent(reflectiveUFO->GetTransform(), glm::vec3(0, 360, 0)));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// TREES
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Exalted::Ref<Exalted::Mesh> treeMesh = Exalted::Mesh::Create();
		treeMesh->SetVertexArray(Exalted::ObjLoader::Load(TREE_MESH, true));
		Exalted::Ref<Exalted::Mesh> leafMesh = Exalted::Mesh::Create();
		leafMesh->SetVertexArray(Exalted::ObjLoader::Load(LEAF_MESH, true));
		
		////////////////////////////////////////////////////////////////////////
		//// TREE 1
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject = new Exalted::GameObject("Tree-A");
		
		treeObject->SetMesh(treeMesh);
		treeObject->SetShader(multipleLightsShader);
		treeObject->SetMaterial(debugMaterial);
		treeObject->SetTransparency(false);
		treeObject->SetBoundingRadius(10.f);
		treeObject->GetTransform()->Scale = glm::vec3(0.01);
		treeObject->GetTransform()->Position = glm::vec3(1000.f, 1.5f, 1000.f);
		treeObject->AddGameComponent(new Exalted::ScaleGrowComponent(treeObject->GetTransform(), glm::vec3(0.35f), glm::vec3(15), 50.f));

		Exalted::GameObject* leafObject = new Exalted::GameObject("Tree-Leaves-A");
		leafObject->SetMesh(leafMesh);
		leafObject->SetShader(multipleLightsShader);
		leafObject->SetMaterial(leafMaterial);
		leafObject->SetTransparency(true);
		leafObject->SetBoundingRadius(15.f);

		// add leaves to tree
		treeObject->AddChildObject(leafObject);

		////////////////////////////////////////////////////////////////////////
		//// TREE 2
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject2 = new Exalted::GameObject("Tree-B");
		treeObject2->SetMesh(treeMesh);
		treeObject2->SetShader(multipleLightsShader);
		treeObject2->SetMaterial(debugMaterial);
		treeObject2->SetTransparency(true);
		treeObject2->SetBoundingRadius(200.f);
		treeObject2->GetTransform()->Scale = glm::vec3(0.01);
		treeObject2->GetTransform()->Position = glm::vec3(-1000.f, 19.5f, 1000.f);
		treeObject2->AddGameComponent(new Exalted::ScaleGrowComponent(treeObject2->GetTransform(), glm::vec3(0.35f), glm::vec3(15), 50.f));

		Exalted::GameObject* leafObject2 = new Exalted::GameObject("Tree-Leaves-B");
		leafObject2->SetMesh(leafMesh);
		leafObject2->SetShader(multipleLightsShader);
		leafObject2->SetMaterial(leafMaterial);
		leafObject2->SetTransparency(true);
		leafObject2->SetBoundingRadius(15.f);
		
		// add leaves to tree
		treeObject2->AddChildObject(leafObject2);

		////////////////////////////////////////////////////////////////////////
		//// TREE 3
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject3 = new Exalted::GameObject("Tree-C");
		treeObject3->SetMesh(treeMesh);
		treeObject3->SetShader(multipleLightsShader);
		treeObject3->SetMaterial(debugMaterial);
		treeObject3->SetTransparency(true);
		treeObject3->SetBoundingRadius(200.f);
		treeObject3->GetTransform()->Scale = glm::vec3(0.01);
		treeObject3->GetTransform()->Position = glm::vec3(-1000.f, 3.f, -1000.f);
		treeObject3->AddGameComponent(new Exalted::ScaleGrowComponent(treeObject3->GetTransform(), glm::vec3(0.35f), glm::vec3(15), 50.f));

		Exalted::GameObject* leafObject3 = new Exalted::GameObject("Tree-Leaves-C");
		leafObject3->SetMesh(leafMesh);
		leafObject3->SetShader(multipleLightsShader);
		leafObject3->SetMaterial(leafMaterial);
		leafObject3->SetTransparency(true);
		leafObject3->SetBoundingRadius(15.f);
		
		// add leaves to tree
		treeObject3->AddChildObject(leafObject3);

		////////////////////////////////////////////////////////////////////////
		//// TREE 4
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* treeObject4 = new Exalted::GameObject("Tree-D");
		treeObject4->SetMesh(treeMesh);
		treeObject4->SetShader(multipleLightsShader);
		treeObject4->SetMaterial(debugMaterial);
		treeObject4->SetTransparency(true);
		treeObject4->SetBoundingRadius(200.f);
		treeObject4->GetTransform()->Scale = glm::vec3(0.01);
		treeObject4->GetTransform()->Position = glm::vec3(1000.f, 9.4f, -1000.f);
		treeObject4->AddGameComponent(new Exalted::ScaleGrowComponent(treeObject4->GetTransform(), glm::vec3(0.35f), glm::vec3(15), 50.f));

		Exalted::GameObject* leafObject4 = new Exalted::GameObject("Tree-Leaves-D");
		leafObject4->SetMesh(leafMesh);
		leafObject4->SetShader(multipleLightsShader);
		leafObject4->SetMaterial(leafMaterial);
		leafObject4->SetTransparency(true);
		leafObject4->SetBoundingRadius(15.f);
		
		// add leaves to tree
		treeObject4->AddChildObject(leafObject4);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		////////////////////////////////////////////////////////////////////////
		//// Terrain
		////////////////////////////////////////////////////////////////////////
		Exalted::GameObject* terrainObject = new Exalted::GameObject("Terrain");
		Exalted::Ref<Exalted::Mesh> terrainMesh = Exalted::Mesh::Create();
		terrainMesh->SetVertexArray(Exalted::ObjLoader::Load(ISLAND_MAIN_MESH));
		terrainObject->GetTransform()->Scale = glm::vec3(10,0,10);
		terrainObject->GetTransform()->Position = glm::vec3(0,0,0);
		terrainObject->SetMesh(terrainMesh);
		terrainObject->SetShader(multipleLightsShader);
		terrainObject->SetMaterial(islandMaterial);
		terrainObject->SetTransparency(false);
		terrainObject->SetBoundingRadius(FLT_MAX);
		terrainObject->AddGameComponent(new Exalted::ScaleGrowComponent(terrainObject->GetTransform(), glm::vec3(0,0.25,0), glm::vec3(10)));

		// 5. Add all gameobjects to scene manager
		m_SceneManager->GetSceneRoot()->AddChildObject(UFOA);
		m_SceneManager->GetSceneRoot()->AddChildObject(UFOB);
		m_SceneManager->GetSceneRoot()->AddChildObject(UFOC);
		m_SceneManager->GetSceneRoot()->AddChildObject(UFOD);
		m_SceneManager->GetSceneRoot()->AddChildObject(reflectiveUFO);
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject);
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject2);
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject3);
		m_SceneManager->GetSceneRoot()->AddChildObject(treeObject4);
		m_SceneManager->GetSceneRoot()->AddChildObject(terrainObject);
		m_SceneManager->GetSceneRoot()->GetTransform()->Scale = glm::vec3(.1f);
		
		Exalted::Ref<Exalted::LightManager> lightManager = Exalted::CreateRef<Exalted::LightManager>(1,3);
		for (int i = 0; i < m_PointLights.size(); ++i)
		{
			lightManager->AddPointLight(m_PointLights[i]); // todo: note the point light will currently not have a mesh as it is not attached to an object
		}
		lightManager->AddDirectionalLight(m_DirectionalLight);
		for (int i = 0; i < m_SpotLights.size(); ++i)
		{
			lightManager->AddSpotLight(m_SpotLights[i]);
		}
		m_SceneManager->SetLightManager(lightManager);
		m_SceneManager->SetupSceneLightUBOs(); //todo: verify this works
		// lighting UBO setup 
		// // // const Exalted::Bytes noPointLights = 1;
		// // // const Exalted::Bytes noDirectionalLights = 1;
		//const Exalted::Bytes noSpotLights = 2;
		// // // const Exalted::Bytes lightsBBI = 1;
		// // // const Exalted::Bytes lightsOffset = 0;
		// // // Exalted::Bytes lightsBufferSize = noPointLights * Exalted::PointLight::UBSize() + noDirectionalLights * Exalted::DirectionalLight::UBSize() + noSpotLights * Exalted::SpotLight::UBSize();
		// // // m_LightUniformBuffer = Exalted::UniformBuffer::Create(lightsBufferSize);
		// // // m_LightUniformBuffer->BindBufferRange(lightsBBI, lightsOffset, lightsBufferSize);

		//////////////////////////////////////////////////////////////////////
		////todo: Stuff that needs to go into the scene graph ////////////////
		//////////////////////////////////////////////////////////////////////
		// number of point lights reference in gameobject draw 
		
		//todo: move this from here into
		// const Exalted::Bytes noSpotLights = 2;
		// Exalted::GameObject::NumberOfSpotLights = noSpotLights;

		/////////////////todo: Light space data uniform buffers
		//todo: move this to the scene graph, contains the light space matrices data.
		// // // const Exalted::Bytes lightsSpaceOffset = 0;
		// // // const Exalted::Bytes lightSpaceDataBBI = 3; //todo: move this to light manager static lights section
		// // // Exalted::Bytes lightSpaceBufferSize = sizeof(glm::mat4) * noSpotLights;
		// // // m_LightSpaceDataUniformBuffer = Exalted::UniformBuffer::Create(lightSpaceBufferSize);
		// // // m_LightSpaceDataUniformBuffer->BindBufferRange(lightSpaceDataBBI, lightsSpaceOffset, lightSpaceBufferSize);

		//todo: create a shadow manager
		//todo: move to shadow manager --> Shadow Data Setup

// ----------------------------------------------------------------------------------------------------------------

		//todo: THIS REFERENCES THE NUMBER OF SPOT LIGHTS, MAKE THIS VALUE DYNAMIC IN THE SHADOW MANAGER
		for (int i = 0; i < m_SpotLights.size(); i++)
		{
			m_DepthFrameBuffers.emplace_back(Exalted::FrameBuffer::Create(4096, 4096, true));
		}
		m_ObjectDepthShader = Exalted::Shader::Create(DIRECTIONAL_SHADOW_SHADER_VERTEX_DEPTH, DIRECTIONAL_SHADOW_SHADER_FRAGMENT_DEPTH); //todo: render models into this
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformBlockIndex("Light_Space_Uniforms", 3);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformBlockIndex("Directional_Light_Space_Uniforms", 4);
	
		m_SunlightDepthFrameBuffers.emplace_back(Exalted::FrameBuffer::Create(4096, 4096, true));

		for(int i = 0; i < m_PointLights.size(); i++)
		{
			EX_CORE_CRITICAL("INITIALIZE SHADERS AND MATRICES FOR THE POINT LIGHT SHADOWS!");
			m_PointlightDepthFrameBuffers.emplace_back(Exalted::FrameBuffer::Create(4096, 4096, true));
		}
		//m_PointLightDepthShader = Exalted::Shader::Create(, );
		//std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_PointLightDepthShader)->SetUniformBlockIndex("Directional_Light_Space_Uniforms", 5);

		//todo: render debug quad to this
		m_QuadDepthShader = Exalted::Shader::Create(SHADOW_QUAD_TEST_SHADER_VERTEX, SHADOW_QUAD_TEST_SHADER_FRAGMENT);
		m_QuadMesh = Exalted::Mesh::Create();
		m_QuadMesh->SetVertexArray(Exalted::ShapeGenerator::GenerateIndexedQuad());

// ----------------------------------------------------------------------------------------------------------------

		/////////////////////////////////////////////
		//// Camera Tracks data /////////////////////
		/////////////////////////////////////////////
		Exalted::Ref<Exalted::CameraTrack> mainTrack = Exalted::CreateRef<Exalted::CameraTrack>(1);
		// start position 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-182,30, -182), 45, -15, 0.f)); // 1st corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-182,30, -172), 35, -15, 4.f)); // move at 1st corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-182,30, 182), -45, -15, 2.f)); // 2nd corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-172,30, 182), -55, -15, 4.f)); // move at 2nd corner 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(182, 30, 182), -135, -15, 2.f)); // 3rd corner 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(182, 30, 172), -145, -15, 4.f)); // move at 3rd corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(190, 30, -190), -225, -15, 2.f)); // 4th corner 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(190, 30, -195), -235, -15, 4.f)); // move at 4th corner

		// move to middle 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(0, 175, -200), -275, -60, 2.f));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(0, 575, 0), -275, -90, 3.f)); // rise up

		// in the air
		// pan around environment mapped spaceship 
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-65, 115, 10), -405, -20, 3.f)); // move towards environment mapped ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(50, 115, 85), -475, -20, 3.f, 3)); // move around mapped ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(130, 115, -85), -555, -20, 3.f, 2)); // move around mapped ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(0, 115, -185), -630, -20, 3.f)); // move around mapped ship

		//// blurringly fast move towards panning tree growth
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-182, 60, -100), -720, -35, 1.f, 4)); //post processed blur  || drop to tree pan

		// pan tree growth
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-145, 60, -182), -650, -35, 2.f, 1));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-34, 60, -113), -550, -40, 2.f, 1));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-70, 55, -40), -480, -40, 2.f, 1));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-120, 15, -65), -415, -20, 4.f, 1)); // lower view of tree growth 

		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-130, 15, -100), -350, -20, 4.f, 0));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-85, 15, -140), -250, -20, 4.f, 0));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-117, 10, -130), -300, -15, 4.f, 0));		
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-90, 11, -127.4), -250, -15, 4.f, 0));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-74, 11, -100), -180, -20, 4.f, 0));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-90, 12, -75), -105, -20, 4.f, 0));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-112, 14, -75), -60, -25, 4.f, 5));
		
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-125.5, 15, -95), -5, -30, 4.f, 5));
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-102, 23, -99), -16, -89, 2.f, 5));
		// move towards the ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-100.6, 30.43, -100.57), 0, -7, 1.f, 5)); // looking at left corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-100.6, 30.43, -100.57), 30.5, 21, 4.f, 5)); // looking at center ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-100.6, 30.43, -100.57), 94.3, -8.7, 4.f, 5)); // looking at right corner
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-100.6, 30.43, -100.57), 30.5, 21, 4.f, 5)); // looking at center ship
		mainTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-100.6, 30.43, -100.57), 30.5, 21, 0.1f, 0)); // final change, remove post processing

		Exalted::Ref<Exalted::CameraTrack> secondTrack = Exalted::CreateRef<Exalted::CameraTrack>(2);
		secondTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-131, 34.6, -105.3), 7.6, -35, 0.f));
		secondTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(-127, 34.6, -88.3), -23, -35, 2.f));
		secondTrack->AddTrackPoint(Exalted::CameraTrackPoint(glm::vec3(0, 0, 0), 0, 0, 10.f)); // dummy

		// add tracks to track data
		m_EditorCamera->AddTrack(mainTrack);
		m_EditorCamera->AddTrack(secondTrack);

		// preapre track for startup
		m_EditorCamera->ResetMovementVariables();
		m_EditorCamera->SetTrack(0);

		/////////////////////////////////////
		//// DEBUG DATA /////////////////////
		/////////////////////////////////////
		m_LightSourceMesh = Exalted::Mesh::Create();
		m_LightSourceMesh->SetVertexArray(Exalted::ObjLoader::Load(UFO_MESH));//Exalted::ShapeGenerator::GenerateCubeIT(1.f));

		m_SpotLightMesh = Exalted::Mesh::Create();
		m_SpotLightMesh->SetVertexArray(Exalted::ObjLoader::Load(SPOT_LIGHTHOUSE_MESH));

		m_LightSourceShader = Exalted::Shader::Create(LIGHT_SOURCE_SHADER_VERTEX, LIGHT_SOURCE_SHADER_FRAGMENT);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_LightSourceShader)->SetUniformBlockIndex("Camera_Uniforms", 2);

		/////////////////////////////////////////////////
		////// Post Processing //////////////////////////
		/////////////////////////////////////////////////
		// using the m_QuadMesh model
		m_PostProcessingFrameBuffer = Exalted::FrameBuffer::Create(static_cast<uint32_t>(Exalted::Application::Get().GetWindow().GetWindowWidth()), static_cast<uint32_t>(Exalted::Application::Get().GetWindow().GetWindowHeight()), Exalted::FrameBufferFormat::RGBA8);
		m_PostProcessingShader = Exalted::Shader::Create("Resources/Environment_Scene/Shaders/PostProcessing/Vertex.glsl", "Resources/Environment_Scene/Shaders/PostProcessing/Fragment.glsl");
		//m_SceneManager->SetPostProcessingFrameBuffer(m_PostProcessingFrameBuffer);
	}
	
	void EnvironmentSceneLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		// 1. Update the camera transformation and buffer objects, either dependent on lerping motion or manual movement
		//m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		if(DEBUG_ColorChange)
		{
			glm::vec3 lightColor;
			lightColor.x = sin(TIME);
			lightColor.y = sin(TIME * 0.5f);
			lightColor.z = sin(TIME * 1.5f);

			m_SpotLights[0]->Diffuse = lightColor;
			m_SpotLights[0]->Ambient = lightColor * glm::vec3(0.5);
			m_SpotLights[0]->Specular = lightColor;
		}
		
		/////////////////////////////////////////////////////////////////////////////
		//// LIGHT UNIFORM DATA /////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		//todo: remove this as implemented in light manager 
		// // // m_LightUniformBuffer->Bind();
		// // // Exalted::Bytes lightBufferOffset = 0;
		// // // for (int i = 0; i < m_PointLights.size(); ++i)
		// // // {
		// // // 	m_PointLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		// // // 	m_PointLightTransforms[i]->Position = m_PointLights[i]->Position; 
		// // // }
		// // // m_DirectionalLight->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		// // // for (int i = 0; i < m_SpotLights.size(); ++i)
		// // // {
		// // // 	m_SpotLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
		// // // 	m_SpotLightTransforms[i]->Position = m_SpotLights[i]->Position;
		// // // }
		// // // m_LightUniformBuffer->Unbind();

		/////////////////////////////////////////////////////////////////////////////
		//// Sort the scene objects for rendering ///////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		m_SceneManager->RenderScene(); // sorts scene

		///////////////////////////////////////////////////////////////////////////////////////
		////todo: If static, don't loop
		////todo: move from here -> Directional Light projection calculation
		/////////////////////////////////////////////////////////////////////////////////////
		//todo: move this to light manager
		// // // float near_plane = DEBUG_NEAR;
		// // // float far_plane = DEBUG_FAR;
		// // // glm::mat4 lightProjection = glm::perspective<float>(glm::radians(DEBUG_FOV), 1.0f, near_plane, far_plane);

		// // // m_LightSpaceDataUniformBuffer->Bind();
		// // // Exalted::Bytes offset = 0;
		// // // Exalted::Bytes sizeOfMat4 = sizeof(glm::mat4);
		// // // for (int i = 0; i < m_SpotLights.size(); i++)
		// // // {
		// // // 	glm::mat4 lightView = glm::lookAt(m_SpotLights[i]->Position, m_SpotLights[i]->Position + m_SpotLights[i]->Direction, glm::vec3(0, 1, 0)); //glm::normalize(
		// // // 	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		// // // 	m_LightSpaceDataUniformBuffer->SetBufferSubData(offset, sizeOfMat4, glm::value_ptr(lightSpaceMatrix));
		// // // 	offset += sizeof(glm::mat4);
		// // // }
		// // // m_LightSpaceDataUniformBuffer->Unbind();

		/////////////////////////////////////////////////////////////////
		//// Initial render to depth map for shadow mapping ///////////// 
		/////////////////////////////////////////////////////////////////
		
		Exalted::OpenGLConfigurations::EnableFaceCulling();
		//Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::FRONT);
		m_ObjectDepthShader->Bind();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		for (int i = 0; i < m_DepthFrameBuffers.size(); i++)
		{
			m_DepthFrameBuffers[i]->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_ObjectDepthShader)->SetUniformInt1("u_ShadowIndex", i);
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformBool1("IsDirectional", false);
			Exalted::RenderCommand::ClearColorDepthBuffers();
			m_SceneManager->DrawOpaqueBindless(m_ObjectDepthShader);
			m_SceneManager->DrawTransparentBindless(m_ObjectDepthShader);
			m_DepthFrameBuffers[i]->UnbindMiniFrame();
		}
		for (int i = 0; i < m_SunlightDepthFrameBuffers.size(); i++)
		{
			m_SunlightDepthFrameBuffers[i]->Bind();
			std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformBool1("IsDirectional", true);
			Exalted::RenderCommand::ClearColorDepthBuffers();
			m_SceneManager->DrawOpaqueBindless(m_ObjectDepthShader);
			m_SceneManager->DrawTransparentBindless(m_ObjectDepthShader);
			m_SunlightDepthFrameBuffers[i]->UnbindMiniFrame();
		}
		m_ObjectDepthShader->Unbind();
		Exalted::RenderCommand::ClearColorDepthBuffers();

		//Exalted::OpenGLConfigurations::SetFaceCullingMode(Exalted::FaceCullMode::BACK);
		Exalted::OpenGLConfigurations::DisableFaceCulling();		

		/////////////////////////////////////////////////////////////////////////////
		//// Render scene output //////////////////////////////////////////////////// 
		/////////////////////////////////////////////////////////////////////////////

		// setup post processing frame buffer //
		//////////////////////////////////////////////////////////////////////////////
		m_PostProcessingFrameBuffer->Bind();
		Exalted::RenderCommand::ClearColorDepthBuffers();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		//////////////////////////////////////////////////////////////////////////////
		// render the scene 
		//////////////////////////////////////////////////////////////////////////////
		int count = 0;
		for (int i = 0; i < m_DepthFrameBuffers.size(); i++)
		{
			m_DepthFrameBuffers[i]->BindTexture(4 + i);
			count++;
		}
		m_SunlightDepthFrameBuffers[0]->BindTexture(count + 4); //todo; iterate this for all directional lights

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
		m_LightSourceShader->Unbind();

		// render the rest of the scene, skyboxes and transparent objects
		m_SceneManager->RenderSkybox();
		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);
		m_SceneManager->DrawTransparentObjects();
		m_SceneManager->ClearObjectLists();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();

		/////////////////////////////////////////////////
		////// Post Processing //////////////////////////
		/////////////////////////////////////////////////
		// second pass 
		m_PostProcessingFrameBuffer->UnbindMiniFrame();
		Exalted::RenderCommand::ClearColorBuffer();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		m_PostProcessingShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_PostProcessingShader)->SetUniformInt1("u_ScreenTexture", 10);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_PostProcessingShader)->SetUniformInt1("u_PostProcess", Exalted::CameraTrackFlags::PostProcessingChoice);
		m_PostProcessingFrameBuffer->BindTexture(10);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_PostProcessingShader->Unbind();

		/////////////////////////////////////////////////////////////////////////////
		//// Sunlight Light Debugging ///////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		
		m_QuadDepthShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformBool1("IsDirectional", true);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformInt1("depthMap", 4);
		Exalted::OpenGLConfigurations::SetViewport(1024, 0, 256, 256);
		m_SunlightDepthFrameBuffers[0]->BindTexture(4); 
		Exalted::Renderer::Submit(m_QuadMesh);
		m_SunlightDepthFrameBuffers[0]->Unbind();
		m_QuadDepthShader->Unbind();
		Exalted::OpenGLConfigurations::SetViewport(0, 0, Exalted::Application::Get().GetWindow().GetWindowWidth(), Exalted::Application::Get().GetWindow().GetWindowHeight());

		/////////////////////////////////////////////////////////////////////////////
		//// Spot Light Debugging /////////////////////////////////////////////////// 
		/////////////////////////////////////////////////////////////////////////////
		
		m_QuadDepthShader->Bind();
		float near_plane = 1.f;
		float far_plane = 25.f;
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("near_plane", near_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformFloat1("far_plane", far_plane);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformInt1("depthMap", 4);
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_QuadDepthShader)->SetUniformBool1("IsDirectional", false);


		// spot light 1
		Exalted::OpenGLConfigurations::SetViewport(0, 0, 256, 256);
		m_DepthFrameBuffers[0]->BindTexture(4);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_DepthFrameBuffers[0]->Unbind();

		//spot light 2
		Exalted::OpenGLConfigurations::SetViewport(256, 0, 256, 256);
		m_DepthFrameBuffers[1]->BindTexture(4);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_DepthFrameBuffers[1]->Unbind();

		//spot light 3
		Exalted::OpenGLConfigurations::SetViewport(512, 0, 256, 256);
		m_DepthFrameBuffers[2]->BindTexture(4);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_DepthFrameBuffers[2]->Unbind();
		
		// spot light 4
		Exalted::OpenGLConfigurations::SetViewport(768, 0, 256, 256);
		m_DepthFrameBuffers[3]->BindTexture(4);
		Exalted::Renderer::Submit(m_QuadMesh);
		m_DepthFrameBuffers[3]->Unbind();

		m_QuadDepthShader->Unbind();
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
		ImGui::Text("Switch between different post processing techniques by dragging the bar below:");
		ImGui::Text("0 -> No Effect");
		ImGui::Text("1 -> Color Inversion");
		ImGui::Text("2 -> Weighted Gray Scaling");
		ImGui::Text("3 -> Sharpening");
		ImGui::Text("4 -> Blurring");
		ImGui::Text("5 -> Edge Detection");
		ImGui::DragInt(" ", &Exalted::CameraTrackFlags::PostProcessingChoice, 1, 0, 5);
		ImGui::Text("----------------------------");
		ImGui::Text("---- Directional Light ------");
		ImGui::InputFloat3("D-Direction", glm::value_ptr(m_DirectionalLight->Direction));
		ImGui::InputFloat3("D-Ambient", glm::value_ptr(m_DirectionalLight->Ambient));
		ImGui::InputFloat3("D-Diffuse", glm::value_ptr(m_DirectionalLight->Diffuse));
		ImGui::InputFloat3("D-Specular", glm::value_ptr(m_DirectionalLight->Specular));
		ImGui::Text("----------------------------");

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
		m_SceneManager->OnEvent(event);
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_0)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 0;
			}
			if (e.GetKeyCode() == EX_KEY_1)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 1;
			}
			if (e.GetKeyCode() == EX_KEY_2)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 2;
			}
			if (e.GetKeyCode() == EX_KEY_3)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 3;
			}
			if (e.GetKeyCode() == EX_KEY_4)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 4;
			}
			if (e.GetKeyCode() == EX_KEY_5)
			{
				Exalted::CameraTrackFlags::PostProcessingChoice = 5;
			}
		}
		//if (event.GetEventType() == Exalted::EventType::WindowResize)
		//{
		//	const auto resizeEvent = dynamic_cast<Exalted::WindowResizeEvent&>(event);
		//	const auto windowWidth = resizeEvent.GetWidth();
		//	const auto windowHeight = resizeEvent.GetHeight();
		//	if (windowWidth != 0 && windowHeight != 0)
		//		m_PostProcessingFrameBuffer->Resize(static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight));
		//	event.m_Handled = false;
		//}

	}

	void EnvironmentSceneLayer::OnDetach()
	{
		EX_INFO("Environment Scene Layer detatched successfully.");
	}
}
