// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef TITLEMODEAUDIO_H
#define TITLEMODEAUDIO_H

#include "IModeAudio.h"

/// <summary>
/// NameEntryMode specific functions to Play and Stop background music.
/// </summary>
class TitleModeAudio : public IModeAudio
{
public:
  TitleModeAudio();
  ~TitleModeAudio();

  virtual void PlayBGM() override;
  virtual void StopBGM() override;

private:
  unsigned int bgm_ = 1;
};

#endif // !TITLEMODEAUDIO_H
