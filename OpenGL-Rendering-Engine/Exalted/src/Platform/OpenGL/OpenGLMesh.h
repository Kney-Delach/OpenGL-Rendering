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
# define RAW_WIDTH 257
# define RAW_HEIGHT 257
# define HEIGHTMAP_X 16.0f
# define HEIGHTMAP_Z 16.0f
# define HEIGHTMAP_Y 1.25f
# define HEIGHTMAP_TEX_X 1.0f / 16.0f
# define HEIGHTMAP_TEX_Z 1.0f / 16.0f

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
		virtual void CreateHeightMap(const std::string path) override;
		inline virtual const Ref<VertexArray>& GetVertexArray() const override { return m_VertexArray; }
	private:
		Ref<VertexArray> m_VertexArray;
	};
}