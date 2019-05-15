// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameCamera.h"

GameCamera::GameCamera()
  : pObjectToFollow_(nullptr), cameraPosition_(kDefCamPos_), cameraLookAt_(kDefCameraLookAt_), staticCamOffset_(kDefStaticCamOffset_), followCamOffset_(kDefFollowCamOffset_), followCameraEnabled_(false)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::Initialise(GameObject* pPlayer)
{
  pObjectToFollow_ = pPlayer;
}

void GameCamera::Start()
{
  if (pObjectToFollow_ != nullptr)
  {
    EnableFollow(pObjectToFollow_);
  }
}

DirectX::SimpleMath::Vector3 GameCamera::GetPosition()
{
  return cameraPosition_;
}

DirectX::SimpleMath::Vector3 GameCamera::GetPosition() const
{
  return cameraPosition_;
}

void GameCamera::PreRender() const
{
  SetupViewMatrix();
  SetupProjectionMatrix();
}

void GameCamera::Update(float dTime)
{
  if (followCameraEnabled_)
  {
    DirectX::SimpleMath::Vector3 newPos = pObjectToFollow_->GetPosition() + followCamOffset_;
    MoveFollowerCamera(newPos);
    cameraPosition_ = ClampPosition(cameraPosition_);
  }
  UpdateLookAtPoint();
}

void GameCamera::MoveFollowerCamera(DirectX::SimpleMath::Vector3 newPosition)
{
  DirectX::SimpleMath::Vector3 localOrigin = cameraLookAt_;

  if (pObjectToFollow_->GetPosition().x <= localOrigin.x - kDeadzoneSizeH_)
    cameraPosition_.x = newPosition.x + kDeadzoneSizeH_;
  else if (pObjectToFollow_->GetPosition().x >= localOrigin.x + kDeadzoneSizeH_)
    cameraPosition_.x = newPosition.x - kDeadzoneSizeH_;

  if (pObjectToFollow_->GetPosition().y <= localOrigin.y - kDeadzoneSizeV_)
    cameraPosition_.y = newPosition.y + kDeadzoneSizeV_;
  else if (pObjectToFollow_->GetPosition().y >= localOrigin.y + kDeadzoneSizeV_)
    cameraPosition_.y = newPosition.y - kDeadzoneSizeV_;

  cameraPosition_.z = newPosition.z;
}

void GameCamera::UpdateLookAtPoint()
{
  cameraLookAt_ = cameraPosition_;
  cameraLookAt_.z = 0;
}

bool GameCamera::IsFollowEnabled() const
{
  return followCameraEnabled_;
}

void GameCamera::EnableFollow(GameObject * objectToFollow)
{
  followCameraEnabled_ = true;
  pObjectToFollow_ = objectToFollow;
  cameraPosition_ = objectToFollow->GetPosition() + kDefFollowCamPos_ + followCamOffset_;
  cameraLookAt_ = objectToFollow->GetPosition();
}

void GameCamera::DisableFollow()
{
  pObjectToFollow_ = nullptr;
  followCameraEnabled_ = false;
}

void GameCamera::MoveStaticCamera(CamDirection dir, float dTime)
{
  if (dir == GameCamera::UP)
    cameraPosition_.y += kStaticCameraMovementIncrement * dTime;
  if (dir == GameCamera::DOWN)
    cameraPosition_.y -= kStaticCameraMovementIncrement * dTime;
  if (dir == GameCamera::LEFT)
    cameraPosition_.x -= kStaticCameraMovementIncrement * dTime;
  if (dir == GameCamera::RIGHT)
    cameraPosition_.x += kStaticCameraMovementIncrement * dTime;
  if (dir == GameCamera::FORWARD)
    cameraPosition_.z += kStaticCameraMovementIncrement * dTime;
  if (dir == GameCamera::BACK)
    cameraPosition_.z -= kStaticCameraMovementIncrement * dTime;
  cameraLookAt_ = GetPosition() + DirectX::SimpleMath::Vector3::Backward;
}

float GameCamera::GetFOVAngle() const
{
  return kFieldOfViewDegrees_;
}

void GameCamera::SetupViewMatrix() const
{
  FX::SetPerFrameConsts(gd3dImmediateContext, cameraPosition_);
  CreateViewMatrix(FX::GetViewMatrix(), cameraPosition_, cameraLookAt_, kDefCameraUpDirection_);
}

void GameCamera::SetupProjectionMatrix() const
{
  CreateProjectionMatrix(FX::GetProjectionMatrix(), DirectX::XMConvertToRadians(kFieldOfViewDegrees_), GetAspectRatio(), kNearClippingPlane_, kFarClippingPlane_);
}

const DirectX::SimpleMath::Vector3 GameCamera::ClampPosition(const DirectX::SimpleMath::Vector3 & pos) const
{
  DirectX::SimpleMath::Vector3 newPos = pos;

  if (pos.y > kVerticalLimit_)
    newPos.y = kVerticalLimit_;
  else if (pos.y < -kVerticalLimit_)
    newPos.y = -kVerticalLimit_;

  return newPos;
}