// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "../CRC.h"

// Application includes
#include "Mode.h"
#include "ModeManager.h"

Mode::Mode(const char* apName) :
  active_(false), visible_(false), pAudioManager_(GetIAudioMgr())
{
  nameHash_ = SCRC::GenerateCRC32(apName, strlen(apName));
  GetModeManager()->AddMode(this);
}

Mode::~Mode()
{
}

bool Mode::IsActive() const
{
  return active_;
}

bool Mode::IsVisible() const
{
  return visible_;
}

void Mode::SetVisible(bool visible)
{
  visible_ = visible;
}

bool Mode::HasHash(unsigned int aHash) const
{
  return nameHash_ == aHash;
}

unsigned int Mode::GetHash() const
{
  return nameHash_;
}

bool Mode::HasName(const std::string& name)
{
  unsigned int h = SCRC::GenerateCRC32(name.c_str(), (int)name.length());
  return h == nameHash_;
}

void Mode::Update(float aTime)
{
  pAudioManager_->Update();
}

void Mode::Exit()
{
  active_ = false;
  visible_ = false;
}

void Mode::Enter()
{
  active_ = true;
  visible_ = true;
}
