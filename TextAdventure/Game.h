#pragma once
#include <map>
#include <memory>

#include "Adventurer.h"
#include "Room.h"

struct CommandInfo {
  std::string description;
  std::vector<std::string> synonyms;
};

typedef std::map<std::string, std::unique_ptr<CommandInfo>> controlMap;

///
/// Game class, contains core game loop
///
class Game {
public:
  /// Run - handles all user input and output
  void Run();

private:
  /// Init - loads data files and creates "physical" space.
  void Init();
  void LoadItemData();
  void LoadRoomData();
  void LoadConfig();
  ExitDirections GetDirectionFromString(std::string dir);

  bool ChangeRoom(std::shared_ptr<Room> NewRoom);
  void HandleMovement(std::string direction);

  void DoRoomLook();
  void DoItemLook(std::string item_name);
  bool SearchInventory(std::string item_name,
                       const std::vector<std::unique_ptr<Item>> &inventory);
  void HandleItemTake(std::string item_name);
  void HandleDropItem(std::string item_name);
  void PrintHelp();

  std::string StandardiseCommandInput(std::string command);

  std::shared_ptr<Room> CurrentRoom;
  std::vector<std::shared_ptr<Room>> rooms;
  std::unique_ptr<Adventurer> Player;
  controlMap commandMap;
};
