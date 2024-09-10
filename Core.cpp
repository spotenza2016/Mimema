#include "Core.h"
const std::vector<std::string> Mimema::Core::shaderNames = {"phongShading"};
const std::vector<std::string> Mimema::Core::modelNames = {"oliveotter.obj", "plane.obj", "sphere.obj"};

void Mimema::Core::error_callback(int error, const char* description) {
    std::cout << "Error: " + std::string(description) << std::endl;
}

void Mimema::Core::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Mimema::Core::init(int width, int height, const std::string& windowName) {
    // GLFW/GLEW Setup
    glfwInit();
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window..." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    // Enable Depth Drawing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // VSync On
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    for (int i = 0; i < shaderNames.size(); i++) {
        shaders[shaderNames[i]] = new Shader(shaderNames[i]);
    }

    for (int i = 0; i < modelNames.size(); i++) {
        renderables[modelNames[i]] = new Renderable(modelNames[i]);
    }
}

void Mimema::Core::shutdown() {
    window = nullptr;
    for (auto iter = shaders.begin(); iter != shaders.end(); iter++) {
        delete iter->second;
    }
    shaders.clear();
    for (auto iter = renderables.begin(); iter != renderables.end(); iter++) {
        delete iter->second;
    }
    renderables.clear();
    level = Level();
    glfwTerminate();
}

Mimema::Core::~Core() {
    shutdown();
}

void Mimema::Core::engineLoop() {
    if (window == nullptr) {
        std::cout << "Failed to run engine loop due to not being initialized" << std::endl;
    }

    TracyGpuContext;

    HMI hmi(window);
    RenderState previousState = level.getState();

    float accumulator = 0.0f;
    auto t1 = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto t2 = std::chrono::high_resolution_clock::now();
        float deltaT = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
        t1 = t2;
        accumulator += deltaT;

        // Clamp if necessary
        if (accumulator > maxDeltaT) {
            accumulator = maxDeltaT;
        }

        // Run physics frames
        while (accumulator >= simulationDeltaT) {
            accumulator -= simulationDeltaT;

            previousState = level.getState();

            hmi.moveCameraWithKeyboard(window, level.camera.cameraState, simulationDeltaT);
            hmi.moveCameraWithMouse(window, level.camera.cameraState);
            runPhysics();
        }

        RenderState interpolatedState = RenderState::lerp(previousState, level.getState(), accumulator / simulationDeltaT);

        drawFrame(interpolatedState);

        glfwPollEvents();
        glfwSwapBuffers(window);

        unsigned int error;
        while((error = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGLError: 0x" << std::hex << error << std::endl;
        }

        ZoneScoped;
        FrameMark;
    }
}

void Mimema::Core::runPhysics() {
    Octree octree(level.bbPos, level.bbSize);
    for (auto iter = level.objects.begin(); iter != level.objects.end(); iter++) {
        octree.addObject(&*iter);
    }

    for (auto iter = level.objects.begin(); iter != level.objects.end(); iter++) {
        if (iter->physicsMask == 0) {
            continue;
        }

        iter->passTime(simulationDeltaT);

        glm::vec3 res = octree.checkCollision(&*iter);
        if (res != glm::vec3(0)) {
            iter->collisionOccurred();
            iter->addToTranslate(res);
        }
    }
}

void Mimema::Core::drawFrame(const RenderState& renderState) {
    ZoneScoped;
    TracyGpuZone("drawFrame");

    // Background
    glClearColor(renderState.backgroundColor.x, renderState.backgroundColor.y, renderState.backgroundColor.z, renderState.backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto iter = shaders.begin(); iter != shaders.end(); iter++) {
        iter->second->drawWithShader(renderState);
    }

    glBindVertexArray(0);

    TracyGpuCollect;
}

void Mimema::Core::loadLevel(const std::string& levelName) {
    level = Level(levelName, renderables);
}
