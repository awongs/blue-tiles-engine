#pragma once

#include <sdl2/SDL_mixer.h>
#include <vector>
#include <iostream>

class Sound {
public:
    // Default constuctor of Sound
    Mix_Music *m_music;
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
        delete m_music;
        m_chunks.clear();
        std::cout << "Sound dtor called" << '\n';
    }

    // Copy Assignment of Sound
    Sound & operator= (const Sound other) {
        if(this != &other) {
            delete m_music;
            m_chunks.clear();
        }
        m_music = other.m_music;
        m_chunks = other.m_chunks;
        return *this;
    }

    bool addMusic(const std::string &music);
private:
    //Pointer of type Mix_Music used to play music
    
    std::vector <Mix_Chunk> m_chunks;
};