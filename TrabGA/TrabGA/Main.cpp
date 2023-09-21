#include <iostream>
#include <fstream>	
#include <string>

using namespace std;

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "./interfaces/ObjReader.h"
#include "./interfaces/Camera.h"
#include "./interfaces/MovementHandler.h"


void global_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void global_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void get_version();


const GLuint WIDTH = 1000, HEIGHT = 1000;
camera camera(WIDTH, HEIGHT, 0.08f);
movement_handler movement(camera, 0.08f);

int main()
{
	glfwInit();
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, global_key_callback);
	glfwSetCursorPosCallback(window, global_mouse_callback);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}
	
	get_version();
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	Shader shader("./shaders/Viewer.vs", "./shaders/Viewer.fs");
	
	int n_verts;
	const GLuint vao = obj_reader::load_simple_obj("../../3DModels/suzanneTriLowPoly.obj", n_verts);

	glUseProgram(shader.ID);
	
	auto model = glm::mat4(1);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", value_ptr(model));
	
	glm::mat4 projection = glm::perspective(glm::radians(40.0f),
	                                        static_cast<GLfloat>(WIDTH) / static_cast<GLfloat>(HEIGHT), 0.1f, 100.0f);
	shader.setMat4("projection", value_ptr(projection));
	
	auto view = lookAt(movement.get_camera_pos(), glm::vec3(0.0, 0.0, 0.0), camera.get_camera_up());
	shader.setMat4("view", value_ptr(view));

	glEnable(GL_DEPTH_TEST);

	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		const auto angle = static_cast<GLfloat>(glfwGetTime());

		model = glm::mat4(1);

		if (movement.get_rotate_x())
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (movement.get_rotate_y())
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (movement.get_rotate_z())
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		shader.setMat4("model", value_ptr(model));

		view = lookAt(movement.get_camera_pos(), movement.get_camera_pos() + camera.get_camera_front(), camera.get_camera_up());
		shader.setMat4("view", value_ptr(view));
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, n_verts);
		
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glfwTerminate();
	return 0;
}

void global_key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode)
{
	movement.key_callback(window, key, scancode, action, mode);
}

void global_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}

void get_version()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
}