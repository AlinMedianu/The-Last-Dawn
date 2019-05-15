// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef ISHAPE_H
#define ISHAPE_H

#include "D3D.h"

#include <cmath>

using namespace DirectX;
using namespace SimpleMath;
using namespace std;

namespace SpawnArea
{
  /// <summary>
  /// Abstract class that provides the interface for accessing spawn shape patterns
  /// </summary>
  class IShape
  {
  public:
    virtual bool Contains(const Vector2& position) const = 0;
    virtual Vector2 GetMin() const = 0;
    virtual Vector2 GetMax() const = 0;
  };
}

#endif // !ISHAPE_H
