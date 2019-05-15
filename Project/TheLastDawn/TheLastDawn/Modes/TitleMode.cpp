// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include <thread>

#include "CommonStates.h"

#include "TitleMode.h"
#include "ModeManager.h"
#include "../D3D.h"
#include "../FX.h"
#include "../GameModel.h"
#include "../Mesh.h"
#include "../WindowUtils.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

extern bool g_okToQuit;

void ResetMaterials(Mesh& ms, const Vector4& diff, const Vector4& amb, const Vector4& spec)
{
  for (int i = 0; i < ms.GetNumSubMeshes(); ++i)
  {
    MaterialExt& mat = ms.GetSubMesh(i).material_;
    mat.gfxData.Set(diff, amb, spec);
  }
}

TitleMode::TitleMode()
  : Mode("TITLE"), loadThread_(), pSpriteBatch_(nullptr), pTitleTex_(nullptr), pControlsTex_(nullptr), pCurrentImage_(nullptr), titleDim_(DirectX::SimpleMath::Vector2::Zero),
    audio_(), loadFinished_(false), showingControls_(false)
{
}

TitleMode::~TitleMode()
{
  delete pSpriteBatch_;
}

void TitleMode::Update(float aTime)
{
  Mode::Update(aTime);
}

void TitleMode::Render(float dTime)
{
  BeginRender(Colours::Black);

  //background
  int w, h;
  GetClientExtents(w, h);
  float scale = h / titleDim_.y;

  CommonStates state(gd3dDevice);
  pSpriteBatch_->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
  pSpriteBatch_->Draw(pCurrentImage_, Vector2((w - titleDim_.x * scale) / 2, 0), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(scale, scale));
  pSpriteBatch_->End();

  if (loadThread_.valid() && loadThread_._Is_ready())
  {
    loadThread_.get();
    loadFinished_ = true;
  }

  EndRender();
}

void TitleMode::Enter()
{
  Mode::Enter();

  audio_.PlayBGM();
}

void TitleMode::Exit()
{
  Mode::Exit();
}

void TitleMode::Load()
{
  //load the title as fast as possible
  FX::MyFX& fx = *FX::GetMyFX();

  pSpriteBatch_ = new SpriteBatch(gd3dImmediateContext);

  pTitleTex_ = fx.mCache.LoadTexture("./titles/TitleScreen.dds", true, gd3dDevice);
  titleDim_ = fx.mCache.Get(pTitleTex_).dim;
	pControlsTex_ = fx.mCache.LoadTexture("./titles/Controls.dds", true, gd3dDevice);
	pCurrentImage_ = pTitleTex_;

  //start all other resources loading
  loadThread_ = std::async(launch::async, &TitleMode::LoadWorker, this);

}

void TitleMode::OnKey(char key)
{
  if (!loadFinished_)
    return;
  switch (key)
  {
  case 27:
    PostQuitMessage(0);
    break;
  case 'h':
  case 'H':
    GetModeManager()->SwitchMode("HIGHSCORES");
    break;
  case ' ':
    if (loadFinished_) {
			if (showingControls_) {
				showingControls_ = false;
				pCurrentImage_ = pTitleTex_;
				audio_.StopBGM();
				GetModeManager()->SwitchMode("GAME");
			}
			else {
				pCurrentImage_ = pControlsTex_;
				showingControls_ = true;
			}
    }
    break;
  }
}


void TitleMode::LoadWorker()
{
  /*Mesh* ms = &GetMeshManager()->CreateMesh("rock");
  ms->CreateFrom("../bin/data/asteroid1.x", gd3dDevice, FX::GetMyFX()->mCache);
  ResetMaterials(*ms, Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(0.05f, 0.05f, 0.05f, 5));
  ms = &GetMeshManager()->CreateMesh("barrel");
  ms->CreateFrom("../bin/data/barrel/barrel_unsmoothed.FBX", gd3dDevice, FX::GetMyFX()->mCache);
  ResetMaterials(*ms, Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(0.05f, 0.07f, 0.05f, 50));

  FX::MyFX& fx = *FX::GetMyFX();
  fx.mCache.LoadTexture("bullet.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("blue_shot.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("explosion_opaque.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("flames.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("radar.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("ship_icon.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("starfield_noalpha.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("starscape4.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("sweep.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("warp.dds", true, gd3dDevice);
  fx.mCache.LoadTexture("gameover.dds", true, gd3dDevice);*/

  GetModeManager()->Init();
}

const float TitleMode::skLOAD_DELAY_SECS = 2;