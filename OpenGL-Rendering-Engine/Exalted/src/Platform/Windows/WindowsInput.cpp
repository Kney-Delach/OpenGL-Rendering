/***************************************************************************
 * Filename		: WindowsInput.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the input handling implementations for windows.
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
#include "WindowsInput.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Exalted
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return {static_cast<float>(xpos), static_cast<float>(ypos)};
	}

	float WindowsInput::GetMouseXImpl()
	{
		const auto pos = GetMousePositionImpl();

		return pos.first;
	}

	float WindowsInput::GetMouseYImpl()
	{
		const auto pos = GetMousePositionImpl();

		return pos.second;
	}
}
