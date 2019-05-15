// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameMode.h"

GameMode::GameMode()
  : Mode("GAME"), fX_(*FX::GetMyFX()), mKInput_(*GetMouseAndKeys()), gamepad_(), meshMgr_(),
    world_(), backgroundImages_(), pParticleSys_(GetParticleSys()), currentScore_(),
    ui_(&currentScore_, &world_), audio_(), lightPos_(DirectX::SimpleMath::Vector3::Zero),
    pSpriteBatch_(nullptr), isQuitting_(false)
{
}

GameMode::~GameMode()
{
  meshMgr_.Release();
  world_.Release();
  Mode::~Mode();
}

void GameMode::Init()
{
  pSpriteBatch_ = fX_.mSpriteBatch;
  assert(pSpriteBatch_);

  fX_.mCache.LoadTexture("Star Background.dds", true, gd3dDevice);
  backgroundImages_.emplace_back(fX_.mCache.Get(fX_.mCache.GetIdx("Star Background.dds")));
  fX_.mCache.LoadTexture("Star Foreground.dds", true, gd3dDevice);
  backgroundImages_.emplace_back(fX_.mCache.Get(fX_.mCache.GetIdx("Star Foreground.dds")));

  Mesh& ms = meshMgr_.CreateMesh("spaceship");
  ms.CreateFrom("../bin/data/spaceship/ship.fbx", gd3dDevice, fX_.mCache);

  BuildSphere(meshMgr_, 128, 128);

  Mesh& ms2 = meshMgr_.CreateMesh("fuel");
  ms2.CreateFrom("../bin/data/fuelbarrel/fuelbarrel.fbx", gd3dDevice, fX_.mCache);

  world_.Initialise(&meshMgr_, &fX_, gd3dImmediateContext);
  ui_.Init();
  gamepad_.Initialise();
}

void GameMode::Enter()
{
  Mode::Enter();

  isQuitting_ = false;
  world_.GetCamera()->EnableFollow(world_.GetPlayer());
  currentScore_.ResetScore(world_.GetPlayer()->GetMaximumFuel());
  world_.Start();
  audio_.PlayBGM();
}

//main update coordinates a lot of work
void GameMode::Update(float dTime)
{
  gamepad_.Update();

  if (world_.GetCamera()->IsFollowEnabled())
  {
    if (mKInput_.IsPressed(VK_W))
      world_.GetPlayer()->Boost(dTime);
    if (mKInput_.IsPressed(VK_A))
      world_.GetPlayer()->DirectionalBoost(1, dTime);
    else if (mKInput_.IsPressed(VK_D))
      world_.GetPlayer()->DirectionalBoost(-1, dTime);
    world_.Update(dTime);

    currentScore_.Update(dTime, world_.GetDistanceTravelled(), world_.GetPlayer()->GetFuel());
    pParticleSys_->UpdateParticles(dTime, world_.GetCamera()->GetPosition());
  }
  else
  {
    if (mKInput_.IsPressed(VK_A))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::LEFT, dTime);
    if (mKInput_.IsPressed(VK_D))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::RIGHT, dTime);
    if (mKInput_.IsPressed(VK_Z))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::FORWARD, dTime);
    if (mKInput_.IsPressed(VK_X))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::BACK, dTime);
    if (mKInput_.IsPressed(VK_W))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::UP, dTime);
    if (mKInput_.IsPressed(VK_S))
      world_.GetCamera()->MoveStaticCamera(GameCamera::CamDirection::DOWN, dTime);
  }

  Mode::Update(dTime); // Using this here to update the audio manager as its common to all modes.
}

void GameMode::Render(float dTime)
{
  BeginRender(Colours::White);

  FX::SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.8f, 0.8f, 0.8f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));

  int windowWidth, windowHeight;
  GetClientExtents(windowWidth, windowHeight);
  CommonStates state(gd3dDevice);

  pSpriteBatch_->Begin(SpriteSortMode_Deferred, state.NonPremultiplied(), state.AnisotropicWrap());
  pSpriteBatch_->Draw(backgroundImages_.at(0).GetTextureData().pTex, Vector2::Zero,
    &backgroundImages_.at(0).GetScrollingRect(static_cast<Vector2>(world_.GetCamera()->GetPosition() * 10)),
    Colours::White, 0, Vector2::Zero, backgroundImages_.at(0).GetScale(windowWidth, windowHeight));
  pSpriteBatch_->End();
  pSpriteBatch_->Begin(SpriteSortMode_Deferred, state.Additive(), state.AnisotropicWrap());
  pSpriteBatch_->Draw(backgroundImages_.at(1).GetTextureData().pTex, Vector2::Zero,
    &backgroundImages_.at(1).GetScrollingRect(static_cast<Vector2>(world_.GetCamera()->GetPosition() * 20)),
    Colours::White, 0, Vector2::Zero, backgroundImages_.at(1).GetScale(windowWidth, windowHeight));
  pSpriteBatch_->End();

  //this is just to switch off from additive mode
  pSpriteBatch_->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
  pSpriteBatch_->End();

  pParticleSys_->Render(dTime);
  world_.Render(dTime);

  ui_.Render(dTime);
  EndRender();

  mKInput_.PostProcess();
}

void GameMode::OnKey(char key)
{
  switch (key)
  {
  case 'p':
  case 'P':
    if (world_.GetCamera()->IsFollowEnabled())
      world_.GetCamera()->DisableFollow();
    else
      world_.GetCamera()->EnableFollow(world_.GetPlayer());
    world_.UpdateSpawner();
    break;
  case 27:
    isQuitting_ = true;
		GetModeManager()->SwitchMode("TITLE");
    break;
  }
}

//game is exiting so tidy up
void GameMode::Exit()
{
  world_.Stop();
  Mode::Exit();
  if (!isQuitting_)
  {
    if (GetHighScoreManager()->IsHighScore(currentScore_))
    {
      audio_.StopBGM();
      GetModeManager()->SwitchMode("NAMEENTRY");
    }
  }
  else {
    GetModeManager()->SwitchMode("GAMEOVER");
  }
}

Score& GameMode::GetCurrentScore()
{
  return currentScore_;
}

const Score& GameMode::GetCurrentScore() const
{
  return currentScore_;
}
