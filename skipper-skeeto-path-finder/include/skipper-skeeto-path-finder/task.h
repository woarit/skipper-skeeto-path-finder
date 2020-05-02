#pragma once

#include <string>
#include <vector>

class Room;
class Item;

class Task {
public:
  int uniqueIndex = -1;
  std::string key;
  Room *room = nullptr;
  Task *taskObstacle = nullptr;
  std::vector<Item *> itemsNeeded;
};