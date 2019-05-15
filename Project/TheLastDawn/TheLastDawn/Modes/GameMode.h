// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "../Singleton.h"
#include "Mode.h" // For Parent Class construction

#include "SpriteFont.h"
#include "../Mesh.h"
#include "../FX.h"
#include "../Input.h"
#include "../World.h"
#include "../ScrollingImage.h"
#include "../UtilsDefines.h"
#include "../ParticleSys.h"
#include "../Score.h" // For tracking the current games score
#include "../HighScoreManager.h"
#include "../GeometryBuilder.h" // For building the object models
#include "../GameUI.h" // For rendering text/images as screen overlay
#include "../GameModeAudio.h"

/// <summary>
/// The main class of the game, manages and delegates all game related tasks to appropriate classes.
/// </summary>
class GameMode : public Mode, public Singleton<GameMode>
{
public:
  GameMode();
  virtual ~GameMode();

  // Virtual methods from Mode
  void Init() override;
  void Enter() override;
  void Update(float aTime) override;
  void Render(float dTime) override;
  void OnKey(char key) override;
  void Exit() override;

  Score& GetCurrentScore();
  const Score& GetCurrentScore() const;

private:
  GameMode& operator=(const GameMode&) = delete;
  GameMode(const GameMode& m) = delete;

  FX::MyFX& fX_;
  MouseAndKeys& mKInput_;
  Gamepad gamepad_;
  MeshManager meshMgr_;
  World world_;
  vector<ScrollingImage> backgroundImages_;
  ParticleSys* pParticleSys_;
  Score currentScore_;
  GameUI ui_;
  GameModeAudio audio_;

  //light position
  DirectX::SimpleMath::Vector3 lightPos_;
  //text
  DirectX::SpriteBatch *pSpriteBatch_;
  bool isQuitting_;
};

// Declare the static singleton access method
SINGLETON_GET(GameMode);

#endif // !GAMEMODE_H
