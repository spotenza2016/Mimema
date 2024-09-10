#include "Shader.h"
const std::string Mimema::Shader::shaderFolderLocation = "Shaders/";
const std::string Mimema::Shader::vertexShaderFileExtension = ".vs";
const std::string Mimema::Shader::fragmentShaderFileExtension = ".fs";

/*
GLSL Uniforms:

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 viewPos;
uniform float ambientLightIntensity;
uniform vec3 ambientLightColor;
uniform int directionalLightCount;
uniform float[100] directionalLightIntensities;
uniform vec3[100] directionalLightColors;
uniform vec3[100] directionalLightDirections;
uniform int pointLightCount;
uniform float[100] pointLightIntensities;
uniform vec3[100] pointLightColors;
uniform vec3[100] pointLightPositions;
uniform vec3[100] pointLightAttenuations;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform float dissolve;
uniform float specularExponent;
uniform vec3 specularColor;
uniform int illuminationModel;
uniform sampler2D ambientColorTexture;
uniform sampler2D diffuseColorTexture;
uniform sampler2D specularColorTexture;
uniform sampler2D specularExponentTexture;
uniform sampler2D dissolveTexture;
*/

const char* Mimema::Shader::mvpMatrixUniform = "mvpMatrix";
const char* Mimema::Shader::modelMatrixUniform = "modelMatrix";
const char* Mimema::Shader::normalMatrixUniform = "normalMatrix";
const char* Mimema::Shader::viewPosUniform = "viewPos";
const char* Mimema::Shader::ambientLightIntensityUniform = "ambientLightIntensity";
const char* Mimema::Shader::ambientLightColorUniform = "ambientLightColor";
const char* Mimema::Shader::directionalLightCountUniform = "directionalLightCount";
const char* Mimema::Shader::directionalLightIntensitiesUniform = "directionalLightIntensities";
const char* Mimema::Shader::directionalLightColorsUniform = "directionalLightColors";
const char* Mimema::Shader::directionalLightDirectionsUniform = "directionalLightDirections";
const char* Mimema::Shader::pointLightCountUniform = "pointLightCount";
const char* Mimema::Shader::pointLightIntensitiesUniform = "pointLightIntensities";
const char* Mimema::Shader::pointLightColorsUniform = "pointLightColors";
const char* Mimema::Shader::pointLightPositionsUniform = "pointLightPositions";
const char* Mimema::Shader::pointLightAttenuationsUniform = "pointLightAttenuations";
const char* Mimema::Shader::ambientColorUniform = "ambientColor";
const char* Mimema::Shader::diffuseColorUniform = "diffuseColor";
const char* Mimema::Shader::dissolveUniform = "dissolve";
const char* Mimema::Shader::specularExponentUniform = "specularExponent";
const char* Mimema::Shader::specularColorUniform = "specularColor";
const char* Mimema::Shader::illuminationModelUniform = "illuminationModel";
const char* Mimema::Shader::ambientColorTextureUniform = "ambientColorTexture";
const char* Mimema::Shader::diffuseColorTextureUniform = "diffuseColorTexture";
const char* Mimema::Shader::specularColorTextureUniform = "specularColorTexture";
const char* Mimema::Shader::specularExponentTextureUniform = "specularExponentTexture";
const char* Mimema::Shader::dissolveTextureUniform = "dissolveTexture";

std::string Mimema::Shader::readShaderFile(const std::string& fileLocation) {
    // Open the File
    std::ifstream shaderFile(shaderFolderLocation + fileLocation);
    if (!shaderFile.is_open()) {
        std::cout << "Shader: \'" << fileLocation << "\' failed to open." << std::endl;
        return {};
    }

    // Convert to a string
    std::stringstream shaderFileStream;
    shaderFileStream << shaderFile.rdbuf();
    std::string shaderFileString = shaderFileStream.str();
    shaderFile.close();

    return shaderFileString;
}

