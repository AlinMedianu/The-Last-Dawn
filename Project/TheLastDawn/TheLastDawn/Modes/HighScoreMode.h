// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef HIGHSCOREMODE_H
#define HIGHSCOREMODE_H

#include "Mode.h"
#include "../Singleton.h"

#include "../HighScoreUI.h" // For rendering text/ui elements
#include "ModeManager.h" // To switch back out of mode

/// <summary>
/// Manages behaviour of the high score mode and allows players to exit it.
/// </summary>
class HighScoreMode :
  public Mode, public Singleton<HighScoreMode>
{
public:
  HighScoreMode();
  virtual ~HighScoreMode();

  // Virtual methods from Mode
  void Init() override;
  void Enter() override;
  void Update(float dTime) override;
  void Render(float dTime) override;
  void OnKey(char key) override;
  void Exit() override;

private:
  HighScoreUI ui_;
};

// Declare the static singleton access method
SINGLETON_GET(HighScoreMode);

#endif // !HIGHSCOREMODE_H
