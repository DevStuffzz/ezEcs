-- premake5.lua
workspace "ezEcs"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ezEcs"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ezEcs"
