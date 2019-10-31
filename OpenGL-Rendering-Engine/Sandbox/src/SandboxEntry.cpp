#include "1-Your_First_Triangle/TriangleRenderer.h"
#include "2-Vertex_Transformations/VertexTransformRenderer.h"
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
		}
		virtual ~OpenGLRenderingApplication() { EX_INFO("OpenGL Rendering Application Destroyed"); }
	};
}


Exalted::Application* Exalted::CreateApplication()
{
	return new Sandbox::OpenGLRenderingApplication();
}