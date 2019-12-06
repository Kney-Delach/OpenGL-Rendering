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
#include "5-Depth_Testing/DepthTestingLayer.h"
#include "6-Stencil_Testing/StencilTestingLayer.h"
#include "7-Blending/BlendingLayer.h"
#include "8-Scene_Graphs/SceneGraphLayer.h"
#include "9-Scene_Class/SceneClassLayer.h"
#include "10-Height_Map/HeightMapLayer.h"
#include "11-Model_Loading/ModelLoadingLayer.h"
#include "12-Frame_Buffers/FrameBufferLayer.h"
#include "13-Texture_Cubes_Skybox/SkyboxLayer.h"
#include "14-GLSL_Exploration/GLSLExplorationLayer.h"

// never run these layers simultaneously
#include "15-Instancing/InstancingLayer.h"
#include "16-Tesselation/TessellationLayer.h"
#include "17-Lighting/LightingLayer.h"
#include "18-Shadows/ShadowLayer.h"
#include "19-Multiple_Shadows/MultipleShadowsLayer.h"
#include "20-Bump_Mapping/BumpMapping.h"

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
			PushLayer(new DepthTestingLayer());
			PushLayer(new StencilTestingLayer());
			PushLayer(new BlendingLayer());
			PushLayer(new SceneClassLayer());
			PushLayer(new HeightMapLayer());
			PushLayer(new ModelLoadingLayer());
			PushLayer(new FrameBufferLayer());
			PushLayer(new SkyboxLayer());
			PushLayer(new GLSLExplorationLayer);
			
			//todo: Note, don't run the following layers simultaneously, this is directly due to the usage of uniform buffers.
			PushLayer(new InstancingLayer());
			//PushLayer(new TessellationLayer()); 
			//PushLayer(new LightingLayer());
			//PushLayer(new ShadowLayer());
			//PushLayer(new MultipleShadowsLayer());
			//PushLayer(new BumpMappingLayer());
		}
		virtual ~OpenGLRenderingApplication()
		{
			EX_INFO("OpenGL Rendering Application Destroyed");
		}
	};
}


Exalted::Application* Exalted::CreateApplication()
{
	return new Sandbox::OpenGLRenderingApplication();
}