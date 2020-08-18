#ifndef SOUND_H
#define SOUND_H
#include <SDL2_mixer/SDL_mixer.h>

#include <string>

class Sound {
public:
    Sound(std::string const& path);
    ~Sound();

    void play();
private:
    Mix_Chunk* sound_chunk;
};
#endif
