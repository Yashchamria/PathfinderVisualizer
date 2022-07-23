--Workspace Setup
workspace "Pathfinder"
	configurations	{ "Debug", "Release" }
	platforms	{ "x64" }
	startproject	"Pathfinder"
	cppdialect      "C++20"
	location	"build"

filter "configurations:Debug"
	defines "PF_DEBUG"
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	defines "PF_RELEASE"
	runtime "Release"
	optimize "on"

filter "system:windows"
	defines "PLATFORM_WINDOWS"
	systemversion "latest"

--Pathfinder Project Setup
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pathfinder"
	location "build/%{prj.name}"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Executables/")
	objdir("build/intermediate/" ..outputDir.."/")
	
	pchheader "FrameworkPCH.h"
	pchsource "%{prj.name}/source/main.cpp"
	
	includedirs{
		"Framework/include",
		"%{prj.name}/source" 
	}

	libdirs{
		"Framework/lib"
	}

	files{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp",
		"%{prj.name}/data/font/**.ttf"
	}
	
	filter "configurations:Debug"
		links{
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib"
		}
		
	filter "configurations:Release"
		links{
			"sfml-system.lib",
			"sfml-graphics.lib",
			"sfml-window.lib"
		}