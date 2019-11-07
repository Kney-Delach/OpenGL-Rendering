/***************************************************************************
 * Filename		: RendererCommand.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implements the rendering command singleton reference 
 *                for this engine. This needs to be multi-threaded!
	 .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "RenderCommand.h"

//todo: Get the rendering api to configure automatically upon system start. 

namespace Exalted 
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}