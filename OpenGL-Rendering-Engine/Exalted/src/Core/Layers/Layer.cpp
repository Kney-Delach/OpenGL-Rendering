/***************************************************************************
 * Filename		: Layer.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation of the layer interface, for the layered architecture of this engine.
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
#include "Layer.h"

namespace Exalted 
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}
}