#pragma once

#include <sdl2/SDL_mixer.h>
#include <sdl2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include "../debugbt/DebugLog.h"

class Sound {
public:
    // Constuctor of sound.
    Sound(const std::string & file);

    // Deconstructor of sound
    ~Sound();

    // Plays the sound
    void play() const;
private:
    //Used for sound
     Mix_Chunk *m_sound;
};