// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include "GameModel.h"
#include "FuelBuff.h"
#include "Random.h"
#include "Timer.h"


class FuelBuff;

/// <summary>
/// Spawnable game item that the player can use to orbit around and gain speed.
/// <para>
/// Automatically destroy themselves after a period of time with a cool effect!
/// </para>
/// </summary>
class Planet : public GameModel
{
public:
  Planet(Mesh& mesh, float radius, float gravityRadius, Mesh* pPickupMesh);
  ~Planet();

  void SetPosition(DirectX::SimpleMath::Vector3 pos);
  void SetPosition(float xPos, float yPos, float zPos);

  const DirectX::BoundingSphere& GetGravityCollider() const;
  const DirectX::BoundingSphere& GetCoreCollider() const;

  void Reset(float pickUpSpawningRange);
  void Update(float dTime);
  void Disintegrate();

  bool IsDisintegrating() const;
  bool IsDisintegrated();

  FuelBuff* GetPickUp();
private:
  FuelBuff* pPickUp_;
  Timer deathTimer_;
  DirectX::BoundingSphere planetCore_;
  DirectX::BoundingSphere planetGravityRange_;

  const int kNumberOfBuffsToPool = 4;
  static vector<FuelBuff*> fuelbuffs_;

  void PoolFuelBuffs(Mesh* mesh);
  FuelBuff* GetAvailableBuff();
  void ReleaseFuelBuffs();

};

#endif // !PLANET_H
