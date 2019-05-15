// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "TitleModeAudio.h"

TitleModeAudio::TitleModeAudio()
{
}

TitleModeAudio::~TitleModeAudio()
{
}

void TitleModeAudio::PlayBGM()
{
  if (!GetIAudioMgr()->GetSongMgr()->IsPlaying(bgm_))
  {
    IModeAudio::StopBGM();
    GetIAudioMgr()->GetSongMgr()->Play("Opening", true, false, &bgm_, NORMAL_VOLUME);
  }
}

void TitleModeAudio::StopBGM()
{
  GetIAudioMgr()->GetSongMgr()->Stop(bgm_);
}
