// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "LoadSaveManager.h"

LoadSaveManager::LoadSaveManager()
  : cipher_()
{
}

LoadSaveManager::~LoadSaveManager()
{
}

void LoadSaveManager::SaveItem(const ISerializable& item)
{
  std::ofstream fileOut;
  fileOut.open("../bin/saves/saveData.sav", std::ios::binary | std::ios::out);
  if (!fileOut.fail())
  {
    std::string parsedData;
    cipher_.Encrypt(item, parsedData);
    int fileLength = parsedData.length();
    int fileChecksum = GetChecksum(parsedData);
    fileOut.write((char*)& fileLength, sizeof(int));
    fileOut.write((char*)& fileChecksum, sizeof(int));
    fileOut.write(parsedData.c_str(), sizeof(char) * fileLength);
  }
  else {
    assert(false);
  }
  fileOut.close();
}

void LoadSaveManager::Load(ISerializable& item)
{
  std::ifstream fileIn;
  fileIn.open("../bin/saves/saveData.sav", std::ios::binary | std::ios::in);
  if (!fileIn.fail())
  {
    int expectedFileLength;
    fileIn.read((char*)& expectedFileLength, sizeof(int));
    int expectedFileChecksum;
    fileIn.read((char*)& expectedFileChecksum, sizeof(int));
    std::string parsedData;
    char temp;
    int iterator = 0;
    while (!fileIn.std::ios::eof() && iterator < expectedFileLength)
    {
      fileIn.read((char*)& temp, sizeof(char));
      parsedData += temp;
      ++iterator;
    }
    if (expectedFileChecksum == GetChecksum(parsedData))
    {
      cipher_.Decrypt(parsedData);
      std::istringstream decodedInput(parsedData);
      decodedInput >> item;
    }
  }
  else {
    //assert(false);
  }
  fileIn.close();
}

int LoadSaveManager::GetChecksum(const std::string& data) const
{
  int totalCharacterValues = 0;
  for (size_t i = 0; i < data.length(); i++)
  {
    totalCharacterValues += (int)data.at(i);
  }
  return totalCharacterValues;
}

