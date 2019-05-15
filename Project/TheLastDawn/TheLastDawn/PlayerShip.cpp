// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "PlayerShip.h"

PlayerShip::PlayerShip()
  : GameModel(), mobileBody_(this), collider_(), audio_(), pEngineEmitter_(nullptr), 
	pCurrentlyOrbiting_(nullptr), currentState_(PlayerState::FLYING), 
	orbitDir_(OrbitDirection::NONE), boostActive_(false), currentFuel_(0.0f), currentOrbitAngle_(0.0f), 
	timeUntilNextBoost_(0.0f), radius(0.0f), anglePerSec(0.0f), 
	mOrbitEntryDirectionNormal(DirectX::SimpleMath::Vector3::Right), mOrbitEntryVelocity(DirectX::SimpleMath::Vector3::Right)
{

}

PlayerShip::~PlayerShip()
{

}

void PlayerShip::Initialise(Mesh& mesh, float colliderRadius)
{
  GameModel::Initialise(mesh);
  MaterialExt playerMaterial = mesh.GetSubMesh(0).material_;
  playerMaterial.passIterator = 2;
  playerMaterial.flags |= MaterialExt::OUTLINED;
  //outline colour and thickness
  playerMaterial.shaderVariables = { static_cast<Vector3>(Colors::Yellow), kShaderOutlineSize_ };
  SetOverrideMat(&playerMaterial);
  collider_.Radius = colliderRadius;
}

void PlayerShip::Start()
{
  SetPosition(kInitialPlayerStartPosition_);
  collider_.Center = GetPosition();
  SetVelocity(kInitialPlayerVelocity_);
  currentFuel_ = kMaximumFuel_;
  currentState_ = PlayerState::FLYING;
  orbitDir_ = OrbitDirection::NONE;
  currentOrbitAngle_ = 0;
  boostActive_ = false;
  StartEngineEmitter();
  audio_.PlayEngineSound();
}

DirectX::SimpleMath::Vector3 PlayerShip::GetVelocity() const
{
  return mobileBody_.GetVelocity();
}

void PlayerShip::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
  collider_.Center = pos;
  GameObject::SetPosition(pos);
}

void PlayerShip::SetPosition(float xPos, float yPos, float zPos)
{
  collider_.Center = DirectX::SimpleMath::Vector3(xPos, yPos, zPos);
  GameObject::SetPosition(xPos, yPos, zPos);
}

float PlayerShip::GetFuel() const
{
  return currentFuel_;
}

float PlayerShip::GetMaximumFuel() const
{
  return kMaximumFuel_;
}

void PlayerShip::Boost(float dTime)
{
  switch (currentState_)
  {
  case PlayerShip::LAUNCHING:
    break;
  case PlayerShip::FLYING:
    if (!boostActive_)
      if (currentFuel_ > kFuelPerBoost_)
      {
        currentFuel_ -= kFuelPerBoost_;
        {
          boostActive_ = true;
          timeUntilNextBoost_ = kTimeBetweenBoosts_;
          mobileBody_.IncreaseVelocity(kBoostForce_);
          audio_.PlayBoostActivatedSound();
        }
      }
      else {
        audio_.PlayBoostFailedSound();
      }
    break;
  case PlayerShip::ORBITING:
    currentState_ = LAUNCHING;
    mobileBody_.IncreaseVelocity(kBoostForce_ / 2);
    audio_.PlayBoostActivatedSound();
    break;
  default:
    break;
  }
}


void PlayerShip::DirectionalBoost(int direction, float dTime)
{
  if (currentFuel_ > kFuelPerBoost_ * dTime) {
    currentFuel_ -= kFuelPerBoost_ * dTime;
    DirectX::SimpleMath::Vector3 currentVelocity = mobileBody_.GetVelocity();
    float horizontalMotion;// xcos thet - ysin thet
    float verticalMotion;// xsin thet + ycos thet	

    float angle = dTime * 2 * PI / 4 * direction; // Dividing by 4 here to make it so that we can do a quarter turn in a second
    horizontalMotion = currentVelocity.x * cos(angle) - currentVelocity.y * sin(angle);
    verticalMotion = currentVelocity.x * sin(angle) + currentVelocity.y * cos(angle);
    SetVelocity(DirectX::SimpleMath::Vector3(horizontalMotion, verticalMotion, 0.0f));
  }
  else {
    audio_.PlayBoostFailedSound();
  }
}

