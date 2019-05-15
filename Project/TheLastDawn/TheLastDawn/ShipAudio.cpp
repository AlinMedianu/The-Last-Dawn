// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "ShipAudio.h"

ShipAudio::ShipAudio()
  : pSfxManager_(GetIAudioMgr()->GetSfxMgr())
{
}

ShipAudio::~ShipAudio()
{
}

void ShipAudio::PlayBoostActivatedSound() const
{
  pSfxManager_->Play("RcktFly", false, false, 0, QUIET_VOLUME);
}

void ShipAudio::PlayBoostReadySound() const
{
  pSfxManager_->Play("Bell_10_b", false, false, 0, NORMAL_VOLUME);
}

void ShipAudio::PlayBoostFailedSound()
{
  if (!pSfxManager_->IsPlaying(fuelErrorSoundHandle_))
  {
    pSfxManager_->Play("Negative_Notification_25", false, false, &fuelErrorSoundHandle_, QUIET_VOLUME);
  }
}

void ShipAudio::PlayEngineSound()
{
  pSfxManager_->Play("Low Hum", true, false, &engineSoundHandle_, NORMAL_VOLUME);
}

void ShipAudio::PlayExplosion() const
{
  pSfxManager_->Play("explosion_med_long_tail_01", false, false, 0, NORMAL_VOLUME);
}

void ShipAudio::PlayEnteredOrbit() const
{
  pSfxManager_->Play("Spaceship Interface 33", false, false, 0, NORMAL_VOLUME);
}

void ShipAudio::PlayOrbitingSound()
{
  if (!pSfxManager_->IsPlaying(orbitingSoundHandle_))
  {
    pSfxManager_->Play("SF_sdb_WHOOSH_15_space_vehicle", true, false, &orbitingSoundHandle_, NORMAL_VOLUME);
  }
}
