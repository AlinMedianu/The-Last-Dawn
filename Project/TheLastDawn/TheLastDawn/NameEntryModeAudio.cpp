// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "NameEntryModeAudio.h"

NameEntryModeAudio::NameEntryModeAudio()
{
}

NameEntryModeAudio::~NameEntryModeAudio()
{
}

void NameEntryModeAudio::PlayBGM()
{
  GetIAudioMgr()->GetSongMgr()->Play("HighScore", true, false, 0, NORMAL_VOLUME);
}

void NameEntryModeAudio::StopBGM()
{
  IModeAudio::StopBGM();
}
