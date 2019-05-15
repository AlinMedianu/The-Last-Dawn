// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef CIPHER_H
#define CIPHER_H

#include <sstream>          // For parsing data
#include "ISerializable.h"  // For access to the insertion operator of Iserializable classes

#include <cassert>          // For debugging

/// <summary>
/// Provides the functionality to encrypt state data directly from a serialisable object, and decrypt from string.
/// </summary>
class Cipher
{
public:
  Cipher();
  ~Cipher();

  void Encrypt(const ISerializable& rawOutput, std::string& resultOutput);
  void Decrypt(std::string& rawInputData);

private:
  /// <summary>
  /// The number of characters to use in the repeating encryption key.
  /// </summary>
  const int kEncryptionKeyLength_ = 10;

  /// <summary>
  /// The actual encryption key, make sure it is the same length as defined by mEncryptionKeyLength.
  /// </summary>
  const char kEncryptionKey_[10] = { 'b', 'x', 'w', '4', 'Q', 'a', '!', 'Z', '£', '9' };

  void PerformEncryption(std::string& outputData);
  void PerformDecryption(std::string& inputData);
};

#endif // !CIPHER_H
