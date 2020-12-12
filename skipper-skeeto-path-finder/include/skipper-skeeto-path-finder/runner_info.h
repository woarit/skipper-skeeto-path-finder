#pragma once

#include <mutex>
#include <vector>

class RunnerInfo {
public:
  RunnerInfo(const std::vector<char> &parentPath);

  unsigned int getIdentifier() const;

  void setHighScore(unsigned char score);

  unsigned char getHighScore() const;

  int getVisitedVerticesCount() const;

  std::vector<char> getRoute() const;

  RunnerInfo makeSubRunner(char vertices);

private:
  static unsigned int createIdentifier();

  static std::mutex identifierMutex;
  static unsigned int nextIdentifier;

  std::vector<char> parentPath;
  unsigned int identifier;
  unsigned char highscore = 255;
};