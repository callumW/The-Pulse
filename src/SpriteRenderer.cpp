#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
    :sprite_shader("src/shaders/sprite.vs", "src/shaders/sprite.fs")
{
    // configure VAO/VBO
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quad_vao);
    glGenBuffers(1, &quad_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quad_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &quad_vao);
    glDeleteBuffers(1, &quad_vbo);
}
void SpriteRenderer::draw(Texture const& tex,
                          glm::vec2 const& pos,
                          glm::vec2 const& size,
                          float rotate)
{
    sprite_shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    sprite_shader.setMat4("model", model);

    sprite_shader.setFloat("pulse_radius", radius);
    sprite_shader.setVec2("pulse_location", pulse_location);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    sprite_shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    tex.use();

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
