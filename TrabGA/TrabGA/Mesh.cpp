#include "interfaces/Mesh.h"

void mesh::initialize(GLuint vao, int n_vertices, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
    this->vao = vao;
    this->n_vertices = n_vertices;
    this->shader = shader;
    this->position = position;
    this->scale = scale;
    this->angle = angle;
    this->axis = axis;
}

void mesh::draw(glm::mat4& model) const
{
	shader->setMat4("model", value_ptr(model));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, n_vertices);
	glBindVertexArray(0);
}
