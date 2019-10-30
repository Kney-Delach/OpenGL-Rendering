/***************************************************************************
 * Filename		: RendererAPI.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Assigns current rendering api to OpenGL.
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
#include "RendererAPI.h"

namespace Exalted 
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
}