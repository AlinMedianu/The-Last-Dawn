// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Cipher.h"

Cipher::Cipher()
{
  // Precondition: None
  // Postcondition: Cipher object constructed.
  assert(true);
}

Cipher::~Cipher()
{
  // Precondition: None
  // Postcondition: Cipher object destroyed.
  assert(true);
}

void Cipher::Encrypt(const ISerializable& rawOutput, std::string& resultOutput)
{
  // Precondition: rawOutput and resultOutput exist
  // Postcondition: rawOutput has had the data extracted from it, resultOutput has been populated with the encrypted data.
  assert(true);

  std::ostringstream parser;
  parser << rawOutput;
  std::string data = parser.str(); // Extracting the data out of the stream by simply converting it to string, nothing fancy needed at this stage.
  PerformEncryption(data);
  resultOutput = data;
}

void Cipher::Decrypt(std::string& rawInputData)
{
  // Precondition: rawInputData exists.
  // Postcondition: rawInputData is passed through to the decryption function.
  assert(true);

  PerformDecryption(rawInputData); // This call was abstracted out to allow for any additional checks to occur in here before decryption takes place.
}

void Cipher::PerformEncryption(std::string& outputData)
{
  // Precondition: There is data to encrypt in the outputData string.
  // Postcondition: Each character in outputData is encrypted with the appropriate key.
  assert(outputData.length() > 0);

  for (size_t i = 0; i < outputData.length(); i++)
  {
    char currentKey = kEncryptionKey_[i % kEncryptionKeyLength_]; // By using modulo here with the iteration number and the total number of characters in the key array, we get the array index of the appropriate key to use.
    outputData.at(i) = outputData.at(i) ^ currentKey;             // No fancy encryption, just an XOR. Can be replaced with a more robust algorithm if required.
  }
}

void Cipher::PerformDecryption(std::string& inputData)
{
  // Precondition: There is data to decrypt in the inputData string.
  // Postcondition: Each character in inputData is decrypted with the appropriate key.
  assert(inputData.length() > 0);

  for (size_t i = 0; i < inputData.length(); i++)
  {
    char currentKey = kEncryptionKey_[i % kEncryptionKeyLength_]; // By using modulo with the iteration number and the total number of characters in the key array, we get the array index of the appropriate key.
    inputData.at(i) = inputData.at(i) ^ currentKey;               // No fancy decryption, just an XOR to reverse the encryption. Can be replaced with a more robust algorithm if required.
  }
}
