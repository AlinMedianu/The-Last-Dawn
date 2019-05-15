// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef LOADSAVEMANAGER_H
#define LOADSAVEMANAGER_H

#include "Singleton.h"

#include <fstream>

#include "ISerializable.h"
#include "Cipher.h" // For encryption

#include <cassert> // For debugging

/// <summary>
/// Provides simple interface for loading and saving ISerializable items to file.
/// <para>
/// Makes use of an encryption cipher for more secure data storage.
/// </para>
/// </summary>
class LoadSaveManager : public Singleton<LoadSaveManager>
{
public:
  LoadSaveManager();
  ~LoadSaveManager();

  void SaveItem(const ISerializable& item);
  void Load(ISerializable& item);

private:
  Cipher cipher_;

  int GetChecksum(const std::string& data) const;
};

// Declare the static singleton access method
SINGLETON_GET(LoadSaveManager);

#endif // !LOADSAVEMANAGER_H
