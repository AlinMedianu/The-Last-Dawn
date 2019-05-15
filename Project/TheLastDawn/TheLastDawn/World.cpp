// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "World.h"

World::World()
  :pHandle_(nullptr), pContext_(nullptr), pMeshmanager_(nullptr), camera_(), sun_(), player_(), pSpawner_(nullptr)
{
}

World::~World()
{
}

void World::Initialise(MeshManager* meshManager, FX::MyFX* handle, ID3D11DeviceContext* context)
{
  assert(meshManager != NULL);
  pMeshmanager_ = meshManager;
  pHandle_ = handle;
  pContext_ = context;

  player_.Initialise(*pMeshmanager_->GetMesh("../bin/data/spaceship/ship.fbx"), kPlayerColliderRadius_);

  sun_.Initialise(*pMeshmanager_->GetMesh("sphere"), kInitialSunRadius_, &player_);

  camera_.Initialise(&player_);

  vector<ID3D11ShaderResourceView*> planetTextures;
  planetTextures.reserve(10);
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Noise.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Earth.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet0.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet1.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet2.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet3.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet4.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet5.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet6.dds", true, gd3dDevice));
  planetTextures.push_back(pHandle_->mCache.LoadTexture("Planet7.dds", true, gd3dDevice));
  pSpawner_ = new Spawner<Planet>(10u, 10.0f, 3.0f, pMeshmanager_->GetMesh("sphere"),
    pMeshmanager_->GetMesh("../bin/data/fuelbarrel/fuelbarrel.fbx"), planetTextures);
  pSpawner_->GetSpawnTriangle().Set(static_cast<Vector2>(player_.GetPosition()),
    Vector2::UnitX, 40.0f, 100.0f);
  pSpawner_->GetSpawnRect().Set(static_cast<Vector2>(camera_.GetPosition()), camera_.GetFOVAngle(),
    sun_.GetPosition().z - camera_.GetPosition().z, 3.0f);
  pSpawner_->GetSpawnRect().SetMultiplier(1.3f);
}

void World::Start()
{
  player_.Start();
  sun_.Start();
  camera_.Start();
  pSpawner_->Start();
}

void World::Update(float dTime)
{
  CheckForCollisions(dTime);
	CheckPlayerBounds();
  player_.Update(dTime);
  sun_.Update(dTime);
  camera_.Update(dTime);
  pSpawner_->Update(dTime, 0.3f, static_cast<Vector2>(camera_.GetPosition()),
    static_cast<Vector2>(player_.GetPosition()),
    static_cast<Vector2>(player_.GetVelocity()));
}

void World::Render(float dTime)
{
  assert(pHandle_ != nullptr && pContext_ != nullptr);
  camera_.PreRender();
  pHandle_->Render(dTime, sun_, pContext_);
  pHandle_->Render(dTime, player_, pContext_);

  for (auto& var : pSpawner_->GetSpawnedObjects())
  {
    pHandle_->Render(dTime, *var, pContext_);
    if (var->GetPickUp() != nullptr) {
      if (var->GetPickUp()->IsActive())
      {
        pHandle_->Render(dTime, *var->GetPickUp(), pContext_);
      }
    }
  }
}

const Sun* const World::GetSun() const
{
  return &sun_;
}

PlayerShip* World::GetPlayer()
{
  return &player_;
}

const PlayerShip* World::GetPlayer() const
{
  return &player_;
}

GameCamera* World::GetCamera()
{
  return &camera_;
}

const GameCamera* World::GetCamera() const
{
  return &camera_;
}

void World::UpdateSpawner()
{
  pSpawner_->GetSpawnRect().SetCameraToRect(sun_.GetPosition().z - camera_.GetPosition().z);
}

void World::Stop()
{
  player_.Stop();
}

void World::Release()
{
  delete pSpawner_;
  pSpawner_ = nullptr;
}

float World::GetDistanceTravelled() const
{
  return player_.GetPosition().Length();
}

void World::CheckForCollisions(float dTime)
{
  DirectX::SimpleMath::Vector3 collision;
  if (SphereToSphere(player_.GetCollider(), sun_.GetCollider(), collision))
  {
    // ship has hit sun, game over
    player_.Die();
    GetModeManager()->SwitchMode("GAMEOVER");
  }
  else
  {
    for (auto& spawnedObject : pSpawner_->GetSpawnedObjects())
    {
      FuelBuff* pickUp = spawnedObject->GetPickUp();
      if (SphereToSphere(player_.GetCollider(), spawnedObject->GetGravityCollider(), collision))
      {
        // Gravity field entered, process accordingly
        if (SphereToSphere(player_.GetCollider(), spawnedObject->GetCoreCollider(), collision))
        {
          //Ship has hit the planet core
          player_.Die();
          GetModeManager()->SwitchMode("GAMEOVER");
        }
        else // no collision with core, apply gravity force
          player_.CheckOrbit(spawnedObject);
      }
      else if (pickUp != nullptr && pickUp->IsActive() &&
        SphereToSphere(player_.GetCollider(), pickUp->GetCollider(), collision))
      {
        player_.CollectFuel(pickUp->GetFuelAmount());
        pickUp->Deplete();
      }
    }
  }
}

void World::CheckPlayerBounds()
{
	if (std::abs(player_.GetPosition().y) >= kPlayerVerticalLimit_) {
		player_.Die();
		GetModeManager()->SwitchMode("GAMEOVER");
	}
}