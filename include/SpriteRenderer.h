#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer {
public:
    SpriteRenderer();
    ~SpriteRenderer();

    void draw(Texture const& tex, glm::vec2 const& pos, glm::vec2 const& size, float rotate);

private:
    Shader sprite_shader;
    unsigned int quad_vao;
    unsigned int quad_vbo;
};
#endif
