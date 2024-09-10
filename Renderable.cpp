#include "Renderable.h"

const std::string Mimema::Renderable::modelFolderLocation = "Models/";
const std::string Mimema::Renderable::materialFolderLocation = "Materials/";

const glm::vec3 Mimema::Renderable::defaultNormal = {0, 1, 0};
const glm::vec2 Mimema::Renderable::defaultTextureCoord = {0, 0};

void Mimema::Renderable::readMtlFile(const std::string& fileLocation, std::map<std::string, Material*>& materials) {
    Material* currentMat = nullptr;

    // Open the file
    std::ifstream file(materialFolderLocation + fileLocation);
    if (!file.is_open()) {
        std::cout << "Material: \'" + fileLocation + "\' failed to open." << std::endl;
        return;
    }

    std::string currLine;

    // Read each line
    while (getline(file, currLine)) {
        std::stringstream currLineStream(currLine);
        std::vector<std::string> parts;

        // Read each part
        std::string currLinePart;
        while (getline(currLineStream, currLinePart, ' ')) {
            parts.push_back(currLinePart);
        }

        // Based on first part in line
        if (parts.size() == 0) {
            continue;
        }
        // Change current materialList
        else if (parts.at(0) == "newmtl") {
            if (parts.size() != 2) {
                continue;
            }

            if (materials.count(parts.at(1)) > 0) {
                std::cout << "Material: Multiple materials of the same name defined..." << std::endl;
                return;
            }
            currentMat = new Material();
            materials[parts.at(1)] = currentMat;
        }
        else if (parts.at(0) == "Ns") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            currentMat->specularExponent = stof(parts.at(1));
        }
        else if (parts.at(0) == "Ka") {
            if (parts.size() != 4 || currentMat == nullptr) {
                continue;
            }

            glm::vec3 ka = glm::vec3(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3)));
            currentMat->ambientColor = ka;
        }
        else if (parts.at(0) == "Kd") {
            if (parts.size() != 4 || currentMat == nullptr) {
                continue;
            }

            glm::vec3 kd = glm::vec3(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3)));
            currentMat->diffuseColor = kd;
        }
        else if (parts.at(0) == "Ks") {
            if (parts.size() != 4 || currentMat == nullptr) {
                continue;
            }

            glm::vec3 ks = glm::vec3(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3)));
            currentMat->specularColor = ks;
        }
        else if (parts.at(0) == "d") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            currentMat->dissolve = stof(parts.at(1));
        }
        else if (parts.at(0) == "illum") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            currentMat->illuminationModel = stoi(parts.at(1));
        }
        else if (parts.at(0) == "map_Ka") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            std::string map_ka = parts.at(1);
            currentMat->setAmbientColorTexture(map_ka);
        }
        else if (parts.at(0) == "map_Kd") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            std::string map_kd = parts.at(1);
            currentMat->setDiffuseColorTexture(map_kd);
        }
        else if (parts.at(0) == "map_Ks") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            std::string map_ks = parts.at(1);
            currentMat->setSpecularColorTexture(map_ks);
        }
        else if (parts.at(0) == "map_Ns") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            std::string map_ns = parts.at(1);
            currentMat->setSpecularExponentTexture(map_ns);
        }
        else if (parts.at(0) == "map_d") {
            if (parts.size() != 2 || currentMat == nullptr) {
                continue;
            }

            std::string map_d = parts.at(1);
            currentMat->setDissolveTexture(map_d);
        }
    }

    file.close();
}

