#include "NameEntryUI.h"



NameEntryUI::NameEntryUI(std::string* desiredName)
	: mpDesiredName(desiredName)
{
}

NameEntryUI::~NameEntryUI()
{
	delete mpSpriteBatch;
	delete mpFont;
	delete mpFont2;
}

void NameEntryUI::Init()
{
	mpSpriteBatch = new SpriteBatch(gd3dImmediateContext);
	assert(mpSpriteBatch);

	mpFont = new SpriteFont(gd3dDevice, L"../bin/data/coldWarm24.spritefont");
	assert(mpFont);

	mpFont2 = new SpriteFont(gd3dDevice, L"../bin/data/coldWarm36.spritefont");
	assert(mpFont2);

	mpNameEntryTex = FX::GetMyFX()->mCache.LoadTexture("./titles/NameEntry.dds", true, gd3dDevice);
	mNameEntryDim = FX::GetMyFX()->mCache.Get(mpNameEntryTex).dim;
	assert(mpNameEntryTex);
}

void NameEntryUI::Render(float dTime)
{
	CommonStates state(gd3dDevice);

	mpSpriteBatch->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
	//general messages
	wstringstream ss;

	DrawNameEntryTitle();
	DrawCurrentlyEnteredName(ss);

	mpSpriteBatch->End();
}

void NameEntryUI::DrawNameEntryTitle() const
{
	int width, height;
	GetClientExtents(width, height);

	float scale = height / mNameEntryDim.y;
	mpSpriteBatch->Draw(mpNameEntryTex, Vector2((width - mNameEntryDim.x * scale) / 2, 0), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(scale, scale));
}

void NameEntryUI::DrawCurrentlyEnteredName(wstringstream& ss) const
{
	int width, height;
	GetClientExtents(width, height);

	ss.str(std::wstring());

	ss << mpDesiredName->data();

	if (height > 720)
	{
		float ssWidth = XMVectorGetX(mpFont2->MeasureString(ss.str().c_str()));
		mpFont2->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - (ssWidth / 2)), static_cast<float>((height / 2) + 10)), Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
	}
	else
	{
		float ssWidth = XMVectorGetX(mpFont->MeasureString(ss.str().c_str()));
		mpFont->DrawString(mpSpriteBatch, ss.str().c_str(), Vector2(static_cast<float>((width / 2) - (ssWidth / 2)), static_cast<float>((height / 2) + 10)), Colours::White, 0, Vector2(0, 0), Vector2(1, 1));
	}
}
