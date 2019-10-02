#pragma once

#include <sdl2/SDL_mixer.h>
#include <memory>
#include <string>
#include <map>
class Sound;
class Music;
class SoundManager {
public:
    SoundManager();
    ~SoundManager();
    std::shared_ptr<Music> getMusic(std::string musickey);
    std::shared_ptr<Sound> getSound(std::string soundKey);

private:
     /*Map of Music
    * Key = file path, value = music object
    */
    std::map<std::string, std::shared_ptr<Music>> m_musicMap;
    /*Map of sounds
    * Key = file path, value = sound object
    */
    std::map<std::string, std::shared_ptr<Sound>> m_soundMap;
};