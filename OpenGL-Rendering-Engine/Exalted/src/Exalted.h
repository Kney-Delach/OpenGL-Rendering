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

#define TIME static_cast<float>(Exalted::Application::Get().GetWindow().GetTime()) //todo: move this from here

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

#include "Core/Renderer/Lights/Light.h"
#include "Core/Renderer/Lights/Material.h"

#include "Core/Renderer/UniformBuffer.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Mesh.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Cameras.h"
#include "Core/Renderer/EditorCamera.h"
#include "Core/Renderer/Skybox/Skybox.h"

#include "Core/Renderer/Generators/ShapeGenerator.h"

#include "Core/Renderer/Loaders/ObjLoader.h"
#include "Core/Renderer/Loaders/HeightMapLoader.h"

#include "Platform/OpenGL/OpenGLConfigurations.h" //todo: abstract this

#include "Core/SceneGraph/GameObject.h"
#include "Core/SceneGraph/GameTransform.h"
#include "Core/SceneGraph/GameComponent.h"
#include "Core/SceneGraph/Scene.h"

#include "Core/SceneGraph/FrustumCulling/Frustum.h"
#include "Core/SceneGraph/FrustumCulling/FrustumPlane.h"

// camera track flags
#include "Core/Renderer/CameraTrack/CameraTrackFlags.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

// -------------- Object Macros -------------- //

#define TEAPOT "Resources/Meshes/Teapot.obj"
#define SUZANNE "Resources/Meshes/Suzanne2.obj"
#define NANOSUIT "Resources/Meshes/nanosuit.obj"
#define F16 "Resources/Meshes/f16.obj"
#define BBIRD "Resources/Meshes/BastionBird.obj"
#define BUNNY "Resources/Meshes/bunny.obj"
#define SYMMETRA "Resources/Meshes/Symmetra/symmetra.obj"
#define DABROVIC_SPONZA "Resources/Meshes/Dabrovic-Sponza/sponza.obj"
#define CRYTEK_SPONZA "Resources/Meshes/Crytek-Sponza/sponza.obj"
#define HALLWAY "Resources/Meshes/Hallway.obj"

#define TERRAIN "Resources/Meshes/Terrain/Model/Terrain_50000.obj"
#define TERRAIN_COLOR "Resources/Meshes/Terrain/Textures/Color.tif"

// ------------- Skydome macros ---------------- // 

#define SKYDOME "Resources/Defaults/Skydome/Meshes/Dome.obj"
#define ATMOSPHERE_VERTEX "Resources/Defaults/Skydome/Shaders/AtmosphereVertex.glsl"
#define ATMOSPHERE_FRAGMENT "Resources/Defaults/Skydome/Shaders/AtmosphereFragment.glsl"

// -------------- Texture Macros -------------- //

#define SKYBOX_RIGHT	"Resources/Textures/Skyboxes/Crater/Right.tga"
#define SKYBOX_LEFT	"Resources/Textures/Skyboxes/Crater/Left.tga"
#define SKYBOX_TOP		"Resources/Textures/Skyboxes/Crater/Top.tga"
#define SKYBOX_BOTTOM	"Resources/Textures/Skyboxes/Crater/Bottom.tga"
#define SKYBOX_FRONT	"Resources/Textures/Skyboxes/Crater/Front.tga"
#define SKYBOX_BACK	"Resources/Textures/Skyboxes/Crater/Back.tga"
