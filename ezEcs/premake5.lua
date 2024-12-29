project "ezEcs"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { "src/**.cpp", "**.h"}

   includedirs
   {
      "include/",
      "src/"
   }

   links 
   { 
     
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      kind "ConsoleApp"
      defines { "_DEBUG", "EZ_BUILD_DLL" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      kind "ConsoleApp"
      defines { "_RELEASE", "EZ_BUILD_DLL" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "_DIST", "EZ_BUILD_DLL" }
      runtime "Release"
      optimize "On"
      symbols "Off"