void Mimema::Renderable::setBuffers(const TriangleBuffers& data,
                                    const std::map<std::string, TriangleGroup*>& triangleGroups) {
    // Precalculate number of triangles
    int numberOfTriangles = 0;
    for (auto iter = triangleGroups.begin(); iter != triangleGroups.end(); iter++) {
        numberOfTriangles += iter->second->vertexIndices.size();
    }
    float* vertexArray = new float[numberOfTriangles * 3 * vboBlockSize];

    // Add each group to the vertex array
    int vboIndex = 0;
    for (auto iter = triangleGroups.begin(); iter != triangleGroups.end(); iter++) {
        const std::vector<glm::vec<3, int>>& vertexIndices = iter->second->vertexIndices;
        const std::vector<glm::vec<3, int>>& normalIndices = iter->second->normalIndices;
        const std::vector<glm::vec<3, int>>& textureIndices = iter->second->textureIndices;

        for (int j = 0; j < vertexIndices.size(); j++) {
            for (int k = 0; k < 3; k++, vboIndex += vboBlockSize) {
                glm::vec3 vertex = data.vertices[vertexIndices[j][k]];
                glm::vec3 normal = defaultNormal;
                if (normalIndices[j][k] != -1) {
                    normal = data.normals[normalIndices[j][k]];
                }
                glm::vec2 texture = defaultTextureCoord;
                if (textureIndices[j][k] != -1) {
                    texture = data.textures[textureIndices[j][k]];
                }

                vertexArray[vboIndex] = vertex.x;
                vertexArray[vboIndex + 1] = vertex.y;
                vertexArray[vboIndex + 2] = vertex.z;
                vertexArray[vboIndex + 3] = 1;
                vertexArray[vboIndex + 4] = normal.x;
                vertexArray[vboIndex + 5] = normal.y;
                vertexArray[vboIndex + 6] = normal.z;
                vertexArray[vboIndex + 7] = texture.x;
                vertexArray[vboIndex + 8] = texture.y;
            }
        }
    }

    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Assign the VBO data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numberOfTriangles * 3 * vboBlockSize * sizeof(float), vertexArray, GL_STATIC_DRAW);

    // Vertices for VBO
    glVertexAttribPointer(0, vboVertexBlockSize, GL_FLOAT, GL_FALSE, vboBlockSize * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Normals for VBO
    glVertexAttribPointer(1, vboNormalBlockSize, GL_FLOAT, GL_FALSE, vboBlockSize * sizeof(float), (void*) ((vboVertexBlockSize) * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Textures for VBO
    glVertexAttribPointer(2, vboTextureBlockSize, GL_FLOAT, GL_FALSE, vboBlockSize * sizeof(float), (void*) ((vboVertexBlockSize + vboNormalBlockSize) * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    delete[] vertexArray;
}

Mimema::Renderable::Renderable(const std::string& fileLocation) {
    // Open the file
    std::ifstream file(modelFolderLocation + fileLocation);
    if (!file.is_open()) {
        std::cout << "Obj File: \'" + fileLocation + "\' failed to open." << std::endl;
        return;
    }

    std::map<std::string, Material*> materials;
    TriangleBuffers buffers;
    std::map<std::string, TriangleGroup*> triangleGroups;

    Material* defaultMaterial = new Material;
    defaultMaterial->ambientColor = {1, 1, 1};
    defaultMaterial->diffuseColor = {1, 0.1, 1};
    defaultMaterial->specularColor = {1, 1, 1};
    defaultMaterial->specularExponent = 32;
    materials[""] = defaultMaterial;
    triangleGroups[""] = new TriangleGroup;
    TriangleGroup* currGroup = triangleGroups[""];

    glm::vec3 negativeBound;
    glm::vec3 positiveBound;

    std::string currLine;

    // Read all lines
    while (getline(file, currLine)) {
        std::stringstream currLineStream(currLine);
        std::vector<std::string> parts;

        // Read each part of the current line
        std::string currLinePart;
        while (getline(currLineStream, currLinePart, ' ')) {
            parts.push_back(currLinePart);
        }

        if (parts.size() == 0) {
            continue;
        }

        // Read a materialList library
        if (parts[0] == "mtllib") {
            if (parts.size() < 2) {
                continue;
            }

            readMtlFile(parts[1], materials);

            for (auto iter = materials.begin(); iter != materials.end(); iter++) {
                if (triangleGroups.count(iter->first) == 0) {
                    triangleGroups[iter->first] = new TriangleGroup();
                }
            }
        }
        // Change current materialList
        else if (parts.at(0) == "usemtl") {
            if (parts.size() < 2) {
                continue;
            }

            if (triangleGroups.count(parts.at(1)) == 0) {
                std::cout << "Could not usemtl " + parts.at(1) + "... Using default..." << std::endl;
                currGroup = triangleGroups[""];
                continue;
            }

            currGroup = triangleGroups[parts.at(1)];
        }
        // Use first part to decide what to do
        if (parts.size() == 0) {
            continue;
        }
        // Vertex
        else if (parts.at(0) == "v") {
            if (parts.size() < 4) {
                continue;
            }

            buffers.vertices.push_back({stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3))});

            if (buffers.vertices.size() == 1) {
                negativeBound = {buffers.vertices[0].x, buffers.vertices[0].y, buffers.vertices[0].z};
                positiveBound = negativeBound;
            }
            else {
                glm::vec3* vertex = &buffers.vertices[buffers.vertices.size() - 1];
                if (vertex->x < negativeBound.x) {
                    negativeBound.x = vertex->x;
                }
                if (vertex->y < negativeBound.y) {
                    negativeBound.y = vertex->y;
                }
                if (vertex->z < negativeBound.z) {
                    negativeBound.z = vertex->z;
                }
                if (vertex->x > positiveBound.x) {
                    positiveBound.x = vertex->x;
                }
                if (vertex->y > positiveBound.y) {
                    positiveBound.y = vertex->y;
                }
                if (vertex->z > positiveBound.z) {
                    positiveBound.z = vertex->z;
                }
            }
        }
        // Texture Vertex
        else if (parts.at(0) == "vt") {
            if (parts.size() < 3) {
                continue;
            }

            buffers.textures.push_back({stof(parts.at(1)), stof(parts.at(2))});
        }
        // Normal Vertex
        else if (parts.at(0) == "vn") {
            if (parts.size() < 4) {
                continue;
            }

            buffers.normals.push_back({stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3))});
        }
        // Face
        else if (parts.at(0) == "f") {
            std::vector<int> faceVertices;
            std::vector<int> faceTextures;
            std::vector<int> faceNormals;

            // For each part
            for (int i = 1; i < parts.size(); i++) {
                std::string currVertex;
                std::stringstream currVertexStream(parts.at(i));
                std::vector<std::string> vertexParts;

                // Read each vertex
                while (getline(currVertexStream, currVertex, '/')) {
                    vertexParts.push_back(currVertex);
                }

                // Place into proper vector
                if (vertexParts.size() > 0 && vertexParts.at(0) != "") {
                    faceVertices.push_back(stoi(vertexParts.at(0)));
                }
                if (vertexParts.size() > 1 && vertexParts.at(1) != "") {
                    faceTextures.push_back(stoi(vertexParts.at(1)));
                }
                if (vertexParts.size() > 2 && vertexParts.at(2) != "") {
                    faceNormals.push_back(stoi(vertexParts.at(2)));
                }
            }

            // Subdivide faces into triangles
            if (faceVertices.size() >= 3) {
                int numTriangles = faceVertices.size() - 2;
                for (int i = 0; i < numTriangles; i++) {
                    int currVertex = i + 2;
                    currGroup->vertexIndices.push_back({faceVertices.at(0) - 1, faceVertices.at(currVertex - 1) - 1, faceVertices.at(currVertex) - 1});
                    if (faceNormals.size() == faceVertices.size()) {
                        currGroup->normalIndices.push_back({faceNormals.at(0) - 1, faceNormals.at(currVertex - 1) - 1,
                                                   faceNormals.at(currVertex) - 1});
                    }
                    else {
                        currGroup->normalIndices.push_back({-1, -1, -1});
                    }
                    if (faceTextures.size() == faceVertices.size()) {
                        currGroup->textureIndices.push_back({faceTextures.at(0) - 1, faceTextures.at(currVertex - 1) - 1,
                                                    faceTextures.at(currVertex) - 1});
                    }
                    else {
                        currGroup->textureIndices.push_back({-1, -1, -1});
                    }
                }
            }
        }
    }

    file.close();

    bbPos = negativeBound;
    bbSize = positiveBound - negativeBound;

    setBuffers(buffers, triangleGroups);

    for (auto iter = materials.begin(); iter != materials.end(); iter++) {
        triangles.push_back({iter->second, triangleGroups[iter->first]->vertexIndices.size()});
        delete triangleGroups[iter->first];
    }
}

