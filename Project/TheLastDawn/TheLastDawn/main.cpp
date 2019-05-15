// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "WindowUtils.h"
#include "D3D.h"
#include "File.h"
#include "AudioMgr.h"
#include "AudioMgrFMOD.h"
#include "HighScoreManager.h"
#include "./Modes/Mode.h"
#include "./Modes/ModeManager.h"
#include "./Modes/TitleMode.h"
#include "./Modes/GameMode.h"
#include "./Modes/GameOverMode.h"
#include "./Modes/HighScoreMode.h"
#include "./Modes/NameEntryMode.h"
#include "LoadSaveManager.h"
#include "ParticleSys.h"

void Update(float dTime)
{
  //gGame.Update(dTime);
  GetModeManager()->Update(dTime);	// CHARLIE Update active mode
}

void Render(float dTime)
{
  //gGame.Render(dTime);
  GetModeManager()->Render(dTime);	// CHARLE Run render function for active mode
}

void OnResize(int screenWidth, int screenHeight)
{
  OnResize_Default(screenWidth, screenHeight);
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  //return gGame.WindowsMssgHandler(hwnd, msg, wParam, lParam);
  if (msg == WM_CHAR)
    GetModeManager()->OnKey(wParam);
  if (msg == WM_INPUT)
  {
    GetMouseAndKeys()->MessageEvent((HRAWINPUT)lParam);
  }
  return DefaultMssgHandler(hwnd, msg, wParam, lParam);	// Pass key input to current mode and return default behaviour
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

  if (!InitMainWindow(1280, 720, hInstance, "The Last Dawn", MainWndProc))
    assert(false);

  if (!InitDirect3D(OnResize))
    assert(false);

  File::initialiseSystem();

  AudioMgrFMOD* pAudioMgrFMOD = new AudioMgrFMOD;
  pAudioMgrFMOD->Initialise();

  FX::MyFX* pMyFX = new FX::MyFX();
  pMyFX->Init(gd3dDevice);

  ParticleSys* pParticleSys = new ParticleSys(pMyFX);
  pParticleSys->InitialisePS();

  MouseAndKeys* pMouseAndKeys = new MouseAndKeys;
  pMouseAndKeys->Initialise(GetMainWnd(), true, false);

  LoadSaveManager* pLoadSaveManager = new LoadSaveManager;
  HighScoreManager* pHighScoreManager = new HighScoreManager;
  pHighScoreManager->LoadHighScores();

  ModeManager* pModeManager = new ModeManager();
  TitleMode* pTitleMode = new TitleMode;
  GameMode* pGameMode = new GameMode;
  GameOverMode* pGameOverMode = new GameOverMode;
  HighScoreMode* pHighScoreMode = new HighScoreMode;
  NameEntryMode* pNameEntryMode = new NameEntryMode;
  pTitleMode->Load();
  GetModeManager()->SwitchMode("TITLE");

  Run(Update, Render);

  pHighScoreManager->SaveHighScores();

  delete pModeManager;
  pModeManager = nullptr;
  delete pHighScoreManager;
  pHighScoreManager = nullptr;
  delete pLoadSaveManager;
  pLoadSaveManager = nullptr;
  delete pMouseAndKeys;
  pMouseAndKeys = nullptr;
  delete pParticleSys;
  pParticleSys = nullptr;
  delete pMyFX;
  pMyFX = nullptr;
  delete pAudioMgrFMOD;
  pAudioMgrFMOD = nullptr;

  ReleaseD3D(true); // true here for extra debug info

  return 0;
}
