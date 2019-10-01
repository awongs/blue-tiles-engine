#pragma once

#include <sdl2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include "../debugbt/DebugLog.h"

class Music {
public:
    // Constuctor of music.
   
    Music(const std::string & file);

    // Deconstructor of music
    ~Music();

    // Plays the music
    void play() const;
private:
    //Used for Music
     Mix_Music *m_music;
};