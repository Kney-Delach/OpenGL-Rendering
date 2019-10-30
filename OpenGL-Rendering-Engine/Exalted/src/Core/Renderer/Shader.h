/***************************************************************************
 * Filename		: Shader.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: A declaration of a shader wrapper for this engine.
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
#include <string>

namespace Exalted 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static Shader* Create(const std::string& filepath);
	};
}