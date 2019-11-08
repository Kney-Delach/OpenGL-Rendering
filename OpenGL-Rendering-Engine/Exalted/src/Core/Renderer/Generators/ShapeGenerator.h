/***************************************************************************
 * Filename		: ShapeGenerator.h
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: A static class which generates shapes vertex arrays.
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

namespace Exalted
{
	class ShapeGenerator
	{
	public:
		static Ref<VertexArray> GenerateSphere();
		static Ref<VertexArray> GenerateCube();
		static Ref<VertexArray> GenerateQuad();
		static Ref<VertexArray> GenerateIndexedQuad();
	};
}
