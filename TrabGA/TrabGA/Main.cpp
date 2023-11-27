#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>

#include "interfaces/Mesh.h"
#include "interfaces/Object.h"
#include "interfaces/SceneReader.h"

using namespace std;

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "./interfaces/ObjReader.h"
#include "./interfaces/Camera.h"
#include "./interfaces/MovementHandler.h"


void global_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void global_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void get_version();
void handler_translation(Object& obj);
void handler_scale(Object& obj);
void handler_rotation(Object& obj);
void handler_operation(Object& obj);
float get_direction();


const GLuint WIDTH = 1600, HEIGHT = 900;
camera camera(WIDTH, HEIGHT, 0.1f);
movement_handler movement(camera, 0.2f);

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

    glUseProgram(shader.ID);

    glm::mat4 projection = glm::perspective(glm::radians(40.0f),
                                            static_cast<GLfloat>(WIDTH) / static_cast<GLfloat>(HEIGHT), 0.1f, 100.0f);
    shader.setMat4("projection", value_ptr(projection));

    auto view = lookAt(movement.get_camera_pos(), glm::vec3(0.0, 0.0, 0.0), camera.get_camera_up());
    shader.setMat4("view", value_ptr(view));

    glEnable(GL_DEPTH_TEST);

    scene_reader scene;
    scene.initialize("./scene/scene.txt", &shader);

    shader.setVec3("lightPos", 3.0, 2.0, 10.0);
    shader.setVec3("lightColor", 1.0, 1.0, 1.0);


    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        view = lookAt(movement.get_camera_pos(), movement.get_camera_pos() + camera.get_camera_front(),
                      camera.get_camera_up());
        shader.setMat4("view", value_ptr(view));

        if (movement.get_selected_mesh_index() != -1)
        {
            Object& currentObj = scene.objects[movement.get_selected_mesh_index() - 1];
            handler_rotation(currentObj);
            handler_translation(currentObj);
            handler_scale(currentObj);
        }
        
        for (auto obj : scene.objects)
        {
            obj.draw();
        }

        glfwSwapBuffers(window);
    }

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

void handler_translation(Object& obj)
{
    glm::vec3 current_position = obj.get_position();
    if (movement.get_translate_x())
    {
        current_position.x += get_direction();
        obj.translate(current_position);
        movement.set_translate_x();
    }
    else if (movement.get_translate_y())
    {
        current_position.y += get_direction();
        obj.translate(current_position);
        movement.set_translate_y();
    }
    else if (movement.get_translate_z())
    {
        current_position.z += get_direction();
        obj.translate(current_position);
        movement.set_translate_z();
    }
}

void handler_scale(Object& obj)
{
    obj.scale_object(glm::vec3(movement.get_scale_x(), movement.get_scale_y(), movement.get_scale_z()));
}

void handler_rotation(Object& obj)
{
    if (movement.get_rotate_x())
    {
        obj.rotate(1, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    else if (movement.get_rotate_y())
    {
        obj.rotate(1, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (movement.get_rotate_z())
    {
        obj.rotate(1, glm::vec3(0.0f, 0.0f, 1.0f));
    }
}

float get_direction()
{
    if (movement.get_up())
    {
        return 1.0f;
    }
    return -1.0f;
}