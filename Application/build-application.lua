project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"
    
    files {
        "source/**.h",
        "source/**.cpp",
        "source/**.c"
     }
    
    includedirs{
       "source",
        "$(SolutionDir)Dependencies/Glad/include",
        "$(SolutionDir)Dependencies/glm",
        "$(SolutionDir)Submodules/ImGui/",
    }

    sysincludedirs {
        "$(SolutionDir)Dependencies/include"
    }

    syslibdirs {
        "$(SolutionDir)Dependencies/lib"
    }
   
    links {
       "opengl32",
    }
   
    targetdir ("../Binaries/" .. OutputDir .. "/")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/")
   
    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS", "ST_PLATFORM_WINDOWS" }
   
    filter "configurations:Debug"
        defines { "DEBUG", "ST_PLATFORM_WINDOWS" }
        runtime "Debug"
        symbols "On"
   
    filter "configurations:Release"
        defines { "RELEASE", "ST_PLATFORM_WINDOWS" }
        runtime "Release"
        optimize "On"
        symbols "On"