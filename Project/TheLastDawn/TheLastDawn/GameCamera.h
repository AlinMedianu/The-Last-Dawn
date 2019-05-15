// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include <d3d11.h>      // Prerequisite for SimpleMath
#include "SimpleMath.h" // For use of Vector3
#include "D3D.h"        // For access to the g3gcontext
#include "FX.h"         // For updating the per frame constants

#include "GameObject.h"

/// <summary>
/// Provides a follow camera as main functionality. Ability to request new targets to follow.
/// <para>
/// Also provides debug camera functionality when enabled.
/// </para>
/// </summary>
class GameCamera
{
public:
  enum CamDirection { UP, DOWN, LEFT, RIGHT, FORWARD, BACK };

  GameCamera();
  ~GameCamera();

  void Initialise(GameObject* player);
  void Start();
  DirectX::SimpleMath::Vector3 GetPosition();  // ***CHARLIE - Debug function to get camera position modifiable
  DirectX::SimpleMath::Vector3 GetPosition() const;
  float GetFOVAngle() const;
  void Update(float dTime);
  void PreRender() const;
  bool IsFollowEnabled() const;
  void EnableFollow(GameObject* objectToFollow);
  void DisableFollow();
  void MoveStaticCamera(CamDirection dir, float dTime);

private:
  const DirectX::SimpleMath::Vector3 kDefCamPos_ = DirectX::SimpleMath::Vector3(0, 0, -75);
  const DirectX::SimpleMath::Vector3 kDefFollowCamPos_ = DirectX::SimpleMath::Vector3::Zero;
  const DirectX::SimpleMath::Vector3 kDefCameraLookAt_ = DirectX::SimpleMath::Vector3::Zero;
  const DirectX::SimpleMath::Vector3 kDefCameraUpDirection_ = DirectX::SimpleMath::Vector3::Up;
  const DirectX::SimpleMath::Vector3 kDefStaticCamOffset_ = DirectX::SimpleMath::Vector3::Zero;
  const DirectX::SimpleMath::Vector3 kDefFollowCamOffset_ = DirectX::SimpleMath::Vector3(0, 0, -50);
  const float kStaticCameraMovementIncrement = 30.0f;
  const float kFieldOfViewDegrees_ = 45.0f;
  const float kNearClippingPlane_ = 1.0f;
  const float kFarClippingPlane_ = 1000.0f;
  // ***CHARLIE - Limit for camera y position, and deadzone for ship movement
  const float kVerticalLimit_ = 25.0f;
  const float kVerticalDeadzone_ = 15.0f;
  const float kHorizontalDeadzone_ = 20.0f;
  const float kDeadzoneSizeH_ = 5.0f;
  const float kDeadzoneSizeV_ = 5.0f;

  GameObject * pObjectToFollow_;
  DirectX::SimpleMath::Vector3 cameraPosition_;
  DirectX::SimpleMath::Vector3 cameraLookAt_;
  DirectX::SimpleMath::Vector3 staticCamOffset_;
  DirectX::SimpleMath::Vector3 followCamOffset_;
  bool followCameraEnabled_;

  void SetupViewMatrix() const;
  void SetupProjectionMatrix() const;
  void MoveFollowerCamera(DirectX::SimpleMath::Vector3 newPosition);
  void UpdateLookAtPoint();

  // ***CHARLIE - Clamp vector3 to vertical limit
  const DirectX::SimpleMath::Vector3 ClampPosition(const DirectX::SimpleMath::Vector3 & pos) const;
};

#endif // !GAMECAMERA_H