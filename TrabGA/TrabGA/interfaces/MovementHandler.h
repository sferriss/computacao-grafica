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
    int get_selected_mesh_index() const;
    void set_translate_x();
    void set_translate_y();
    void set_translate_z();
    void set_rotate_x();
    void set_rotate_y();
    void set_rotate_z();
    bool get_up() const;
    void set_up();

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
    float position_x_;
    float position_y_;
    float position_z_;
    glm::vec3 camera_pos_;
    camera& camera_;
    int selected_mesh_index_;
    bool up;
};