Mimema::Renderable::Renderable(const glm::vec3& position, const glm::vec3& size) {
    // Define a cube
    std::vector<glm::vec3> vertices = {position,
                position + glm::vec3(size.x, 0, 0),
                position + glm::vec3(0, 0, size.z),
                position + glm::vec3(size.x, 0, size.z),
                position + glm::vec3(0, size.y, 0),
                position + glm::vec3(size.x, size.y, 0),
                position + glm::vec3(0,size.y, size.z),
                position + size};

    std::vector<glm::vec<3, int>> vertexIndices = {{0, 2, 1},
                                                   {1, 2, 3},
                                                   {0, 1, 4},
                                                   {1, 5, 4},
                                                   {2, 6, 3},
                                                   {3, 6, 7},
                                                   {0, 4, 2},
                                                   {2, 4, 6},
                                                   {1, 3, 5},
                                                   {3, 7, 5},
                                                   {4, 5, 6},
                                                   {5, 7, 6}};

    bbPos = position;
    bbSize = size;

    TriangleBuffers data;
    data.vertices = vertices;
    std::map<std::string, TriangleGroup*> group;
    group[""] = new TriangleGroup;
    group[""]->vertexIndices = vertexIndices;

    setBuffers(data, group);

    Material* material = new Material;
    triangles.push_back({material, vertexIndices.size()});
    delete group[""];
}

Mimema::Renderable::~Renderable() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    for (int i = 0; i < triangles.size(); i++) {
        delete triangles[i].first;
    }
}

unsigned int Mimema::Renderable::getVAO() const {
    return VAO;
}

unsigned int Mimema::Renderable::getVBO() const {
    return VBO;
}

const glm::vec3& Mimema::Renderable::getBBPos() const {
    return bbPos;
}

const glm::vec3& Mimema::Renderable::getBBSize() const {
    return bbSize;
}

const std::vector<std::pair<Mimema::Material*, int>>& Mimema::Renderable::getTriangles() const {
    return triangles;
}