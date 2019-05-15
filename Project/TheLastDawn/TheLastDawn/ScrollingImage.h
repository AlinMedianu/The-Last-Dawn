// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef SCROLLINGIMAGE_H
#define SCROLLINGIMAGE_H

#include <vector>

#include "FX.h"
#include "SimpleMath.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace FX;

/// <summary>
/// Provides access to a scrolling rectangular area used for parallax effects.
/// </summary>
class ScrollingImage
{
public:
  ScrollingImage(const TexCache::Data& texture);
  ~ScrollingImage();

  Vector2 GetScale(const int windowWidth, const int windowHeight);
  RECT GetScrollingRect(Vector2 velocity);
  const TexCache::Data& GetTextureData() const;

private:
  TexCache::Data textureData_;

};

#endif // !SCROLLINGIMAGE_H
