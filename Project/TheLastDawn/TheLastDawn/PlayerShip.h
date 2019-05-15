// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "GameModel.h"    // For base class derivation

#include "MobileBody.h"   // For moving in the game world
#include "ParticleSys.h"  // For engine trails
#include "ShipAudio.h"    // For sound effects
#include "Planet.h"       // For keeping track of the planet currently bring orbited

#include "Random.h"

/// <summary>
/// Spawnable game item that the player controls.
/// <para>
/// Manages and delegates all player related functionality(input, audio, visual effects)!
/// </para>
/// </summary>
class PlayerShip : public GameModel
{
public:
  PlayerShip();
  ~PlayerShip();

  void Initialise(Mesh& mesh, float colliderRadius);
  void Start();

  DirectX::SimpleMath::Vector3 GetVelocity() const;
  virtual void SetPosition(DirectX::SimpleMath::Vector3 pos) override;
  virtual void SetPosition(float xPos, float yPos, float zPos) override;
  float GetFuel() const;
  float GetMaximumFuel() const;
  void Boost(float dTime);
  void DirectionalBoost(int direction, float dTime);
  const DirectX::BoundingSphere& GetCollider() const;
  void Update(float dTime);
  void CheckOrbit(Planet* nearestPlanet);
  void SetVelocity(DirectX::SimpleMath::Vector3 velocity);
  void Die();
  void CollectFuel(int amount);
  void Stop();

private:
  enum PlayerState { FLYING, ORBITING, LAUNCHING };
  enum OrbitDirection {NONE, CLOCKWISE, ANTICLOCKWISE };

  const Emitter::Data kThrusterEmitterDef{
  Vector3(0,0,0),                   // position
  0.01f,                            // time between emissions
  8,                                // how many particles to emit at once
  -1,                               // how many to emit in total, -1 = forever
  Emitter::Type::DIRECTION,
  Vector2(0.3f, 0.5f),              // size of particles, random between values
  Vector2(0, 2 * PI),               // initial sprite rotation
  Vector2(-1.f, 1.f),               // speed and direction of rotation
  0,                                // flags, alive on by default
  Vector2(3, 4.f),                  // particle lifetime, random between values
  true,                             // release once run out of particles
  Matrix::CreateRotationZ(D2R(90)), // direction of emissions
  Vector2(-PI / 5, PI / 5),         // variation from specified direction
  0,                                // sprite to use
  Vector2(0.5f,1.f)                 // speed of particle travel
  };
  const float kMaximumFuel_ = 100.0f;
  const float kBoostForce_ = 1.0f;
  const float kFuelPerBoost_ = 10.0f;
  const float kTimeBetweenBoosts_ = 2.0f;
  const DirectX::SimpleMath::Vector3 kInitialPlayerStartPosition_ = DirectX::SimpleMath::Vector3::Zero;
  const DirectX::SimpleMath::Vector3 kInitialPlayerVelocity_ = { 5.0f, 0.0f, 0.0f };
  const int kFirstEmitterSpriteIndex_ = 6;
  const int kLastEmitterSpriteIndex_ = 10;
  const float kShaderOutlineSize_ = 0.1f;

  MobileBody mobileBody_;
  DirectX::BoundingSphere collider_;
  ShipAudio audio_;
  Emitter* pEngineEmitter_;
  Planet* pCurrentlyOrbiting_;

  PlayerState currentState_;
  OrbitDirection orbitDir_;
  bool boostActive_;

  float currentFuel_;
  float currentOrbitAngle_;

  float timeUntilNextBoost_;

  float radius = 0;
  float anglePerSec = 0;
  DirectX::SimpleMath::Vector3 mOrbitEntryDirectionNormal;
  DirectX::SimpleMath::Vector3 mOrbitEntryVelocity;

  void RotateShipToVelocityFacing();
  void SetOrbitalVelocity();
  void SetOrbitDirection();
  void SetClockwise();
  void SetAntiClockwise();
  void ApplyEngineEmitterPositionOffset();
  void StartEngineEmitter();
};

#endif // !PLAYERSHIP_H
