// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef MOBILEBODY_H
#define MOBILEBODY_H

#include <d3d11.h>      // Prerequisite for SimpleMath
#include <SimpleMath.h> // For access to Vector3

#include "GameObject.h" // For setting the position of the gameobject

/// <summary>
/// Stores the objects velocity and uses it to update the position in the game world.
/// </summary>
class MobileBody
{
public:
  MobileBody(GameObject* pGameObject);
  ~MobileBody();

  DirectX::SimpleMath::Vector3 GetVelocity() const;
  void AddVelocity(DirectX::SimpleMath::Vector3 velocity);
  void IncreaseVelocity(float velocity);
  void SetVelocity(DirectX::SimpleMath::Vector3 rotation);

  DirectX::SimpleMath::Vector3 GetDirectionVector() const;
  float GetZAxisRotation() const;

  void Update(float dTime);

private:
  GameObject* pGameObject_;
  DirectX::SimpleMath::Vector3 velocity_;
};

#endif // !MOBILEBODY_H
