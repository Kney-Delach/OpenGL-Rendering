/***************************************************************************
 * Filename		: Renderer.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declares the rendering interface for this engine. 
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
#pragma once
#include "RenderCommand.h"
#include "Cameras.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

//todo: Alter Submit functionality such that it processes render requests in a multi-threaded manner.

namespace Exalted 
{
	class Renderer
	{
	public:
		static void Init();
		static void BeginScene() {};
		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Mesh>& mesh);
		static void Submit(const Ref<Mesh>& mesh, const unsigned numberOfVertices);
		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const unsigned numberOfVertices, const glm::mat4& transform = glm::mat4(1.0f));

		static void SubmitInstanced(const Ref<Mesh>& mesh, unsigned long long& quantity);
		static void SubmitTrianglesInstanced(const Ref<Mesh>& mesh, const unsigned numberOfVertices, unsigned long long& quantity);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
	private:
		static Scope<SceneData> s_SceneData;
	};
}