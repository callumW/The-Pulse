#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "utils.h"
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

float pulse_triggered = false;
float const PULSE_RADIUS_INCREASE = 1.0f;


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
    const float cameraSpeed = 0.05f; // adjust accordingly
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pulse_triggered = true;
    }
    else {
        pulse_triggered = false;
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

int main(void)
{
    glfwInit();
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPos(window, lastX, lastY);     // TODO this isn't really working :(
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader triangle_shader{"src/shaders/triangle.vs", "src/shaders/triangle.fs"};

    // Cube
    float vertices[] = {
        -1.0f, 1.0f, 1.0f,  // top left
        1.0f, 1.0, 1.0f,    // top right
        1.0f, -1.0f, 1.0f,  // bottom right
        -1.0f, -1.0f, 1.0f  // bttom left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
    // vertex attributes(s).
    glBindVertexArray(VAO);
    // Now we have begun recording our actions

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // location 0 is the vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
    // attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
    // this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    // Stopped recording our actions

    Shader sub_shader{"src/shaders/sub.vs", "src/shaders/sub.fs"};

    float sub_vertices[] = {
        -0.5f, 0.5f, 1.0f,  // top left
        0.5f, 0.5, 1.0f,    // top right
        0.5f, -0.5f, 1.0f,  // bottom right
        -0.5f, -0.5f, 1.0f  // bttom left
    };
    unsigned int sub_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int sub_VAO, sub_VBO, sub_EBO;
    glGenVertexArrays(1, &sub_VAO);
    glGenBuffers(1, &sub_VBO);
    glGenBuffers(1, &sub_EBO);

    glBindVertexArray(sub_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, sub_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sub_vertices), sub_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sub_indices), sub_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but
    // this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        triangle_shader.use();
        if (pulse_triggered) {
            triangle_shader.setVec2("pulse_location", glm::vec2(lastX, lastY));
            triangle_shader.setFloat("pulse_radius", 150.0f);
        }
        else {
            triangle_shader.setFloat("pulse_radius", 0.0f);
        }

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it
                                // every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time

        sub_shader.use();
        if (pulse_triggered) {
            sub_shader.setVec2("pulse_location", glm::vec2(lastX, lastY));
            sub_shader.setFloat("pulse_radius", 150.0f);
        }
        else {
            sub_shader.setFloat("pulse_radius", 0.0f);
        }

        glBindVertexArray(sub_VAO); // seeing as we only have a single VAO there's no need to bind it
                                // every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
