#include "OpenGlHandler.h"

char* OpenGlHandler::readShader(const string& fileName) {
    // Open the File
    ifstream shader(fileName);
    if (!shader.is_open()) {
        cout << "File: \'" << fileName << "\' failed to open." << endl;
        return nullptr;
    }

    // Convert to a string
    stringstream stream;
    stream << shader.rdbuf();
    string shaderString = stream.str();
    char* shaderArray = new char[shaderString.size() + 1];
    for (int i = 0; i < shaderString.size(); i++) {
        shaderArray[i] = shaderString.at(i);
    }
    shaderArray[shaderString.size()] = '\0';

    shader.close();

    return shaderArray;
}

void OpenGlHandler::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void OpenGlHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* OpenGlHandler::init(int width, int height, const string& windowName) {
    glfwInit();
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}

int OpenGlHandler::initShaders() {
    // Create Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexShaderSource = readShader(vertexShaderFileName);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    // Create Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderSource = readShader(fragmentShaderFileName);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return -1;
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Enable Depth Drawing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    // VSync causes mouse issues when most time in a frame is spent waiting as mouse is not polled and mouse is not time step sync
    // Fixing this requires frame to be spent fully in polling (either by no Vsync, or by more accurate delta T?)
    // Likely requires own synchronization method and would require a LOT of effort, eventual TODO
    glfwSwapInterval(0);

    return 0;
}

void OpenGlHandler::initUniforms() {
    // Create a Uniform Matrix
    uniformMatrixID = glGetUniformLocation(shaderProgram, vertexMatrixUniformName);

    // Create a normal matrix uniform
    uniformNormalMatrixID = glGetUniformLocation(shaderProgram, normalMatrixUniformName);

    // Create a model view matrix uniform
    uniformModelViewMatrixID = glGetUniformLocation(shaderProgram, modelViewMatrixUniformName);

    // Add the ambient light intensity uniform
    uniformAmbientLightIntensityID = glGetUniformLocation(shaderProgram, ambientLightIntensityUniformName);

    // Add the light intensity uniform
    uniformLightIntensityID = glGetUniformLocation(shaderProgram, lightIntensityUniformName);

    // Add the phong exponent uniform
    uniformPhongExponentID = glGetUniformLocation(shaderProgram, phongExponentUniformName);

    // Add the light vec uniform
    uniformLightVecID = glGetUniformLocation(shaderProgram, lightVecUniformName);

    // Add the specular color uniform
    uniformSpecularColorID = glGetUniformLocation(shaderProgram, specularColorUniformName);
}

// TODO more clean up here
void OpenGlHandler::drawFrame(GLFWwindow* window, EngineState& engineState, double alpha) {
    // Background
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform1f(uniformAmbientLightIntensityID, engineState.level.getLevelState().getAmbientLightIntensity() * alpha + engineState.levelPrevState.getAmbientLightIntensity() * (1 - alpha));
    glUniform1f(uniformLightIntensityID, engineState.level.getLevelState().getLightIntensity() * alpha + engineState.levelPrevState.getLightIntensity() * (1 - alpha));
    glm::vec3 cameraPosition = engineState.camera.getCameraState().getCameraPosition() * (float)alpha + engineState.cameraPrevState.getCameraPosition() * (float)(1 - alpha);
    glm::vec3 cameraTarget = engineState.camera.getCameraState().getCameraTarget() * (float)alpha + engineState.cameraPrevState.getCameraTarget() * (float)(1 - alpha);
    glm::vec3 upVec = engineState.camera.getCameraState().getUpVec() * (float)alpha + engineState.cameraPrevState.getUpVec() * (float)(1 - alpha);
    float fov = engineState.camera.getCameraState().getFOV() * (float)alpha + engineState.cameraPrevState.getFOV() * (float)(1 - alpha);
    float nearClippingPlane = engineState.camera.getCameraState().getNearClippingPlane() * (float)alpha + engineState.cameraPrevState.getNearClippingPlane() * (float)(1 - alpha);
    float farClippingPlane = engineState.camera.getCameraState().getFarClippingPlane() * (float)alpha + engineState.cameraPrevState.getFarClippingPlane() * (float)(1 - alpha);
    float aspectRatio = engineState.camera.getCameraState().getAspectRatio() * (float)alpha + engineState.cameraPrevState.getAspectRatio() * (float)(1 - alpha);
    glm::mat4 projectionMatrix = Camera::CameraState::calculateProjectionMatrix(fov, nearClippingPlane, farClippingPlane, aspectRatio);
    glm::mat4 viewMatrix = Camera::CameraState::calculateViewMatrix(cameraPosition, cameraTarget, upVec);
    glm::vec3 lightVec = viewMatrix * glm::vec4(engineState.level.getLevelState().getLightVec() * (float)alpha + engineState.levelPrevState.getLightVec() * (float)(1 - alpha), 0);
    glUniform3fv(uniformLightVecID, 1, &lightVec[0]);

    // todo will need to be adjusted for add/delete
    for (int i = 0; i < engineState.objects.size(); i++) {
        // Draw Triangles
        if (engineState.objects.at(i)->model == nullptr) {
            continue;
        }
        Model::ModelState& prevState = engineState.objectPrevStates.at(i);
        Model::ModelState& nextState = engineState.objects.at(i)->model->getModelState();
        Model* model = engineState.objects.at(i)->model;
        glm::vec3 translate = nextState.getTranslate() * (float)alpha + prevState.getTranslate() * (float)(1 - alpha);
        float angleX = nextState.getAngleX() * (float)alpha + prevState.getAngleX() * (float)(1 - alpha);
        float angleY = nextState.getAngleY() * (float)alpha + prevState.getAngleY() * (float)(1 - alpha);
        float angleZ = nextState.getAngleZ() * (float)alpha + prevState.getAngleZ() * (float)(1 - alpha);
        glm::vec3 scale = nextState.getScale() * (float)alpha + prevState.getScale() * (float)(1 - alpha);
        glm::mat4 modelViewMatrix = viewMatrix * Model::ModelState::calculateModelMatrix(translate, angleX, angleY, angleZ, scale);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));
        glm::mat4 mvpMatrix = projectionMatrix * modelViewMatrix;
        glUniformMatrix4fv(uniformMatrixID, 1, false, &mvpMatrix[0][0]);
        glUniformMatrix3fv(uniformNormalMatrixID, 1, false, &normalMatrix[0][0]);
        glUniformMatrix4fv(uniformModelViewMatrixID, 1, false, &modelViewMatrix[0][0]);

        for (int j = 0; j < model->getNumGroups(); j++) {
            glUseProgram(shaderProgram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model->getTexture(j));

            glUniform1f(uniformPhongExponentID, model->getPhongExponent());
            glUniform3fv(uniformSpecularColorID, 1, &model->getSpecularColor()[0]);
            glBindVertexArray(model->getVAO(j));
            glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices(j));
        }
    }

    glBindVertexArray(0);
    glfwPollEvents();
    glfwSwapBuffers(window);
}

OpenGlHandler::~OpenGlHandler() {
    delete vertexShaderSource;
    delete fragmentShaderSource;
}
