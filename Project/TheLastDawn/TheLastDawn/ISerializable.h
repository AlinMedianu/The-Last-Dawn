// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <iostream> // To provide access to the streams for serializing

#include <cassert>  // For debugging purposes

/// <summary>
/// Abstract class providing the necessary functions to insert/extract object state data.
/// <para>
/// This class provides overloads allowing &lt; &lt; and &gt; &gt; operators to be used intuitively with streams.
/// </para>
/// </summary>
class ISerializable
{
public:

  /// <summary>
  /// Pure virtual function, to be overriden by derived classes to feed the required data into a stream
  /// </summary>
  /// In:
  /// <param name='os'> Reference to the output stream to use.</param>
  /// Out:
  /// <returns>Returns the reference to output stream object used.</returns>
  virtual std::ostream& ToStream(std::ostream& os) const = 0;

  /// <summary>
  /// Pure virtual function, to be overriden by derived classes to extract the required data from a stream
  /// </summary>
  /// In:
  /// <param name='os'> Reference to the input stream to use.</param>
  /// Out:
  /// <returns>Returns the reference to input stream object used.</returns>
  virtual std::istream& FromStream(std::istream& is) = 0;
};

std::ostream& operator<<(std::ostream& os, const ISerializable& rhs);
std::istream& operator>>(std::istream& is, ISerializable& rhs);

#endif // !ISERIALIZABLE_H
