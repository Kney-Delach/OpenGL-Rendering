/***************************************************************************
 * Filename		: LayerStack.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declaration of a layer vector wrapper, contains all active application 
 *                which are iterated over every frame. 
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
#include "Layer.h"
#include <vector>

//todo: Implement reverse iterator, for propagating events in reverse 

namespace Exalted 
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}