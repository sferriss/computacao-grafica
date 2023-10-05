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
    bool get_translate_x() const;
    bool get_translate_y() const;
    bool get_translate_z() const;
    float get_scale_x() const;
    float get_scale_y() const;
    float get_scale_z() const;

private:
    bool rotate_x_;
    bool rotate_y_;
    bool rotate_z_;
    bool translate_x_;
    bool translate_y_;
    bool translate_z_;
    float sensitivity_;
    float scale_x_;
    float scale_y_;
    float scale_z_;
    glm::vec3 camera_pos_;
    camera& camera_;
};
