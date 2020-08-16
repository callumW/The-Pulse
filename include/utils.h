#ifndef LEARNING_OPENGL_UTILS_H
#define LEARNING_OPENGL_UTILS_H

// Caller becomes owner of char* and must delete it
char* load_shader_code(std::string const& path);
#endif
