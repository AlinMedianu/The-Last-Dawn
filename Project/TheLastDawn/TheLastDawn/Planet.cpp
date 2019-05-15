// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Planet.h"

Planet::Planet(Mesh& mesh, float radius, float gravityRadius, Mesh* pPickupMesh)
  : GameModel(), pPickUp_(nullptr), deathTimer_(), planetCore_(GetPosition(), radius), planetGravityRange_(GetPosition(), gravityRadius)
{
  GameModel::Initialise(mesh);
  SetScale(radius, radius, radius);

  if (fuelbuffs_.size() != kNumberOfBuffsToPool)
  {
    PoolFuelBuffs(pPickupMesh);
  }
}

Planet::~Planet()
{
  ReleaseFuelBuffs();
}

void Planet::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
  planetCore_.Center = pos;
  planetGravityRange_.Center = pos;
  GameObject::SetPosition(pos);
}

void Planet::SetPosition(float xPos, float yPos, float zPos)
{
  planetCore_.Center = DirectX::SimpleMath::Vector3(xPos, yPos, zPos);
  planetGravityRange_.Center = DirectX::SimpleMath::Vector3(xPos, yPos, zPos);
  GameObject::SetPosition(xPos, yPos, zPos);
}

const DirectX::BoundingSphere& Planet::GetGravityCollider() const
{
  return planetGravityRange_;
}

const DirectX::BoundingSphere& Planet::GetCoreCollider() const
{
  return planetCore_;
}

void Planet::Reset(float pickUpSpawningRange)
{
  deathTimer_.Reset(Random::Instance()->Range(20.0f, 30.0f));
  //2% chance to spawn a pick up
  if (Random::Instance()->Range(0, 100) > 80)
  {
    pPickUp_ = GetAvailableBuff();
    if (pPickUp_ != nullptr) {
      //spawns the pickup in the vicinity of the planet, 
      //at the specified distance (pickUpSpawningRange)
      float angle = Random::Instance()->Range(0.0f, 2 * PI);
      pPickUp_->SetPosition(GetPosition() +
        pickUpSpawningRange * Vector2(cosf(angle), sinf(angle)));
      pPickUp_->Start();
    }
  }
}

  void Planet::Update(float dTime)
  {
    if (HasOverrideMat()->flags & MaterialExt::TFlags::DISSOLVABLE)
      return;
    if (!deathTimer_.HasStopped())
    {
      deathTimer_.Tick(dTime);
      return;
    }
    MaterialExt* mat = HasOverrideMat();
    mat->flags |= MaterialExt::TFlags::DISSOLVABLE;
    mat->shaderVariables = { static_cast<Vector3>(Colors::Yellow), 0.01f };
    mat->dissolveTimer.Reset(2.0f);
  }

  void Planet::Disintegrate()
  {
    deathTimer_.Stop();
  }

  bool Planet::IsDisintegrating() const
  {
    return deathTimer_.HasStopped();
  }

  bool Planet::IsDisintegrated()
  {
    return HasOverrideMat()->dissolveTimer.HasStopped();
  }

  FuelBuff* Planet::GetPickUp()
  {
    return pPickUp_;
  }

  void Planet::PoolFuelBuffs(Mesh * mesh)
  {
    for (size_t i = 0; i < kNumberOfBuffsToPool; i++)
    {
      FuelBuff* b = new FuelBuff();
      b->Initialise(*mesh, 1);
      fuelbuffs_.push_back(b);
    }
  }

  FuelBuff* Planet::GetAvailableBuff()
  {
    bool found = false;
    int i = 0;
    while (i < kNumberOfBuffsToPool && !found)
    {
      if (!fuelbuffs_.at(i)->IsActive())
      {
        found = true;
        return fuelbuffs_.at(i);
      }
      ++i;
    }
    return nullptr;
  }

  void Planet::ReleaseFuelBuffs()
  {
    for (size_t i = 0; i < fuelbuffs_.size(); i++)
    {
      delete fuelbuffs_.at(i);
    }
    fuelbuffs_.clear();
  }

  vector<FuelBuff*> Planet::fuelbuffs_;

