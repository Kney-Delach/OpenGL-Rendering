/***************************************************************************
 * Filename		: Shader.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation of tje shader wrapper for this engine.
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
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Exalted {

	Shader* Shader::Create(const std::string& filepath)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None:    EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}