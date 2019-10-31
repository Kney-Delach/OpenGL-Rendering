/***************************************************************************
 * Filename		: EntryPoint.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the implementation of the engine's entry point.
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

#ifdef EX_PLATFORM_WINDOWS

extern Exalted::Application* Exalted::CreateApplication();

int main(int argc, char** argv)
{
	// initialize logging system
	// --------------------------
	Exalted::Log::Init(); 

	// Entry point for applications 
	// --------------------------
	auto app = Exalted::CreateApplication();
	EX_CORE_ASSERT(app, "Client Application shouldn't be null!");
	app->Run();
	delete app;
	EX_CORE_INFO("Application Shutdown Successfully!");
}
#endif