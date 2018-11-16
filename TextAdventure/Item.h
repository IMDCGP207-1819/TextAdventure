#pragma once

#include <string>

class Item {
public:
  Item(std::string _name, std::string desc, int room = 0)
      : room_id(room), name(_name), description(desc) {}

  const std::string GetName() const;
  const std::string GetDescription() const { return description; }
  const int GetTargetRoom() const { return room_id; }

private:
  int room_id;
  std::string name;
  std::string description;
};
