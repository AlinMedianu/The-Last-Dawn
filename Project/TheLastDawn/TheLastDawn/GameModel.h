// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "GameObject.h"

#include "Mesh.h"

#include <cassert> // For debugging

// 

/// <summary>
/// Provides access to a spawnable gameobject which has a game world location and can hold a Mesh for rendering into the gameworld.
/// <para>
/// Additional behaviour modules will need to be included for movement(MobileBody).
/// </para>
/// </summary>
class GameModel : public GameObject
{
public:
  GameModel();
  ~GameModel();

  void Initialise(Mesh &mesh);
  Mesh& GetMesh();
  MaterialExt* HasOverrideMat();
  void SetOverrideMat(MaterialExt *pMat = nullptr);
  GameModel& operator=(const GameModel& m);

private:

  Mesh *pMesh_ = nullptr;
  MaterialExt overrideMaterial_;
  bool useOverrideMat_ = false;
};

#endif
