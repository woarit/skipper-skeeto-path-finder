#pragma once

#include <iostream>
#include <vector>

using State = unsigned long long int;

class GraphPath {
public:
  GraphPath(char startVertexIndex);

  GraphPath(char vertexIndex, GraphPath *previousPath, char extraDistance);

  void initialize(const std::vector<GraphPath> &paths);

  char getCurrentVertex() const;

  bool isInitialized() const;

  bool isExhausted() const;

  bool isFinished() const;

  State getUniqueState() const;

  unsigned char getDistance() const;

  State getVisitedVertices() const;

  bool meetsCondition(const State &condition) const;

  bool hasVisitedVertex(char vertexIndex) const;

  std::vector<GraphPath>::iterator getFocusedNextPath();

  void eraseNextPath(const std::vector<GraphPath>::iterator &pathIterator);

  void bumpFocusedNextPath();

  int getNextPathsCount() const;

  std::vector<char> getRoute() const;

  void serialize(std::ostream &outstream) const;

  void deserialize(std::istream &instream, const GraphPath *previousPath);

  void cleanUp();

private:
  GraphPath() = default;

  void setCurrentVertex(char vertexIndex);

  template <size_t StartIndex, size_t Count>
  void setState(State newState) {
    state &= (~(((1ULL << Count) - 1) << StartIndex));
    state |= (newState << StartIndex);
  };

  template <size_t StartIndex, size_t Count>
  State getState() const {
    return (state >> StartIndex) & ((1ULL << Count) - 1);
  };

  State state{};

  const GraphPath *previousPath = nullptr;
  std::vector<GraphPath> nextPaths;
};