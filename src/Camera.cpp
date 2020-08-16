#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 cam_pos, glm::vec3 world_up)
{
    position = cam_pos;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->world_up = world_up;
    update_cam_vectors();
}

glm::mat4 Camera::view() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::update_cam_vectors()
{
    // calculate the new Front vector
    glm::vec3 tmp_front;
    tmp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tmp_front.y = sin(glm::radians(pitch));
    tmp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(tmp_front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(tmp_front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

void Camera::move(glm::vec3 dir, float delta_time)
{
    dir = glm::normalize(dir);

    float const total_dist = movement_speed * delta_time;

    float x_dist = dir.x * total_dist;
    float y_dist = dir.y * total_dist;
    float z_dist = dir.z * total_dist;

    position += front * z_dist;
    position += up * y_dist;
    position += right * x_dist;
}

void Camera::add_mouse_input(float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (constrain_pitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        else if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    update_cam_vectors();
}

void Camera::add_mouse_scroll(float y_offset)
{
    fov -= y_offset;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    else if (fov > 45.0f) {
        fov = 45.0f;
    }
}
