#include "BattleField.h"
#include "terranShip.h"

void generateTerranFleet(BattleField *battleField, const char *terranFleetStr)
{
    int fleetSize = strlen(terranFleetStr);
    Vector *terranFleet = &battleField->terranFleet;

    vectorInit(terranFleet, sizeof(terranFleetStr));
    for (int idx = 0; idx < fleetSize; idx++)
    {
        switch (terranFleetStr[idx])
        {
        case 'v':
        case 'V':
        {
            vectorPush(terranFleet, createVikingShip(idx));
            break;
        }
        case 'b':
        case 'B':
        {
            vectorPush(terranFleet, createBattleShip(idx));
            break;
        }
        }
    }
}

void generateProtossFleet(BattleField *battleField, const char *protossFleetStr)
{
}

void startBattle(BattleField *battleField)
{
    while (true)
    {
        if (processTerranTurn(battleField))
        {
            printf("TERRAN has won!\n");
            break;
        }

        if (processProtossTurn(battleField))
        {
            printf("PROTOSS has won!\n");
            break;
        }
    }
}

void deinit(BattleField *battleField)
{
}

bool processTerranTurn(BattleField *battleField)
{
    return false;
}

bool processProtossTurn(BattleField *battleField)
{
    return false;
}
