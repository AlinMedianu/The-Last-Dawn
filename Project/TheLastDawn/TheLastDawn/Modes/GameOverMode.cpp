// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "GameOverMode.h"

GameOverMode::GameOverMode()
  : Mode("GAMEOVER"), pFx_(FX::GetMyFX()), pGameOverTex_(nullptr), gameOverDim_(DirectX::SimpleMath::Vector2::Zero)
{
}

GameOverMode::~GameOverMode()
{
}

void GameOverMode::Update(float aTime)
{
}

void GameOverMode::Render(float dTime)
{
  BeginRender(Colours::Black);

  DirectX::CommonStates state(gd3dDevice);
  pFx_->mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, state.NonPremultiplied());

  //background
  int w, h;
  GetClientExtents(w, h);
  float scale = h / gameOverDim_.y;
  pFx_->mSpriteBatch->Draw(pGameOverTex_, DirectX::SimpleMath::Vector2((w - gameOverDim_.x * scale) / 2, 0), nullptr, Colours::White, 0, DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(scale, scale));
  pFx_->mSpriteBatch->End();

  EndRender();
}

void GameOverMode::Enter()
{
  Mode::Enter();

  pGameOverTex_ = pFx_->mCache.LoadTexture("./titles/GameOverScreen.dds", true, gd3dDevice);
  gameOverDim_ = pFx_->mCache.Get(pGameOverTex_).dim;
}

void GameOverMode::Exit()
{
  Mode::Exit();
}

void GameOverMode::OnKey(char key)
{
  switch (key)
  {
  case ' ':
    GetModeManager()->SwitchMode("GAME");
    break;
  case 27:
    GetModeManager()->SwitchMode("TITLE");
    break;
  }
}