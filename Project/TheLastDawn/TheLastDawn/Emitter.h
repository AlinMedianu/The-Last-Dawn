// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef EMITTER_H
#define EMITTER_H

#include <d3d11.h>
#include <SimpleMath.h>
#include "D3DUtil.h" // For access to clock

#include "ParticleSys.h"
#include "Particle.h"
#include "Random.h"

class ParticleSys;
/// <summary>
/// There are 20~30 emitters, each instance generates particles over time and sets the
/// particles initial parameters
/// </summary>
class Emitter
{
public:

  //configuring an emitter
  enum class Type {
    OMNI,     //emit in all directions
    DIRECTION //emit in a specific direction
  };
  struct Data
  {
    //where is the emitter in the world
    DirectX::SimpleMath::Vector3 pos_;
    //time between emissions
    float mEmitDelaySec = 0;
    //how many to emit at once
    int mNumEmit = 0;
    //how many to emit in total, -1 means keep going forever
    int mTotal = 0;
    //how should the emitter emit
    Type mType = Type::OMNI;
    //a random size between these two limits
    DirectX::SimpleMath::Vector2 mScaleMinMax = DirectX::SimpleMath::Vector2(1, 1);
    //initial sprite rotation (radians)
    DirectX::SimpleMath::Vector2 mInitRotMinMax = DirectX::SimpleMath::Vector2(0, 0);
    //how fast and which direction should sprites spin (radians per sec)
    DirectX::SimpleMath::Vector2 mRotSpMinMax = DirectX::SimpleMath::Vector2(0, 0);
    //any initial flags on the particle (ALIVE is always one of them)
    int mParticleFlags = 0;
    //how long should a particle live
    DirectX::SimpleMath::Vector2 mInitLifeSecMinMax = DirectX::SimpleMath::Vector2(1, 1);
    //should this emitter return itself to the free pool once it runs out of particles?
    bool mReleaseOnEmpty = false;
    //emitter orientation - important if type=DIRECTION
    DirectX::SimpleMath::Matrix mOrientation;
    //if emitting in a specific direction, this lets you randomly vary that direction (in radians)
    DirectX::SimpleMath::Vector2 mInitAngleMinMax = DirectX::SimpleMath::Vector2(0, 0);
    //which sprite to use
    int spriteIdx_ = 0;
    //how fast are sprites moving
    DirectX::SimpleMath::Vector2 mInitSpeedMinMax = DirectX::SimpleMath::Vector2(1, 1);
  };
  Data data_;
  //track how often we've been emitting
  float lastEmitSec_ = 0;
  //if true then this emitter is emitting
  bool alive_ = false;

  //keep it going
  void Update(ParticleSys* ps);

  //guess
  void StopAndRelease();

private:

  //an emitter wanting to emit some particles
  bool CreateParticle(ParticleSys* ps);
};

#endif // !EMITTER_H
