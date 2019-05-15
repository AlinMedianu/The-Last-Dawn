// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameModel.h"

GameModel::GameModel()
{
}

GameModel::~GameModel()
{
}

void GameModel::Initialise(Mesh &mesh)
{
  pMesh_ = &mesh;
}

Mesh& GameModel::GetMesh() {
  assert(pMesh_);
  return *pMesh_;
}

MaterialExt* GameModel::HasOverrideMat() {
  if (useOverrideMat_)
    return &overrideMaterial_;
  return nullptr;
}

void GameModel::SetOverrideMat(MaterialExt *pMat) {
  if (!pMat) {
    useOverrideMat_ = false;
    return;
  }
  useOverrideMat_ = true;
  overrideMaterial_ = *pMat;
}

GameModel& GameModel::operator=(const GameModel& m)
{
  pMesh_ = m.pMesh_;
  SetPosition(m.GetPosition());
  SetRotation(m.GetRotation());
  SetScale(m.GetScale());
  overrideMaterial_ = m.overrideMaterial_;
  useOverrideMat_ = m.useOverrideMat_;
  return *this;
}