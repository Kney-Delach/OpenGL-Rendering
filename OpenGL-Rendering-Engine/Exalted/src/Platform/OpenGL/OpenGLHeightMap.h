/***************************************************************************
 * Filename		: OpenGLHeightMap.h
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: An abstract interface declaration for a mesh height map
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
#include "Core/Renderer/HeightMap.h"

namespace Exalted
{
	# define RAW_WIDTH 257
	# define RAW_HEIGHT 257
	# define HEIGHTMAP_X 16.0f
	# define HEIGHTMAP_Z 16.0f
	# define HEIGHTMAP_Y 1.25f
	# define HEIGHTMAP_TEX_X 1.0f / 16.0f
	# define HEIGHTMAP_TEX_Z 1.0f / 16.0f
	class OpenGLHeightMap : public HeightMap
	{
	public:
		OpenGLHeightMap(const std::string path);
		virtual ~OpenGLHeightMap() = default;
	};
}