const DirectX::BoundingSphere& PlayerShip::GetCollider() const
{
  return collider_;
}

void PlayerShip::Update(float dTime)
{
  switch (currentState_)
  {
  case PlayerShip::FLYING:
    if (boostActive_)
    {
      timeUntilNextBoost_ -= dTime; // TODO - Nico convert this into a lerp for smoother behaviour and to stop rounding errors
      if (timeUntilNextBoost_ < 0)
      {
        boostActive_ = false;
        timeUntilNextBoost_ = 0;
        audio_.PlayBoostReadySound();
      }
    }
    break;
  case PlayerShip::ORBITING:
    if (!pCurrentlyOrbiting_->IsDisintegrated())
    {
      switch (orbitDir_)
      {
      case PlayerShip::CLOCKWISE:
        currentOrbitAngle_ -= anglePerSec * dTime;
        break;
      case PlayerShip::ANTICLOCKWISE:
        currentOrbitAngle_ += anglePerSec * dTime;
        break;
      }
      SetOrbitalVelocity();
      audio_.PlayOrbitingSound();
    }
    else {
      Boost(dTime);
    }
    break;
  case PlayerShip::LAUNCHING:
    DirectX::SimpleMath::Vector3 tempCollision;
    if (!SphereToSphere(collider_, pCurrentlyOrbiting_->GetGravityCollider(), tempCollision))
    {
      currentState_ = FLYING;
      pCurrentlyOrbiting_ = nullptr;
    }
    break;
  }
  /*Move(dTime);*/

  mobileBody_.Update(dTime);
  RotateShipToVelocityFacing();
  ApplyEngineEmitterPositionOffset();


}

void PlayerShip::CheckOrbit(Planet * nearestPlanet)
{
  assert(nearestPlanet != nullptr);
  if (currentState_ == FLYING)
  {
    //DirectX::SimpleMath::Vector3 vel = mobileBody_.GetVelocity();
    //vel.Normalize();
    mOrbitEntryDirectionNormal = nearestPlanet->GetPosition() - GetPosition(); // Direction from ship to planet
    mOrbitEntryDirectionNormal.Normalize();

    if (mOrbitEntryDirectionNormal.Dot(mobileBody_.GetDirectionVector()) <= 0)
    {
      // we are travelling perpendicular to the planet core now, start to orbit
      DirectX::SimpleMath::Vector3 radiusOfmotion = GetPosition() - nearestPlanet->GetPosition(); // Direction from planet to ship
      radius = radiusOfmotion.Length();
      radiusOfmotion.Normalize();
      mOrbitEntryVelocity = mobileBody_.GetVelocity();

      DirectX::SimpleMath::Vector3 zeroRotation = DirectX::SimpleMath::Vector3::Right;

      float dotProduct = radiusOfmotion.Dot(zeroRotation);
      float determinant = (zeroRotation.x * radiusOfmotion.y) - (zeroRotation.y * radiusOfmotion.x);

      float rotationAngle = atan2f(determinant, dotProduct);

      currentOrbitAngle_ = rotationAngle + PI / 2; // The + 90 degrees is to correct the fact that our planet rotation cycle starts from -90 as the ship is pointing right
      anglePerSec = (mOrbitEntryVelocity.Length() / (2 * PI * radius)) * 2 * PI;

      currentState_ = ORBITING;
      pCurrentlyOrbiting_ = nearestPlanet;
      SetOrbitDirection();
      audio_.PlayEnteredOrbit();
    }
  }
}

void PlayerShip::SetVelocity(DirectX::SimpleMath::Vector3 velocity)
{
  mobileBody_.SetVelocity(velocity);
}

void PlayerShip::Die()
{
  audio_.PlayExplosion();
}

void PlayerShip::CollectFuel(int amount)
{
  if (currentFuel_ + amount <= kMaximumFuel_)
  {
    currentFuel_ += amount;
  }
  else {
    currentFuel_ = kMaximumFuel_;
  }
}


void PlayerShip::Stop()
{
  pEngineEmitter_->alive_ = false;
}

