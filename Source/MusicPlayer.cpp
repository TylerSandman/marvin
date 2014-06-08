#include "MusicPlayer.h"

MusicPlayer::MusicPlayer() : 
        mMusic(),
        mVolume(25.f){

    mFileMap[MainTheme] = "Resources/Audio/Music/menu_loop.ogg";
    mFileMap[LevelTheme] = "Resources/Audio/Music/level_theme.ogg";
}

void MusicPlayer::play(MusicID theme){

    std::string fileName = mFileMap[theme];

    if (!mMusic.openFromFile(fileName))
        throw std::runtime_error("Music player could not open " + fileName);

    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop(){
    mMusic.stop();
}

void MusicPlayer::setPaused(bool paused){
    paused ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setVolume(float volume){
    mVolume = volume;
}

bool MusicPlayer::isPlaying(){
    sf::Music::Status status = mMusic.getStatus();
    if (status == sf::Music::Status::Playing)
        return true;
    return false;
}
