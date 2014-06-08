#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() : 
        mSoundManager(),
        mSounds(){

    mSoundManager.load(SoundEffectID::Switch, "Resources/Audio/Effects/switch.wav");
    mSoundManager.load(SoundEffectID::PlayerFade, "Resources/Audio/Effects/player_fade.ogg");
    mSoundManager.load(SoundEffectID::PlayerDie, "Resources/Audio/Effects/player_die.ogg");
    mSoundManager.load(SoundEffectID::GemPickup, "Resources/Audio/Effects/gem_pickup.ogg");
}

void SoundPlayer::play(SoundEffectID effect){

    mSounds.push_back(sf::Sound(mSoundManager.get(effect)));
    mSounds.back().setVolume(100.f);
    mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds(){

    mSounds.remove_if([] (const sf::Sound& s){
        return s.getStatus() == sf::Sound::Stopped;
    });
}
