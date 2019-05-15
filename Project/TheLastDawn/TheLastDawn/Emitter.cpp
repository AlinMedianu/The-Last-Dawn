// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Emitter.h"

void Emitter::Update(ParticleSys* ps)
{
  if (data_.mTotal == 0)
  {
    if (data_.mReleaseOnEmpty)
      StopAndRelease();
    return;
  }
  float elapsed = GetClock() - lastEmitSec_;
  if (elapsed < data_.mEmitDelaySec)
    return;
  lastEmitSec_ = GetClock();

  if (data_.mTotal == -1)
  {
    for (int i = 0; i < data_.mNumEmit; ++i)
      CreateParticle(ps);
  }
  else
  {
    int n = (data_.mTotal > data_.mNumEmit) ? data_.mNumEmit : data_.mTotal;
    for (int i = 0; i < n; ++i)
      if (!CreateParticle(ps))
        --n;
    data_.mTotal -= n;
  }
}

void Emitter::StopAndRelease()
{
  alive_ = false;
}

bool Emitter::CreateParticle(ParticleSys * ps)
{
  Particle*& pFreeList = ps->GetFreeList();
  if (!pFreeList)
    return false;
  //remove
  Particle* p = pFreeList;
  pFreeList = p->pNext_;
  if (pFreeList)
    pFreeList->pPrev_ = nullptr;
  //insert
  Particle*& pBusyList = ps->GetBusyList();
  if (pBusyList)
    pBusyList->pPrev_ = p;
  p->pNext_ = pBusyList;
  p->pPrev_ = nullptr;
  pBusyList = p;

  //initialise
  assert((p->flags_ & Particle::ALIVE) == false);
  Random * rng = Random::Instance();
  p->lifeSec_ = rng->Range(data_.mInitLifeSecMinMax.x, data_.mInitLifeSecMinMax.y);
  p->pos_ = data_.pos_;
  p->spawnSec_ = GetClock();
  p->spriteIdx_ = data_.spriteIdx_;


  switch (data_.mType)
  {
  case Type::OMNI:
  {
    p->vel_ = Vector3(0, rng->Range(data_.mInitSpeedMinMax.x, data_.mInitSpeedMinMax.y), 0);
    Matrix mtx = Matrix::CreateRotationZ(rng->Range(0.f, 2 * PI)) * Matrix::CreateRotationX(rng->Range(0.f, 2 * PI));
    p->vel_ = Vector3::TransformNormal(p->vel_, mtx);
    break;
  }
  case Type::DIRECTION:
  {
    if (data_.mInitAngleMinMax.x == 0 && data_.mInitAngleMinMax.y == 0)
    {
      p->vel_ = Vector3::Transform(Vector3(0, 1, 0), data_.mOrientation);
    }
    else
    {
      //normal random inside cone
      p->vel_ = Vector3(0, 1, 0);
      Matrix mtx = Matrix::CreateRotationZ(rng->Range(data_.mInitAngleMinMax.x, data_.mInitAngleMinMax.y));
      p->vel_ = Vector3::TransformNormal(p->vel_, mtx);
      mtx = Matrix::CreateRotationY(rng->Range(0.f, 2 * PI));
      p->vel_ = Vector3::TransformNormal(p->vel_, mtx);
      p->vel_ = Vector3::Transform(p->vel_, data_.mOrientation);
    }
    p->vel_ *= rng->Range(data_.mInitSpeedMinMax.x, data_.mInitSpeedMinMax.y);
    break;
  }
  default:
    assert(false);
    break;
  }

  p->flags_ = data_.mParticleFlags | Particle::ALIVE;
  p->scale_ = rng->Range(data_.mScaleMinMax.x, data_.mScaleMinMax.y);

  p->angle_ = rng->Range(data_.mInitRotMinMax.x, data_.mInitRotMinMax.y);
  p->rotSpeed_ = rng->Range(data_.mRotSpMinMax.x, data_.mRotSpMinMax.y);
  return true;
}
