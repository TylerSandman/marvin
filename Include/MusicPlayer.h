#pragma once
#include <SFML/Audio/Music.hpp>
#include <map>

enum MusicID{
    MainTheme,
    LevelTheme
};

class MusicPlayer : private sf::NonCopyable{

public:
    MusicPlayer();
    void play(MusicID theme);
    void stop();
    void setPaused(bool paused);
    void setVolume(float volume);
    bool isPlaying();

private:
    sf::Music mMusic;
    std::map<MusicID, std::string> mFileMap;
    float mVolume;
};