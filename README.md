# Mimema
Mimema (greek for "to imitate") is a "from scratch" C++ 3D Game Engine using OpenGL. It's an expansion on my old 2D Game Engine (https://github.com/spotenza2016/CPP-Game-Engine) with the intention of developing it into a complete and functional 3D Game Engine from scratch for practice/as a personal challenge. I intend to use this to create a pretty simple 3D puzzle exploration game for fun :D.

## Current Dependencies (end goal is to only use OpenGL wrappers)
- GLFW
- GLEW
- GLM
- STB

## Current Functionality
- 3D Model and Texturing Support (Olive Otter model from Shipwrecked64 for Testing)

![Mimema](https://github.com/user-attachments/assets/cca26a3b-b28b-47bf-82c7-19cc69e5b136)
- Phong Shading with GLSL Shaders
- Multicolored Point Lights (with attenuation) and Direction Lights

![Lighting](https://github.com/user-attachments/assets/249a0b6c-d557-492d-b130-4116b9502a43)
- Camera Movement with HMI Class
- Fixed Timestep Engine Loop with Linear Interpolation between frames based on Accumulator
- Custom Level File System (.MIMEMA_LEVEL files)
- Octree Collision Detection and Gravity Physics

![Basic Collisions](https://github.com/user-attachments/assets/4f13f023-d4ab-424d-9a9e-eaf46003c0d6)
- Separating Axis Theorem for convex colliders

![Collision Boxes](https://github.com/user-attachments/assets/23b6253f-af2e-4845-a995-594c28415fef)

## Future Goals (check out the Issues tab for full list :D)
- Shadows and Reflections
- Scripting Support
- Model Animation
- Audio using OpenAL
- Setup CI with smell checks and style requirements
- Netcode and Proxy Chat (long term goal)
