// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameObject.h"

GameObject::GameObject()
  :position_(DirectX::SimpleMath::Vector3::Zero), rotation_(DirectX::SimpleMath::Vector3::Zero), scale_(DirectX::SimpleMath::Vector3::One)
{
}

GameObject::~GameObject()
{
}

DirectX::SimpleMath::Vector3 GameObject::GetPosition() const {
  return position_;
}

DirectX::SimpleMath::Vector3 GameObject::GetScale() const {
  return scale_;
}

DirectX::SimpleMath::Vector3 GameObject::GetRotation() const {
  return rotation_;
}

void GameObject::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
  position_ = pos;
}

void GameObject::SetPosition(float xPos, float yPos, float zPos)
{
  position_.x = xPos;
  position_.y = yPos;
  position_.z = zPos;
}

void GameObject::SetRotation(DirectX::SimpleMath::Vector3 rotation)
{
  rotation_ = rotation;
}

void GameObject::SetRotation(float xRot, float yRot, float zRot)
{
  rotation_.x = xRot;
  rotation_.y = yRot;
  rotation_.z = zRot;
}

void GameObject::SetScale(DirectX::SimpleMath::Vector3 scale)
{
  scale_ = scale;
}

void GameObject::SetScale(float xScale, float yScale, float zScale)
{
  scale_.x = xScale;
  scale_.y = yScale;
  scale_.z = zScale;
}

void GameObject::GetWorldMatrix(DirectX::SimpleMath::Matrix& worldMatrix) const
{
  worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale_) *
    DirectX::SimpleMath::Matrix::CreateRotationX(rotation_.x) *
    DirectX::SimpleMath::Matrix::CreateRotationY(rotation_.y) *
    DirectX::SimpleMath::Matrix::CreateRotationZ(rotation_.z) *
    DirectX::SimpleMath::Matrix::CreateTranslation(position_);
}
