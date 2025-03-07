#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2_mixer/SDL_mixer.h>

#include <iostream>

#include "utils.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "SpriteRenderer.h"
#include "Sound.h"
#include "PulseRenderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// Camera vars
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float pitch = 0.0f;
float yaw = -90.0f;
float roll = 0.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float fov = 45.0f;

Camera camera = {};


double const PULSE_SPEED = SCR_WIDTH / 3.0;  // pixels per second
double const MAX_PULSE_RADIUS = SCR_WIDTH / 2.0;
double const MAX_PULSE_TIME = MAX_PULSE_RADIUS / PULSE_SPEED;

pulse_state_t pulse_state = {};
Sound* pulse_sound = nullptr;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "window resize!" << std::endl;
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec3 movement_dir{0.0f, 0.0f, 0.0f};
    bool moved = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement_dir.z = 1.0f;
        moved = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement_dir.z = -1.0f;
        moved = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement_dir.x = -1.0f;
        moved = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement_dir.x = 1.0f;
        moved = true;
    }

    if (moved) {
        camera.move(movement_dir, 0.016);    // pretend it's 60fps for now
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.add_mouse_input(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.add_mouse_scroll(static_cast<float>(yoffset));
}

void error_callback(int err, char const* err_str)
{
    std::cout << "GLFW error: " << err_str << std::endl;
}

void update_pulse(GLFWwindow* window, double time)
{
    if (pulse_state.charge_start >= 0.0) {   // charging pulse
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) { // kick off pulse
            pulse_state.pulse_start = time;
            double pulse_length = pulse_state.pulse_start - pulse_state.charge_start;
            pulse_state.pulse_end =
                pulse_state.pulse_start + glm::min(MAX_PULSE_TIME, pulse_length);

            pulse_state.pulse_position = {lastX, lastY};

            pulse_state.charge_start = -1.0;    // reset

            if (pulse_sound != nullptr) {
                pulse_sound->play(static_cast<int>(1000.0 * (pulse_state.pulse_end - pulse_state.pulse_start)));
            }
        }
    }
    else {  // Charging not charging pulse
        if (pulse_state.pulse_start >= 0.0) {   // pulse in progress
            if (time > pulse_state.pulse_end) {
                pulse_state.pulse_end = -1.0;
                pulse_state.pulse_start = -1.0;
                pulse_state.radius = -1.0f;
                pulse_state.charge_start = -1.0;
            }
            else {
                // update radius
                pulse_state.radius = PULSE_SPEED * (time - pulse_state.pulse_start);
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            pulse_state.charge_start = time;
        }
    }
}

int main(void)
{

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        std::cout << "Failed to init sound system: " << Mix_GetError() << std::endl;
        exit(1);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        std::cout << "Failed to open audio system: " << Mix_GetError() << std::endl;
        exit(1);
    }

    pulse_sound = new Sound("sounds/sonar_ping.wav");

    glfwInit();
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Pulse", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int num_vertex_attribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_vertex_attribs);
    std::cout << "Num supported vertex attributes: " << num_vertex_attribs << std::endl;

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPos(window, lastX, lastY);     // TODO this isn't really working :(
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader pulse_shader{"src/shaders/pulse_wavefront.vs", "src/shaders/pulse_wavefront.fs"};

    Texture* sprite_tex = new Texture("images/awesomeface.png");
    SpriteRenderer* renderer = new SpriteRenderer();
    PulseRenderer* pulse_renderer = new PulseRenderer();

    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        update_pulse(window, glfwGetTime());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pulse_renderer->draw(pulse_state);

        renderer->set_pulse_radius(pulse_state.radius);
        renderer->set_pulse_location(pulse_state.pulse_position);
        renderer->draw(*sprite_tex, {10.0f, 10.0f}, {50.0f, 50.0f}, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    Mix_Quit();
    return 0;
}
