#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class mesh
{
public:
    mesh()
    {
    }

    ~mesh()
    {
    }

    void initialize(GLuint vao, int n_vertices, Shader* shader, glm::vec3 position = glm::vec3(0.0f),
                    glm::vec3 scale = glm::vec3(1), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0));
    void draw(glm::mat4& model) const;
    
    GLuint vao;
    int n_vertices;
    glm::vec3 position;
    float angle;
    glm::vec3 axis;
    glm::vec3 scale;
    Shader* shader;
};
