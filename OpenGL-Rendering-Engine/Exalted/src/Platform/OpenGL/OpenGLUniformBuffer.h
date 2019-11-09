/***************************************************************************
 * Filename		: UniformBuffer.cpp
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Uniform buffer implementation using OpenGL.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
 ***************************************************************************/
#include "Core/Renderer/UniformBuffer.h"

#pragma once
//todo: Implement this wrapper.
namespace Exalted
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer();
		virtual ~OpenGLUniformBuffer();
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID = 0;
	};
}