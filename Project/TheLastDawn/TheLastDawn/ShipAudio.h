// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef SHIPAUDIO_H
#define SHIPAUDIO_H

#include "AudioMgr.h"

/// <summary>
/// Provide functionality to play ship related sound effects in game.
/// </summary>
class ShipAudio
{
public:
  ShipAudio();
  ~ShipAudio();

  void PlayBoostActivatedSound() const;
  void PlayBoostReadySound() const;
  void PlayBoostFailedSound();
  void PlayEngineSound();
  void PlayExplosion() const;
  void PlayEnteredOrbit() const;
  void PlayOrbitingSound();

private:
  IAudioGroup* pSfxManager_;
  unsigned int orbitingSoundHandle_ = 1;
  unsigned int fuelErrorSoundHandle_ = 2;
  unsigned int engineSoundHandle_ = 3;
};

#endif // !SHIPAUDIO_H
