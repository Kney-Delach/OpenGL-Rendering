/***************************************************************************
 * Filename		: TriangleRenderer.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the code for rendering a simple triangle to the screen.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "Exalted.h"
#include "imgui/imgui.h"

class TriangleLayer : public Exalted::Layer
{
public:
	TriangleLayer()
		: Layer("Triangle Layer"), m_SceneCamera(-1.f, 1.f, -1.f, 1.f)
	{
		// ----------------- Triangle Mesh ----------------- // 

		m_VertexArray.reset(Exalted::VertexArray::Create());
		float colouredVertices[3 * 7] =
		{
			 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Exalted::Ref<Exalted::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Exalted::VertexBuffer::Create(colouredVertices, sizeof(colouredVertices)));
		Exalted::BufferLayout layout =
		{
			{Exalted::ShaderDataType::Float3, "a_Position" },
			{Exalted::ShaderDataType::Float4, "a_Colour" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		Exalted::Ref<Exalted::IndexBuffer> indexBuffer;
		indexBuffer.reset(Exalted::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// ------------------------ Shader setup ------------------------ // 

		m_Shader.reset(Exalted::Shader::Create("Resources/Shaders/Tutorial-1/BasicShader.glsl"));
	}

	virtual void OnUpdate(Exalted::Timestep deltaTime) override
	{
		Exalted::RenderCommand::SetClearColor({ .1f, 0.1f, 0.6f, 1 });
		Exalted::RenderCommand::Clear();

		Exalted::Renderer::BeginScene(m_SceneCamera);
		Exalted::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
		Exalted::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	virtual void OnEvent(Exalted::Event& event) override
	{
	}

	virtual void OnAttach() override
	{
		EX_INFO("Tutorial 1 - Layer Attached");
	}

	virtual void OnDetach() override
	{
		EX_INFO("Tutorial 1 - Layer Detached");
	}

private:
	Exalted::Ref<Exalted::VertexArray> m_VertexArray;
	Exalted::Ref<Exalted::Shader> m_Shader;
	Exalted::OrthographicCamera m_SceneCamera;
};
class TriangleRenderer : public Exalted::Application
{
public:
	TriangleRenderer()
	{
		PushLayer(new TriangleLayer());
	}
	virtual ~TriangleRenderer() = default;
};

Exalted::Application* Exalted::CreateApplication()
{
	return new TriangleRenderer();
}