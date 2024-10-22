/***************************************************************************
 * Filename		: HeightMapLoader.h
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: A static class which loads a heightmap to a vertex array.
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
	class HeightMapLoader
	{
	public:
		static Ref<VertexArray> Load(const std::string& path = "Resources/Defaults/Terrain/terrain.raw");
	};
}