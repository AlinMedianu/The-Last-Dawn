// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef FUELBUFF_H
#define FUELBUFF_H

#include "GameModel.h"

#include "ParticleSys.h"  // For particle effects
#include "AudioMgr.h"

#include <cassert>        // For debugging

/// <summary>
/// Spawnable game item that provides an increase to the players' fuel.
/// <para>
/// Has a pulsing particle effect that culminates with an explosion when collected.
/// </para>
/// </summary>
class FuelBuff : public GameModel
{
public:
  FuelBuff();
  virtual ~FuelBuff();

  void Initialise(Mesh& mesh, float colliderRadius);
  void Start();
  const BoundingSphere& GetCollider() const;
  int GetFuelAmount() const;
  void Deplete();
  void Sparkle();
  void SparkleBurst();
  bool IsActive();
  void SetPosition(float x, float y, float z);
  void SetPosition(Vector3 position);
private:

  const int kFuelAmount = 10;
  const Emitter::Data kSparklesEmitterDef{
  Vector3(0,0,0),                   // position
  0.25f,                            // time between emissions
  128,                                // how many particles to emit at once
  -1,                               // how many to emit in total, -1 = forever
  Emitter::Type::OMNI,
  Vector2(0.1f, 0.2f),              // size of particles, random between values
  Vector2(0, 2 * PI),               // initial sprite rotation
  Vector2(-1.f, 1.f),               // speed and direction of rotation
  0,                                // flags, alive on by default
  Vector2(1.0f, 1.0f),                  // particle lifetime, random between values
  true,                             // release once run out of particles
  Matrix::Identity, // direction of emissions
  Vector2(-PI / 5, PI / 5),         // variation from specified direction
  3,                                // sprite to use
  Vector2(3.0f,3.0f)                 // speed of particle travel
    };
  const int kNumberOfParticlesInExplosion = 1024;
  const DirectX::SimpleMath::Vector2 kExplosionParticleSpeed = DirectX::SimpleMath::Vector2(10.0f, 10.0f);

  DirectX::BoundingSphere collider_;
  Emitter* pSparkles_;
  bool active_;
};

#endif // !FUELBUFF_H
