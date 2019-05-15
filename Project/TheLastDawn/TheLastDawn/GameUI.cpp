#include "GameUI.h"

GameUI::GameUI(const Score* const gameScore, const World* const world)
  : pSpriteBatch_(nullptr), pFont_(nullptr), pFont2_(nullptr),
    pBar_(nullptr), pBarFrame_(nullptr), pGameScore_(gameScore), pGameWorld_(world), initComplete_(false)
{
}

GameUI::~GameUI()
{
  delete pSpriteBatch_;
  delete pFont_;
  delete pFont2_;
}

void GameUI::Init()
{
  pSpriteBatch_ = new SpriteBatch(gd3dImmediateContext);
  assert(pSpriteBatch_);

  pFont_ = new SpriteFont(gd3dDevice, L"../bin/data/coldwarm24.spritefont");
  assert(pFont_);

  pFont2_ = new SpriteFont(gd3dDevice, L"../bin/data/coldwarm48.spritefont");
  assert(pFont2_);

  FX::MyFX& fx = *FX::GetMyFX();
  pBar_ = fx.mCache.LoadTexture("./fuelBar/fuelBar.dds", true, gd3dDevice);
  pBarFrame_ = fx.mCache.LoadTexture("./fuelBar/fuelFrame.dds", true, gd3dDevice);
  assert(pBar_);
  assert(pBarFrame_);

  initComplete_ = true;
}

void GameUI::Render(float dTime)
{
  CommonStates state(gd3dDevice);

  pSpriteBatch_->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
  //general messages
  wstringstream ss;

  /*DrawGameStats(ss);
  DrawCameraDebugInfo(ss);
  DrawFPSDebugInfo(ss, dTime);*/

  DrawTimeElapsed(ss);
  DrawScore(ss);
  DrawFuelBar(ss);
  DrawSpeedBar(ss);

  pSpriteBatch_->End();
}

void GameUI::DrawScore(wstringstream& ss)
{
  int w, h;
  GetClientExtents(w, h);

  ss.str(std::wstring());
  ss << L"Score: " << pGameScore_->GetScore();

  float ssWidth = XMVectorGetX(pFont2_->MeasureString(ss.str().c_str()));

  pFont2_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(w / 2 - ssWidth / 2 + 3, 20), Colours::Red, 0, Vector2(0, 0), Vector2(1, 1));
  pFont2_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(w / 2 - ssWidth / 2, 17), Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
}

void GameUI::DrawTimeElapsed(wstringstream& ss)
{
  int width, height;
  GetClientExtents(width, height);

  ss.str(std::wstring());
  ss << fixed << setprecision(2);
  ss << L"\nTime elapsed:" << setw(5) << pGameScore_->GetTimeSurvived();

  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - 147), 43), Colours::Red, 0, Vector2(0, 0), Vector2(1, 1));
  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - 150), 40), Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
}

void GameUI::DrawFuelBar(wstringstream& ss)
{
  ss.str(std::wstring());
  ss << fixed << setprecision(2);
  ss << L"Fuel";

  int width, height;
  GetClientExtents(width, height);
  Color fuelBarColor;
  float ssWidth = XMVectorGetX(pFont_->MeasureString(ss.str().c_str()));
  int frameLength = static_cast<int>(FX::GetMyFX()->mCache.Get(pBarFrame_).dim.x);

  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(120 + (frameLength - ssWidth) / 2, 20), Colors::White, 0, Vector2(0, 0), Vector2(1, 1));

  if (initComplete_) {
    float fuelLeft = pGameScore_->GetFuelLeft();
    float maxFuel = pGameScore_->GetMaxFuelThisGame();
    if (fuelLeft > maxFuel * kHealthyFuelBound)        // if over healthy level
      fuelBarColor = Colors::Green;
    else if (fuelLeft > maxFuel * kWarningFuelBound)  // if over warning level
      fuelBarColor = Colors::Orange;
    else
      fuelBarColor = Colors::Red;

    pSpriteBatch_->Draw(pBar_, Vector2(132, 70), nullptr, fuelBarColor, 0, Vector2(0, 0), Vector2(fuelLeft / maxFuel, 1));
    pSpriteBatch_->Draw(pBarFrame_, Vector2(120, 57), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
  }
}

void GameUI::DrawSpeedBar(wstringstream& ss)
{
  ss.str(std::wstring());
  ss << fixed << setprecision(2);
  ss << L"Speed";

  int width, height;
  GetClientExtents(width, height);
  float ssWidth = XMVectorGetX(pFont_->MeasureString(ss.str().c_str()));
  int barLength = static_cast<int>(FX::GetMyFX()->mCache.Get(pBarFrame_).dim.x);
  float playerSpeedScalar = pGameWorld_->GetPlayer()->GetVelocity().Length() / 25;
  if (playerSpeedScalar > 1)
    playerSpeedScalar = 1;

  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2((width - 320) + (barLength - ssWidth) / 2, 20), Colors::White, 0, Vector2(0, 0), Vector2(1, 1));

  if (initComplete_) {
    pSpriteBatch_->Draw(pBar_, Vector2(static_cast<float>(width - 318), 70), nullptr, Colors::Green, 0, Vector2(0, 0), Vector2(playerSpeedScalar, 1));
    pSpriteBatch_->Draw(pBarFrame_, Vector2(static_cast<float>(width - 320), 57), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
  }
}

void GameUI::DrawGameStats(wstringstream & ss)
{
  ss.str(std::wstring());
  ss << L"Sun Radius: " << pGameWorld_->GetSun()->GetCurrentRadius() <<
    L"\nDistance: " << pGameScore_->GetScore();

  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(550, 550), Colours::Red, 0, Vector2(0, 0), Vector2(1, 1));
}


void GameUI::DrawCameraDebugInfo(wstringstream & ss)
{
  // ***CHARLIE - Debug text to draw camera and ship position
  ss.str(std::wstring());
  ss << L"DEBUG: Camera position: " << pGameWorld_->GetCamera()->GetPosition().x << ", " << pGameWorld_->GetCamera()->GetPosition().y << ", " << pGameWorld_->GetCamera()->GetPosition().z << endl;
  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(300, 500), Colours::Green, 0, Vector2(0, 0), Vector2(1, 1));

  ss << L"DEBUG: Ship position: " << pGameWorld_->GetPlayer()->GetPosition().x << ", " << pGameWorld_->GetPlayer()->GetPosition().y << ", " << pGameWorld_->GetPlayer()->GetPosition().z << endl;
  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(300, 500), Colours::Green, 0, Vector2(0, 0), Vector2(1, 1));
}

void GameUI::DrawFPSDebugInfo(wstringstream & ss, float dTime)
{
  ss.str(std::wstring());
  if (dTime > 0)
    ss << L"FPS: " << (int)(1.f / dTime);
  else
    ss << L"FPS: 0";
  ss << "\nParticles: " << GetParticleSys()->numParticles_;
  pFont_->DrawString(pSpriteBatch_, ss.str().c_str(), Vector2(10, 550), Colours::Green, 0, Vector2(0, 0), Vector2(1.0f, 1.0f));
}