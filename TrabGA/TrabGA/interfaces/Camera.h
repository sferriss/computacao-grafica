#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class camera
{
public:
    camera();
    camera(GLuint width, GLuint height, float sensitivity);
    
    void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
    
    glm::vec3 get_camera_front() const;
    glm::vec3 get_camera_up() const;

private:
    bool first_mouse_;
    float last_x_, last_y_;
    float yaw_, pitch_;
    float sensitivity_;
    glm::vec3 camera_front_;
    glm::vec3 camera_up_;
};
