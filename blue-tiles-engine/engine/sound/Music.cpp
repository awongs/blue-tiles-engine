#include "Music.h"

/*
* Function that Attempts to load a music file.
* For now it will just use wave files.
* Later it might load .mp3
*/

Music::Music(const std::string & file) {
    //SDL Mixer function that loads wave files.
    m_music = Mix_LoadMUS (&file[0]);
    // Checks if m_music is null.
    if(!m_music) {
        DebugLog::Error(Mix_GetError());
    }
}

//Deconstructor
 Music::~Music() {
    DebugLog::Info("music dtor called");
    if(!m_music) {
        Mix_FreeMusic(m_music);
    }
}

//Play music
void Music::play() const {
    if(!m_music) {
        DebugLog::Error(Mix_GetError());
        return;
    }
    if((Mix_PlayMusic(m_music, -1))!=0) {
		DebugLog::Error("Failed");
	}
}