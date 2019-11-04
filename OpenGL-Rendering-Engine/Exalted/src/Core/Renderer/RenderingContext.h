/***************************************************************************
 * Filename		: RenderingContext.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Declares the Rendering context abstract interface. 
 *                Inherit from this per rendering platform.
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

namespace Exalted
{
	class RenderingContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void GetInfo() = 0;

		static Scope<RenderingContext> Create(void* window);
	};
}
