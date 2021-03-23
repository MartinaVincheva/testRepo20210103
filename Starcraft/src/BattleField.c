#include "BattleField.h"

void generateTerranFleet(BattleField *battleField, const char *terranFleetStr) {
}

void generateProtossFleet(BattleField *battleField, const char *protossFleetStr) {
}

void startBattle(BattleField *battleField) {
  while (true) {
    if (processTerranTurn(battleField)) {
      printf("TERRAN has won!\n");
      break;
    }

    if (processProtossTurn(battleField)) {
      printf("PROTOSS has won!\n");
      break;
    }
  }
}

void deinit(BattleField *battleField) {
}

bool processTerranTurn(BattleField *battleField) {
  return false;
}

bool processProtossTurn(BattleField *battleField) {
  return false;
}

