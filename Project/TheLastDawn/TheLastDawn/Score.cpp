// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "Score.h"

Score::Score()
  : playerName_("Player"), timeSurvived_(0.0f), distanceTravelled_(0.0f), fuelLeft_(0.0f), maximumFuelThisGame_(0.0f)
{
}

Score::~Score()
{
}

Score::Score(const Score& obj)
{
  playerName_ = obj.playerName_;
  timeSurvived_ = obj.timeSurvived_;
  distanceTravelled_ = obj.distanceTravelled_;
  fuelLeft_ = obj.fuelLeft_;
  maximumFuelThisGame_ = obj.maximumFuelThisGame_;
}

void Score::ResetScore(float maximumFuel)
{
  timeSurvived_ = 0.0f;
  distanceTravelled_ = 0.0f;
  maximumFuelThisGame_ = maximumFuel;
  fuelLeft_ = maximumFuelThisGame_;
}

void Score::Update(float dTime, float distance, float fuel)
{
  IncrementTime(dTime);
  distanceTravelled_ = distance;
  fuelLeft_ = fuel;
}

std::string Score::GetPlayerName() const
{
  return playerName_;
}

float Score::GetTimeSurvived() const
{
  return timeSurvived_;
}

int Score::GetScore() const
{
  return static_cast<int>(distanceTravelled_ + timeSurvived_ + (maximumFuelThisGame_ - fuelLeft_));
}

float Score::GetFuelLeft() const
{
	return fuelLeft_;
}

float Score::GetMaxFuelThisGame() const
{
  return maximumFuelThisGame_;
}

void Score::SetPlayerName(const std::string& desiredName)
{
  assert(playerName_.length() <= kMaximumPlayerNameLength_);
  playerName_ = desiredName;
}

bool Score::operator>(const Score& rhs) const
{
  return this->GetScore() > rhs.GetScore();
}

bool Score::operator<(const Score& rhs) const
{
  return !(*this > rhs);
}

Score& Score::operator=(const Score& rhs)
{
  playerName_ = rhs.playerName_;
  timeSurvived_ = rhs.timeSurvived_;
  distanceTravelled_ = rhs.distanceTravelled_;
  fuelLeft_ = rhs.fuelLeft_;
  maximumFuelThisGame_ = rhs.maximumFuelThisGame_;
  return *this;
}

std::ostream& Score::ToStream(std::ostream& os) const
{
  os << "SCOREDATA" << ";" <<  playerName_ << ";" << timeSurvived_ << ";" << distanceTravelled_ << ";" << fuelLeft_ << ";" << maximumFuelThisGame_ << ";";
  return os;
}

std::istream& Score::FromStream(std::istream& is)
{
  char separator;
  std::string verify;
  std::getline(is, verify, ';');
  if (verify == "SCOREDATA")
  {
    std::getline(is, playerName_, ';');
    is >> timeSurvived_ >> separator >> distanceTravelled_ >> separator >> fuelLeft_ >> separator >> maximumFuelThisGame_ >> separator;
  }
  else {
    assert(false);
  }
  return is;
}

void Score::IncrementTime(float dTime)
{
  timeSurvived_ += dTime;
}

