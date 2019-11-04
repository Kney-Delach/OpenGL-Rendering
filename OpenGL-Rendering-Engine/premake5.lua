-- Project  : Exalted
-- Filename : premake5.lua
-- Author   : Ori Lazar
-- Date     : 29/10/2019

workspace "OpenGL-Rendering-Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Exalted/Dependencies/GLFW/include"
IncludeDir["Glad"] = "Exalted/Dependencies/Glad/include"
IncludeDir["ImGui"] = "Exalted/Dependencies/imgui"
IncludeDir["glm"] = "Exalted/Dependencies/glm"
IncludeDir["Assimp"] = "Exalted/Dependencies/Assimp/include"

group "Dependencies"
	include "Exalted/Dependencies/GLFW"
	include "Exalted/Dependencies/Glad"
	include "Exalted/Dependencies/imgui"
group ""

-- Exalted graphics engine
project "Exalted"
	location "Exalted"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "expch.h"
	pchsource "Exalted/src/expch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Dependencies/glm/glm/**.hpp",
		"%{prj.name}/Dependencies/glm/glm/**.inl",
		"%{prj.name}/Dependencies/Assimp/include/assimp/**.hpp",
		"%{prj.name}/Dependencies/Assimp/include/assimp/**.inl",
		"%{prj.name}/Dependencies/Assimp/include/assimp/**.h"

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/Dependencies/spdlog/include",
		"%{prj.name}/Dependencies/stb/include",
		"Resources/",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Assimp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"OpenGL-Rendering-Engine/Exalted/Dependencies/assimp/lib/assimp-vc142-mtd.lib",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EX_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"EX_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "EX_DEBUGGING"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EX_DIST"
		runtime "Release"
		optimize "on"

-- Sandbox
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Exalted/Dependencies/spdlog/include",
		"Exalted/Dependencies/stb/include",
		"Exalted/src",
		"Resources/",
		"Exalted/Dependencies",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Assimp}"
	}

	links
	{
		"Exalted"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"EX_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "EX_DEBUGGING"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EX_DIST"
		runtime "Release"
		optimize "on"	
