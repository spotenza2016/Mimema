#include "Model.h"

// Constructors
Model::Vertex::Vertex(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
Model::VertexTexture::VertexTexture(float x, float y) {
    this->x = x;
    this->y = y;
}
Model::Triangle::Triangle(int p1, int p2, int p3) {
    this->vertices = {p1, p2, p3};
}

Model::TriangleGroup::~TriangleGroup() {
    for (int i = 0; i < triangles.size(); i++) {
        delete triangles.at(i);
    }

    // Delete OpenGL Buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Model::Model(string fileName) {
    TriangleGroup* noMaterial = new TriangleGroup();
    triangleGroups.push_back(noMaterial);

    // Open an object file as a model
    string currMaterial = "";
    map<string, int> materialList;

    // Open the file
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "File: \'" + fileName + "\' failed to open." << endl;
        return;
    }

    glm::vec3 negativeBound;
    glm::vec3 positiveBound;

    string currLine;

    // Read all lines
    while (getline(file, currLine)) {
        stringstream currLineStream(currLine);
        vector<string> parts;
        string currLinePart;

        // Read each part of the current line
        while (getline(currLineStream, currLinePart, ' ')) {
            parts.push_back(currLinePart);
        }

        TriangleGroup* currGroup;
        if (materialList.count(currMaterial) == 0) {
            currGroup = triangleGroups.at(0);
        }
        else {
            currGroup = triangleGroups.at(materialList.at(currMaterial));
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

            vertices.push_back(new Vertex(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3))));

            if (vertices.size() == 1) {
                negativeBound = {vertices[0]->x, vertices[0]->y, vertices[0]->z};
                positiveBound = negativeBound;
            }
            else {
                Vertex* vertex = vertices[vertices.size() - 1];
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

            vertexTextures.push_back(new VertexTexture(stof(parts.at(1)), stof(parts.at(2))));
        }
        // Normal Vertex
        else if (parts.at(0) == "vn") {
            if (parts.size() < 4) {
                continue;
            }

            vertexNormals.push_back(new Vertex(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3))));
        }
        // Face
        else if (parts.at(0) == "f") {
            vector<int> faceVertices;
            vector<int> faceTextures;
            vector<int> faceNormals;

            // For each part
            for (int i = 1; i < parts.size(); i++) {
                string currVertex;
                stringstream currVertexStream(parts.at(i));
                vector<string> vertexParts;

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
                int numTriangles = faceVertices.size()  - 2;
                for (int i = 0; i < numTriangles; i++) {
                    int currVertex = i + 2;
                    // Set Vertex Colors
                    Triangle* triangle = new Triangle(faceVertices.at(0), faceVertices.at(currVertex - 1), faceVertices.at(currVertex));
                    if (faceNormals.size() == faceVertices.size()) {
                        triangle->normals = {faceNormals.at(0), faceNormals.at(currVertex - 1), faceNormals.at(currVertex)};
                    }
                    if (faceTextures.size() == faceVertices.size()) {
                        triangle->textures = {faceTextures.at(0), faceTextures.at(currVertex - 1), faceTextures.at(currVertex)};
                    }
                    currGroup->triangles.push_back(triangle);
                }
            }
        }
        // Read a materialList library
        else if (parts.at(0) == "mtllib") {
            if (parts.size() < 2) {
                continue;
            }

            // TODO Major! Make this not hardcoded
            materialList = readMaterial("TestFiles/" + parts.at(1));
        }
        // Change current materialList
        else if (parts.at(0) == "usemtl") {
            if (parts.size() < 2) {
                continue;
            }

            currMaterial = parts.at(1);
        }
    }

    file.close();

    glm::vec3 position = negativeBound;
    glm::vec3 size = positiveBound - negativeBound;
    collision = CollisionBox(position, size);
    collision.generateVAO();

    for (int i = 0; i < triangleGroups.size(); i++) {
        triangleGroups.at(i)->generateVAO(this);
    }
}

