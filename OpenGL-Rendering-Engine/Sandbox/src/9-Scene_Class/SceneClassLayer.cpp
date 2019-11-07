/***************************************************************************
 * Filename		: SceneClassLayer.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: This layer contains a scene which utilizes this engines
				  scene storage structure.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "SceneClassLayer.h"

namespace Sandbox
{
	SceneClassLayer::SceneClassLayer()
		: Layer("Scene Class Layer", false)
	{
		m_EditorCamera = Exalted::CreateRef<Exalted::EditorCamera>(45.f, 
			static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowWidth()) / static_cast<float>(Exalted::Application::Get().GetWindow().GetWindowHeight()),
			0.1f,
			10000.f);
	}

	void SceneClassLayer::OnAttach()
	{
		EX_INFO("Scene Class layer attached successfully.");

		// initialize shaders
		m_Shader = Exalted::Shader::Create("Resources/Shaders/VTextured.glsl", "Resources/Shaders/FTextured.glsl");
		m_Shader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_Shader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_Shader->Unbind();

		// ------------------------ 
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

		// initialize scene objects

		m_pBoxObject = new Exalted::GameObject("Cube");
		m_pBoxObject->SetMesh(cubeMesh);
		m_pBoxObject->GetTransform()->Scale = glm::vec3(0.5f);
		m_pBoxObject->GetTransform()->Position = glm::vec3(5.0f, 2.0f, 2.0f);

		m_SceneRoot = Exalted::CreateRef<Exalted::GameObject>("Scene Root1");
		m_SceneRoot->SetMesh(m_MeshQuad);
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
		robotHead->GetTransform()->Scale = glm::vec3(.5f, .25f, .5f);
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

		robotBody->AddChildObject(robotHead);
		robotBody->AddChildObject(robotLeftArm);
		robotBody->AddChildObject(robotRightArm);
		robotBody->AddChildObject(robotLeftLeg);
		robotBody->AddChildObject(robotRightLeg);
		m_SceneRoot->AddChildObject(robotBody);

		// ---------------- Transparent Objects Initialization ---------------- //
		m_StainedWindowTexture = Exalted::Texture2D::Create("Resources/Textures/TexTransparentGlassStained.tga",
			Exalted::TextureFormat::RGBA,
			Exalted::TextureWrap::CLAMP,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			false,
			0);

		m_WindowShader = Exalted::Shader::Create("Resources/Shaders/Blending/WindowVertexShader.glsl", "Resources/Shaders/Blending/WindowFragmentShader.glsl");
		m_WindowShader->Bind();
		std::dynamic_pointer_cast<Exalted::OpenGLShader>(m_WindowShader)->SetUniformInt1("u_DiffuseTexture", 0);
		m_WindowShader->Unbind();

		Exalted::GameObject* transparentWindowA = new Exalted::GameObject("Window [A]");
		transparentWindowA->GetTransform()->Position = glm::vec3(1, 0, 1);
		transparentWindowA->SetShader(m_WindowShader);
		transparentWindowA->SetTexture(m_StainedWindowTexture);
		transparentWindowA->SetMesh(m_MeshQuad);
		transparentWindowA->SetBoundingRadius(1.f);

		Exalted::GameObject* transparentWindowB = new Exalted::GameObject("Window [B]");
		transparentWindowB->GetTransform()->Position = glm::vec3(2, 0, 2);
		transparentWindowB->SetShader(m_WindowShader);
		transparentWindowB->SetTexture(m_StainedWindowTexture);
		transparentWindowB->SetMesh(m_MeshQuad);
		transparentWindowB->SetBoundingRadius(1.f);

		Exalted::GameObject* transparentWindowC = new Exalted::GameObject("Window [C]");
		transparentWindowC->GetTransform()->Position = glm::vec3(-1, 0, 3);
		transparentWindowC->SetShader(m_WindowShader);
		transparentWindowC->SetTexture(m_StainedWindowTexture);
		transparentWindowC->SetMesh(m_MeshQuad);
		transparentWindowC->SetBoundingRadius(1.f);

		Exalted::GameObject* transparentWindowD = new Exalted::GameObject("Window [D]");
		transparentWindowD->GetTransform()->Position = glm::vec3(0, 1., 0);
		transparentWindowD->SetShader(m_WindowShader);
		transparentWindowD->SetTexture(m_StainedWindowTexture);
		transparentWindowD->SetMesh(m_MeshQuad);
		transparentWindowD->SetBoundingRadius(1.f);

		m_SceneRoot->AddChildObject(transparentWindowA);
		m_SceneRoot->AddChildObject(transparentWindowB);
		m_SceneRoot->AddChildObject(transparentWindowC);
		robotBody->AddChildObject(transparentWindowD); //todo: Verify that D was added to the robot body and still sorted successfully.

		// -------------- Scene manager stuff ------------ //
		m_SceneManager = Exalted::CreateRef<Exalted::Scene>(m_EditorCamera);
		m_SceneManager->SetSceneRoot(m_SceneRoot);

		// ----------------- Floor ----------------- //
		m_FloorTexture = Exalted::Texture2D::Create("Resources/Textures/TexGridOrange.png",
			Exalted::TextureFormat::RGB,
			Exalted::TextureWrap::REPEAT,
			Exalted::TextureMagFilter::LINEAR,
			Exalted::TextureMinFilter::LINEAR_LINEAR,
			true,
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
			m_FloorTransforms.push_back(meshTransform);
		}
	}

	void SceneClassLayer::OnDetach()
	{
		EX_INFO("Scene Class Layer detached successfully.");
	}

	void SceneClassLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		m_EditorCamera->UpdateCamera(deltaTime);
		m_SceneManager->UpdateScene(deltaTime);

		Exalted::RenderCommand::SetClearColor({ .05f, 0.2f, 0.5f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::OpenGLConfigurations::EnableDepthTesting();
		Exalted::OpenGLConfigurations::EnableBlending();
		Exalted::OpenGLConfigurations::SetBlendFunction(Exalted::BlendFactors::SOURCE_ALPHA, Exalted::BlendFactors::SOURCE_ALPHA_MINUS);

		Exalted::Renderer::BeginScene(*m_EditorCamera);

		m_FloorTexture->Bind();
		for (unsigned i = 0; i < m_FloorTileCount * m_FloorTileCount; i++)
			Exalted::Renderer::Submit(m_Shader, m_MeshQuad, m_FloorTransforms[i]);
		m_FloorTexture->Unbind();

		m_SceneManager->RenderScene();

		Exalted::Renderer::EndScene();
		Exalted::OpenGLConfigurations::DisableDepthTesting();
		Exalted::OpenGLConfigurations::DisableBlending();
	}

	void SceneClassLayer::OnImGuiRender()
	{
		m_EditorCamera->OnImGuiRender();

		ImGui::Begin("Scene Class Scene Settings");
		if (ImGui::Button("Disable Scene"))
			m_IsActive = false;
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("----------------------------");
		if (ImGui::Button("Remove Cube From Root"))
			m_SceneRoot->RemoveChildObject(m_pBoxObject);
		ImGui::End();

		m_SceneRoot->RenderHierarchyGUI();
	}

	void SceneClassLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [9] -> Scene Class"))
			m_IsActive = true;
		ImGui::End();
	}

	void SceneClassLayer::OnEvent(Exalted::Event& event)
	{
		m_EditorCamera->OnEvent(event);
		m_SceneManager->OnEvent(event);
	}
}