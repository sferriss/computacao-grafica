#include "interfaces/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

camera::camera()
= default;

camera::camera(const GLuint width, const GLuint height, float sensitivity)
{
    this->first_mouse_ = true;
    this->last_x_ = width / 2.0;
    this->last_y_ = height / 2.0;
    this->yaw_ = -90.0;
    this->pitch_ = -0.0f;
    this->camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
    this->camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    this->sensitivity_ = sensitivity;
}

void camera::mouse_callback(GLFWwindow* window, const double x_pos, const double y_pos)
{
    constexpr float pitch_upper_limit = 89.0f;
    constexpr float pitch_lower_limit = -89.0f;

    if (first_mouse_)
    {
        last_x_ = x_pos;
        last_y_ = y_pos;
        first_mouse_ = false;
    }

    float xoffset = x_pos - last_x_;
    float yoffset = last_y_ - y_pos;
    last_x_ = x_pos;
    last_y_ = y_pos;

    xoffset *= this->sensitivity_;
    yoffset *= this->sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (pitch_ > pitch_upper_limit)
    {
        pitch_ = pitch_upper_limit;
    }
    if (pitch_ < pitch_lower_limit)
    {
        pitch_ = pitch_lower_limit;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    camera_front_ = normalize(front);

    glm::vec3 right = normalize(glm::cross(camera_front_, glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_up_ = normalize(glm::cross(right, camera_front_));
}

glm::vec3 camera::get_camera_front() const
{
    return camera_front_;
}

glm::vec3 camera::get_camera_up() const
{
    return camera_up_;
}