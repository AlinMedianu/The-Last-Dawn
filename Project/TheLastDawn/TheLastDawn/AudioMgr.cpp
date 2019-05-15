// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "AudioMgr.h"

//**************************************************************************************************
//just think of vectors as safer arrays, you can add things, search them, access an element, etc.
std::vector<unsigned int> IAudioMgr::s_hdlStartedNow;

bool IAudioMgr::CheckDuplicates(const unsigned int handle)
{
  // Precondition: None.
  // Postcondition: Returns true if the sound is already playing, returns false and flags the sound as now playing if not.
  assert(true);

  if (!s_hdlStartedNow.empty() && (find(s_hdlStartedNow.begin(), s_hdlStartedNow.end(), handle) != s_hdlStartedNow.end()))
    return true;
  s_hdlStartedNow.push_back(handle);
  return false;
}

void IAudioMgr::NewFrame()
{
  // Precondition: None.
  // Postcondition: Clears any currently playing sound references from the vector.
  assert(true);

  s_hdlStartedNow.clear();
}

IAudioMgr::IAudioMgr()
  : pSfxMgr_(nullptr), pSongMgr_(nullptr)
{
  // Precondition: None.
  // Postcondition: IAudioMgr is constructed.
  assert(true);
}

IAudioMgr::~IAudioMgr()
{
  // Precondition: None.
  // Postcondition: All dynamically allocated memory is released before destruction.
  assert(true);

  Shutdown();
}

void IAudioMgr::Update()
{
  // Precondition: None.
  // Postcondition: None.
  assert(true);

  NewFrame();
}

//release all sounds
void IAudioMgr::Shutdown()
{
  // Precondition: None.
  // Postcondition: All dynamically allocated memory is released.
  assert(true);

  delete pSongMgr_;
  pSongMgr_ = nullptr;
  delete pSfxMgr_;
  pSfxMgr_ = nullptr;
}

IAudioGroup* const IAudioMgr::GetSongMgr()
{
  // Precondition: None.
  // Postcondition: Pointer containing the address of the song manager is returned.
  assert(true);

  return pSongMgr_;
}

IAudioGroup* const IAudioMgr::GetSfxMgr()
{
  // Precondition: None.
  // Postcondition: Pointer containing the address of the sfx manager is returned.
  assert(true);

  return pSfxMgr_;
}



