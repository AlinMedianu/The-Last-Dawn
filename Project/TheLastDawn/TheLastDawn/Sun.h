// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef SUN_H
#define SUN_H

#include "GameModel.h"

#include "PlayerShip.h" // For calculating the expansion rate

/// <summary>
/// Spawnable game item that constantly expands in an attempt to kill the player/force them to make a mistake.
/// <para>
/// Constant values can be modified to change the difficulty of the experience.
/// </para>
/// </summary>
class Sun : public GameModel
{
public:
  Sun();
  ~Sun();

  void Initialise(Mesh& mesh, float minRadius, PlayerShip* player);
  void Start();
  void SetPosition(DirectX::SimpleMath::Vector3 pos);
  void SetPosition(float xPos, float yPos, float zPos);
  void Update(float dTime);
  float GetCurrentRadius() const;
  const DirectX::BoundingSphere& GetCollider() const;

private:
  const DirectX::SimpleMath::Vector3 kInitialSunPosition_ = { -40.0f, 0.0f, 0.0f };
  const float kDesiredDistanceAwayFromPlayer = 20.0f;
  const float kExpansionSpeedWhenTooFarAway = 1.05f;
  const float kExpansionSpeedWhenCloseToPlayer = 0.6f;

  DirectX::BoundingSphere collider_;
  PlayerShip* pPlayer_;
  float initialRadius_;
  float currentRadius_;

};
#endif // !SUN_H
