 /***************************************************************************
 * Filename		: SandboxEntity.cpp
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: The entry point for this OpenGL Rendering Application.
 * Youtube Link : https://www.youtube.com/watch?v=IS9GLXPMrL8&feature=youtu.be
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "Environment_Scene/EnvironmentSceneLayer.h" 
#include "Core/EntryPoint.h"

namespace Sandbox
{
	class OpenGLRenderingApplication : public Exalted::Application
	{
	public:
		OpenGLRenderingApplication()
		{
			PushLayer(new EnvironmentSceneLayer());
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