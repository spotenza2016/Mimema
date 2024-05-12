#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include "Material.h"
using namespace std;

class Model {
public:
    class ModelState {
        // Model Matrix
        glm::vec3 translate = glm::vec3(0, 0, 0);
        float angleX = 0;
        float angleY = 0;
        float angleZ = 0;
        glm::vec3 scale = glm::vec3(0.025, 0.025, 0.025);

    public:
        glm::mat4 getModelMatrix();
        glm::mat4 static calculateModelMatrix(const glm::vec3& translate, float angleX, float angleY, float angleZ, const glm::vec3& scale);
        const glm::vec3& getTranslate();
        float getAngleX();
        float getAngleY();
        float getAngleZ();
        const glm::vec3& getScale();

        // Setters
        void setTranslate(const glm::vec3& translate);
        void setAngleX(float angleX);
        void setAngleY(float angleY);
        void setAngleZ(float angleZ);
        void setScale(const glm::vec3& scale);
    };

private:
    const static int vboBlockSize = 13;
    const static int vboVertexBlockSize = 4;
    const static int vboColorBlockSize = 4;
    const static int vboNormalBlockSize = 3;
    const static int vboTextureBlockSize = 2;

    // 3D Vertex (for Vertices and Normals)
    struct Vertex {
        Vertex(float x, float y, float z);
        float x, y, z;
    };

    // 2D Vertex Texture
    struct VertexTexture {
        VertexTexture(float x, float y);
        float x, y;
    };

    // Triangle
    struct Triangle {
        Triangle(int p1, int p2, int p3);
        vector<int> vertices;
        vector<int> normals;
        vector<int> textures;
    };

    // Triangle Group
    struct TriangleGroup {
        ~TriangleGroup();
        // Stored vertices/triangles of model
        Material material;
        vector<Triangle*> triangles;
        unsigned int VAO, VBO;

        // Generates VAO for the Object
        void generateVAO(Model* model);

        int getNumVertices();

        // Generates EBO Vertex/Indices Arrays
        float* generateVBOVertexArray(Model* model);
    };

    // Model Uniforms (will be moved to material and array eventually)
    float phongExponent = 12.0f;
    glm::vec3 specularColor = glm::vec3(0.3f, 0.3f, 0.3f);

    vector<Vertex*> vertices;
    vector<Vertex*> vertexNormals;
    vector<VertexTexture*> vertexTextures;
    vector<TriangleGroup*> triangleGroups;

    ModelState modelState;

    // Read a material file
    map<string, int> readMaterial(string fileName);

public:
    // Constructor
    Model(string fileName);

    // Getters
    unsigned int getVAO(int index);
    unsigned int getVBO(int index);
    int getNumVertices(int index);
    int getNumGroups();
    unsigned int getTexture(int index);
    ModelState& getModelState();
    float getPhongExponent();
    const glm::vec3& getSpecularColor();

    // Setters
    void setPhongExponent(float phongExponent);
    void setSpecularColor(const glm::vec3& specularColor);

    // Destructor
    ~Model();
};