//void PlayerShip::Move(float dTime)
//{
//  SetPosition(GetPosition() + (mobileBody_.GetVelocity() * dTime));
//}

void PlayerShip::RotateShipToVelocityFacing()
{
  DirectX::SimpleMath::Vector3 directionVector = mobileBody_.GetDirectionVector(); //mobileBody_.GetVelocity();
  DirectX::SimpleMath::Vector3 zeroRotation = DirectX::SimpleMath::Vector3::Right; // Nico - Ship Model is rotated right by default, so any rotations applied need to have Right as rotation 0;

  //directionVector.Normalize();
  float dotProduct = directionVector.Dot(zeroRotation);
  float determinant = (zeroRotation.x * directionVector.y) - (zeroRotation.y * directionVector.x);

  float rotationAngle = atan2f(determinant, dotProduct);

  SetRotation(DirectX::SimpleMath::Vector3(0, 0, rotationAngle));
  pEngineEmitter_->data_.mOrientation = Matrix::CreateRotationZ(rotationAngle + PI / 2);

}

void PlayerShip::SetOrbitalVelocity()
{
  float horizontalMotion;
  float verticalMotion;
  horizontalMotion = cos(currentOrbitAngle_);
  verticalMotion = sin(currentOrbitAngle_);
  SetVelocity(DirectX::SimpleMath::Vector3(mobileBody_.GetVelocity().Length() * horizontalMotion, mobileBody_.GetVelocity().Length() * verticalMotion, 0.0f));
}

void PlayerShip::SetOrbitDirection()
{
  if (mOrbitEntryVelocity.x >= 0)					// Moving to the right
  {
    if (mOrbitEntryVelocity.y > 0)					// Moving Upwards
    {
      if (mOrbitEntryDirectionNormal.x >= 0)			// If on the left side
      {
        SetClockwise();
      }
      else {											// If on right side
        SetAntiClockwise();
      }
    }
    else {
      if (mOrbitEntryVelocity.y < 0) {											// Moving Down
        if (mOrbitEntryDirectionNormal.x >= 0)			// If on the left side
        {
          SetAntiClockwise();
        }
        else {											// If on right side
          SetClockwise();
        }
      }
      else {
        if (mOrbitEntryDirectionNormal.y > 0) // Travelling directly horizontal, need to use above/below to determine rotation direction
        {
          SetAntiClockwise();
        }
        else {
          SetClockwise();
        }
      }
    }
  }
  else {											// Moving Left
    if (mOrbitEntryVelocity.y > 0)					// Moving Upwards
    {
      if (mOrbitEntryDirectionNormal.x >= 0)			// If on left side
      {
        SetClockwise();
      }
      else {											 // If on the right side
        SetAntiClockwise();
      }
    }
    else {											// Moving Down
      if (mOrbitEntryVelocity.y < 0) {
        if (mOrbitEntryDirectionNormal.x >= 0)			// If on the left side
        {
          SetAntiClockwise();
        }
        else {											// If on right side
          SetClockwise();
        }
      }
      else {
        if (mOrbitEntryDirectionNormal.y > 0) // Travelling directly horizontal, need to use above/below to determine rotation direction
        {
          SetAntiClockwise();
        }
        else {
          SetClockwise();
        }
        SetAntiClockwise();
      }
    }
  }
}

void PlayerShip::SetClockwise()
{
  orbitDir_ = CLOCKWISE;
  currentOrbitAngle_ += PI;
}

void PlayerShip::SetAntiClockwise()
{
  orbitDir_ = ANTICLOCKWISE;
}

void PlayerShip::ApplyEngineEmitterPositionOffset()
{
  pEngineEmitter_->data_.pos_ = GetPosition() - (mobileBody_.GetDirectionVector());
}

void PlayerShip::StartEngineEmitter()
{
  pEngineEmitter_ = GetParticleSys()->GetNewEmitter();
  pEngineEmitter_->data_ = kThrusterEmitterDef;
  pEngineEmitter_->data_.spriteIdx_ = Random::Instance()->Range(kFirstEmitterSpriteIndex_, kLastEmitterSpriteIndex_);
  pEngineEmitter_->alive_ = true;
  ApplyEngineEmitterPositionOffset();
}
