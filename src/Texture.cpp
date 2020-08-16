#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "utils.h"


Texture::Texture(std::string const& texture_path)
{
    texture_id = loadTexture(texture_path);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture_id);
}

void Texture::use() const
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
