// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "ISerializable.h"

std::ostream& operator<<(std::ostream& os, const ISerializable& rhs) {
  // Precondition: os and rhs exist
  // Postcondition: Reference to output stream returned with new data inserted.
  assert(true);

  return rhs.ToStream(os);
}

std::istream& operator>>(std::istream& is, ISerializable& rhs)
{
  // Precondition: is and rhs exist
  // Postcondition: Reference to input stream returned with required data removed.
  assert(true);

  return rhs.FromStream(is);
}
