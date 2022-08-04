--Workspace Setup
workspace "Pathfinder"
	configurations	{ "Debug", "Release" }
	platforms	{ "x64" }
	startproject	"Pathfinder"
	cppdialect      "C++20"

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
	location "%{wks.location}/intermediate"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/binaries/" ..outputDir.. "/")
	objdir ("%{wks.location}/intermediate/" ..outputDir.. "/")
	
	pchheader "FrameworkPCH.h"
	pchsource "%{prj.name}/source/main.cpp"
	
	includedirs{
		"External/SFML/include",
		"%{prj.name}/source" 
	}

	libdirs{
		"External/SFML/lib"
	}

	files{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp",
		"%{prj.name}/data/font/**.ttf"
	}
	
	filter "configurations:Debug"
		links
		{
			"sfml-system-d.lib",
			"sfml-graphics-d.lib",
			"sfml-window-d.lib"
		}
		
	filter "configurations:Release"
		links
		{
			"sfml-system.lib",
			"sfml-graphics.lib",
			"sfml-window.lib"
		}