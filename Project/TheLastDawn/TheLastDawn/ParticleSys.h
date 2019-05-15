// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef PARTICLESYS_H
#define PARTICLESYS_H

#include "Singleton.h"


#include <d3d11.h>
#include "SimpleMath.h"
#include "FX.h"

#include <vector>
#include <algorithm>

#include "Emitter.h"
#include "Particle.h"


class Emitter;

/// <summary>
/// Manages the emitters and particles, as a manager this is a good candidate for the singleton pattern really
/// </summary>
class ParticleSys : public Singleton<ParticleSys>
{
public:
  //upper limits on the system
  const int kMAX_PARTICLES = 10000;
  const int kMAX_EMITTERS = 5;
  //how many particles are active at any one time
  int numParticles_ = 0;

  ParticleSys(FX::MyFX* fx);
  ~ParticleSys();

  //start the whole thing up
  void InitialisePS();
  //keep it going
  //dTime=elapsed time per frame
  //camPos=the camera world position (sprites face the camera always)
  void UpdateParticles(float dTime, const DirectX::SimpleMath::Vector3& camPos);
  //there are only 20 emitters, so it's possible to ask for one and get nullptr
  Emitter* GetNewEmitter();
  //render particles last, the z buffer is usually disabled (it doesn't have to be)
  void Render(float dTime);
  //shut it all down and release everything
  void ReleasePS();

  //get hold of the internal particle buffers
  Particle*& GetFreeList() {
    return pFreeList_;
  }
  Particle*& GetBusyList() {
    return pBusyList_;
  }

  //texture coordinates of rectangles in the sprite atlas
  struct TexRect
  {
    float u, v, w, h;  //these are resolution independant sprite texture coordinates (so 0->1=full texture)
  };
  std::vector<TexRect>& GetSpriteLibrary() { return sprites_; }

private:

  Particle* pFreeList_, * pBusyList_; //linked list particle buffers, it's done this way to avoid searching  
  std::vector<Emitter> emitters_; //all the emitters POD  
  std::vector<Particle> particles_; //all the particles POD  
  ID3D11Buffer* pVBuffer_; //dynamic vbuffer to render sprites into, index buffer can be static
  ID3D11Buffer* pIBuffer_;
  std::vector<TexRect> sprites_; //the sprite library
  FX::MyFX* pFx_;
};

SINGLETON_GET(ParticleSys);

#endif // !PARTICLESYS_H
