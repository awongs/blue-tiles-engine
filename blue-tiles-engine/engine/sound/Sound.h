#pragma once

#include <sdl2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include "../debugbt/DebugLog.h"

class Sound {
public:
    // Default constuctor of Sound
   
    Sound() {
        m_music = NULL;
    }

    // Copy constructor of Sound
    Sound(const Sound &sound) {
        this->m_music = sound.m_music;
        this->m_chunks = sound.m_chunks;
    }

    // Deconstructor of Sound
    ~Sound() {
        if(m_music != 0) {
            delete m_music;    
        }
        DebugLog::Info("Sound dtor called");
    }

    // Copy Assignment of Sound
    Sound & operator= (const Sound & other) {
        if(this != & other) {
            delete m_music;
            m_chunks.clear();
        }
        m_music = other.m_music;
        m_chunks = other.m_chunks;
        return *this;
    }

    bool addMusic(const std::string & music);
    Mix_Music * getMusic() const;
private:
    //Used for Music
     Mix_Music *m_music;
    //Used for Sound Effects.
    std::vector <Mix_Chunk> m_chunks;
};