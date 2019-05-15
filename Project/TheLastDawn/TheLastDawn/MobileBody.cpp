// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "MobileBody.h"

MobileBody::MobileBody(GameObject* pGameObject)
  : pGameObject_(pGameObject), velocity_(DirectX::SimpleMath::Vector3::Zero)
{
}

MobileBody::~MobileBody()
{
}

void MobileBody::AddVelocity(DirectX::SimpleMath::Vector3 velocity)
{
  velocity_ += velocity;
}

void MobileBody::IncreaseVelocity(float velocity)
{
	DirectX::SimpleMath::Vector3 direction = GetVelocity();
	direction.Normalize();
	direction *= velocity;
	AddVelocity(direction);
}

DirectX::SimpleMath::Vector3 MobileBody::GetVelocity() const
{
	return velocity_;
}

void MobileBody::SetVelocity(DirectX::SimpleMath::Vector3 rotation)
{
	velocity_ = rotation;
}

DirectX::SimpleMath::Vector3 MobileBody::GetDirectionVector() const
{
  DirectX::SimpleMath::Vector3 temp(velocity_);
  temp.Normalize();
  return temp;
}

float MobileBody::GetZAxisRotation() const
{



  return 0.0f;
}

void MobileBody::Update(float dTime)
{
  pGameObject_->SetPosition(pGameObject_->GetPosition() + velocity_ * dTime);
}

