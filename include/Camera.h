#ifndef LEARNING_OPENGL_CAMERA_H
#define LEARNING_OPENGL_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>


float const DEFAULT_YAW = -90.0f;
float const DEFAULT_PITCH = 0.0f;
float const DEFAULT_SPEED = 2.5f;
float const DEFAULT_MOUSE_SENSITIVITY = 0.1f;
float const DEFAULT_FOV = 45.0f;
float const DEFAULT_ROLL = 0.0f;

class Camera {
public:
    Camera(glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 cam_up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view() const;

    // Move camera in direction specified by local space vector, note: the passed vector will be normalised
    void move(glm::vec3 dir, float delta_time);

    void add_mouse_input(float x_offset, float y_offset, bool constrain_pitch = true);

    void add_mouse_scroll(float y_offset);

    float fov = DEFAULT_FOV;
    float movement_speed = DEFAULT_SPEED;
    float mouse_sensitivity = DEFAULT_MOUSE_SENSITIVITY;

private:
    void update_cam_vectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw = DEFAULT_YAW;
    float pitch = DEFAULT_PITCH;
    float roll = DEFAULT_ROLL;
};

#endif
