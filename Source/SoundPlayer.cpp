#include "SoundPlayer.h"

SoundPlayer::SoundPlayer() : 
        mSoundManager(),
        mSounds(),
        mVolume(100.f){

    mSoundManager.load(SoundEffectID::Switch, "Resources/Audio/Effects/switch.ogg");
    mSoundManager.load(SoundEffectID::PlayerFade, "Resources/Audio/Effects/player_fade.ogg");
    mSoundManager.load(SoundEffectID::PlayerDie, "Resources/Audio/Effects/player_die.ogg");
    mSoundManager.load(SoundEffectID::GemPickup, "Resources/Audio/Effects/gem_pickup.ogg");
}

void SoundPlayer::play(SoundEffectID effect){

    mSounds.push_back(sf::Sound(mSoundManager.get(effect)));
    mSounds.back().setVolume(mVolume);
    mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds(){

    mSounds.remove_if([] (const sf::Sound& s){
        return s.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setVolume(float volume){
    mVolume = volume;
}

float SoundPlayer::getVolume(){
    return mVolume;
}
