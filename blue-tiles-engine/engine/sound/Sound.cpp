#include "Sound.h"

/*
* Function that Attempts to load a sound file.
* For now it will just use wave files.
* Later it might load .mp3
*/

// Constructor to sound taking in a string that has the path to the .mp3
Sound::Sound(const std::string & file) {
    //SDL Mixer function that loads wave files.
    m_sound = Mix_LoadWAV(&file[0]);
    // Checks if m_sound is null.
    if(!m_sound) {
        DebugLog::Error(Mix_GetError());
    }
}

// Deconstructor
Sound::~Sound() {
    DebugLog::Info("Sound dtor called");
    if(!m_sound) {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

// Play the soundeffect
void Sound::play() const {
    if(!m_sound) {
        DebugLog::Error(Mix_GetError());
        return;
    }
    if((Mix_PlayChannel(-1,m_sound, 0))!=0) {
		DebugLog::Error("Failed");
	}
}