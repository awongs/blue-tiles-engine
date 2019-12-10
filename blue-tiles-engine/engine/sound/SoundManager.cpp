#include "SoundManager.h"
#include "Music.h"
#include "Sound.h"
#include "../debugbt/DebugLog.h"

// This is used to store a list of keys. Which is the path to the music files and sound files.
namespace { 
    const std::unordered_map<std::string, std::string> ID_TO_MUSIC_PATH = {
        {"alarm", "../Assets/sound-assets/Alert.mp3"},
		{"music", "../Assets/sound-assets/Espionage.wav"}
    };

    const std::unordered_map<std::string, std::string> ID_TO_SOUND_PATH = {
		{"detected", "../Assets/sound-assets/detected.wav"},
        {"door-locked", "../Assets/sound-assets/Door_Locked.wav"},
        {"door-unlocked", "../Assets/sound-assets/Door_Unlocked.wav"},
        {"key-pickup", "../Assets/sound-assets/Key_Pickup.wav"},
		{"win", "../Assets/sound-assets/Win.wav"},
		{"tech-pickup", "../Assets/sound-assets/success.wav"},
		{"lose", "../Assets/sound-assets/Lose.wav"},
		{"alarm", "../Assets/sound-assets/Alert.mp3"}
    };
}

// SoundManager constructor that initialize Mix_OpenAudio and also two maps containing Sound and Music files in a map.
SoundManager::SoundManager() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
	    DebugLog::Error("Could not initialize SDL_mixer:" + std::string(Mix_GetError()));
	  }

    for(auto it = ID_TO_MUSIC_PATH.begin(); it!=ID_TO_MUSIC_PATH.end(); it++) {
        m_musicMap.insert({it->first, std::make_shared<Music>(it->second)});
    }

    for(auto it = ID_TO_SOUND_PATH.begin(); it!=ID_TO_SOUND_PATH.end(); it++) {
        m_soundMap.insert({it->first, std::make_shared<Sound>(it->second)});
    }
}

SoundManager & SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}
// Deconstructor to the SoundManager
SoundManager::~SoundManager() {
    // Ensure that the sound and music assets are freed before
    // quitting sdl_mixer.
    m_musicMap.clear();
    m_soundMap.clear();

    Mix_CloseAudio();
}

// Function that returns a shared_ptr to a Music object.
 std::shared_ptr<Music> SoundManager::getMusic(std::string musicKey) {
    auto it = m_musicMap.find(musicKey); 
    if( it == m_musicMap.end()) {
        return NULL;
    }
    return it->second;
 }

 // Function that returns a shared_ptr to a Sound object.
 std::shared_ptr<Sound> SoundManager::getSound(std::string soundKey) {
      auto it = m_soundMap.find(soundKey); 
    if( it == m_soundMap.end()) {
        return NULL;
    }
    return it->second;
 }