// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "ScrollingImage.h"

ScrollingImage::ScrollingImage(const TexCache::Data& texture)
  : textureData_(texture)
{
}

ScrollingImage::~ScrollingImage()
{
}

Vector2 ScrollingImage::GetScale(const int windowWidth, const int windowHeight)
{
  return Vector2(windowWidth / textureData_.dim.x, windowHeight / textureData_.dim.y);
}

RECT ScrollingImage::GetScrollingRect(Vector2 velocity)
{
  return RECT{ static_cast<LONG>(velocity.x), static_cast<LONG>(-velocity.y),
    static_cast<LONG>(textureData_.dim.x + velocity.x),
    static_cast<LONG>(textureData_.dim.y - velocity.y) };
}

const TexCache::Data & ScrollingImage::GetTextureData() const
{
  return textureData_;
}