Mimema::Shader::Shader(const std::string& fileName) : name(fileName) {
    // Create Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderString = readShaderFile(fileName + vertexShaderFileExtension);
    const char* vertexShaderStringPtr = vertexShaderString.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderStringPtr, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Create Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderString = readShaderFile(fileName + fragmentShaderFileExtension);
    const char* fragmentShaderStringPtr = fragmentShaderString.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderStringPtr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Link shaders
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Initialize Uniforms
    mvpMatrixUID = glGetUniformLocation(shaderID, mvpMatrixUniform);
    modelMatrixUID = glGetUniformLocation(shaderID, modelMatrixUniform);
    normalMatrixUID = glGetUniformLocation(shaderID, normalMatrixUniform);
    viewPosUID = glGetUniformLocation(shaderID, viewPosUniform);
    ambientLightIntensityUID = glGetUniformLocation(shaderID, ambientLightIntensityUniform);
    ambientLightColorUID = glGetUniformLocation(shaderID, ambientLightColorUniform);
    directionalLightCountUID = glGetUniformLocation(shaderID, directionalLightCountUniform);
    directionalLightIntensitiesUID = glGetUniformLocation(shaderID, directionalLightIntensitiesUniform);
    directionalLightColorsUID = glGetUniformLocation(shaderID, directionalLightColorsUniform);
    directionalLightDirectionsUID = glGetUniformLocation(shaderID, directionalLightDirectionsUniform);
    pointLightCountUID = glGetUniformLocation(shaderID, pointLightCountUniform);
    pointLightIntensitiesUID = glGetUniformLocation(shaderID, pointLightIntensitiesUniform);
    pointLightColorsUID = glGetUniformLocation(shaderID, pointLightColorsUniform);
    pointLightPositionsUID = glGetUniformLocation(shaderID, pointLightPositionsUniform);
    pointLightAttenuationsUID = glGetUniformLocation(shaderID, pointLightAttenuationsUniform);
    ambientColorUID = glGetUniformLocation(shaderID, ambientColorUniform);
    diffuseColorUID = glGetUniformLocation(shaderID, diffuseColorUniform);
    dissolveUID = glGetUniformLocation(shaderID, dissolveUniform);
    specularExponentUID = glGetUniformLocation(shaderID, specularExponentUniform);
    specularColorUID = glGetUniformLocation(shaderID, specularColorUniform);
    illuminationModelUID = glGetUniformLocation(shaderID, illuminationModelUniform);
    ambientColorTextureUID = glGetUniformLocation(shaderID, ambientColorTextureUniform);
    diffuseColorTextureUID = glGetUniformLocation(shaderID, diffuseColorTextureUniform);
    specularColorTextureUID = glGetUniformLocation(shaderID, specularColorTextureUniform);
    specularExponentTextureUID = glGetUniformLocation(shaderID, specularExponentTextureUniform);
    dissolveTextureUID = glGetUniformLocation(shaderID, dissolveTextureUniform);

    glUseProgram(shaderID);
    glUniform1i(ambientColorTextureUID, ambientColorTextureID);
    glUniform1i(diffuseColorTextureUID, diffuseColorTextureID);
    glUniform1i(specularColorTextureUID, specularColorTextureID);
    glUniform1i(specularExponentTextureUID, specularExponentTextureID);
    glUniform1i(dissolveTextureUID, dissolveTextureID);
}

Mimema::Shader::~Shader() {
    glDeleteProgram(shaderID);
}

