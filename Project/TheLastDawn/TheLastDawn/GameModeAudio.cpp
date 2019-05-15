// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameModeAudio.h"

GameModeAudio::GameModeAudio()
{
}

GameModeAudio::~GameModeAudio()
{
}

void GameModeAudio::PlayBGM()
{
  GetIAudioMgr()->GetSongMgr()->Stop();
  GetIAudioMgr()->GetSongMgr()->Play("Game", true, false, &bgm_, NORMAL_VOLUME);
}

void GameModeAudio::StopBGM()
{
  GetIAudioMgr()->GetSongMgr()->Stop(bgm_);
}
