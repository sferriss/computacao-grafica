#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class mesh
{
public:
    mesh() {}
    ~mesh() {}
    void initialize(GLuint VAO, int nVertices, Shader* shader, GLuint texID);
    void update(glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis);
    void draw() const;

protected:
    GLuint VAO;
    int nVertices;
    Shader* shader;
    GLuint texID;
    glm::mat4 model;
};
