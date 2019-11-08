/***************************************************************************
 * Filename		: ObjLoader.h
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: A static class which uses tinyobjloader to load an obj model.
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
#include "Core/Core.h"
#include "Core/Renderer/VertexArray.h"

#include <string>

namespace Exalted 
{
	class ObjLoader 
	{
	public:
		static Ref<VertexArray> Load(const std::string& path);
	};
}