#include <iostream>
#include <fstream>	
#include <string>

using namespace std;


//GLM
#include <glm/gtc/matrix_transform.hpp>

//Classe gerenciadora dos shaders
#include "Shader.h"
#include "./interfaces/ObjReader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX=false, rotateY=false, rotateZ=false;

//Variáveis de controle da câmera
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

bool firstMouse = true;
float lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0; //para calcular o quanto que o mouse deslocou​
float yaw = -90.0, pitch = 0.0; //rotação em x e y​

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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
	Shader shader("./shaders/HelloPyramid.vs", "./shaders/HelloPyramid.fs");
	obj_reader obj_reader;

	// Gerando um buffer simples, com a geometria de um triângulo
	int nVerts;
	GLuint VAO = obj_reader.load_simple_obj("../../3DModels/suzanneTriLowPoly.obj", nVerts);


	glUseProgram(shader.ID);

	//Criando a matriz de modelo
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", glm::value_ptr(model));


	//Criando a matriz de projeção
	glm::mat4 projection = glm::mat4(1); //matriz identidade;
	//projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
	projection = glm::perspective(glm::radians(40.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", glm::value_ptr(projection));

	//Criando a matriz de view
	glm::mat4 view = glm::mat4(1);
	view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), cameraUp);
	shader.setMat4("view", glm::value_ptr(view));

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

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);

		float offset = cos((GLfloat)glfwGetTime());
		//model = glm::translate(model, glm::vec3(0.0, 0.0, offset));

		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));

		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		shader.setMat4("model", glm::value_ptr(model));

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", glm::value_ptr(view));
		
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, nVerts);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		// VERTICES - GL_POINTS
		
		//glDrawArrays(GL_POINTS, 0, 18);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	float cameraSpeed = 0.05;

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront,cameraUp));
	}
	if (key == GLFW_KEY_D)
	{
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//cout << xpos << "\t" << ypos << endl;

	if (firstMouse)

	{

		lastX = xpos;

		lastY = ypos;

		firstMouse = false;

	}

	float xoffset = xpos - lastX;

	float yoffset = lastY - ypos;

	lastX = xpos;

	lastY = ypos;



	float sensitivity = 0.05;

	xoffset *= sensitivity;

	yoffset *= sensitivity;


	yaw += xoffset;

	pitch += yoffset;



	if (pitch > 89.0f)

		pitch = 89.0f;

	if (pitch < -89.0f)

		pitch = -89.0f;



	glm::vec3 front;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	front.y = sin(glm::radians(pitch));

	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(front);



	//Precisamos também atualizar o cameraUp!! Pra isso, usamos o Up do  

	//mundo (y), recalculamos Right e depois o Up

	glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0, 1.0, 0.0)));

	cameraUp = glm::normalize(glm::cross(right, cameraFront));

}