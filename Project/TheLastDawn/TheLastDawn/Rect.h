// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef RECT_H
#define RECT_H

#include "IShape.h"
#undef min
#undef max

namespace SpawnArea
{
  /// <summary>
  /// Rectangular spawning pattern, uses camera position to determine correct spawn locations
  /// </summary>
  class Rect : public IShape
  {
  public:
    void SetFOVAngle(float fOVAngle);
    void SetCameraToRect(float cameraToRect);
    void SetMultiplier(float multiplier);
    void Set(const Vector2& center, float fOVAngle, float cameraToRect, float multiplier = 1);
    void Update(const Vector2& center);
    bool Contains(const Vector2& position) const override;
    Vector2 GetMin() const override;
    Vector2 GetMax() const override;

  private:
    float fOVAngle_;
    float cameraToRect_;
    float multiplier_;
    Vector2 center_;
    Vector2 min_;
    Vector2 max_;

    void Calculate();
  };
}

#endif // !RECT_H
