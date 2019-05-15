// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef IMODEAUDIO_H
#define IMODEAUDIO_H

#include "AudioMgr.h"

/// <summary>
/// Abstract class providing the necessary functions to Play and Stop background music.
/// </summary>
class IModeAudio
{
public:
  IModeAudio();
  virtual ~IModeAudio();

  virtual void PlayBGM() = 0;
  virtual void StopBGM() = 0;
};

#endif // !IMODEAUDIO_H
