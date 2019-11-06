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

//todo: Maybe make this a GameComponent in the future.

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
		virtual void CreateTexturedCube(float textureScale) = 0;
		virtual void CreateHeightMap(const std::string path) = 0;
		_NODISCARD inline virtual const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
	public:
		static Ref<Mesh> Create();
	protected:
		Ref<VertexArray> m_VertexArray;
	};
}