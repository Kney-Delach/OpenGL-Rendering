/***************************************************************************
 * Filename		: OpenGLRenderer.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation of OpenGL specific drawing process calls wrapper.
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
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace Exalted 
{
	void OpenGLRendererAPI::Init()
	{
		auto& capabilities = GetCapabilities();
		capabilities.Vendor = (const char*) glGetString(GL_VENDOR);
		capabilities.Renderer = (const char*)glGetString(GL_RENDERER);
		capabilities.Version = (const char*) glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_SAMPLES, &capabilities.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &capabilities.MaxAnisotropy);
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &capabilities.MaxVertexUniformComponents);
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &capabilities.MaxUniformBufferComponents);
	}

	void OpenGLRendererAPI::SetViewport(const int xOffset, const int yOffset, const unsigned windowWidth, const unsigned windowHeight)
	{
		glViewport(xOffset, yOffset, windowWidth, windowHeight);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetClearStencil(const float stencilValue)
	{
		glClearStencil(static_cast<int>(stencilValue));
	}

	void OpenGLRendererAPI::Clear() //todo: Implement different clears for different flags.
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //todo: Potentially set the different buffer clearing flags to configurable? i.e, maybe we don't always want it.
	}

	void OpenGLRendererAPI::ClearColorBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::ClearColorDepthBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::ClearColorDepthStencilBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawTriangles(const unsigned numberOfVertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
	}

	void OpenGLRendererAPI::DrawMesh(const Ref<Mesh>& mesh)
	{
		glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawMeshInstanced(const Ref<Mesh>& mesh, unsigned long long& quantity)
	{
		glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, quantity);
	}

	void OpenGLRendererAPI::DrawTrianglesInstanced(const unsigned numberOfVertices, unsigned long long& quantity)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, numberOfVertices, quantity);
	}

	void OpenGLRendererAPI::DrawTessellatedMesh(const Ref<Mesh>& mesh)
	{
		glDrawElements(GL_PATCHES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawTessellatedInstanced(const Ref<Mesh>& mesh, unsigned long long& quantity)
	{
		glDrawElementsInstanced(GL_PATCHES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, quantity);
	}
	void OpenGLRendererAPI::DrawTessellatedTrianglesInstanced(int numberOfVertices, unsigned long long& quantity)
	{
		glDrawArraysInstanced(GL_PATCHES, 0, numberOfVertices, quantity);
	}
}