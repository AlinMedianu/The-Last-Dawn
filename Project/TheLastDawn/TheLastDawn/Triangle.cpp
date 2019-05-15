// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Triangle.h"

namespace SpawnArea
{
  void Triangle::SetAngle(float angle)
  {
    this->angle_ = angle;
  }

  void Triangle::SetHeight(float height)
  {
    this->height_ = height;
  }

  void Triangle::Set(const Vector2& apex, const Vector2& direction, float angle, float height)
  {
    angle_ = angle;
    height_ = height;
    direction_ = direction;
    apex_ = apex;
    Calculate();
  }

  void Triangle::Update(const Vector2& apex, const Vector2& direction)
  {
    apex_ = apex;
    direction_ = direction;
    Calculate();
  }

  bool Triangle::Contains(const Vector2& position) const
  {
    float denominator = (left_.y - right_.y) * (apex_.x - right_.x) +
      (right_.x - left_.x) * (apex_.y - right_.y);
    float a = ((left_.y - right_.y) * (position.x - right_.x) +
      (right_.x - left_.x) * (position.y - right_.y)) / denominator;
    float b = ((right_.y - apex_.y) * (position.x - right_.x) +
      (apex_.x - right_.x) * (position.y - right_.y)) / denominator;
    float c = 1 - a - b;
    return a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1;
  }

  Vector2 Triangle::GetMin() const
  {
    return Vector2(min({ apex_.x, left_.x, right_.x }),
      min({ apex_.y, left_.y, right_.y }));
  }

  Vector2 Triangle::GetMax() const
  {
    return Vector2(max({ apex_.x, left_.x, right_.x }),
      max({ apex_.y, left_.y, right_.y }));
  }

  void Triangle::Calculate()
  {
    float halfAngleCosine = cosf(XMConvertToRadians(angle_ * 0.5f));
    float halfAngleSine = sinf(XMConvertToRadians(angle_ * 0.5f));
    float side = height_ / halfAngleCosine;
    Vector2 apexToLeft(halfAngleCosine * direction_.x -
      halfAngleSine * direction_.y,
      halfAngleSine * direction_.x +
      halfAngleCosine * direction_.y);
    apexToLeft.Normalize();
    left_ = apexToLeft * side + apex_;
    Vector2 apexToRight(halfAngleCosine * direction_.x +
      halfAngleSine * direction_.y,
      halfAngleCosine * direction_.y -
      halfAngleSine * direction_.x);
    apexToRight.Normalize();
    right_ = apexToRight * side + apex_;
  }
}