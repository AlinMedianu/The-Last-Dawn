// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef SCORE_H
#define SCORE_H

#include "ISerializable.h"

#include <string>   // For storing player name

#include <cassert>  // For debugging

/// <summary>
/// Keeps track of a game state and can calculate the score obtained.
/// <para>
/// Can be used with the &lt;&lt; and &gt;&gt; operators to serialise the score intuitively;
/// </para>
/// </summary>
class Score : public ISerializable
{
public:
  Score();
  ~Score();

  Score(const Score& obj);

  void ResetScore(float maximumFuel);
  void Update(float dTime, float distance, float fuel);

  std::string GetPlayerName() const;
  float GetTimeSurvived() const;
  int GetScore() const;
  float GetFuelLeft() const;
  float GetMaxFuelThisGame() const;

  void SetPlayerName(const std::string& desiredName);

  bool operator>(const Score& rhs) const;
  bool operator<(const Score& rhs) const;

  Score& operator=(const Score& rhs);

  std::ostream& ToStream(std::ostream& os) const final;
  std::istream& FromStream(std::istream& is) final;

private:
  const size_t kMaximumPlayerNameLength_ = 10;

  std::string playerName_;
  float timeSurvived_;
  float distanceTravelled_;
  float fuelLeft_;
  float maximumFuelThisGame_;

  void IncrementTime(float dTime);
};

#endif // !SCORE_H
