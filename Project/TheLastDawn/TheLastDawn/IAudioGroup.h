// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef IAUDIOGROUP_H
#define IAUDIOGROUP_H

#include "UtilsDefines.h" // For access to typedef UDEF::utf8string

/// <summary>
/// A collection of similar type of sounds i.e. sfx or music.
/// <para>
/// One 'sound' can be playing on many 'channels' at the same time.
/// </para>
/// <para>
/// The interface means we can implement a group using fmod, bass, openAL as needed.
/// </para>
/// </summary>
class IAudioGroup
{
public:
  //explicit constructor prevent any auto compiler conversion of parameters
  //C++ has a nasty habit of quietly trying to convert the wrong type into the right one
  explicit IAudioGroup(const UDEF::utf8string& grpName)
    : grpName_(grpName), volume_(1), channelCutOffVol_(1.f) {};
  virtual ~IAudioGroup() {};
  ///stop everything in the group
  virtual void Stop() = 0;
  ///stop this particular channel
  virtual void Stop(const unsigned int channelHandle) = 0;
  ///mute everything or not
  virtual void Mute(const bool state) = 0;
  ///set things up either as all streaming sounds or preloaded
  virtual bool Initialise(const bool asStreams) = 0;
  ///grab all sound files in this folder
  virtual bool Load(const UDEF::utf8string& folder) = 0;
  ///how many channels are going
  virtual unsigned int NumChannelsPlaying() = 0;
  ///how many sounds were loaded
  virtual unsigned int NumSoundsLoaded() = 0;
  ///grab a channel's handle
  virtual unsigned int GetChannelHandle(const int idx) = 0;
  ///set volume of everything
  virtual void SetVolume(const float vol) { volume_ = vol; }
  ///set volume of one channel
  virtual void SetVolume(const float vol, const unsigned int channelHandle) = 0;
  ///pan one channel
  virtual void SetPan(const float pan, const unsigned int channelHandle) = 0;
  ///pause one channel
  virtual void SetPause(const bool state, const unsigned int channelHandle = UDEF::MAX_UINT) = 0;
  ///volume of entire group
  virtual float GetVolume() const { return volume_; }
  ///play a sound by its short name
  ///@param name - name of sound to play, no path, no extension, just the filename
  ///@param loop - loop forever
  ///@param paused - don't immediately start it playing
  ///@param pChannelHandle - optional pointer will have the channel handle written into it
  virtual bool Play(const UDEF::utf8string& name, const bool loop, const bool paused, unsigned int* pChannelHandle = NULL,
    const float vol = 1.f) = 0;
  ///play a sound given a 'sound handle'
  ///@param soundhandle - all the loaded sounds can be referred to either by name or handle
  virtual bool Play(const unsigned int soundHandle, const bool loop, const bool paused, unsigned int* pChannelHandle = NULL,
    const float vol = 1.f) = 0;
  ///does the named sound exist in the group
  virtual bool Exists(const UDEF::utf8string& name, int* pSoundIndex = NULL) = 0;
  ///is the channel currently playing, could be paused or finished
  virtual bool IsPlaying(const unsigned int channelHandle) = 0;
  ///get the short name of the sound playing through this channel
  virtual const UDEF::utf8string& GetName(const unsigned int channelHandle) = 0;
  ///get the sound index for the sound playing through this channel
  virtual const unsigned int& GetSoundIndex(const unsigned int channelHandle) = 0;
  ///set a limit to channel volumes 
  void SetChannelVolCutoff(const float cutOffVol) { channelCutOffVol_ = cutOffVol; }
protected:
  UDEF::utf8string grpName_;        //< friendly name for group
  float channelCutOffVol_;          //< no channel can be louder than this
private:
  float volume_;

  const IAudioGroup& operator=(const IAudioGroup&);  //prevent an audio group being assigned
};


#endif // !IAUDIOGROUP_H

