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
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		void DrawTriangles(const unsigned numberOfVertices) override;
		virtual void DrawMesh(const Ref<Mesh>& mesh) override;
		
	};
}