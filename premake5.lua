-- https://github.com/premake/premake-core/wiki/buildoptions ADD -f/-p flags

workspace "Cathedral"
    configurations { "Debug", "Release" }

project "Cathedral"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    files {"src/cathedral/**.hpp", "src/cathedral/**.cpp"}

    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "ct"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    links {"Cathedral"}

    files {"src/server/**.cpp", "src/server/**.hpp"}

    filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"