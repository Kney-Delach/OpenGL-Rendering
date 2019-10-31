/***************************************************************************
 * Filename		: TriangleRenderer.cpp
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: Contains the code for rendering a simple triangle to the screen.
 * Useful Links	: For map buffering: https://learnopengl.com/Advanced-OpenGL/Advanced-Data
 *                For primitive drawing: https://www.khronos.org/opengl/wiki/Primitive
 *                Buffered object drawing: https://www.khronos.org/opengl/wiki/Buffer_Object#Buffer_Object_Usage
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "1-Your_First_Triangle/TriangleRenderer.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Sandbox
{
	TriangleLayer::TriangleLayer()
		: Layer("[1]: Triangle Layer", false), m_SceneCamera(-1.f, 1.f, -1.f, 1.f)
	{
		// ------------------------ Mesh Setup ------------------------ // 

		m_MeshTriangle.reset(Exalted::Mesh::Create());
		m_MeshTriangle->CreateTriangle();
		m_MeshQuad.reset(Exalted::Mesh::Create());
		m_MeshQuad->CreateQuad();

		// ------------------------ Shader setup ------------------------ // 

		m_ShaderFlat.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderFLAT.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderFLAT.glsl"));
		m_ShaderSmooth.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderSMOOTH.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderSMOOTH.glsl"));
		m_ShaderNoPerspective.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/VBasicShaderNP.glsl", "Resources/Shaders/Tutorial-1/FBasicShaderNP.glsl"));

		// ------------------------ Transform setup ------------------------ // 

		m_TriangleTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
		m_QuadTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
	}

	void TriangleLayer::OnUpdate(Exalted::Timestep deltaTime)
	{
		Exalted::OpenGLConfigurations::SetPointSize(m_PointSize);

		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.3f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_SceneCamera);

		if (m_SmoothInterpolation)
		{
			Exalted::Renderer::Submit(m_ShaderSmooth, m_MeshTriangle, m_TriangleTransform);
			Exalted::Renderer::Submit(m_ShaderSmooth, m_MeshQuad, m_QuadTransform);
		}
		else if (m_FlatInterpolation)
		{
			Exalted::Renderer::Submit(m_ShaderFlat, m_MeshTriangle, m_TriangleTransform);
			Exalted::Renderer::Submit(m_ShaderFlat, m_MeshQuad, m_QuadTransform);
		}
		else if (m_NoPerspInterpolation)
		{
			Exalted::Renderer::Submit(m_ShaderNoPerspective, m_MeshTriangle, m_TriangleTransform);
			Exalted::Renderer::Submit(m_ShaderNoPerspective, m_MeshQuad, m_QuadTransform);
		}

		// ------------------------ glMapBuffer Segment ------------------------ // 

		if (m_RecolourTriangle)
		{
			m_MeshTriangle->GetVertexArray()->GetVertexBuffers()[0]->Bind();
			void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); // getting the buffer pointer from opengl
			float* currentBufferVertices = static_cast<float*>(ptr);
			for (int i = 0; i < 3; i++)
			{
				float randomFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.f);
				currentBufferVertices[3 + i] = 1.0f * sin(randomFloat);
				randomFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.f);
				currentBufferVertices[11 + i] = 1.0f * sin(randomFloat);
				randomFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.f);
				currentBufferVertices[19 + i] = 1.0f * sin(randomFloat);
			}
			memcpy(ptr, currentBufferVertices, sizeof(currentBufferVertices));
			glUnmapBuffer(GL_ARRAY_BUFFER); // informing opengl that done with pointer
			m_MeshTriangle->GetVertexArray()->GetVertexBuffers()[0]->Unbind();
		}

		Exalted::Renderer::EndScene();
	}

	void TriangleLayer::OnImGuiRender()
	{
		ImGui::Begin("Triangle Render Scene Settings");
		if (ImGui::Button("Disable Scene"))
		{
			m_IsActive = false;
		}
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text(m_DrawMode.c_str());
		ImGui::SliderInt("Point Size", &m_PointSize, 1, 240);
		if (ImGui::Button("Color Interpolation: SMOOTH"))
		{
			m_SmoothInterpolation = true;
			m_FlatInterpolation = false;
			m_NoPerspInterpolation = false;
		}
		if (ImGui::Button("Color Interpolation: FLAT"))
		{
			m_SmoothInterpolation = false;
			m_FlatInterpolation = true;
			m_NoPerspInterpolation = false;
		}
		if (ImGui::Button("Color Interpolation: NO PERSPECTIVE"))
		{
			m_SmoothInterpolation = false;
			m_FlatInterpolation = false;
			m_NoPerspInterpolation = true;
		}
		if (ImGui::Button("Generate Triangle Mesh Color"))
		{
			m_RecolourTriangle = !m_RecolourTriangle;
		}

		ImGui::End();
	}

	void TriangleLayer::OnInactiveImGuiRender()
	{
		ImGui::Begin("Disabled Scenes Settings");
		if (ImGui::Button("Enable Scene [1] -> Triangle Rendering"))
		{
			m_IsActive = true;
		}
		ImGui::End();
	}

	void TriangleLayer::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			/** controls to switch between line, fill and point rendering */
			if (e.GetKeyCode() == EX_KEY_1)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);
				m_DrawMode = "Current Draw Mode: POINT";
			}
			if (e.GetKeyCode() == EX_KEY_2)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);
				m_DrawMode = "Current Draw Mode: LINE";
			}
			if (e.GetKeyCode() == EX_KEY_3)
			{
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
				m_DrawMode = "Current Draw Mode: FILL";
			}
		}
	}

	void TriangleLayer::OnAttach()
	{
		EX_INFO("Tutorial 1 - Layer Attached");
	}

	void TriangleLayer::OnDetach()
	{
		EX_INFO("Tutorial 1 - Layer Detached");
	}
}