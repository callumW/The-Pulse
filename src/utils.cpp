#include <glad/glad.h>

#include "stb_image.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "utils.h"

char* load_char_file(std::string const& path)
{
    assert(!path.empty());
    std::ifstream file_stream{path, std::ios_base::in | std::ios_base::ate};

    if (!file_stream) {
        std::cout << "Failed to open file: " << path << " for reading" << std::endl;
        return nullptr;
    }

    size_t const file_size = file_stream.tellg();
    char* data = new char[file_size + 1];

    file_stream.seekg(0);
    file_stream.read(data, file_size);
    data[file_size] = '\0';

    return data;
}

char* load_shader_code(std::string const& path)
{
    assert(!path.empty());
    return load_char_file(path);
}

unsigned int loadTexture(std::string const& path, bool flip)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data != nullptr) {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
        exit(1);
    }

    return texture;
}
