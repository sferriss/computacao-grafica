#include "interfaces/MovementHandler.h"

movement_handler::movement_handler(camera& cam, const float sensitivity) :
    rotate_x_(false), rotate_y_(false), rotate_z_(false), 
    translate_x_(false), translate_y_(false), translate_z_(false),
    scale_x_(1.0f), scale_y_(1.0f), scale_z_(1.0f),
    camera_pos_(glm::vec3(0.0, 0.0, 3.0)), camera_(cam), selected_mesh_index_(-1)
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
    
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        translate_x_ = !translate_x_;
        translate_y_ = false;
        translate_z_ = false;
    }
    
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        translate_x_ = false;
        translate_y_ = !translate_y_;
        translate_z_ = false;
    }
    
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        translate_x_ = false;
        translate_y_ = false;
        translate_z_ = !translate_z_;
    }

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        scale_x_ += 0.1f;
    }
    
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        scale_y_ += 0.1f;
    }
    
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        scale_z_ += 0.1f;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        scale_x_ -= 0.1f;
    }
    
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        scale_y_ -= 0.1f;
    }
    
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        scale_z_ -= 0.1f;
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_1)
        {
            selected_mesh_index_ = 1;
        }
        if (key == GLFW_KEY_2)
        {
            selected_mesh_index_ = 2;
        }
        if (key == GLFW_KEY_3)
        {
            selected_mesh_index_ = 3;
        }
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

bool movement_handler::get_translate_x() const
{
    return translate_x_;
}

bool movement_handler::get_translate_y() const
{
    return translate_y_;
}

bool movement_handler::get_translate_z() const
{
    return translate_z_;
}

float movement_handler::get_scale_x() const
{
    return scale_x_;
}

float movement_handler::get_scale_y() const
{
    return scale_y_;
}

float movement_handler::get_scale_z() const
{
    return scale_z_;
}

int movement_handler::get_selected_mesh_index() const
{
    return selected_mesh_index_;
}
