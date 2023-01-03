workspace "ArkanoidClone"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ArkanoidClone"
		location "ArkanoidClone"
		kind "SharedLib"
		language "C++"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
		includedirs
		{
			"Dependencies/Framework/Include"
		}
		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"
			
			defines
			{
				"_WINDOWS",
				"NDEBUG", 
				"_CONSOLE"
			}



		filter "configurations:Debug"
			defines "ArkanoidClone_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "ArkanoidClone_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "ArkanoidClone_DIST"
			optimize "On"