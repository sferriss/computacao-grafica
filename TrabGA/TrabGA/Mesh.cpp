#include "interfaces/Mesh.h"

void mesh::initialize(GLuint VAO, int nVertices, Shader* shader, GLuint texID)
{
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->texID = texID;
}

void mesh::update(glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->model = glm::mat4(1);
	this->model = translate(this->model, position);
	this->model = glm::rotate(this->model, glm::radians(angle), axis);
	this->model = glm::scale(this->model, scale);
	shader->setMat4("model", value_ptr(this->model));
}

void mesh::draw() const
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
}
