// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMEMODEAUDIO_H
#define GAMEMODEAUDIO_H

#include "IModeAudio.h" // For base class derivation

/// <summary>
/// Control the non-object related audio played during the Game Mode.
/// <para>
/// Means we can replace the implementation with fmod, bass, opanALand the game code won't be touched.
/// </para>
/// </summary>
class GameModeAudio : public IModeAudio
{
public:
  GameModeAudio();
  virtual ~GameModeAudio();

  virtual void PlayBGM() override;
  virtual void StopBGM() override;

private:
  unsigned int bgm_ = 3;
};

#endif // !GAMEMODEAUDIO_H
