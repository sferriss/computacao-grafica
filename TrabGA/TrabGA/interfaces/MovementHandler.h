#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"

class movement_handler
{
public:
    movement_handler(camera& cam, float sensitivity);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    glm::vec3 get_camera_pos() const;
    bool get_rotate_x() const;
    bool get_rotate_y() const;
    bool get_rotate_z() const;

private:
    bool rotate_x_;
    bool rotate_y_;
    bool rotate_z_;
    float sensitivity_;
    glm::vec3 camera_pos_;
    camera& camera_;
};
