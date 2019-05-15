// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "NameEntryMode.h"

NameEntryMode::NameEntryMode()
  : Mode("NAMEENTRY"), desiredPlayerName_(""), ui_(&desiredPlayerName_), audio_()
{
}

NameEntryMode::~NameEntryMode()
{
}

void NameEntryMode::Init()
{
  ui_.Init();
}

void NameEntryMode::Enter()
{
  Mode::Enter();
  audio_.PlayBGM();
}

void NameEntryMode::Update(float aTime)
{
  Mode::Update(aTime);
}

void NameEntryMode::Render(float dTime)
{
  BeginRender(Colours::Black);
  ui_.Render(dTime);
  EndRender();
}

void NameEntryMode::OnKey(char key)
{
  switch (key)
  {
  case 8: // backspace
    if (desiredPlayerName_.length() > 0)
    {
      desiredPlayerName_ = desiredPlayerName_.substr(0, desiredPlayerName_.length() - 1);
    }
    break;
  case 13: // Carriage Return
    if (desiredPlayerName_ == "")
      desiredPlayerName_ = "Player";
    GetGameMode()->GetCurrentScore().SetPlayerName(desiredPlayerName_);
    GetHighScoreManager()->AddNewHighScore(GetGameMode()->GetCurrentScore());
    GetModeManager()->SwitchMode("HIGHSCORES");
    break;
  default:
    if (IsValidLetter(key) && desiredPlayerName_.length() < 10)
    {
      desiredPlayerName_ += key;
    }
    break;
  }
}

void NameEntryMode::Exit()
{
  Mode::Exit();
}

bool NameEntryMode::IsValidLetter(char key) const
{
  // Dont allow any special characters in the name, other than space
  return ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9') || key == ' ');
}

