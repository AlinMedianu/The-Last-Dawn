// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef TITLEMODE_H
#define TITLEMODE_H

#include <future>
#include <SimpleMath.h>
#include <SpriteFont.h>

// Application includes
#include "Mode.h"
#include "../UtilsDefines.h"

#include "../TitleModeAudio.h"

/// <summary>
// The title screen, explains the controls and loads the rest of the game
/// </summary>
class TitleMode : public Mode
{
public:
  TitleMode();
  ~TitleMode();

  void Update(float aTime) override;
  void Render(float dTime) override;
  void Enter() override;
  void Exit() override;
  void OnKey(char key) override;

  void Load();

private:
  static const float skLOAD_DELAY_SECS;

  std::future<void> loadThread_;
  DirectX::SpriteBatch* pSpriteBatch_;
  ID3D11ShaderResourceView* pTitleTex_, *pControlsTex_, *pCurrentImage_;
  DirectX::SimpleMath::Vector2 titleDim_;
  TitleModeAudio audio_;
  bool loadFinished_;
	bool showingControls_;

  void LoadWorker();

};

#endif // !TITLEMODE_H
