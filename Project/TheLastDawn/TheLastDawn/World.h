// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef WORLD_H
#define WORLD_H

#include "Mesh.h" // For assigning the correct models to objects
#include "Sun.h" // For spawning the sun
#include "PlayerShip.h" // For spawning the player
#include "Modes/ModeManager.h" // For changing the mode upon death/quit
#include "GameCamera.h" // For viewing the scene
#include "Planet.h" // For spawning the planets
#include "Spawner.h" // For spawning the planets

#include <cassert> // For debugging

/// <summary>
/// Contains all elements of the game world and detects collisions between them.
/// </summary>
class World
{
public:
  World();
  ~World();

  void Initialise(MeshManager* meshManager, FX::MyFX* handle, ID3D11DeviceContext* context);
  void Start();
  const Sun * const GetSun() const;
  PlayerShip* GetPlayer();
  const PlayerShip* GetPlayer() const;
  GameCamera* GetCamera();
  const GameCamera* GetCamera() const;
  float GetDistanceTravelled() const;
  void Update(float dTime);
  void UpdateSpawner();
  void Render(float dTime);
  void Stop();
  void Release();

private:
  const float kInitialSunRadius_ = 20.0f;
  const float kPlayerColliderRadius_ = 0.5f;
	const float kPlayerVerticalLimit_ = 55.0f;

  FX::MyFX* pHandle_;
  ID3D11DeviceContext* pContext_;
  MeshManager* pMeshmanager_;
  GameCamera camera_;
  PlayerShip player_;
  Sun sun_;
  Spawner<Planet>* pSpawner_;

  void CheckForCollisions(float dTime);
	void CheckPlayerBounds();
};
#endif // !WORLD_H
