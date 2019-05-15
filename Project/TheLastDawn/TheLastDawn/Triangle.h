// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "IShape.h"

#include <algorithm>

#undef min
#undef max

namespace SpawnArea
{
  /// <summary>
  /// Controls a customisable triangular area that can be used to spawn objects in.
  /// </summary>
  class Triangle : public IShape
  {
  public:
    void SetAngle(float angle);
    void SetHeight(float height);
    void Set(const Vector2& apex, const Vector2& direction, float angle, float height);
    void Update(const Vector2& apex, const Vector2& direction);
    bool Contains(const Vector2& position) const override;
    Vector2 GetMin() const override;
    Vector2 GetMax() const override;

  private:
    float angle_;
    float height_;
    Vector2 direction_;
    Vector2 apex_;
    Vector2 left_;
    Vector2 right_;

    void Calculate();
  };
}

#endif // !TRIANGLE_H
