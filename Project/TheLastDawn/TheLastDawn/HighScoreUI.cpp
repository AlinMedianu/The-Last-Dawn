#include "HighScoreUI.h"


HighScoreUI::HighScoreUI()
	: mpHighScoreManager(GetHighScoreManager())
{
}

HighScoreUI::~HighScoreUI()
{
  delete mpSpriteBatch;
  delete mpFont;
  delete mpFont2;
}

void HighScoreUI::Init()
{
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);

	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/anonPro24.spritefont");
	assert(mpFont);

	mpFont2 = new SpriteFont(gd3dDevice, L"../bin/data/anonPro36.spritefont");
	assert(mpFont2);

	mpHighScoreTex = FX::GetMyFX()->mCache.LoadTexture("./titles/HighScore.dds", true, gd3dDevice);
	mHighScoreDim = FX::GetMyFX()->mCache.Get(mpHighScoreTex).dim;
	assert(mpHighScoreTex);
}

void HighScoreUI::Render(float dTime)
{
	CommonStates state(gd3dDevice);

	mpSpriteBatch->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
	//general messages
	wstringstream ss;

	DrawHighScoreTitle();
	DrawHighScoreTable(ss);

	mpSpriteBatch->End();
}

void HighScoreUI::DrawHighScoreTitle()
{
	assert(mpHighScoreManager != nullptr);

	int width, height;
	GetClientExtents(width, height);

	float scale = height / mHighScoreDim.y;
	mpSpriteBatch->Draw(mpHighScoreTex, Vector2((width - mHighScoreDim.x * scale) / 2, 0), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(scale, scale));
}

void HighScoreUI::DrawHighScoreTable(wstringstream& ss)
{
	assert(mpHighScoreManager != nullptr);

	int width, height;
	GetClientExtents(width, height);

	ss.str(std::wstring());

	int entries = mpHighScoreManager->GetNumOfHighScoresStored();
	for (int i = entries - 1; i >= 0; --i)
	{
		ss << setw(2) << (entries - i) << " : " << setw(10) << left << mpHighScoreManager->GetHighScoreAt(i).GetPlayerName().c_str() << ' ';
		ss << setw(3) << right << mpHighScoreManager->GetHighScoreAt(i).GetScore();
		ss << "\n";
	}

	if (height <= 720)
	{
		float ssWidth = XMVectorGetX(mpFont->MeasureString(L"...................")); // Max line length = 19 characters + monospaced font
		mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - (ssWidth / 2)), height * 0.35f), Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
	}
	else
	{
		float ssWidth = XMVectorGetX(mpFont2->MeasureString(L"...................")); // Max line length = 19 characters + monospaced font
		float ratio = static_cast<float>(height) / 1080;
		mpFont2->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - (ssWidth / 2)), static_cast<float>(height) * 0.35f), Colours::White, 0, Vector2(0, 0), Vector2(ratio, ratio));
	}
}
