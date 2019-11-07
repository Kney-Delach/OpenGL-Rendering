/***************************************************************************
 * Filename		: Renderer.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implements the rendering interface for this engine.
 *                Contains per-scene parameters, cubemap, camera, lighting calculations, etc....
 *                Processes render api requests as a command queue structure.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "Renderer.h"

//#include "Platform/OpenGL/OpenGLShader.h"

namespace Exalted 
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Mesh>& mesh)
	{
		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawMesh(mesh);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_Model", transform);

		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawMesh(mesh);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const unsigned numberOfVertices, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatFloat4("u_Model", transform);

		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawTriangles(numberOfVertices);
	}
}