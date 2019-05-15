// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef MODE_H
#define MODE_H

#include <string>

#include "../AudioMgr.h" // For updating the audio manager as it will be present in every mode;

/// <summary>
/// Modes are really finite state machines
/// Modes are things like: intro, playing the game, game over
/// This class defines common behaviour to all game modes.
/// </summary>
class Mode
{
public:
  Mode(const char* apName);
  virtual ~Mode();

  // is the mode active and being rendered
  bool IsActive() const;
  bool IsVisible() const;
  void SetVisible(bool visible);
  // modes are named, but the names are used to generate a unique ID
  bool HasHash(unsigned int aHash) const;
  unsigned int GetHash() const;
  bool HasName(const std::string& name);

  // Update method is called by ModeManager::Update to update the current
  // status of this mode if its active flag is set
  virtual void Update(float aTime) = 0;

  // Render method is called by ModeManager::Render to display this mode,
  // if its visible flag is set
  virtual void Render(float dTime) {}

  //when switching mode let the current tidy up first
  virtual void Exit();
  //when entering a new mode let it prepare itself
  virtual void Enter();
  //go from window to full screen and some resources need resetting
  virtual void OnResetDevice() {};
  virtual void OnLostDevice() {};
  //done once at start, don't add anything slow!
  virtual void Init() {};
  //respond to key presses
  virtual void OnKey(char key) {}

private:
  // When true, this mode is active
  bool active_;

  // When true, this mode is visible
  bool visible_;

  // Hash value of the name string passed into the object constructor
  unsigned int nameHash_;

  IAudioMgr* pAudioManager_;
};

#endif // !MODE_H
