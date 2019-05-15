// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "IModeAudio.h"

IModeAudio::IModeAudio()
{
}

IModeAudio::~IModeAudio()
{
}

void IModeAudio::StopBGM()
{
  GetIAudioMgr()->GetSongMgr()->Stop();
}
