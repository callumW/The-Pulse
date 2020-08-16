#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

class Texture {
public:
    Texture(std::string const& texture_path);
    ~Texture();

    void use() const;
private:
    unsigned int texture_id = 0;
};
#endif
