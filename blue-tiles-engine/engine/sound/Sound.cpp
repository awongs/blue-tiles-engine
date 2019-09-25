#include "Sound.h"

/*
* Function that Attempts to load a music file.
* For now it will just use wave files.
* Later it might load .mp3
*/
bool Sound::addMusic(const std::string &music) {
    //SDL Mixer function that loads wave files.
    m_music = Mix_LoadMUS (&music[0]);
    // Checks if m_music is null.
    if(!m_music) {
        std::cout << "Failed to load " << Mix_GetError() << '\n';
        return false;
    }
    return true;
}