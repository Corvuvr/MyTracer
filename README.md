# MyTracer
MyTracer is my university project about application of volumetric visualization algorithms with ray tracing without hardware support (Nvidia RT/AMD RA cores).
![MyTracerCover](https://sun9-19.userapi.com/impg/6G4fHayPt6U2mNqSRDQ-2YRrLLxythVdA7iYXA/WYYj_PaBn8U.jpg?size=1119x711&quality=96&sign=c23ad5d303e63832ff9e1c796011c846&type=album)

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
