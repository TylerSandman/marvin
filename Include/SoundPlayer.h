#pragma once
#include <SFML/Audio/Music.hpp>
#include <list>
#include "ResourceManager.h"

class SoundPlayer : private sf::NonCopyable{

public:
    SoundPlayer();
    void play(SoundEffectID effect);
    void removeStoppedSounds();

private:
    SoundBufferManager mSoundManager;
    std::list<sf::Sound> mSounds;

};