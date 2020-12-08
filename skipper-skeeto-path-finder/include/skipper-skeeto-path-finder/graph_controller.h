#pragma once

#include "skipper-skeeto-path-finder/graph_common_state.h"

class GraphData;

class GraphController {
public:
  GraphController(const GraphData *data);

  void start();

private:
  static const char *MEMORY_DUMP_DIR;

  void setupStartRunner();

  bool moveOnDistributed(GraphPathPool *pool, RunnerInfo *runnerInfo, unsigned long int pathIndex, GraphPath *path, unsigned char visitedVertices);

  bool initializePath(GraphPathPool *pool, unsigned long int pathIndex, GraphPath *path);

  void splitAndRemove(GraphPathPool *pool, RunnerInfo *runnerInfo);

  std::pair<unsigned long int, GraphPath *> movePathData(GraphPathPool *sourcePool, GraphPathPool *destinationPool, unsigned long int sourcePathIndex, unsigned long int destinationParentPathIndex);

  void serializePool(GraphPathPool *pool, RunnerInfo *runnerInfo);

  void serializePool(GraphPathPool *pool, unsigned int runnerInfoIdentifier);

  void deserializePool(GraphPathPool *pool, RunnerInfo *runnerInfo);

  GraphCommonState commonState;
  const GraphData *data;

  std::string resultDirName;
};