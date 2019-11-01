/***************************************************************************
 * Filename		: Mesh.h
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: An abstract interface declaration for a mesh object, currently 
 *                used to auto generate primitives.
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
#include "Core/Renderer/VertexArray.h"

namespace Exalted
{
	class Mesh
	{
	public:
		virtual ~Mesh() = default;
		virtual void CreateTriangle() = 0;
		virtual void CreateQuad() = 0;
		virtual void CreateCube() = 0;
		virtual void CreateTexturedQuad(float textureScale) = 0;
		_NODISCARD virtual const Ref<VertexArray>& GetVertexArray() const = 0;
	public:
		static Mesh* Create();
	};
}
