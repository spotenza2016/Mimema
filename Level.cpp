#include "Level.h"

const std::string Mimema::Level::levelFolderLocation = "Levels/";

Mimema::RenderState Mimema::Level::getState() {
    RenderState savedState;
    savedState.backgroundColor = backgroundColor;
    savedState.ambientLightIntensity = ambientLightIntensity;
    savedState.ambientLightColor = ambientLightColor;

    savedState.cameraState = camera.cameraState;

    std::unordered_map<DirectionalLight*, DirectionalLight::DirectionalLightState> directionalLightStates;
    for (auto iter = directionalLights.begin(); iter != directionalLights.end(); iter++) {
        directionalLightStates[&(*iter)] = iter->directionalLightState;
    }

    std::unordered_map<PointLight*, PointLight::PointLightState> pointLightStates;
    for (auto iter = pointLights.begin(); iter != pointLights.end(); iter++) {
        pointLightStates[&(*iter)] = iter->pointLightState;
    }

    std::unordered_map<Object*, Object::ObjectState> objectStates;
    for (auto iter = objects.begin(); iter != objects.end(); iter++) {
        objectStates[&(*iter)] = iter->getObjectState();
    }

    savedState.directionalLightStates = directionalLightStates;
    savedState.pointLightStates = pointLightStates;
    savedState.objectStates = objectStates;

    return savedState;
}

Mimema::Level::Level(const std::string& levelFile, const std::map<std::string, Renderable*>& renderables) {
    // Open the file
    std::ifstream file(levelFolderLocation + levelFile);
    if (!file.is_open()) {
        std::cout << "Level: \'" + levelFile + "\' failed to open." << std::endl;
        return;
    }

    // Read each line
    std::string currLine;
    while (getline(file, currLine)) {
        std::stringstream currLineStream(currLine);
        std::vector<std::string> parts;

        // Read each part
        std::string currLinePart;
        while (getline(currLineStream, currLinePart, ' ')) {
            parts.push_back(currLinePart);
        }

        if (parts.empty()) {
            continue;
        }
        else if (parts[0] == "levelBBPos") {
            if (parts.size() != 4) {
                continue;
            }

            bbPos = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])};
        }
        else if (parts[0] == "levelBackgroundColor") {
            if (parts.size() != 5) {
                continue;
            }

            backgroundColor = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]), std::stof(parts[4])};
        }
        else if (parts[0] == "levelBBSize") {
            if (parts.size() != 4) {
                continue;
            }

            bbSize = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])};
        }
        else if (parts[0] == "levelAmbientLightColor") {
            if (parts.size() != 4) {
                continue;
            }

            ambientLightColor = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])};
        }
        else if (parts[0] == "levelAmbientLightIntensity") {
            if (parts.size() != 2) {
                continue;
            }

            ambientLightIntensity = std::stof(parts[1]);
        }
        else if (parts[0] == "DirectionalLight") {
            DirectionalLight directionalLight;
            DirectionalLight::DirectionalLightState& state = directionalLight.directionalLightState;

            // Read each line
            while (getline(file, currLine)) {
                currLineStream = std::stringstream(currLine);
                std::vector<std::string> subParts;

                // Read each part
                std::string currSubLinePart;
                while (getline(currLineStream, currSubLinePart, ' ')) {
                    subParts.push_back(currSubLinePart);
                }

                if (subParts.empty()) {
                    continue;
                }
                else if (subParts[0] == "END_DirectionalLight") {
                    break;
                }
                else if (subParts[0] == "direction") {
                    state.direction = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
                else if (subParts[0] == "color") {
                    state.color = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
                else if (subParts[0] == "intensity") {
                    state.intensity = std::stof(subParts[1]);
                }
            }

            directionalLights.push_back(directionalLight);
        }
        else if (parts[0] == "PointLight") {
            PointLight pointLight;
            PointLight::PointLightState& state = pointLight.pointLightState;

            // Read each line
            while (getline(file, currLine)) {
                currLineStream = std::stringstream(currLine);
                std::vector<std::string> subParts;

                // Read each part
                std::string currSubLinePart;
                while (getline(currLineStream, currSubLinePart, ' ')) {
                    subParts.push_back(currSubLinePart);
                }

                if (subParts.empty()) {
                    continue;
                }
                else if (subParts[0] == "END_PointLight") {
                    break;
                }
                else if (subParts[0] == "position") {
                    state.position = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
                else if (subParts[0] == "color") {
                    state.color = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
                else if (subParts[0] == "intensity") {
                    state.intensity = std::stof(subParts[1]);
                }
                else if (subParts[0] == "attenuation") {
                    state.attenuation = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
            }

            pointLights.push_back(pointLight);
        }
        else if (parts[0] == "Object") {
            if (parts.size() != 2) {
                continue;
            }

            if (renderables.count(parts[1]) == 0) {
                std::cout << "Renderable: Couldn't find " << parts[1] << "..." << std::endl;
                continue;
            }

            Object object(renderables.at(parts[1]));

            // Read each line
            while (getline(file, currLine)) {
                currLineStream = std::stringstream(currLine);
                std::vector<std::string> subParts;

                // Read each part
                std::string currSubLinePart;
                while (getline(currLineStream, currSubLinePart, ' ')) {
                    subParts.push_back(currSubLinePart);
                }

                if (subParts.empty()) {
                    continue;
                }
                else if (subParts[0] == "END_Object") {
                    break;
                }
                else if (subParts[0] == "translate") {
                    object.setTranslate({std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])});
                }
                else if (subParts[0] == "angles") {
                    object.setAngles({glm::radians(std::stof(subParts[1])), glm::radians(std::stof(subParts[2])), glm::radians(std::stof(subParts[3]))});
                }
                else if (subParts[0] == "scale") {
                    object.setScale({std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])});
                }
                else if (subParts[0] == "physicsMask") {
                    object.physicsMask = std::stoi(subParts[1]);
                }
                else if (subParts[0] == "collisionMask") {
                    object.collisionMask = std::stoi(subParts[1]);
                }
                else if (subParts[0] == "velocity") {
                    object.velocity = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
                else if (subParts[0] == "acceleration") {
                    object.acceleration = {std::stof(subParts[1]), std::stof(subParts[2]), std::stof(subParts[3])};
                }
            }

            objects.push_back(object);
        }
    }

    file.close();
}
