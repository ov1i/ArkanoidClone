workspace "ArkanoidClone"
	architecture "x86_64"
	
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.architecture}/%{cfg.buildcfg}"

project "ArkanoidClone"
		location "ArkanoidClone"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" ..outputdir)	
		objdir ("bin-in/" ..outputdir)

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.c",
			"%{prj.name}/src/**.cpp"
		}
		includedirs
		{
			"Dependencies/Framework/Include"
		}

		links {"FrameworkRelease_x64"}
		libdirs "Dependencies/Framework"

		filter "system:windows"
			cppdialect "C++14"
			staticruntime "On"
			systemversion "latest"
			
			defines
			{
				"_WINDOWS"
			}

			filter "configurations:Debug"
				defines "ArkanoidClone_DEBUG"
				runtime "Debug"
				symbols "On"
				buildoptions "/MTd"

			filter "configurations:Release"
				defines "ArkanoidClone_RELEASE"
				runtime "Release"
				optimize "On"
				buildoptions "/MT"

		

			