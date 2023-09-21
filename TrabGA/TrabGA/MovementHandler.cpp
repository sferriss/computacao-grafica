#include "interfaces/MovementHandler.h"

movement_handler::movement_handler(camera& cam, const float sensitivity) :
    rotate_x_(false), rotate_y_(false), rotate_z_(false), camera_pos_(glm::vec3(0.0, 0.0, 3.0)), camera_(cam)
{
    this->sensitivity_ = sensitivity;
}

void movement_handler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        rotate_x_ = !rotate_x_;
        rotate_y_ = false;
        rotate_z_ = false;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        rotate_x_ = false;
        rotate_y_ = !rotate_y_;
        rotate_z_ = false;
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        rotate_x_ = false;
        rotate_y_ = false;
        rotate_z_ = !rotate_z_;
    }

    if (key == GLFW_KEY_W)
    {
        camera_pos_ += this->sensitivity_ * camera_.get_camera_front();
    }
    if (key == GLFW_KEY_S)
    {
        camera_pos_ -= this->sensitivity_ * camera_.get_camera_front();
    }
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
    {
        camera_pos_ -= this->sensitivity_ * normalize(cross(camera_.get_camera_front(), camera_.get_camera_up()));
    }
    if (key == GLFW_KEY_D)
    {
        camera_pos_ += this->sensitivity_ * normalize(cross(camera_.get_camera_front(), camera_.get_camera_up()));
    }
}

glm::vec3 movement_handler::get_camera_pos() const
{
    return camera_pos_;
}

bool movement_handler::get_rotate_x() const
{
    return rotate_x_;
}

bool movement_handler::get_rotate_y() const
{
    return rotate_y_;
}

bool movement_handler::get_rotate_z() const
{
    return rotate_z_;
}
