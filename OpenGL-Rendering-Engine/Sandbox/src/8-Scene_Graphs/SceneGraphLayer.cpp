/***************************************************************************
 * Filename		: SceneGraphLayer.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene which utilizes this engines 
                  scene graph storage structure.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "SceneGraphLayer.h"

namespace Sandbox
{
	SceneGraphLayer::SceneGraphLayer()
		: Layer("Scene Graph Layer", false), 
		m_EditorCamera(45.f,
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f, 10000.f)
	{
	}

	void SceneGraphLayer::OnAttach()
	{
		EX_INFO("Scene Graph layer attached successfully.");

		// initialize shaders
		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();

		Exalted::Ref<Exalted::Shader> redHeadShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_RED.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");
		Exalted::Ref<Exalted::Shader> blueBodyShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_BLUE.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");
		Exalted::Ref<Exalted::Shader> greenLeftArmShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_GREEN.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");
		Exalted::Ref<Exalted::Shader> blackRightArmShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_BLACK.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");
		Exalted::Ref<Exalted::Shader> whiteLeftLegShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_WHITE.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");
		Exalted::Ref<Exalted::Shader> yellowRightLegShader = Exalted::Shader::Create("Resources/Shaders/DEFAULT_VERTEX_YELLOW.glsl", "Resources/Shaders/DEFAULT_FRAGMENT_SHADER.glsl");

		// initialize meshes
		m_MeshQuad = Exalted::Mesh::Create();
		m_MeshQuad->CreateTexturedQuad(1.f);

		Exalted::Ref<Exalted::Mesh> cubeMesh = Exalted::Mesh::Create();
		cubeMesh->CreateCube();

		// initialize scene
		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root");
		m_SceneRoot->SetMesh(m_MeshQuad);

		m_pBoxObject = new Exalted::GameObject("Cube");
		m_pBoxObject->SetMesh(cubeMesh);
		m_pBoxObject->GetTransform()->Scale = glm::vec3(0.5f);
		m_pBoxObject->GetTransform()->Position = glm::vec3(5.0f, 2.0f, 2.0f);

		m_SceneRoot->AddChildObject(m_pBoxObject);
		m_SceneRoot->SetShader(m_Shader);

		// create robot gameobject 
		Exalted::GameObject* robotBody = new Exalted::GameObject("Robot Body");
		robotBody->GetTransform()->Scale = glm::vec3(1, 2, 0.5);
		robotBody->GetTransform()->Position = glm::vec3(0, 0, 0);
		robotBody->SetMesh(cubeMesh);
		robotBody->SetShader(blueBodyShader);
		robotBody->SetBoundingRadius(2.f); // set bounding radius the same as its largest scale. 
		
		//todo: IMPLEMENT BOUNDING RADIUS ASSIGNMENT AUTOMATICALLY THROUGH ITS TRANSOFRM.

		Exalted::GameObject* robotHead = new Exalted::GameObject("Robot Head");
		robotHead->GetTransform()->Scale = glm::vec3(.5f,.25f, .5f);
		robotHead->GetTransform()->Position = glm::vec3(0, 1.f, 0);
		robotHead->SetMesh(cubeMesh);
		robotHead->SetShader(redHeadShader);
		robotHead->SetBoundingRadius(.5f); // set bounding radius the same as its largest scale.

		glm::vec3 armScale = glm::vec3(1, 0.2, .2);
		glm::vec3 legScale = glm::vec3(0.2, 1, .2);

		Exalted::GameObject* robotLeftArm = new Exalted::GameObject("Robot Left Arm");
		robotLeftArm->GetTransform()->Scale = armScale;
		robotLeftArm->GetTransform()->Position = glm::vec3(-2, 0, 0);
		robotLeftArm->SetMesh(cubeMesh);
		robotLeftArm->SetShader(greenLeftArmShader);
		robotLeftArm->SetBoundingRadius(1.f); // set bounding radius the same as its largest scale.

		Exalted::GameObject* robotRightArm = new Exalted::GameObject("Robot Right Arm");
		robotRightArm->GetTransform()->Scale = armScale;
		robotRightArm->GetTransform()->Position = glm::vec3(2, 0, 0);
		robotRightArm->SetMesh(cubeMesh);
		robotRightArm->SetShader(blackRightArmShader);
		robotRightArm->SetBoundingRadius(1.f); // set bounding radius the same as its largest scale.

		Exalted::GameObject* robotLeftLeg = new Exalted::GameObject("Robot Left Leg");;
		robotLeftLeg->GetTransform()->Scale = legScale;
		robotLeftLeg->GetTransform()->Position = glm::vec3(-1, -2, 0);
		robotLeftLeg->SetMesh(cubeMesh);
		robotLeftLeg->SetShader(whiteLeftLegShader);
		robotLeftLeg->SetBoundingRadius(1.f); // set bounding radius the same as its largest scale.

		Exalted::GameObject* robotRightLeg = new Exalted::GameObject("Robot Right Leg");
		robotRightLeg->GetTransform()->Scale = legScale;
		robotRightLeg->GetTransform()->Position = glm::vec3(1, -2, 0);
		robotRightLeg->SetMesh(cubeMesh);
		robotRightLeg->SetShader(yellowRightLegShader);
		robotRightLeg->SetBoundingRadius(1.f); // set bounding radius the same as its largest scale.

		robotBody->AddChildObject(robotBody); // This should report a critical error
		robotBody->AddChildObject(robotHead);
		robotBody->AddChildObject(robotHead); // This should report a critical error 
		robotBody->AddChildObject(robotLeftArm);
		robotBody->AddChildObject(robotRightArm);
		robotBody->AddChildObject(robotLeftLeg);
		robotBody->AddChildObject(robotRightLeg);
		m_SceneRoot->AddChildObject(robotBody);

		// ----------------- Floor ----------------- //
		m_FloorTexture = Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_FloorTransforms.reserve(m_FloorTileCount * m_FloorTileCount);
		unsigned rowLength = 0;
		unsigned column = 0;
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
		{
			if (i % m_FloorTileCount == 0)
			{
				rowLength++;
				column = 0;
			}
			glm::mat4 meshTransform = glm::mat4(1.0f);
			meshTransform = glm::translate(meshTransform, glm::vec3(column++ * 1.0f, 0.f, rowLength));
			meshTransform = glm::rotate(meshTransform, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
			m_FloorTransforms.push_back(meshTransform);
		}
	}

	void SceneGraphLayer::OnDetach()
	{
		EX_INFO("Scene Graph Layer detached successfully.");
	}

	void SceneGraphLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera.UpdateCamera(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();
		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::Renderer::BeginScene(m_EditorCamera);

		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_Shader, m_MeshQuad, m_FloorTransforms[i]);

		m_SceneRoot->Update(deltaTime);
		m_SceneRoot->Draw();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		m_FloorTexture->Unbind();

	}

	void SceneGraphLayer::OnImGuiRender()
	{
		m_EditorCamera.OnImGuiRender();

		ImGui::Begin("Scene Graph Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		if (ImGui::Button("Remove Cube From Root"))
			m_SceneRoot->RemoveChildObject(m_pBoxObject);
		ImGui::End();

		m_SceneRoot->RenderHierarchyGUI();
	}

	void SceneGraphLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [8] -> Scene Graph"))
			m_IsActive = true;
		ImGui::End();
	}


	void SceneGraphLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera.OnEvent(event);
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_I)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);

			if (e.GetKeyCode() == EX_KEY_O)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);

			if (e.GetKeyCode() == EX_KEY_P)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
		}
	}
}