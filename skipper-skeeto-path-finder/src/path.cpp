#include "skipper-skeeto-path-finder/path.h"

#include "skipper-skeeto-path-finder/info.h"
#include "skipper-skeeto-path-finder/item.h"
#include "skipper-skeeto-path-finder/room.h"
#include "skipper-skeeto-path-finder/task.h"

#include <algorithm>

Path::Path(const std::vector<const Item *> &allItems, const std::vector<const Task *> &allTasks, const Room *startRoom) {
  state = getStateWithRoom(state, startRoom);
}

Path::Path(const Path &path) {
  previousPath = path.previousPath;
  steps = path.steps;
  enteredRoomsCount = path.enteredRoomsCount;
  state = path.state;
  foundItems = path.foundItems;
  completedTasks = path.completedTasks;
  depth = path.depth;

  // Note that we don't copy subPathInfo, since that should be clean
}

Path Path::createFromSubPath(std::vector<const Room *> subPath) const {
  Path path(*this);

  path.steps.clear();
  path.previousPath = this;
  path.depth++;

  path.enterRooms(subPath);

  return std::move(path);
}

void Path::pickUpItem(const Item *item) {
  if (hasFoundItem(item)) {
    throw std::exception("Item was already picked up");
  }

  state |= (1ULL << item->stateIndex);
  foundItems |= (1ULL << item->uniqueIndex);
  steps.push_back(item);
}

void Path::pickUpItems(const std::vector<const Item *> &items) {
  for (const auto &item : items) {
    pickUpItem(item);
  }
}

void Path::completeTask(const Task *task) {
  if (hasCompletedTask(task)) {
    throw std::exception("Task was already completed");
  }

  state |= (1ULL << task->stateIndex);
  completedTasks |= (1ULL << task->uniqueIndex);
  steps.push_back(task);
}

void Path::completeTasks(const std::vector<const Task *> &tasks) {
  for (const auto &task : tasks) {
    completeTask(task);
  }
}

void Path::enterRoom(const Room *room) {
  steps.push_back(room);
  ++enteredRoomsCount;
  state = getStateWithRoom(state, room);
}

void Path::enterRooms(const std::vector<const Room *> rooms) {
  for (const auto &room : rooms) {
    enterRoom(room);
  }
}

int Path::getCurrentRoomIndex() const {
  return ((1ULL << STATE_ROOM_INDEX_SIZE) - 1) & state;
}

unsigned char Path::getVisitedRoomsCount() const {
  return enteredRoomsCount;
}

bool Path::isDone() const {
  return (1ULL << STATE_TASK_ITEM_SIZE) - 1 == (state >> STATE_TASK_ITEM_START);
};

const State &Path::getState() const {
  return state;
}

std::vector<const Action *> Path::getSteps() const {
  std::vector<const Action *> combinedSteps;

  if (previousPath != nullptr) {
    auto firstSteps = previousPath->getSteps();
    combinedSteps.insert(combinedSteps.end(), firstSteps.begin(), firstSteps.end());
  }

  combinedSteps.insert(combinedSteps.end(), steps.begin(), steps.end());

  return combinedSteps;
}

bool Path::hasFoundItem(const Item *item) const {
  return foundItems & (1ULL << item->uniqueIndex);
}

bool Path::hasCompletedTask(const Task *task) const {
  return completedTasks & (1ULL << task->uniqueIndex);
}

State Path::getStateWithRoom(const State &state, const Room *room) {
  return ((~((1ULL << STATE_ROOM_INDEX_SIZE) - 1) & state) | room->roomIndex);
}
