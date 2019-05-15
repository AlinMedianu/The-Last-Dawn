// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef PARTICLE_H
#define PARTICLE_H

#include <d3d11.h>
#include <SimpleMath.h>

/// <summary>
/// There are potentially tens of thousands of instances describing
/// a single particle moving through the world
/// </summary>
class Particle
{
public:
  //world space
  DirectX::SimpleMath::Vector3 pos_;
  //where is it going and how fast
  DirectX::SimpleMath::Vector3 vel_;
  //current rotation of the sprite in radians
  float angle_ = 0;
  //how fast and which way is it rotating
  float rotSpeed_ = 0;
  //track how long it should live, once dead it's freed to be used again
  float lifeSec_ = 0, spawnSec_ = 0;
  //how big is the sprite
  float scale_ = 1;
  //which sprite it should use
  int spriteIdx_ = 0;
  //particles are organised in a linked list so it's easy to find all the ones that need
  //rendering (busy) and all the ones that are free to use (free)
  Particle* pPrev_ = nullptr, * pNext_ = nullptr;

  //bit flags
  enum {
    USE_GRAVITY = 1,
    ALIVE = 2       //marked alive when it becomes active and starts rendering
  };
  unsigned int flags_ = 0;

  //gravitational force
  static const float kGRAVITY;

};

#endif // !PARTICLE_H
