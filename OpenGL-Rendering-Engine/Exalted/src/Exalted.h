/***************************************************************************
 * Filename		: Exalted.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Include this file in the applications built using this engine. 
 *                NEVER INCLUDE THIS IN THE ENGINE CODEBASE!
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

#include "Core/Application.h"
#include "Core/Log.h"

#include "Core/Core/Timestep.h"

#include "Core/Events/Event.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

#include "Core/Layers/Layer.h"
#include "Core/Layers/ImGui/ImGuiLayer.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/VertexArray.h"

#include "Core/Renderer/Cameras.h"

#include "Core/EntryPoint.h"