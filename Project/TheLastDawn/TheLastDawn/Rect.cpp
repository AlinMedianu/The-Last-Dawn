// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Rect.h"

namespace SpawnArea
{
  void Rect::SetFOVAngle(float fOVAngle)
  {
    this->fOVAngle_ = fOVAngle;
  }

  void Rect::SetCameraToRect(float cameraToRect)
  {
    this->cameraToRect_ = cameraToRect;
  }

  void Rect::SetMultiplier(float multiplier)
  {
    assert(multiplier > 0);
    this->multiplier_ = multiplier;
  }

  void Rect::Set(const Vector2& center, float fOVAngle, float cameraToRect, float multiplier)
  {
    this->fOVAngle_ = fOVAngle;
    this->cameraToRect_ = cameraToRect;
    this->multiplier_ = multiplier;
    this->center_ = center;
    Calculate();
  }

  void Rect::Update(const Vector2& center)
  {
    this->center_ = center;
    Calculate();
  }

  bool Rect::Contains(const Vector2& position) const
  {
    return position.x > min_.x&& position.x < max_.x&&
      position.y > min_.y&& position.y < max_.y;
  }

  Vector2 Rect::GetMin() const
  {
    return min_;
  }

  Vector2 Rect::GetMax() const
  {
    return max_;
  }

  void Rect::Calculate()
  {
    float yMax = tanf(XMConvertToRadians(fOVAngle_ * 0.5f)) * cameraToRect_;
    float xMax = GetAspectRatio() * yMax;
    Vector2 localMax(xMax, yMax);
    max_ = center_ + localMax * multiplier_;
    min_ = center_ - localMax * multiplier_;
  }
}