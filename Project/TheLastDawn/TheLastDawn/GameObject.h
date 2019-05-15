// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d11.h>        // Prerequesite for SimpleMath
#include "SimpleMath.h"   // For use of Vector3s

/// <summary>
/// Provides access to a spawnable gameobject which has a game world location.
/// <para>
/// No mesh capability included, as this is intended for invisible objects.
/// If you want a game object that does have a renderable game presence, please use derived class GameModel.
/// </para>
/// </summary>
class GameObject
{
public:
  GameObject();
  ~GameObject();

  DirectX::SimpleMath::Vector3 GetPosition() const;
  DirectX::SimpleMath::Vector3 GetScale() const;
  DirectX::SimpleMath::Vector3 GetRotation() const;

  virtual void SetPosition(DirectX::SimpleMath::Vector3 pos);
  virtual void SetPosition(float xPos, float yPos, float zPos);
  virtual void SetRotation(DirectX::SimpleMath::Vector3 rotation);
  virtual void SetRotation(float xRot, float yRot, float zRot);
  virtual void SetScale(DirectX::SimpleMath::Vector3 scale);
  virtual void SetScale(float xScale, float yScale, float zScale);

  void GetWorldMatrix(DirectX::SimpleMath::Matrix& worldMatrix) const;

private:
  DirectX::SimpleMath::Vector3 position_;
  DirectX::SimpleMath::Vector3 rotation_;
  DirectX::SimpleMath::Vector3 scale_;
};

#endif // !GAMEOBJECT_H
