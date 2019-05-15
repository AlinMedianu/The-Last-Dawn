// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include "Singleton.h"
#include "ISerializable.h"

#include <vector>             // For holding the high scores
#include <algorithm>          // For sorting the vector

#include "Score.h"            // For storing the data to save
#include "LoadSaveManager.h"  // For access to load/save out the highscores

#include <cassert>            // For debugging

/// <summary>
/// Keeps track of and allows management of the list of HighScores in the game.
/// <para>
/// Can be used with the &lt;&lt; and &gt;&gt; operators to serialise the entire high score list intuitively;
/// </para>
/// </summary>
class HighScoreManager : public Singleton<HighScoreManager>, public ISerializable
{
public:
  HighScoreManager();
  ~HighScoreManager();

  void LoadHighScores();
  void SaveHighScores();

  bool IsHighScore(const Score& newScore) const;
  void AddNewHighScore(const Score& newScore);
  const Score& GetHighScoreAt(int index) const;
  int GetNumOfHighScoresStored() const;

  std::ostream& ToStream(std::ostream& os) const final;
  std::istream& FromStream(std::istream& os) final;

private:
  const int kNumberOfHighScoresToStore_ = 10;
  std::vector<Score>highScores_;

  void SortScores();
};

// Declare the static singleton access method
SINGLETON_GET(HighScoreManager);

#endif // !HIGHSCOREMANAGER_H
