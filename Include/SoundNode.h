#pragma once
#include <SFML/Audio.hpp>
#include "SoundPlayer.h"
#include "SceneNode.h"

class SoundNode : public SceneNode{

public:
    explicit SoundNode(SoundPlayer &player);
    void play(SoundEffectID sound);
    unsigned int getCategory() const;

private:
    SoundPlayer &mSoundPlayer;
};