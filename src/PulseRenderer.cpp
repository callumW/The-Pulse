#include "PulseRenderer.h"

PulseRenderer::PulseRenderer()
    :pulse_shader("src/shaders/pulse_wavefront.vs", "src/shaders/pulse_wavefront.fs")
{
    static float vertices[] = {
        -1.0f, 1.0f, 1.0f,  // top left
        1.0f, 1.0, 1.0f,    // top right
        1.0f, -1.0f, 1.0f,  // bottom right
        -1.0f, -1.0f, 1.0f  // bttom left
    };
    static unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
    // vertex attributes(s).
    glBindVertexArray(vao);
    // Now we have begun recording our actions

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // location 0 is the vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex
    // attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the vao afterwards so other vao calls won't accidentally modify this vao, but
    // this rarely happens. Modifying other
    // vaos requires a call to glBindVertexArray anyways so we generally don't unbind vaos (nor
    // vbos) when it's not directly necessary.
    glBindVertexArray(0);
}

PulseRenderer::~PulseRenderer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

}

void PulseRenderer::draw(pulse_state const& state)
{
    pulse_shader.use();
    pulse_shader.setVec2("pulse_location", state.pulse_position);
    pulse_shader.setFloat("pulse_radius", state.radius);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
