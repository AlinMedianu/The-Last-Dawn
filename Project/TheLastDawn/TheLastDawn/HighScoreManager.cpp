// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#include "HighScoreManager.h"

HighScoreManager::HighScoreManager()
  :highScores_(kNumberOfHighScoresToStore_)
{
}

HighScoreManager::~HighScoreManager()
{
}

void HighScoreManager::LoadHighScores()
{
  assert(GetLoadSaveManager() != nullptr);

  GetLoadSaveManager()->Load(*this);
}

void HighScoreManager::SaveHighScores()
{
  assert(GetLoadSaveManager() != nullptr);

  GetLoadSaveManager()->SaveItem(*this);
}

bool HighScoreManager::IsHighScore(const Score& newScore) const
{
  // Assumes vector is in a sorted state, lowest score in position 0
  return newScore > highScores_.at(0);
}

void HighScoreManager::AddNewHighScore(const Score& newScore)
{
  highScores_.at(0) = newScore;
  SortScores();
}

const Score& HighScoreManager::GetHighScoreAt(int index) const
{
  //assert(index >= 0 && index < mNumberOfHighScoresToStore);
  return highScores_.at(index);
}

int HighScoreManager::GetNumOfHighScoresStored() const
{
  return kNumberOfHighScoresToStore_;
}

std::ostream& HighScoreManager::ToStream(std::ostream& os) const
{
  os << "HIGHSCORES" << ";";
  for (int i = 0; i < kNumberOfHighScoresToStore_; i++)
  {
    os << highScores_.at(i);
  }
  return os;
}

std::istream& HighScoreManager::FromStream(std::istream& is)
{
  std::string verify;
  std::getline(is, verify, ';');
  if (verify == "HIGHSCORES")
  {
    for (int i = 0; i < kNumberOfHighScoresToStore_; i++)
    {
      is >> highScores_.at(i);
    }
  }
  else {
    //assert(false);
  }
  return is;
}

void HighScoreManager::SortScores()
{
  std::sort(highScores_.begin(), highScores_.end(),
    [](const Score & lhs, const Score & rhs) { return lhs.GetScore() < rhs.GetScore(); }); // sorts using less than, gives ascending order
}

