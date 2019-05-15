// by Mark Featherstone (C) 2015 All Rights Reserved.

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

// Application includes
#include <vector>
#include "../Singleton.h"

// Forward declarations
class Mode;

/// <summary>
/// Controls the switching of modes, mode update and rendering and all things mode related :)
/// </summary>
class ModeManager : public Singleton<ModeManager>
{
public:
  ModeManager();
  ~ModeManager();

  // Update all currently active game modes
  void Update(float aTime);

  // Render all currently visible game modes
  void Render(float dTime);

  // Add a mode instance to the mode manager
  void AddMode(Mode* apMode);

  // Gets the mode instance with the given name
  Mode* GetMode(const char* apModeName);
  Mode* GetMode(int hash);

  void SwitchMode(const char* apModeName);

  void OnKey(char key);
  void Init();

private:
  //handy typedef for a Mode* array
  typedef std::vector<Mode*> Modes;
  Modes modes_;
  //are we trying to switch to a new mode
  Mode* pNextMode_;
  unsigned int nextModeHash_;
};

// Declare the static singleton accessor method
SINGLETON_GET(ModeManager);


#endif
