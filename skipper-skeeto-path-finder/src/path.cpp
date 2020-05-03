#include "skipper-skeeto-path-finder/path.h"

#include "skipper-skeeto-path-finder/item.h"
#include "skipper-skeeto-path-finder/room.h"
#include "skipper-skeeto-path-finder/task.h"

#include <algorithm>

Path::Path(const std::vector<const Item *> &allItems, const std::vector<const Task *> &allTasks, const Room *startRoom) {
  this->currentRoom = startRoom;

  remainingItems = allItems;
  remainingTasks = allTasks;

  state = std::vector<bool>(remainingItems.size() + remainingTasks.size(), false);
}

Path::Path(const Path &path) {
  currentRoom = path.currentRoom;
  foundItems = path.foundItems;
  remainingItems = path.remainingItems;
  completedTasks = path.completedTasks;
  remainingTasks = path.remainingTasks;
  steps = path.steps;
  enteredRoomsCount = path.enteredRoomsCount;
  state = path.state;
  depth = path.depth;

  // Note that we don't copy subPathInfo, since that should be clean
}

Path Path::createFromSubPath(std::vector<const Room *> subPath) const {
  Path path(*this);

  path.depth++;

  path.enterRooms(subPath);

  return std::move(path);
}

void Path::pickUpItem(const Item *item) {
  auto itemIterator = std::find(remainingItems.begin(), remainingItems.end(), item);
  if (itemIterator == remainingItems.end()) {
    throw std::exception("Item was already picked up");
  }

  remainingItems.erase(itemIterator);
  foundItems.push_back(item);
  state[item->uniqueIndex] = true;

  steps.push_back(item);
}

void Path::pickUpItems(const std::vector<const Item *> &items) {
  for (const auto &item : items) {
    pickUpItem(item);
  }
}

void Path::completeTask(const Task *task) {
  auto taskIterator = std::find(remainingTasks.begin(), remainingTasks.end(), task);
  if (taskIterator == remainingTasks.end()) {
    throw std::exception("Task was already completed");
  }
  remainingTasks.erase(taskIterator);
  completedTasks.push_back(task);
  state[task->uniqueIndex] = true;

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
  currentRoom = room;
}

void Path::enterRooms(const std::vector<const Room *> rooms) {
  for (const auto &room : rooms) {
    enterRoom(room);
  }
}

const Room *Path::getCurrentRoom() const {
  return currentRoom;
}

int Path::getStepCount() const {
  return int(steps.size());
}

unsigned char Path::getVisitedRoomsCount() const {
  return enteredRoomsCount;
}

bool Path::isDone() const {
  return remainingItems.empty() && remainingTasks.empty();
};

const State &Path::getState() const {
  return state;
}

std::vector<const Item *> Path::getRemainingItemsForRoom(const Room *room) const {
  std::vector<const Item *> items;

  std::copy_if(
      remainingItems.begin(),
      remainingItems.end(),
      std::back_inserter(items),
      [room](const Item *item) {
        return item->room == room;
      });

  return items;
}

std::vector<const Task *> Path::getRemainingTasksForRoom(const Room *room) const {
  std::vector<const Task *> tasks;

  std::copy_if(
      remainingTasks.begin(),
      remainingTasks.end(),
      std::back_inserter(tasks),
      [room](const Task *task) {
        return task->room == room;
      });

  return tasks;
}

std::vector<const Action *> Path::getSteps() const {
  return steps;
}

bool Path::hasFoundItem(const Item *item) const {
  return std::find(foundItems.begin(), foundItems.end(), item) != foundItems.end();
}

bool Path::hasCompletedTask(const Task *task) const {
  return std::find(completedTasks.begin(), completedTasks.end(), task) != completedTasks.end();
}
