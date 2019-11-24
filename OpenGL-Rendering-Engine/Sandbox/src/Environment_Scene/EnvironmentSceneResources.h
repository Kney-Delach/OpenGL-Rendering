#pragma once

#define BASIC_LIGHTING_SHADER_VERTEX	"Resources/Environment_Scene/Shaders/LightingVertex.glsl"
#define BASIC_LIGHTING_SHADER_FRAGMENT	"Resources/Environment_Scene/Shaders/LightingFragment.glsl"

#define LIGHT_SOURCE_SHADER_VERTEX		"Resources/Environment_Scene/Shaders/LightSourceVertex.glsl"
#define LIGHT_SOURCE_SHADER_FRAGMENT	"Resources/Environment_Scene/Shaders/LightSourceFragment.glsl"

#define TEXTURE_DIFFUSE_BRICKS	"Resources/Environment_Scene/Textures/Brick/Bricks01_COL_VAR1_3K.jpg"
#define TEXTURE_SPECULAR_BRICKS	"Resources/Environment_Scene/Textures/Brick/Bricks01_GLOSS_3K.jpg"
#define TEXTURE_NORMAL_BRICKS	"Resources/Environment_Scene/Textures/Brick/Bricks01_NRM_3K.jpg"

#define CUBE_MESH "Resources/Environment_Scene/Meshes/cube.obj"


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
//// TREE ////////////////////////////////////////////////
//////////////////////////////////////////////////////////

#define TREE_MESH "Resources/Environment_Scene/Meshes/Tree.obj"
#define LEAF_MESH "Resources/Environment_Scene/Meshes/Tree_Leaves.obj"
#define TEXTURE_DIFFUSE_LEAF "Resources/Environment_Scene/Textures/Tree/Leaf3.png"

//////////////////////////////////////////////////////////
//// HUT ////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///
///
#define HUT_MESH_MAIN "Resources/Environment_Scene/Meshes/HutMain.obj" // main
#define HUT_MESH_ROOF "Resources/Environment_Scene/Meshes/HutRoof.obj" // roof
#define HUT_MESH_FLOOR "Resources/Environment_Scene/Meshes/HutFloor.obj" // floor
#define HUT_MESH_BASE "Resources/Environment_Scene/Meshes/HutBase.obj" // base
#define HUT_MESH_BOTTOM "Resources/Environment_Scene/Meshes/HutBottom.obj" // bottom (USE THIS FOR BASE OF SCULPTURE?)


//////////////////////////////////////////////////////////
//// Sculpture ///////////////////////////////////////////
//////////////////////////////////////////////////////////

#define SCULPTURE_MESH "Resources/Environment_Scene/Meshes/Bird.obj"


//////////////////////////////////////////////////////////
//// MISC ////////////////////////////////////////////////
//////////////////////////////////////////////////////////
#define ROCKS_MESH "Resources/Environment_Scene/Meshes/Rocks.obj"

//////////////////////////////////////////////////////////
//// Debugging Utilities 
//////////////////////////////////////////////////////////

#define DEBUG_TEXTURE_GRID_D "Resources/Environment_Scene/Debug/DebugTextureDiffuse.png"
#define DEBUG_TEXTURE_GRID_S "Resources/Environment_Scene/Debug/DebugTextureSpecular.jpg"

//////////////////////////////////////////////////////////
//// SCENE 1 
//////////////////////////////////////////////////////////

// Shader that can take its color from multiple lights using blinn phong with energy conservation
#define MULTIPLE_LIGHTS_VERTEX "Resources/Environment_Scene/Shaders/MultipleLights/MultipleLightsVertex.glsl"
#define MULTIPLE_LIGHTS_FRAGMENT "Resources/Environment_Scene/Shaders/MultipleLights/MultipleLightsFragment.glsl"


// Mesh's and textures for the main island
#define ISLAND_MAIN_MESH "Resources/Environment_Scene/Meshes/IslandMain.obj"

#define TEXTURE_DIFFUSE_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Diffuse.png"
#define TEXTURE_SPECULAR_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Spec.png"
#define TEXTURE_NORMAL_ISLAND "Resources/Environment_Scene/Textures/Island/Island_Normal.png"

// spot light mesh
#define SPOT_LIGHTHOUSE_MESH "Resources/Environment_Scene/Meshes/Lighthouse.obj"

// UFO mesh and textures 

#define UFO_MESH "Resources/Environment_Scene/UFO/UFO.obj"
#define TEXTURE_DIFFUSE_UFO "Resources/Environment_Scene/UFO/Ufo_Diffuse.png"
#define TEXTURE_EMISSION_UFO "Resources/Environment_Scene/UFO/Ufo_Emission.png"
#define TEXTURE_SPECULAR_UFO "Resources/Environment_Scene/UFO/Ufo_Specular.png"
#define TEXTURE_NORMAL_UFO "Resources/Environment_Scene/UFO/Ufo_Normal.png"

// skybox mapping stuff

#define SKYBOX_MAP_VERTEX "Resources/Environment_Scene/Reflected/SkyboxReflectedVertex.glsl"
#define SKYBOX_MAP_FRAGMENT "Resources/Environment_Scene/Reflected/SkyboxReflectedFragment.glsl"

/////////////////////////////////////////////////////////////
//// SCENE 2
//////////////////////////////////////////////////////////