void Mimema::Shader::drawWithShader(const RenderState& renderState) {
    glUseProgram(shaderID);

    glUniform1f(ambientLightIntensityUID, renderState.ambientLightIntensity);
    glUniform3fv(ambientLightColorUID, 1, &renderState.ambientLightColor[0]);
    glUniform1i(directionalLightCountUID, renderState.directionalLightStates.size());
    glUniform1i(pointLightCountUID, renderState.pointLightStates.size());

    const Camera::CameraState& cameraState = renderState.cameraState;
    glm::mat4 projectionMatrix = glm::perspective(cameraState.fov, cameraState.aspectRatio, cameraState.nearClippingPlane, cameraState.farClippingPlane);
    glm::mat4 viewMatrix = glm::lookAt(cameraState.cameraPosition, cameraState.cameraTarget, cameraState.upVec);
    glm::mat4 projectionViewMatrix = projectionMatrix * viewMatrix;

    glUniform3fv(viewPosUID, 1, &cameraState.cameraPosition[0]);

    std::vector<float> directionalLightIntensities;
    std::vector<glm::vec3> directionalLightColors;
    std::vector<glm::vec3> directionalLightDirections;
    for (auto iter = renderState.directionalLightStates.begin(); iter != renderState.directionalLightStates.end(); iter++) {
        directionalLightIntensities.push_back(iter->second.intensity);
        directionalLightColors.push_back(iter->second.color);
        directionalLightDirections.push_back(iter->second.direction);
    }

    glUniform1i(directionalLightCountUID, directionalLightIntensities.size());
    glUniform1fv(directionalLightIntensitiesUID, directionalLightIntensities.size(), &directionalLightIntensities[0]);
    glUniform3fv(directionalLightColorsUID, directionalLightColors.size(), &directionalLightColors[0][0]);
    glUniform3fv(directionalLightDirectionsUID, directionalLightDirections.size(), &directionalLightDirections[0][0]);

    std::vector<float> pointLightIntensities;
    std::vector<glm::vec3> pointLightColors;
    std::vector<glm::vec3> pointLightPositions;
    std::vector<glm::vec3> pointLightAttenuations;
    for (auto iter = renderState.pointLightStates.begin(); iter != renderState.pointLightStates.end(); iter++) {
        pointLightIntensities.push_back(iter->second.intensity);
        pointLightColors.push_back(iter->second.color);
        pointLightPositions.push_back(iter->second.position);
        pointLightAttenuations.push_back(iter->second.attenuation);
    }
    glUniform1i(pointLightCountUID, pointLightIntensities.size());
    glUniform1fv(pointLightIntensitiesUID, pointLightIntensities.size(), &pointLightIntensities[0]);
    glUniform3fv(pointLightColorsUID, pointLightColors.size(), &pointLightColors[0][0]);
    glUniform3fv(pointLightPositionsUID, pointLightPositions.size(), &pointLightPositions[0][0]);
    glUniform3fv(pointLightAttenuationsUID, pointLightAttenuations.size(), &pointLightAttenuations[0][0]);

    for (auto iter = renderState.objectStates.begin(); iter != renderState.objectStates.end(); iter++) {
        const Renderable* renderable = iter->first->model;

        if (renderable == nullptr) {
            continue;
        }

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1), iter->second.translate);
        glm::mat4 rotationMatrixX = glm::rotate(iter->second.angles.x, glm::vec3(1, 0, 0));
        glm::mat4 rotationMatrixY = glm::rotate(iter->second.angles.y, glm::vec3(0, 1, 0));
        glm::mat4 rotationMatrixZ = glm::rotate(iter->second.angles.z, glm::vec3(0, 0, 1));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), iter->second.scale);
        glm::mat4 modelMatrix = translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * scaleMatrix;

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
        glm::mat4 mvpMatrix = projectionViewMatrix * modelMatrix;

        glUniformMatrix4fv(mvpMatrixUID, 1, false, &mvpMatrix[0][0]);
        glUniformMatrix4fv(modelMatrixUID, 1, false, &modelMatrix[0][0]);
        glUniformMatrix3fv(normalMatrixUID, 1, false, &normalMatrix[0][0]);

        glBindVertexArray(renderable->getVAO());
        int verticesDrawn = 0;
        const std::vector<std::pair<Material*, int>>& triangles = renderable->getTriangles();
        for (int i = 0; i < triangles.size(); i++) {
            const Material& material = *triangles[i].first;

            glUniform3fv(ambientColorUID, 1, &material.ambientColor[0]);
            glUniform3fv(diffuseColorUID, 1, &material.diffuseColor[0]);
            glUniform3fv(specularColorUID, 1, &material.specularColor[0]);
            glUniform1f(specularExponentUID, material.specularExponent);
            glUniform1f(dissolveUID, material.dissolve);
            glUniform1i(illuminationModelUID, material.illuminationModel);

            glActiveTexture(GL_TEXTURE0 + ambientColorTextureID);
            glBindTexture(GL_TEXTURE_2D, material.getAmbientColorTexture());
            glActiveTexture(GL_TEXTURE0 + diffuseColorTextureID);
            glBindTexture(GL_TEXTURE_2D, material.getDiffuseColorTexture());
            glActiveTexture(GL_TEXTURE0 + specularColorTextureID);
            glBindTexture(GL_TEXTURE_2D, material.getSpecularColorTexture());
            glActiveTexture(GL_TEXTURE0 + specularExponentTextureID);
            glBindTexture(GL_TEXTURE_2D, material.getSpecularExponentTexture());
            glActiveTexture(GL_TEXTURE0 + dissolveTextureID);
            glBindTexture(GL_TEXTURE_2D, material.getDissolveTexture());

            int verticesToDraw = triangles[i].second * 3;
            glDrawArrays(GL_TRIANGLES, verticesDrawn, verticesToDraw);
            verticesDrawn += verticesToDraw;
        }
        glBindVertexArray(0);
    }
}
