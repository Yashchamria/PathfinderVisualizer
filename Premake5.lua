--Workspace Setup
workspace "Pathfinder"
	configurations	{ "Debug", "Release" }
	platforms		{ "x32", "x64" }
	startproject	"Pathfinder"
	cppdialect      "C++17"
	location		"build"

filter "system:windows"
	systemversion "latest"

filter "configurations:Debug"
	symbols		"on"
	
filter "configurations:Release"
	optimize	"on"

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
		"Framework/include"
	}

	libdirs{
		"Framework/lib"
	}

	files{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
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

















