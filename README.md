# Mimema
Mimema (greek for "to imitate") is a "from scratch" C++ 3D Game Engine using OpenGL. It's an expansion on my old 2D Game Engine (https://github.com/spotenza2016/CPP-Game-Engine) with the intention of developing it into a complete and functional 3D Game Engine from scratch for practice/as a personal challenge.

## Current Dependencies (end goal is to only use OpenGL wrappers)
- GLFW
- GLEW
- GLM
- STB

## Current Functionality
![Basic Collisions](https://github.com/user-attachments/assets/4f13f023-d4ab-424d-9a9e-eaf46003c0d6)
![Collision Boxes](https://github.com/user-attachments/assets/23b6253f-af2e-4845-a995-594c28415fef)

- 3D Model and Texturing Support (Olive Otter model from Shipwrecked64 for Testing)
- Phong Shading with GLSL Shaders
- Point Light (with attenuation) and 
- Camera Movement with HMI Class
- Fixed Timestep Engine Loop with Linear Interpolation between frames based on Accumulator
- Custom Level File System
- Octree Collision Detection and Gravity Physics
- Separating Axis Theorem for convex colliders

## Future Goals (check out the Issues tab for full list :D)
- Shadows and Reflections
- Scripting Support
- Model Animation
- Audio using OpenAL
- Setup CI with smell checks and style requirements
- Netcode and Proxy Chat (long term goal)
