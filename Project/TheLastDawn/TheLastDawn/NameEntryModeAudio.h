// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef NAMEENTRYMODEAUDIO_H
#define NAMEENTRYMODEAUDIO_H

#include "IModeAudio.h"

/// <summary>
/// NameEntryMode specific functions to Play and Stop background music.
/// </summary>
class NameEntryModeAudio : public IModeAudio
{
public:
  NameEntryModeAudio();
  ~NameEntryModeAudio();

  virtual void PlayBGM() override;
  virtual void StopBGM() override;
};

#endif // !NAMEENTRYMODEAUDIO_H
