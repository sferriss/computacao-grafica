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
void handler_translation(glm::mat4& model);
void handler_scale(glm::mat4& model);
void handler_rotation(glm::mat4& model, GLfloat angle);


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
	
	int n_verts1;
	const GLuint vao1 = obj_reader::load_simple_obj("../../3DModels/suzanneTriLowPoly.obj", n_verts1);

	int n_verts2;
	const GLuint vao2 = obj_reader::load_simple_obj("../../3DModels/suzanneTriLowPoly.obj", n_verts2);

	glUseProgram(shader.ID);
	
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

		view = lookAt(movement.get_camera_pos(), movement.get_camera_pos() + camera.get_camera_front(), camera.get_camera_up());
		shader.setMat4("view", value_ptr(view));

		glm::mat4 model1 = glm::mat4(1);
		model1 = translate(model1, glm::vec3(-3.0, 0.0, 0.0));
		glm::mat4 model2 = glm::mat4(1);
		
		if (movement.get_selected_mesh_index() == 1) {
			handler_translation(model1);
			handler_scale(model1);
			handler_rotation(model1, angle);
			
		}
		
		if (movement.get_selected_mesh_index() == 2) {
			handler_translation(model2); 
			handler_scale(model2);
			handler_rotation(model2, angle);
		}
		shader.setMat4("model", value_ptr(model1));
		glBindVertexArray(vao1);
		glDrawArrays(GL_TRIANGLES, 0, n_verts1);
		glBindVertexArray(0);

		shader.setMat4("model", value_ptr(model2));
		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES, 0, n_verts2);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	// glDeleteVertexArrays(1, &vao1);
	glDeleteVertexArrays(1, &vao2);
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

void handler_translation(glm::mat4& model)
{
	if (movement.get_translate_x())
	{
		model = translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	}
	else if (movement.get_translate_y())
	{
		model = translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	}
	else if (movement.get_translate_z())
	{
		model = translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	}
}

void handler_scale(glm::mat4& model)
{
	model = scale(model, glm::vec3(movement.get_scale_x(), movement.get_scale_y(), movement.get_scale_z()));
}

void handler_rotation(glm::mat4& model, const GLfloat angle)
{
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
}