-- premake5.lua
workspace "MyTracer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "MyTracer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "MyTracer"