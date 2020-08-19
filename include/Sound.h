#ifndef SOUND_H
#define SOUND_H
#include <SDL2_mixer/SDL_mixer.h>

#include <string>

class Sound {
public:
    Sound(std::string const& path);
    ~Sound();

    void play();
    void play(int ms);  // play with fade out after ms
private:
    Mix_Chunk* sound_chunk;
};
#endif
