#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer {
public:
    SpriteRenderer();
    ~SpriteRenderer();

    void set_pulse_radius(float const& new_radius) { radius = new_radius; }
    void set_pulse_location(glm::vec2 const pulse_loc) { pulse_location = pulse_loc; }

    void draw(Texture const& tex, glm::vec2 const& pos, glm::vec2 const& size, float rotate);

private:
    Shader sprite_shader;
    unsigned int quad_vao;
    unsigned int quad_vbo;

    float radius = -1.0f;
    glm::vec2 pulse_location;
};
#endif
