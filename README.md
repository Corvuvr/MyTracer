# MyTracer
MyTracer is my university project about application of volumetric visualization algorithms with ray tracing without hardware support (Nvidia RT/AMD RA cores).

Currently supports triangulated meshes in .obj format.

![MyTracerCover](https://sun9-15.userapi.com/impg/BJ-if0QxpoFISeyRfuyIvpV_hr6oGZgTJ7-6Mg/oRm0q5h1YBU.jpg?size=977x534&quality=96&sign=9fa5ef2c4da078855f60473904c1e9de&type=album)

# Walnut

Walnut is a simple application framework built with Dear ImGui and designed to be used with Vulkan - basically this means you can seemlessly blend real-time Vulkan rendering with a great UI library to build desktop applications. The plan is to expand Walnut to include common utilities to make immediate-mode desktop apps and simple Vulkan applications.

Currently supports Windows - with macOS and Linux support planned. Setup scripts support Visual Studio 2022 by default.


_<center>Forest Launcher - an application made with Walnut</center>_

## Requirements
- [Visual Studio 2022](https://visualstudio.com) (not strictly required, however included setup scripts only support this)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) (preferably a recent version)
- OpenCL library - [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases) or [NVIDIA](https://developer.nvidia.com/opencl)

## Getting Started
- [Install vulkan and download OpenCL SDK]
- [Once you've cloned, run `scripts/Setup.bat` to generate Visual Studio 2022 solution/project files]
- [Open MyTracer.sln]
- [IF OPENCL BRANCH: include opencl in MyTracer project settings]
- [Run Releasex64]

## Adding Meshes
- [Create mesh in Blender or something]
- [Apply "triangulate" modifier]
- [Export scene as .obj file]
- [Open WalnutApp.cpp]
- [Go to ExampleLayer() constructor and add .obj file in the same way as current meshes]
- [Comment out unnecessary meshes]
- [Run Releasex64]

### 3rd party libaries
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm) (included for convenience)

### Additional
- Walnut uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))

### To Do
- Auto installer
- UI to upload meshes (currently meshes are hardcoded in ExampleLayer(), someday I will do it)
