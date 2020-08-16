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
