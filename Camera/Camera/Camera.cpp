#include <iostream>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Protótipos das funções
int setup_geometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
constexpr GLuint width = 1000, height = 1000;

bool rotate_x=false, rotate_y=false, rotate_z=false;

glm::vec3 camera_pos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 camera_front = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0.0);
float camera_speed = 0.1;

bool first_mouse = true;
float last_x = 0.0, last_y = 0.0;
float yaw = -90.0, pitch = 0.0;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(width, height, "Ola Cubo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(window, mouse_callback);


	//Desabilita o desenho do cursor do mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	const Shader shader("../shaders/HelloPyramid.vs", "../shaders/HelloPyramid.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	const GLuint vao = setup_geometry();


	glUseProgram(shader.ID);

	auto model = glm::mat4(1); //matriz identidade;
	const GLint model_loc = glGetUniformLocation(shader.ID, "model");

	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(model_loc, 1, FALSE, value_ptr(model));

	//Definindo a matriz de view (posição e orientação da câmera)
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	const GLint view_loc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(view_loc, 1, FALSE, glm::value_ptr(view));

	//Definindo a matriz de projeção perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.0f);
	const GLint proj_loc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(proj_loc, 1, FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		const auto angle = static_cast<GLfloat>(glfwGetTime());

		model = glm::mat4(1); 

		// model = glm::translate(model, glm::vec3(0.0, 0.0, cos(angle) * 10.0));
		if (rotate_x)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			
		}
		else if (rotate_y)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotate_z)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		glUniformMatrix4fv(model_loc, 1, FALSE, value_ptr(model));

		//Alterando a matriz de view (posição e orientação da câmera)
		glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
		glUniformMatrix4fv(view_loc, 1, FALSE, value_ptr(view));



		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 42);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &vao);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
auto key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode) -> void
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotate_x = true;
		rotate_y = false;
		rotate_z = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotate_x = false;
		rotate_y = true;
		rotate_z = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotate_x = false;
		rotate_y = false;
		rotate_z = true;
	}

	if (key == GLFW_KEY_W)
	{
		camera_pos += camera_speed * camera_front;
	}

	if (key == GLFW_KEY_S)
	{
		camera_pos -= camera_speed * camera_front;
	}

	if (key == GLFW_KEY_A)
	{
		camera_pos -= normalize(cross(camera_front, camera_up)) * camera_speed;
	}

	if (key == GLFW_KEY_D)
	{
		camera_pos += normalize(cross(camera_front, camera_up)) * camera_speed;
	}



}

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos)
{

	// cout << xpos << " " << ypos << endl;

	if (first_mouse)
	{
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}

	float offsetx = xpos - last_x;
	float offsety = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	offsetx *= camera_speed;
	offsety *= camera_speed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);

}


// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setup_geometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {

		// Face frontal
		-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
		0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
		-0.5, 0.5, 0.5, 0.0, 0.0, 1.0,

		0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0,

		// Face traseira
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, -0.5, -0.5, 0.0, 1.0, 0.0,
		-0.5, 0.5, -0.5, 0.0, 0.0, 1.0,

		0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, -0.5, 0.0, 1.0, 1.0,
		-0.5, 0.5, -0.5, 1.0, 0.0, 1.0,

		// Face esquerda
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		-0.5, 0.5, -0.5, 0.0, 1.0, 0.0,
		-0.5, -0.5, 0.5, 0.0, 0.0, 1.0,

		-0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		-0.5, 0.5, 0.5, 0.0, 1.0, 1.0,
		-0.5, -0.5, 0.5, 1.0, 0.0, 1.0,

		// Face direita
		0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, 0.5, -0.5, 0.0, 1.0, 0.0,
		0.5, -0.5, 0.5, 0.0, 0.0, 1.0,

		0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,
		0.5, -0.5, 0.5, 1.0, 0.0, 1.0,

		// Face superior
		-0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, 0.5, -0.5, 0.0, 1.0, 0.0,
		-0.5, 0.5, 0.5, 0.0, 0.0, 1.0,

		0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, 0.5, 0.0, 1.0, 1.0,
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0,

		// Face inferior
		-0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		0.5, -0.5, -0.5, 0.0, 1.0, 0.0,
		-0.5, -0.5, 0.5, 0.0, 0.0, 1.0,

		0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		-0.5, -0.5, 0.5, 1.0, 0.0, 1.0,

		// Chão
		-5.0, -0.5, -5.0, 0.5, 0.5, 0.5,
		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5, -5.0, 0.5, 0.5, 0.5,

		-5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		5.0, -0.5, -5.0, 0.5, 0.5, 0.5


	};

	GLuint vbo;
	GLuint vao;

	//Geração do identificador do VBO
	glGenBuffers(1, &vbo);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &vao);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(vao);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return vao;
}