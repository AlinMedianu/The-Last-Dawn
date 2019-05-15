// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include "Singleton.h"    // For base class derivation

#include <vector>         // For storage of sound identifiers
#include <algorithm>      // For find function

#include "IAudioGroup.h"  // For managing different types of sounds

#include <cassert>        // For debugging

// Definitions
#define QUIET_VOLUME 0.25f
#define NORMAL_VOLUME 0.7f
#define LOUD_VOLUME 1.f

/// <summary>
/// Holds the song and sfx managers and controls all audio, as an interface.
/// <para>
/// Means we can replace the implementation with fmod, bass, opanALand the game code won't be touched.
/// </para>
/// </summary>
class IAudioMgr : public Singleton<IAudioMgr>
{
public:
  IAudioMgr();
  virtual ~IAudioMgr();
  //call it before using the audio manager
  virtual bool Initialise(void) = 0;
  //call once per frame
  virtual void Update() = 0;
  //stop everything
  virtual void Shutdown() = 0;
  //do something with streamed music
  IAudioGroup* const GetSongMgr();
  //do something with loaded sfx
  IAudioGroup* const GetSfxMgr();
  ///see if this handle has already been started once this frame

  ///<summary>
  /// Check if the sound handle has already been started this frame.
  /// <para>
  /// Triggering a sound more than once in the same frame sounds wrong.
  /// </para>
  /// </summary>
  static bool CheckDuplicates(const unsigned int handle);
protected:
  IAudioGroup* pSongMgr_;	//streamed audio
  IAudioGroup* pSfxMgr_;		//memory loaded audio, small clips
  static std::vector<unsigned int> s_hdlStartedNow;	///<we shouldn't have the same handle starting more than once in a frame
  ///reset our handle record each time the frame changes
  static void NewFrame();  //static means there is only one of these functions regardless of instantiation
};
SINGLETON_GET(IAudioMgr);

#endif // !AUDIOMGR_H
