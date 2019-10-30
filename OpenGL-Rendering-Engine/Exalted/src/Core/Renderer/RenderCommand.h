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
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawMesh(const Ref<Mesh>& mesh)
		{
			s_RendererAPI->DrawMesh(mesh);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}