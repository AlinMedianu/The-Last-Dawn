#pragma once

#include "Score.h" // To get infor to draw onscreen
#include <SpriteFont.h> // For accessing Spritebatching
#include <sstream> // For holding the text
#include "UtilsDefines.h"
#include <SimpleMath.h> // For use of vectors
#include "D3DUtil.h" // For accessing predefined colours
#include "CommonStates.h"
#include <iomanip> // For modifying string output


#include "WindowUtils.h"
#include "D3D.h"


#include "ParticleSys.h" // For debugging, drawing of particle numbers
#include "World.h" // For debugging, drawing of world data


using namespace UDEF;
using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class GameUI
{
public:
  GameUI(const Score* const gameScore, const World* const world); // TODO, Debug version remove world in final
  ~GameUI();

  void Init();
  void Render(float dTime);

private:
  const float kHealthyFuelBound = 0.5f; // 1.0 is 100% of fuel
  const float kWarningFuelBound = 0.25f;

  DirectX::SpriteBatch* pSpriteBatch_;
  DirectX::SpriteFont* pFont_, *pFont2_;
	ID3D11ShaderResourceView* pBar_, *pBarFrame_;
  const Score* pGameScore_;
  const World* pGameWorld_; // intended for debug only
	bool initComplete_;

	void DrawScore(wstringstream& ss);
  void DrawTimeElapsed(wstringstream& ss);
	void DrawFuelBar(wstringstream& ss);
	void DrawSpeedBar(wstringstream& ss);
	void DrawGameStats(wstringstream& ss);
  void DrawCameraDebugInfo(wstringstream& ss);
  void DrawFPSDebugInfo(wstringstream& ss, float dTime);

};

