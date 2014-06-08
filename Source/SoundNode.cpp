#include "SoundNode.h"

SoundNode::SoundNode(SoundPlayer &player) : 
    mSoundPlayer(player){}

void SoundNode::play(SoundEffectID sound){
    mSoundPlayer.play(sound);
}

unsigned int SoundNode::getCategory(){
    return Category::SoundEffect;
}