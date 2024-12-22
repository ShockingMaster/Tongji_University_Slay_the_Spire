#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "AudioEngine.h"
using namespace std;

extern int backgroundMusicId;  
extern int soundEffectId;
extern float backgroundVolumn;
extern float soundEffectVolumn;
inline void audioPlayer(const std::string& audioPath, bool isLoop)
{

    if (isLoop) {
        cocos2d::experimental::AudioEngine::stop(backgroundMusicId);
        backgroundMusicId = cocos2d::experimental::AudioEngine::play2d(audioPath, isLoop, backgroundVolumn);
        cocos2d::experimental::AudioEngine::setVolume(backgroundMusicId, backgroundVolumn);
    }
    else {
        soundEffectId = cocos2d::experimental::AudioEngine::play2d(audioPath, isLoop, soundEffectVolumn);
        cocos2d::experimental::AudioEngine::setVolume(soundEffectId, soundEffectVolumn);
    }
}


#endif