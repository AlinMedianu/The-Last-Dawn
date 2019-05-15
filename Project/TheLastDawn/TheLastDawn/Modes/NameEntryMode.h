// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef NAMEENTRYMODE_H
#define NAMEENTRYMODE_H

#include "Mode.h"
#include "../Singleton.h"
#include "../D3DUtil.h"
#include "../D3D.h"

#include <sstream>

#include "../NameEntryUI.h"
#include "ModeManager.h"
#include "../HighScoreManager.h"
#include "GameMode.h"
#include "../NameEntryModeAudio.h"

/// <summary>
/// Manages behaviour of the name entry screen after obtaining a  high score.
/// </summary>
class NameEntryMode :
  public Mode, public Singleton<NameEntryMode>
{
public:
  NameEntryMode();
  virtual ~NameEntryMode();

  // Virtual methods from Mode
  void Init() override;
  void Enter() override;
  void Update(float aTime) override;
  void Render(float dTime) override;
  void OnKey(char key) override;
  void Exit() override;

private:
  std::string desiredPlayerName_;
  NameEntryUI ui_;
  NameEntryModeAudio audio_;

  bool IsValidLetter(char key) const;
};

// Declare the static singleton access method
SINGLETON_GET(NameEntryMode);

#endif // !NAMEENTRYMODE_H
