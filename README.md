# MyTracer
MyTracer is my university project about application of volumetric visualization algorithms with ray tracing without hardware support (Nvidia RT/AMD RA cores).

![MyTracerCover](https://sun9-14.userapi.com/impg/CspSGHMbvq9si0zMiWSVmgLWRM0nQftbKlp1Gw/yFYknR-8CFI.jpg?size=747x603&quality=96&sign=39031ca97b7b06a351e371bdf5e1a769&type=album)

# Walnut

Walnut is a simple application framework built with Dear ImGui and designed to be used with Vulkan - basically this means you can seemlessly blend real-time Vulkan rendering with a great UI library to build desktop applications. The plan is to expand Walnut to include common utilities to make immediate-mode desktop apps and simple Vulkan applications.

Currently supports Windows - with macOS and Linux support planned. Setup scripts support Visual Studio 2022 by default.

![WalnutExample](https://hazelengine.com/images/ForestLauncherScreenshot.jpg)
_<center>Forest Launcher - an application made with Walnut</center>_

## Requirements
- [Visual Studio 2022](https://visualstudio.com) (not strictly required, however included setup scripts only support this)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows) (preferably a recent version)

## Getting Started
- [Once you've cloned, run `scripts/Setup.bat` to generate Visual Studio 2022 solution/project files]
- [Open MyTracer.sln]
- [IF OPENCL BRANCH: click "Add existing property sheet" and add "OpenCL.props" file]
- [Run Releasex64]

### 3rd party libaries
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm) (included for convenience)

### Additional
- Walnut uses the [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
