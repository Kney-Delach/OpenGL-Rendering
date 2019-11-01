/***************************************************************************
 * Filename		: OpenGLMesh.h
 * Name			: Ori Lazar
 * Date			: 30/10/2019
 * Description	: A declaration for a mesh object for OpenGL, currently 
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
#include "Core/Renderer/Mesh.h"

namespace Exalted
{
	enum MeshType
	{
		TRIANGLE = 0
	};

	class OpenGLMesh : public Mesh
	{
	public:
		OpenGLMesh() = default;
		virtual ~OpenGLMesh() = default;
		virtual void CreateTriangle() override;
		virtual void CreateQuad() override;
		virtual void CreateCube() override;
		virtual void CreateTexturedQuad() override;
		_NODISCARD inline virtual const Ref<VertexArray>& GetVertexArray() const override { return m_VertexArray; }
	private:
		Ref<VertexArray> m_VertexArray;
	};
}