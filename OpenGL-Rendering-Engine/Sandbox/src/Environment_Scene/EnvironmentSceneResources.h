/***************************************************************************
 * Filename		: EnvironmentSceneResources.h
 * Name			: Ori Lazar
 * Date			: 20/11/2019
 * Description	: This file contains some of the resources used for the environment showcase scene 
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

#define BASIC_LIGHTING_SHADER_VERTEX	"Resources/Environment_Scene/Shaders/LightingVertex.glsl"
#define BASIC_LIGHTING_SHADER_FRAGMENT	"Resources/Environment_Scene/Shaders/LightingFragment.glsl"

#define LIGHT_SOURCE_SHADER_VERTEX		"Resources/Environment_Scene/Shaders/LightSourceVertex.glsl"
#define LIGHT_SOURCE_SHADER_FRAGMENT	"Resources/Environment_Scene/Shaders/LightSourceFragment.glsl"

//////////////////////////////////////////////////////////
//// Shadows /////////////////////////////////
//////////////////////////////////////////////////////////
#define DIRECTIONAL_SHADOW_SHADER_VERTEX "Resources/Environment_Scene/Shaders/DirectionalShadow/LightingVertex.glsl"
#define DIRECTIONAL_SHADOW_SHADER_FRAGMENT "Resources/Environment_Scene/Shaders/DirectionalShadow/LightingFragment.glsl"

#define DIRECTIONAL_SHADOW_SHADER_VERTEX_DEPTH "Resources/Environment_Scene/Shaders/DirectionalShadow/ShadowDepthVertex.glsl"
#define DIRECTIONAL_SHADOW_SHADER_FRAGMENT_DEPTH "Resources/Environment_Scene/Shaders/DirectionalShadow/ShadowDepthFragment.glsl"

#define SHADOW_QUAD_TEST_SHADER_VERTEX "Resources/Environment_Scene/Shaders/DirectionalShadow/ShadowDepthQUADVertex.glsl"
#define SHADOW_QUAD_TEST_SHADER_FRAGMENT "Resources/Environment_Scene/Shaders/DirectionalShadow/ShadowDepthQUADFragment.glsl"

//////////////////////////////////////////////////////////
//// TREES and Leaves ////////////////////////////////////
//////////////////////////////////////////////////////////

#define TREE_MESH "Resources/Environment_Scene/Meshes/Tree.obj"
#define LEAF_MESH "Resources/Environment_Scene/Meshes/Tree_Leaves.obj"
#define TEXTURE_DIFFUSE_LEAF "Resources/Environment_Scene/Textures/Tree/Leaf3.png"

//////////////////////////////////////////////////////////
//// Debugging Utilities 
//////////////////////////////////////////////////////////

#define DEBUG_TEXTURE_GRID_D "Resources/Environment_Scene/Debug/DebugTextureDiffuse.png"
#define DEBUG_TEXTURE_GRID_S "Resources/Environment_Scene/Debug/DebugTextureSpecular.jpg"

// Shader that can take its color from multiple lights using blinn phong with energy conservation
#define MULTIPLE_LIGHTS_VERTEX "Resources/Environment_Scene/Shaders/MultipleLights/MultipleLightsVertex.glsl"
#define MULTIPLE_LIGHTS_FRAGMENT "Resources/Environment_Scene/Shaders/MultipleLights/MultipleLightsFragment.glsl"

// Mesh's and textures for the main island
#define ISLAND_MAIN_MESH "Resources/Environment_Scene/Meshes/IslandMain.obj"

#define TEXTURE_DIFFUSE_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Diffuse.png"
#define TEXTURE_SPECULAR_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Spec.png"
#define TEXTURE_NORMAL_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Normal.png"

// UFO mesh and textures 
#define UFO_MESH "Resources/Environment_Scene/UFO/UFO.obj"
#define TEXTURE_DIFFUSE_UFO "Resources/Environment_Scene/UFO/Ufo_Diffuse.png"
#define TEXTURE_EMISSION_UFO "Resources/Environment_Scene/UFO/Ufo_Emission.png"
#define TEXTURE_SPECULAR_UFO "Resources/Environment_Scene/UFO/Ufo_Specular.png"
#define TEXTURE_NORMAL_UFO "Resources/Environment_Scene/UFO/Ufo_Normal.png"

// skybox mapping shader

#define SKYBOX_MAP_VERTEX "Resources/Environment_Scene/Reflected/SkyboxReflectedVertex.glsl"
#define SKYBOX_MAP_FRAGMENT "Resources/Environment_Scene/Reflected/SkyboxReflectedFragment.glsl"

// Explosion shader
#define EXPLOSION_GEOMETRY "Resources/Environment_Scene/Shaders/Explosion/ExplosionGeometry.glsl"

// point light skybox mapped shaders 
#define PL_FRAGMENT "Resources/Environment_Scene/Reflected/PointLightReflectedFragment.glsl"