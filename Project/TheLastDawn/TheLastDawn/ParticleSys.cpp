// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include <vector>

#include "ParticleSys.h"
#include "D3D.h"
#include "D3DUtil.h"
#include "FX.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//sprites in the atlas (particle.dds)
const int MAX_SPRITES = 11;
ParticleSys::TexRect particleTex[]{
  { 160 / 2048.f, 64 / 1024.f, 138 / 2048.f, 138 / 1024.f },
  { 304 / 2048.f, 76 / 1024.f, 105 / 2048.f, 105 / 1024.f },
  { 424 / 2048.f, 82 / 1024.f, 100 / 2048.f, 100 / 1024.f },
  { 554 / 2048.f, 86 / 1024.f, 92 / 2048.f, 92 / 1024.f },
  { 673 / 2048.f, 80 / 1024.f, 106 / 2048.f, 106 / 1024.f },
  { 793 / 2048.f, 87 / 1024.f, 110 / 2048.f, 110 / 1024.f },
  { 1221 / 2048.f, 842 / 1024.f, 130 / 2048.f, 130 / 1024.f },
  { 25 / 2048.f, 865 / 1024.f, 124 / 2048.f, 100 / 1024.f },
  { 160 / 2048.f, 811 / 1024.f, 145 / 2048.f, 145 / 1024.f },
  { 1373 / 2048.f, 848 / 1024.f, 122 / 2048.f, 122 / 1024.f },
  { 647 / 2048.f, 493 / 1024.f, 120 / 2048.f, 120 / 1024.f }
};


//template for particle geometry - just a quad
const VertexPosTex quad[] =
{	//quad in the XY plane
  { Vector3(-0.5f, 0.5f, 0), Vector2(0, 0) },
  { Vector3(0.5f, 0.5f, 0), Vector2(1, 0) },
  { Vector3(0.5f, -0.5f, 0), Vector2(1, 1) },
  { Vector3(-0.5f, -0.5f, 0), Vector2(0, 1) }
};

void ParticleSys::ReleasePS()
{
  emitters_.clear();
  particles_.clear();
  ReleaseCOM(pVBuffer_);
  ReleaseCOM(pIBuffer_);
}

ParticleSys::ParticleSys(FX::MyFX* fx)
  : pFreeList_(nullptr), pBusyList_(nullptr), emitters_(), particles_(), pVBuffer_(nullptr), pIBuffer_(nullptr), sprites_(), pFx_(fx)
{
}

ParticleSys::~ParticleSys()
{
  ReleasePS();
}

void ParticleSys::InitialisePS()
{
  //particle cache
  Particle default;
  particles_.insert(particles_.begin(), kMAX_PARTICLES, default);
  for (int i = 0; i < kMAX_PARTICLES; ++i)
  {
    if (i < kMAX_PARTICLES - 1)
      particles_[i].pNext_ = &particles_[i + 1];
    if (i > 0)
      particles_[i].pPrev_ = &particles_[i - 1];
  }
  pFreeList_ = &particles_[0];
  pBusyList_ = nullptr;

  //vertex buffer
  VertexPosTex* vertices = new VertexPosTex[kMAX_PARTICLES * 4];
  for (int i = 0; i < kMAX_PARTICLES; ++i)
  {
    int offset = i * 4;
    memcpy(&vertices[offset], quad, sizeof(VertexPosTex) * 4);
  }
  CreateVertexBuffer(sizeof(VertexPosTex) * kMAX_PARTICLES * 4, vertices, pVBuffer_, true);
  delete[] vertices;

  //index buffer
  int nInd = kMAX_PARTICLES * 2 * 3;
  unsigned int* indices = new unsigned int[nInd];
  for (int i = 0; i < kMAX_PARTICLES; ++i)
  {
    int iIdx = i * 2 * 3;
    int vIdx = i * 4;
    indices[iIdx++] = vIdx;
    indices[iIdx++] = vIdx + 1;
    indices[iIdx++] = vIdx + 2;

    indices[iIdx++] = vIdx;
    indices[iIdx++] = vIdx + 2;
    indices[iIdx++] = vIdx + 3;
  }
  CreateIndexBuffer(nInd * sizeof(unsigned int), indices, pIBuffer_);
  delete[] indices;

  emitters_.insert(emitters_.begin(), kMAX_EMITTERS, Emitter());

  GetSpriteLibrary().insert(GetSpriteLibrary().begin(), &particleTex[0], &particleTex[MAX_SPRITES]);
}

