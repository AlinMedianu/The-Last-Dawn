// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GAMEOVERMODE_H
#define GAMEOVERMODE_H


#include "Mode.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include <SpriteFont.h> // For accessing Spritebatching
#include "CommonStates.h"


// Application includes
#include "../D3D.h"
#include "ModeManager.h"
#include "../UtilsDefines.h"
#include "../FX.h"
#include "../WindowUtils.h"


/// <summary>
/// Game Over screen, gives the player the option to restart.
/// </summary>
class GameOverMode : public Mode
{
public:
  GameOverMode();
  ~GameOverMode();

  void Update(float aTime) override;
  void Render(float dTime) override;
  void Enter() override;
  void Exit() override;
  void OnKey(char key) override;

private:
  FX::MyFX* pFx_;
  ID3D11ShaderResourceView* pGameOverTex_;
  DirectX::SimpleMath::Vector2 gameOverDim_;

};

#endif // !GAMEOVERMODE_H
