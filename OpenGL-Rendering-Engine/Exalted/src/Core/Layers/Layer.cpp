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
	Layer::Layer(const std::string& debugName, bool startupActive)
		: m_DebugName(debugName), m_IsActive(startupActive)
	{
	}
}