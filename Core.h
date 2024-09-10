#ifndef MIMEMA_CORE_H
#define MIMEMA_CORE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <string>
#include <unordered_map>

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

#include "Shader.h"
#include "Level.h"
#include "Octree.h"
#include "HMI.h"

namespace Mimema {
    class Core {
        std::map<std::string, Renderable*> renderables;

        GLFWwindow* window = nullptr;

        Level level;

        static const std::vector<std::string> shaderNames;
        static const std::vector<std::string> modelNames;
        std::unordered_map<std::string, Shader*> shaders;

        // Callbacks for OpenGL
        void static error_callback(int error, const char* description);
        void static framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:
        float simulationFPS = 144;
        float minimumFPS = 5;
        float simulationDeltaT = 1.0f / simulationFPS;
        float maxDeltaT = std::max(1.0f / minimumFPS, simulationDeltaT);

        Core() = default;

        ~Core();

        static Core& instance() {
            static Core engine;
            return engine;
        }

        void init(int width, int height, const std::string& windowName);

        void shutdown();

        void engineLoop();

        void runPhysics();

        void drawFrame(const RenderState& renderState);

        void loadLevel(const std::string& levelName);

        Core(const Core& rhs) = delete;
        Core(const Core&& rhs) = delete;
        Core& operator=(const Core& rhs) = delete;
        Core& operator=(Core&&) = delete;
    };
}

#endif // MIMEMA_CORE_H