#include "EngineCore.h"
void EngineCore::engineLoop(int width, int height, const string& name) {
    OpenGlHandler glHandler;
    GLFWwindow* window = glHandler.init(width, height, name);
    if (window == nullptr) {
        return;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    // TODO organize (should maybe not be static maybe fine if in another class?)
    // Hide cursor for mouse support
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwGetCursorPos(window, &lastCursorPosX, &lastCursorPosY);

    glHandler.initShaders();
    glHandler.initUniforms();

    // Todo next steps here
    EngineState engineState = EngineState();
    engineState.camera.getCameraState().setAspectRatio((float)width / (float)height);

    double accumulator = 0;
    auto t1 = chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto t2 = chrono::high_resolution_clock::now();
        double deltaT = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
        t1 = t2;
        accumulator += deltaT;

        if (accumulator > maxDeltaT) {
            accumulator = maxDeltaT;

            // todo remove
            cout << "Accumulator too high, clamping..." << endl;
        }
        while (accumulator >= simulationDeltaT) {
            accumulator -= simulationDeltaT;

            engineState.saveStates();
            // maybe move outside loop? or just grab input outside loop
            processInput(window, &engineState.camera);
            handlePhysics(engineState);
            engineState.objects.at(0)->model->getModelState().setAngleY(engineState.objects.at(0)->model->getModelState().getAngleY() + 0.03 * simulationDeltaT * 100);
        }
        const double alpha = accumulator / simulationDeltaT;

        glHandler.drawFrame(window, engineState, alpha);
    }
    //cout <<  1.0 / (average / 1000000000) << endl;
    // TODO remember deletes and glfw shut down, put in destructor here or in openglhandler, probably a good idea to check for leaks

    //todo after all todos start working on physics (collisison boxes and stuff)
}

// TODO clean this up and pitch limit
void EngineCore::processInput(GLFWwindow* window, Camera* camera) {
    glm::vec3 movementDirection;

    // todo convert to normalized mouse movement so time per frame can be put here
    // todo avoid these divides w/ constant sensitivity?
    float cursorSens = 0.001;
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    glm::vec2 mouseMovement = glm::vec2((float)(cursorX - lastCursorPosX), (float)(cursorY - lastCursorPosY));
    float yaw = (cursorX - lastCursorPosX) * cursorSens;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        yaw += 0.01 * simulationDeltaT;
    }
    float pitch = mouseMovement.y * cursorSens;
    glm::vec3 direction = glm::normalize(camera->getCameraState().getCameraTarget() - camera->getCameraState().getCameraPosition());
    glm::vec3 up = glm::normalize(camera->getCameraState().getUpVec());
    glm::mat4 rotationMatrix = glm::rotate(yaw, glm::vec3(0, -1, 0))
                               * glm::rotate(pitch, glm::cross(up, direction));
    direction = glm::normalize(rotationMatrix * glm::vec4(direction, 0));
    camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraPosition() + direction);
    up = rotationMatrix * glm::vec4(up, 0);
    camera->getCameraState().setUpVec(up);
    lastCursorPosY = cursorY;
    lastCursorPosX = cursorX;

    float movement = 0.01 * simulationDeltaT * 4000;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // todo check normalizations make sure never zero
        glm::vec3 change = glm::normalize(camera->getCameraState().getCameraTarget() - camera->getCameraState().getCameraPosition());
        change.y = 0;
        change = normalize(change) * movement;
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() + change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() + change);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 change = glm::normalize(camera->getCameraState().getCameraTarget() - camera->getCameraState().getCameraPosition());
        change.y = 0;
        change = normalize(change) * movement;
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() - change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() - change);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 change = glm::cross(glm::normalize(camera->getCameraState().getUpVec()), glm::normalize(camera->getCameraState().getCameraTarget() - camera->getCameraState().getCameraPosition()));
        change.y = 0;
        change = normalize(change) * movement;
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() + change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() + change);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 change = glm::cross(glm::normalize(camera->getCameraState().getUpVec()), glm::normalize(camera->getCameraState().getCameraTarget() - camera->getCameraState().getCameraPosition()));
        change.y = 0;
        change = normalize(change) * movement;
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() - change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() - change);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glm::vec3 change = movement * glm::vec3(0, 1, 0);
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() + change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() + change);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        glm::vec3 change = movement * glm::vec3(0, 1, 0);
        camera->getCameraState().setCameraPosition(camera->getCameraState().getCameraPosition() - change);
        camera->getCameraState().setCameraTarget(camera->getCameraState().getCameraTarget() - change);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        glfwSwapInterval(0);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        glfwSwapInterval(1);
    }

    // todo normalize movement vector to avoid diagnal thing
}

void EngineCore::handlePhysics(EngineState& state) {
    for (int i = 0; i < state.objects.size(); i++) {
        Object* object = state.objects.at(i);
        PhysicsObject* physicsObject = dynamic_cast<PhysicsObject*>(object);

        if (physicsObject) {
            //physicsObject->passTime(simulationDeltaT);
        }

        // TODO put quad tree here
    }
}
