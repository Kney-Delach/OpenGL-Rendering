/***************************************************************************
 * Filename		: LayerStack.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation of a layer vector wrapper, contains all active application
 *                which should be processed every frame.
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
#include "LayerStack.h"

namespace Exalted
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex++, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto layerIndex = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (layerIndex != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(layerIndex);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto overlayIndex = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (overlayIndex != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(overlayIndex);
		}
	}
}