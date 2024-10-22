/***************************************************************************
 * Filename		: OpenGLRenderer.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declaration of OpenGL specific drawing process calls wrapper.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once
#include "Core/Renderer/RendererAPI.h"

namespace Exalted 
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		virtual void SetViewport(const int xOffset, const int yOffset, const unsigned windowWidth, const unsigned windowHeight) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetClearStencil(const float stencilValue) override;

		virtual void Clear() override;
		virtual void ClearColorBuffer() override;
		virtual void ClearDepthBuffer() override;
		virtual void ClearColorDepthBuffers() override;
		virtual void ClearColorDepthStencilBuffers() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawTriangles(const unsigned numberOfVertices) override;
		virtual void DrawMesh(const Ref<Mesh>& mesh) override;

		// ------------------ Instanced rendering ------------------- //
		virtual void DrawMeshInstanced(const Ref<Mesh>& mesh, unsigned long long& quantity) override;
		virtual void DrawTrianglesInstanced(const unsigned numberOfVertices, unsigned long long& quantity) override;

		// ------------------- Tessellated Rendering --------------------- //
		virtual void DrawTessellatedMesh(const Ref<Mesh>& mesh) override;
		virtual void DrawTessellatedInstanced(const Ref<Mesh>& mesh, unsigned long long& quantity) override;
		virtual void DrawTessellatedTrianglesInstanced(int numberOfVertices, unsigned long long& quantity) override;
	};
}