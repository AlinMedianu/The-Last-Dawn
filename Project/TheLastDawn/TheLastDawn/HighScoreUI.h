#pragma once
#include "D3D.h"
#include <SpriteFont.h>
#include <sstream> // For holding the text before rendering
#include "UtilsDefines.h"
#include "D3DUtil.h" // For accessing predefined colours
#include "CommonStates.h"
#include <iomanip> // For modifying string output
#include "WindowUtils.h"
#include "FX.h"

#include "HighScoreManager.h" // For accessing the HighScores

#include <cassert> // For debugging


using namespace std;
using namespace DirectX;
using namespace SimpleMath;

class HighScoreUI
{
public:
  HighScoreUI();
  ~HighScoreUI();

  void Init();
  void Render(float dTime);

private:
  HighScoreManager* mpHighScoreManager;
  DirectX::SpriteBatch* mpSpriteBatch = nullptr;
  DirectX::SpriteFont* mpFont = nullptr, * mpFont2 = nullptr;
	ID3D11ShaderResourceView* mpHighScoreTex = nullptr;
	SimpleMath::Vector2 mHighScoreDim;

  void DrawHighScoreTitle();
  void DrawHighScoreTable(wstringstream& ss);
};

