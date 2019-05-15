// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "HighScoreMode.h"

HighScoreMode::HighScoreMode()
  : Mode("HIGHSCORES"), ui_()
{
}

HighScoreMode::~HighScoreMode()
{
  Mode::~Mode();
}

void HighScoreMode::Init()
{
  ui_.Init();
}

void HighScoreMode::Enter()
{
  Mode::Enter();
}

void HighScoreMode::Update(float dTime)
{
  Mode::Update(dTime);
}

void HighScoreMode::Render(float dTime)
{
  BeginRender(Colours::Black);
  ui_.Render(dTime);
  EndRender();
}

void HighScoreMode::OnKey(char key)
{
  switch (key)
  {
  case 27:
    GetModeManager()->SwitchMode("TITLE");
    break;
  case ' ':
    GetModeManager()->SwitchMode("TITLE");
    break;
  }
}

void HighScoreMode::Exit()
{
  Mode::Exit();
}
