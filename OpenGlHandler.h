#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#include "Level.h"
#include "EngineState.h"
using namespace std;

// todo change this to a setting?
#define VISUALIZE_HITBOXES

// Handler for OpenGL Initialization and Drawing
class OpenGlHandler {
    unsigned int shaderProgram, collisionShader;
    const glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 1);

    // Shader File Constants
    string vertexShaderFileName = "source.vs";
    string fragmentShaderFileName = "source.fs";
    string collisionShaderFileName = "collision.vs";
    string collisionFragmentFileName = "collision.fs";
    char* vertexShaderSource = nullptr;
    char* fragmentShaderSource = nullptr;
    char* collisionShaderSource = nullptr;
    char* collisionFragmentSource = nullptr;

    // Uniform Constants
    const char* vertexMatrixUniformName = "matrix";
    const char* normalMatrixUniformName = "normalMatrix";
    const char* modelViewMatrixUniformName = "modelViewMatrix";
    const char* ambientLightIntensityUniformName = "ambientLightIntensity";
    const char* lightIntensityUniformName = "lightIntensity";
    const char* phongExponentUniformName = "phongExponent";
    const char* lightVecUniformName = "lightVec";
    const char* specularColorUniformName = "specularColor";
    unsigned int uniformMatrixID, collisionMatrixID, uniformNormalMatrixID, uniformModelViewMatrixID, uniformAmbientLightIntensityID, uniformLightIntensityID, uniformPhongExponentID, uniformLightVecID, uniformSpecularColorID;

    // Function to read shader files
    static char* readShader(const string& fileName);

    // Callbacks for OpenGL
    void static error_callback(int error, const char* description);
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:
    // Function to Initialize GLFW/GLEW and OpenGL
    static GLFWwindow* init(int width, int height, const string& windowName);

    // Initialize Shaders
    int initShaders();

    // Initialize Uniforms
    void initUniforms();

    // Draw objects
    void drawFrame(GLFWwindow* window, EngineState& engineState, double alpha);

    // Destructor
    ~OpenGlHandler();
};