map<string, int> Model::readMaterial(string fileName) {
    map<string, int> materialList;
    int currentMat = 0;

    // Open the file
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "File: \'" + fileName + "\' failed to open." << endl;
        return materialList;
    }

    string currLine;

    // Read each line
    while (getline(file, currLine)) {
        stringstream currLineStream(currLine);
        vector<string> parts;
        string currLinePart;

        // Read each part
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

            currentMat = triangleGroups.size();
            materialList.emplace(parts.at(1), currentMat);
            TriangleGroup* newGroup = new TriangleGroup();
            triangleGroups.push_back(newGroup);
        }
        // Use Diffusion Constant as materialList
        else if (parts.at(0) == "Kd") {
            if (parts.size() != 4) {
                continue;
            }

            glm::vec3 kd = glm::vec3(stof(parts.at(1)), stof(parts.at(2)), stof(parts.at(3)));
            triangleGroups.at(currentMat)->material.setKd(kd);
        }
        else if (parts.at(0) == "map_Kd") {
            if (parts.size() != 2) {
                continue;
            }

            string map_kd = parts.at(1);
            triangleGroups.at(currentMat)->material.setMap_kd(map_kd);
        }
    }

    file.close();

    return materialList;
}

void Model::TriangleGroup::generateVAO(Model* model) {
    float* vertices = generateVBOVertexArray(model);

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Create VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getNumVertices() * Model::vboBlockSize * sizeof(float), vertices, GL_STATIC_DRAW);

    // Vertices for VBO
    glVertexAttribPointer(0, Model::vboVertexBlockSize, GL_FLOAT, GL_FALSE, Model::vboBlockSize * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Colors for VBO
    glVertexAttribPointer(1, Model::vboColorBlockSize, GL_FLOAT, GL_FALSE, Model::vboBlockSize * sizeof(float), (void*) (Model::vboVertexBlockSize * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normals for VBO
    glVertexAttribPointer(2, Model::vboNormalBlockSize, GL_FLOAT, GL_FALSE, Model::vboBlockSize * sizeof(float), (void*) ((Model::vboVertexBlockSize + Model::vboColorBlockSize) * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Textures for VBO
    glVertexAttribPointer(3, Model::vboTextureBlockSize, GL_FLOAT, GL_FALSE, Model::vboBlockSize * sizeof(float), (void*) ((Model::vboVertexBlockSize + Model::vboColorBlockSize + Model::vboNormalBlockSize) * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    delete vertices;
}

int Model::TriangleGroup::getNumVertices() {
    return triangles.size() * 3;
}

// Get the matrix
glm::mat4 Model::ModelState::getModelMatrix() {
    return Model::ModelState::calculateModelMatrix(translate, angleX, angleY, angleZ, scale);
}

glm::mat4 Model::ModelState::calculateModelMatrix(const glm::vec3& translate, float angleX, float angleY, float angleZ, const glm::vec3& scale) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), translate);
    glm::mat4 rotationMatrixX = glm::rotate(angleX, glm::vec3(1, 0, 0));
    glm::mat4 rotationMatrixY = glm::rotate(angleY, glm::vec3(0, 1, 0));
    glm::mat4 rotationMatrixZ = glm::rotate(angleZ, glm::vec3(0, 0, 1));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), scale);

    return translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * scaleMatrix;
}

// Generate VBO Vertices
float* Model::TriangleGroup::generateVBOVertexArray(Model* model) {
    // Generate Vertices Array
    float* vertexArray = new float[triangles.size() * vboBlockSize * 3];
    for (int i = 0; i < triangles.size(); i++) {
        glm::vec3 triangleNormal;
        if (triangles.at(i)->normals.size() != 3) {
            Vertex* pointOneVertex = model->vertices.at(triangles.at(i)->vertices.at(0) - 1);
            glm::vec3 pointOne = glm::vec3(pointOneVertex->x, pointOneVertex->y, pointOneVertex->z);
            Vertex* pointTwoVertex = model->vertices.at(triangles.at(i)->vertices.at(1) - 1);
            glm::vec3 pointTwo = glm::vec3(pointTwoVertex->x, pointTwoVertex->y, pointTwoVertex->z);
            Vertex* pointThreeVertex = model->vertices.at(triangles.at(i)->vertices.at(2) - 1);
            glm::vec3 pointThree = glm::vec3(pointThreeVertex->x, pointThreeVertex->y, pointThreeVertex->z);

            glm::vec3 u = pointTwo - pointOne;
            glm::vec3 w = pointThree - pointTwo;

            triangleNormal = glm::normalize(glm::cross(u, w));
        }

        for (int j = 0; j < 3; j++) {
            int index = i * vboBlockSize * 3 + j * vboBlockSize;
            int vertexIndex = triangles.at(i)->vertices.at(j) - 1;

            glm::vec3 color = material.getKd();
            glm::vec4 vertex = glm::vec4(model->vertices.at(vertexIndex)->x, model->vertices.at(vertexIndex)->y, model->vertices.at(vertexIndex)->z, 1);

            glm::vec3 normal;
            if (triangles.at(i)->normals.size() == 3) {
                int normalIndex = triangles.at(i)->normals.at(j) - 1;
                normal = glm::vec3(model->vertexNormals.at(normalIndex)->x, model->vertexNormals.at(normalIndex)->y, model->vertexNormals.at(normalIndex)->z);
            }
            else {
                normal = triangleNormal;
            }

            glm::vec2 texture = glm::vec2(0, 0);
            if (triangles.at(i)->textures.size() == 3) {
                int textureIndex = triangles.at(i)->textures.at(j) - 1;
                texture = glm::vec2(model->vertexTextures.at(textureIndex)->x, model->vertexTextures.at(textureIndex)->y);
            }

            vertexArray[index] = vertex.x;
            vertexArray[index + 1] = vertex.y;
            vertexArray[index + 2] = vertex.z;
            vertexArray[index + 3] = vertex.w;
            vertexArray[index + 4] = color.x;
            vertexArray[index + 5] = color.y;
            vertexArray[index + 6] = color.z;
            vertexArray[index + 7] = 1.0f;
            vertexArray[index + 8] = normal.x;
            vertexArray[index + 9] = normal.y;
            vertexArray[index + 10] = normal.z;
            vertexArray[index + 11] = texture.x;
            vertexArray[index + 12] = texture.y;
        }
    }

    return vertexArray;
}

unsigned int Model::getVAO(int index) {
    return triangleGroups.at(index)->VAO;
}

unsigned int Model::getVBO(int index) {
    return triangleGroups.at(index)->VBO;
}

int Model::getNumVertices(int index) {
    return triangleGroups.at(index)->triangles.size() * 3;
}

int Model::getNumGroups() {
    return triangleGroups.size();
}

unsigned int Model::getTexture(int index) {
    return triangleGroups.at(index)->material.getTexture();
}

float Model::getPhongExponent() {
    return phongExponent;
}

const glm::vec3& Model::getSpecularColor() {
    return specularColor;
}

const glm::vec3& Model::ModelState::getTranslate() {
    return translate;
}

float Model::ModelState::getAngleX() {
    return angleX;
}

float Model::ModelState::getAngleY() {
    return angleY;
}

float Model::ModelState::getAngleZ() {
    return angleZ;
}

const glm::vec3 &Model::ModelState::getScale() {
    return scale;
}

void Model::setPhongExponent(float phongExponent) {
    this->phongExponent = phongExponent;
}

void Model::setSpecularColor(const glm::vec3& specularColor) {
    this->specularColor = specularColor;
}

void Model::ModelState::setTranslate(const glm::vec3& translate) {
    this->translate = translate;
}

void Model::ModelState::setAngleX(float angleX) {
    this->angleX = angleX;
}

void Model::ModelState::setAngleY(float angleY) {
    this->angleY = angleY;
}

void Model::ModelState::setAngleZ(float angleZ) {
    this->angleZ = angleZ;
}

void Model::ModelState::setScale(const glm::vec3& scale) {
    this->scale = scale;
}

Model::ModelState& Model::getModelState() {
    return modelState;
}

// Destructor
Model::~Model() {
    // Delete all vertices/triangles
    for (int i = 0; i < triangleGroups.size(); i++) {
        delete triangleGroups.at(i);
    }

    // Delete all vertices/triangles
    for (int i = 0; i < vertices.size(); i++) {
        delete vertices.at(i);
    }

    for (int i = 0; i < vertexNormals.size(); i++) {
        delete vertexNormals.at(i);
    }

    for (int i = 0; i < vertexTextures.size(); i++) {
        delete vertexTextures.at(i);
    }
}

CollisionObject& Model::getCollision() {
    collision.matrix = modelState.getModelMatrix();
    return collision;
}