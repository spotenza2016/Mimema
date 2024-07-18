# Mimema
Mimema (greek for "to imitate") is a "from scratch" C++ 3D Game Engine using OpenGL. It's an expansion on my old 2D Game Engine (https://github.com/spotenza2016/CPP-Game-Engine) with the intention of developing it into a complete and functional 3D Game Engine from scratch for practice/as a personal challenge.

## Current Dependencies (end goal is to only use OpenGL wrappers)
- GLFW
- GLEW
- GLM
- STB

## Current Functionality
![2024-05-0817-56-47-ezgif com-video-to-gif-converter](https://github.com/spotenza2016/Mimema/assets/64374401/b8cedbda-ad5d-47ad-9827-75f8961b5877)

- 3D Model and Texturing Support (Olive Otter model from Shipwrecked64 for Testing)
- Phong Ambient/Diffuse/Specular Shaders
- 3D Model View and Projection Transformation
- Camera Movement
- Fixed Timestep Engine Loop with Interpolation based on Accumulator
- OOP Classes for Materials, Objects, Levels, etc
- Octree/Basic Collision Detection and Gravity Physics
- Github Integration (planned to make fully open source with issues and PRs/branch separation)

## In Progress
- Standardize Material with MTL File
- Rewriting CollisionBox Code and Physics Code
- OOP Class for Controller (maybe XInput Support also?)

## Future Goals
- Shadows and Reflections
- Object Scripting Support
- Standardize Level/Object Storage
- Audio (OpenAL?)
- Netcode and Proxy Chat
- Setup CI with smell checks and style requirements
