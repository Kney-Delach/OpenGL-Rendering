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
#include "OpenGLShader.h" //todo Remove from here.

namespace Exalted 
{
	void RendererAPI::Init()
	{
		auto& capabilities = GetCapabilities();
		capabilities.Vendor = (const char*) glGetString(GL_VENDOR);
		capabilities.Renderer = (const char*)glGetString(GL_RENDERER);
		capabilities.Version = (const char*) glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_SAMPLES, &capabilities.MaxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &capabilities.MaxAnisotropy);
	}

	void OpenGLRendererAPI::SetViewport(const int xOffset, const int yOffset, const unsigned windowWidth, const unsigned windowHeight)
	{
		glViewport(xOffset, yOffset, windowWidth, windowHeight);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}