void ParticleSys::UpdateParticles(float dTime, const Vector3 & camPos)
{
  for (auto& e : emitters_)
    if (e.alive_)
      e.Update(this);

  if (!pBusyList_)
    return;

  D3D11_MAPPED_SUBRESOURCE mappedResource;
  ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
  HR(gd3dImmediateContext->Map(pVBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
  assert(mappedResource.pData);
  numParticles_ = 0;
  Particle * p = pBusyList_;
  while (p)
  {
    assert(p->flags_ & Particle::ALIVE);
    //simulate
    p->pos_ += p->vel_ * dTime;
    if (p->flags_ & Particle::USE_GRAVITY)
      p->vel_ += Vector3(0, Particle::kGRAVITY * dTime, 0);
    p->lifeSec_ -= dTime;
    p->angle_ += p->rotSpeed_ * dTime;
    //is it dead?
    if (p->lifeSec_ <= 0)
    {
      Particle* pOld = p;
      p = p->pNext_;
      pOld->flags_ &= ~Particle::ALIVE;
      //cut out
      if (pOld->pNext_)
        pOld->pNext_->pPrev_ = pOld->pPrev_;
      if (pOld->pPrev_)
        pOld->pPrev_->pNext_ = pOld->pNext_;
      else
      {
        assert(pBusyList_ == pOld);
        pBusyList_ = pOld->pNext_;
      }
      //insert
      if (pFreeList_)
        pFreeList_->pPrev_ = pOld;
      pOld->pPrev_ = nullptr;
      pOld->pNext_ = pFreeList_;
      pFreeList_ = pOld;
    }
    else
    {
      //render it
      VertexPosTex* pV = static_cast<VertexPosTex*>(mappedResource.pData);
      int vIdx = numParticles_ * 4;
      Matrix mtx = Matrix::CreateRotationZ(p->angle_);
      Matrix w = mtx * Matrix::CreateBillboard(p->pos_, camPos, FX::GetViewMatrix().Up(), &FX::GetViewMatrix().Forward());
      for (int i = 0; i < 4; ++i)
      {
        pV[vIdx + i].Pos = Vector3::Transform((quad[i].Pos * p->scale_), w);
      }
      TexRect& tr = sprites_[p->spriteIdx_];
      pV[vIdx].Tex = Vector2(tr.u, tr.v);
      pV[vIdx + 1].Tex = Vector2(tr.u + tr.w, tr.v);
      pV[vIdx + 2].Tex = Vector2(tr.u + tr.w, tr.v + tr.h);
      pV[vIdx + 3].Tex = Vector2(tr.u, tr.v + tr.h);
      numParticles_++;
      p = p->pNext_;
    }
  }
  gd3dImmediateContext->Unmap(pVBuffer_, 0);
}

Emitter * ParticleSys::GetNewEmitter()
{
  for (auto& e : emitters_)
    if (!e.alive_)
    {
      e.alive_ = true;
      return &e;
    }
  return nullptr;
}

void ParticleSys::Render(float dTime)
{
  MaterialExt mat;
  mat.flags |= MaterialExt::TFlags::PARTICLE;
  mat.flags |= MaterialExt::TFlags::ADDITIVE;
  mat.flags &= ~MaterialExt::TFlags::ZTEST;
  mat.pTextureRV = pFx_->mCache.LoadTexture("particles.dds", true, gd3dDevice);

  if (numParticles_ > 0)
    pFx_->RenderParticles(dTime, pVBuffer_, pIBuffer_, gd3dImmediateContext, 0, numParticles_, Matrix::Identity, mat);

}