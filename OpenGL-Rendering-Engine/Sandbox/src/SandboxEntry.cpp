/***************************************************************************
 * Filename		: SandboxEntity.cpp
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: The entry point for this OpenGL Rendering Application. 
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "1-Your_First_Triangle/TriangleRenderer.h"
#include "2-Vertex_Transformations/VertexTransformRenderer.h"
#include "3-Editor_Camera_Showcase/EditorShowcaseLayer.h"
#include "4-Texture_Mapping/TextureMappingLayer.h"

#include "Core/EntryPoint.h"

namespace Sandbox
{
	class OpenGLRenderingApplication : public Exalted::Application
	{
	public:
		OpenGLRenderingApplication()
		{
			PushLayer(new TriangleLayer());
			PushLayer(new VertexTransformLayer());
			PushLayer(new EditorShowcaseLayer());
			PushLayer(new TextureMappingLayer());
		}
		virtual ~OpenGLRenderingApplication() { EX_INFO("OpenGL Rendering Application Destroyed"); }
	};
}


Exalted::Application* Exalted::CreateApplication()
{
	return new Sandbox::OpenGLRenderingApplication();
}