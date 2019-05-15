// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Sun.h"

Sun::Sun()
  : GameModel(), collider_(), pPlayer_(nullptr)
{

}

Sun::~Sun()
{

}

void Sun::Initialise(Mesh& mesh, float minRadius, PlayerShip* player)
{
  GameModel::Initialise(mesh);
  MaterialExt sunMaterial = mesh.GetSubMesh(0).material_;
  sunMaterial.flags &= ~MaterialExt::TFlags::LIT;
  sunMaterial.flags |=  MaterialExt::TFlags::TESSELLATED;
  sunMaterial.gfxData.Emissive = Vector4(1.0f, 0.4f, 0.0f, 1.0f);
  sunMaterial.texTrsfm.scale = 10 * Vector2::One;
  SetOverrideMat(&sunMaterial);
  currentRadius_ = minRadius;
  initialRadius_ = minRadius;
  SetPosition(kInitialSunPosition_);
  collider_.Center = GetPosition();
  SetScale(minRadius, minRadius, minRadius);
  collider_.Radius = minRadius;
  pPlayer_ = player;
}

void Sun::Start()
{
  currentRadius_ = initialRadius_;
  SetScale(initialRadius_, initialRadius_, initialRadius_);
  collider_.Radius = initialRadius_;
}

void Sun::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
  collider_.Center = pos;
  GameObject::SetPosition(pos);
}

void Sun::SetPosition(float xPos, float yPos, float zPos)
{
  collider_.Center = DirectX::SimpleMath::Vector3(xPos, yPos, zPos);
  GameObject::SetPosition(xPos, yPos, zPos);
}

void Sun::Update(float dTime)
{
  Vector3 expansionRate = pPlayer_->GetVelocity();
  float exp = sqrt(expansionRate.LengthSquared());

  Vector3 distance = pPlayer_->GetPosition() - GetPosition();
  float dist = sqrt(distance.LengthSquared()) - currentRadius_;

  currentRadius_ += dTime * exp * (dist > kDesiredDistanceAwayFromPlayer ? kExpansionSpeedWhenTooFarAway : kExpansionSpeedWhenCloseToPlayer);
  SetScale(currentRadius_, currentRadius_, 1);
  collider_.Radius = currentRadius_;
}

float Sun::GetCurrentRadius() const
{
  return currentRadius_;
}

const DirectX::BoundingSphere& Sun::GetCollider() const
{
  return collider_;
}
