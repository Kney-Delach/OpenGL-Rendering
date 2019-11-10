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

namespace Exalted
{
	using Bytes  = signed long long int;

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual uint32_t GetRendererID() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetBufferSubData(Bytes& offset, Bytes& size, const void* data) const = 0;
		virtual void BindBufferRange(const uint32_t blockBindingIndex, const Bytes& offset, const Bytes& size) const = 0; //todo: Maybe replace with pass by pointer
	public:
		static Ref<UniformBuffer> Create(Bytes& bufferSize);
	};
}