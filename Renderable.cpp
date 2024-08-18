#include "Renderable.h"

//todo may want to transfer all these to opengl handler
void Renderable::generateVAO() {
    setVAOVBO = true;
    float* vertexArray = generateVBOVertexArray();

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Create VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, faces.size() * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);

    // Vertices for VBO
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    delete vertexArray;
}


float* Renderable::generateVBOVertexArray() {
    // Generate Vertices Array
    // todo maybe rework faces?
    float* vertexArray = new float[faces.size() * 4];
    for (int i = 0; i < faces.size(); i++) {
        int vboIndex = 4 * i;
        vertexArray[vboIndex] = vertices[faces[i] - 1].x;
        vertexArray[vboIndex + 1] = vertices[faces[i] - 1].y;
        vertexArray[vboIndex + 2] = vertices[faces[i] - 1].z;
        vertexArray[vboIndex + 3] = 1;
    }

    return vertexArray;
}

Renderable::~Renderable() {
    // todo better protection on deleting bad VAO/VBO
    // todo can I somehow prevent from creating temporaries? maybe override the copy assignment?
    // Delete OpenGL Buffers
    if (setVAOVBO) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

Renderable::Renderable(glm::vec3 position, glm::vec3 size) {
    vertices = {position,
                position + glm::vec3(size.x, 0, 0),
                position + glm::vec3(0, 0, size.z),
                position + glm::vec3(size.x, 0, size.z),
                position + glm::vec3(0, size.y, 0),
                position + glm::vec3(size.x, size.y, 0),
                position + glm::vec3(0,size.y, size.z),
                position + size};
    faces = {1, 3, 2,
             2, 3, 4,
             1, 2, 5,
             2, 6, 5,
             3, 7, 4,
             4, 7, 8,
             1, 5, 3,
             3, 5, 7,
             2, 4, 6,
             4, 8, 6,
             5, 6, 7,
             6, 8, 7};

    // todo avoid these single functions, if won't be reused, justplace in here
    calculateNormalsAndEdges();
}

void Renderable::calculateNormalsAndEdges() {
    for (int i = 0; i < faces.size(); i += 3) {
        glm::vec3 vecOne = vertices[faces[i + 1] - 1] - vertices[faces[i] - 1];
        glm::vec3 vecTwo = vertices[faces[i + 2] - 1] - vertices[faces[i] - 1];
        normals.insert(glm::normalize(glm::cross(vecOne, vecTwo)));

        for (int j = 0; j < 3; j++) {
            glm::vec3 edge = vertices[faces[i + ((j + 1) % 3)] - 1] - vertices[faces[i + j] - 1];
            edges.insert(edge);
        }
    }
}
