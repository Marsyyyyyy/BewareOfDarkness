#pragma once
#include <SFML/Audio.hpp>
#include <string>

class AudioManager
{
public:
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    void playMusic(const std::string& path, bool loop = true)
    {
        if (currentPath == path) 
            return;
        music.stop();
        if (!music.openFromFile(path))
        return;
        music.setLooping(loop);
        music.setVolume(volume);
        music.play();
        currentPath = path;
    }

    void setVolume(float vol)
    {
        volume = vol;
        music.setVolume(volume);
    }

    void stop() { music.stop(); currentPath = ""; }

private:
    AudioManager() = default;
    sf::Music music;
    float volume = 80.f;
    std::string currentPath;
};