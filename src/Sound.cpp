#include "Sound.h"

#include <iostream>

Sound::Sound(std::string const& path)
{
    sound_chunk = Mix_LoadWAV(path.c_str());
    if (sound_chunk == nullptr) {
        std::cout << "Failed to load sound file: " << path << ": " << std::endl <<
            Mix_GetError() << std::endl;
        exit(1);
    }
}

void Sound::play()
{
    if (Mix_PlayChannel(-1, sound_chunk, 0) == -1) {
        std::cout << "Failed to play sound: " << Mix_GetError() << std::endl;
    }
}

Sound::~Sound()
{
    Mix_FreeChunk(sound_chunk);
}
