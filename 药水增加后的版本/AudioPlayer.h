#pragma once
#ifndef _AUDIO_PLAYER_H_
#define _AUDIO_PLAYER_H_

#include "AudioEngine.h"
using namespace std;

extern int backgroundMusicId;  
extern int soundEffectId;
extern float backgroundVolumn;
extern float soundEffectVolumn;

void audioPlayer(const string& audioPath, bool isLoop);

#endif