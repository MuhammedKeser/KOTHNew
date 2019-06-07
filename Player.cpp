#include "Player.h"
#include "Gatherer.h"
#include "Warrior.h"
#include "GameEngine.h"


Player::Player(std::string name) :m_Name(name)
{
}


void Player::RequestUnitSpawn(UNIT_TYPE typeOfUnitRequested, int xPosition, int yPosition)
{
	m_requestedUnitsToSpawn.push(new UnitSpawnRequest(typeOfUnitRequested, xPosition,yPosition));

}

void Player::HandleRequestedUnitSpawns(HDC hDC, GameEngine* pGame)
{
	while (!m_requestedUnitsToSpawn.empty())
	{
		UnitSpawnRequest request = *m_requestedUnitsToSpawn.front();
		//Not the most beautiful way to handle it, but I have yet to figure out how to hold types as data
		switch (request.type)
		{
		case UNIT_TYPE::GATHERER:
			SpawnUnit<Gatherer>(hDC, pGame, request.xPosition, request.yPosition);
			break;
		case UNIT_TYPE::WARRIOR:
			SpawnUnit<Warrior>(hDC, pGame, request.xPosition, request.yPosition);
			break;
		}

		m_requestedUnitsToSpawn.pop();
	}
}