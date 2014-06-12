#pragma once
#include <SFML/Audio/Music.hpp>
#include <list>
#include "ResourceManager.h"

class SoundPlayer : private sf::NonCopyable{

public:
    SoundPlayer();
    void play(SoundEffectID effect);
    void removeStoppedSounds();
    void setVolume(float volume);
    float getVolume();

private:
    SoundBufferManager mSoundManager;
    std::list<sf::Sound> mSounds;
    float mVolume;

};