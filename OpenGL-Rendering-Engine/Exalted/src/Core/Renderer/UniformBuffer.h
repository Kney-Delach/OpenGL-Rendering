/***************************************************************************
 * Filename		: UniformBuffer.cpp
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: A wrapper for uniform buffers for the exalted engine.
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

//todo: Implement this wrapper.
namespace Exalted
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual uint32_t GetRendererID() const = 0;
	public:
		static Ref<UniformBuffer> Create(); 
	};
}