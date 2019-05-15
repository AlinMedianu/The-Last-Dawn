// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "FuelBuff.h"

FuelBuff::FuelBuff()
  : collider_(), pSparkles_(nullptr), active_(false)
{
}

FuelBuff::~FuelBuff()
{
}

void FuelBuff::Initialise(Mesh& mesh, float colliderRadius)
{
  GameModel::Initialise(mesh);
  SetScale(1.0, 1.0, 1.0);
  collider_.Radius = colliderRadius;
  collider_.Center = GetPosition();
  active_ = false;

//  Sparkle();
}

void FuelBuff::Start()
{
  active_ = true;
  Sparkle();
}

const DirectX::BoundingSphere& FuelBuff::GetCollider() const
{
  return collider_;
}

int FuelBuff::GetFuelAmount() const
{
  return kFuelAmount;
}

void FuelBuff::Deplete()
{
  SparkleBurst();
  active_ = false;
  GetIAudioMgr()->GetSfxMgr()->Play("fuelpickup", false, false, 0, LOUD_VOLUME);
}

void FuelBuff::Sparkle()
{
  pSparkles_ = GetParticleSys()->GetNewEmitter();
  if (pSparkles_ != nullptr)
  {
    pSparkles_->data_ = kSparklesEmitterDef;
    pSparkles_->alive_ = true;
    pSparkles_->data_.pos_ = GetPosition();
  }
}

void FuelBuff::SparkleBurst()
{
  if (pSparkles_ != nullptr)
  {
    pSparkles_->lastEmitSec_ = 0; // Set this to zero to make the next set of particle emissions occur instantly! The explosion!
    pSparkles_->data_.mTotal = kNumberOfParticlesInExplosion;
    pSparkles_->data_.mNumEmit = kNumberOfParticlesInExplosion;
    pSparkles_->data_.mInitSpeedMinMax = kExplosionParticleSpeed;
  }
}

bool FuelBuff::IsActive()
{
  return active_;
}

void FuelBuff::SetPosition(float x, float y, float z)
{
	GameObject::SetPosition(Vector3(x, y, z));
	collider_.Center = Vector3(x, y, z);
}

void FuelBuff::SetPosition(Vector3 position)
{
	GameObject::SetPosition(position);
	collider_.Center = position;
}