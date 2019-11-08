/***************************************************************************
 * Filename		: RendererCommand.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declares the rendering for this engine. This needs to be multi-threaded!
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
#include "RendererAPI.h"

//todo: Verify potential memory leak with singleton reference. 

namespace Exalted 
{

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void SetViewport(const int xOffset, const int yOffset, const unsigned windowWidth, const unsigned windowHeight)
		{
			s_RendererAPI->SetViewport(xOffset, yOffset, windowWidth, windowHeight);
		}
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void SetClearStencil(const float stencilValue)
		{
			s_RendererAPI->SetClearStencil(stencilValue);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void ClearColorBuffer()
		{
			s_RendererAPI->ClearColorBuffer();
		}
		inline static void ClearColorDepthBuffers()
		{
			s_RendererAPI->ClearColorDepthBuffers();
		}
		inline static void ClearColorDepthStencilBuffers()
		{
			s_RendererAPI->ClearColorDepthStencilBuffers();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
		inline static void DrawTriangles(const unsigned numberOfVertices)
		{
			s_RendererAPI->DrawTriangles(numberOfVertices);
		}
		inline static void DrawMesh(const Ref<Mesh>& mesh)
		{
			s_RendererAPI->DrawMesh(mesh);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}