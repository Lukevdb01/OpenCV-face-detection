-- premake5.lua
workspace "GlowImageDetection"
   architecture "x64"
   configurations { "Debug", "Release"}
   startproject "Application"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Application/build-application.lua"