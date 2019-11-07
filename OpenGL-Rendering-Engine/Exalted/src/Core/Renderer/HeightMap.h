/***************************************************************************
 * Filename		: HeightMap.h
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
#include "Mesh.h"

namespace Exalted
{
	class HeightMap : public Mesh
	{
	public:
		virtual ~HeightMap() = default;
		void CreateTriangle() override {}
		void CreateQuad() override {}
		void CreateCube() override {}
		void CreateTexturedQuad(float textureScale) override {}
		void CreateTexturedCube(float textureScale) override {}
		void CreateHeightMap(const std::string path) override {}
	public:
		static Ref<HeightMap> Create(const std::string path);
	};
}