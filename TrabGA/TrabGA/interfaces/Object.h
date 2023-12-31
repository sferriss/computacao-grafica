#pragma once

#include <vector>
#include <stb_image.h>

#include "Material.h"
#include "Mesh.h"

using namespace std;

class Object
{
public:
    Object() {}
    void initialize(string filePath, Shader *shader, int id, glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0));
    void draw();
    void rotate(float deltaAngle, const glm::vec3& rotationAxis);
    void translate(const glm::vec3& translation);
    void scale_object(const glm::vec3& scale_factor);
    glm::vec3 get_position() const;
    void load_curve(const std::vector<glm::vec3*>& curvePoints);
    int id;
protected:
    void load_obj(const string& filePath);
    GLuint generate_vao_curve(const std::vector<GLfloat>& vbuffer, int& nVerts);
    GLuint generate_vao(vector<GLfloat> vertbuffer, int& nVerts);
    GLuint load_texture(string file_path);
    vector <mesh> groups;
    glm::vec3 position;
    glm::vec3 scale;
    float angle;
    glm::vec3 axis;
    Shader* shader;
    glm::mat4 model;
    Material material;
};