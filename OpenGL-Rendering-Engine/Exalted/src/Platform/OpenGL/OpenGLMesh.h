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
	class OpenGLMesh : public Mesh
	{
	public:
		OpenGLMesh() = default;
		virtual ~OpenGLMesh() = default;
		virtual void CreateTriangle() override;
		virtual void CreateQuad() override;
		virtual void CreateCube() override;
		virtual void CreateTexturedQuad(float textureScale) override;
		virtual void CreateTexturedCube(float textureScale) override;
		inline virtual const Ref<VertexArray>& GetVertexArray() const override { return m_VertexArray; }
	};
}