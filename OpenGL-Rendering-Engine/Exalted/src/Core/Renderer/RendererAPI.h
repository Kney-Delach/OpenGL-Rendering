/***************************************************************************
 * Filename		: RendererAPI.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declares the rendering api interface to be implemented per platform.
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
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>

namespace Exalted 
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawMesh(const Ref<Mesh>& mesh) = 0;
		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}