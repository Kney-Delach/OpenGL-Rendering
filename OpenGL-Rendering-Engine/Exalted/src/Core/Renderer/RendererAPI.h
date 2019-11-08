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
	struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;
		
		int MaxSamples;
		float MaxAnisotropy;
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(const int xOffset, const int yOffset, const unsigned windowWidth, const unsigned windowHeight) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetClearStencil(const float stencilValue) = 0;
		virtual void Clear() = 0;
		virtual void ClearColorBuffer() = 0;
		virtual void ClearColorDepthBuffers() = 0;
		virtual void ClearColorDepthStencilBuffers() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawTriangles(const unsigned numberOfVertices) = 0;
		virtual void DrawMesh(const Ref<Mesh>& mesh) = 0;
	public:
		static RenderAPICapabilities& GetCapabilities()
		{
			static RenderAPICapabilities capabilities;
			return capabilities;
		}
		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};
}