#pragma once
#include "Player.h"
#include "Gatherer.h"
#include "Warrior.h"
#include "GameEngine.h"

std::list<Player*> Player::playerList= std::list<Player*>();

Bitmap* Player::warriorR[3];
Bitmap* Player::warriorRW1[3];
Bitmap* Player::warriorRW2[3];
Bitmap* Player::warriorRA1[3];
Bitmap* Player::warriorRA2[3];
Bitmap* Player::warriorRMounted[3];
	   
Bitmap* Player::warriorL[3];
Bitmap* Player::warriorLW1[3];
Bitmap* Player::warriorLW2[3];
Bitmap* Player::warriorLA1[3];
Bitmap* Player::warriorLA2[3];
Bitmap* Player::warriorLMounted[3];

Player::Player(std::string name) :m_Name(name)
{
	playerIndex = Player::playerList.size();
	
	Player::playerList.push_back(this);


}

void Player::InitializeUnitBitmaps(HDC hDC, HINSTANCE hInstance)
{
	if (!warriorRW1[0])
	{
		//The first/main player
		warriorR		[0] =			new Bitmap(hDC, IDB_WARRIORR, hInstance);
		warriorRW1		[0] =			new Bitmap(hDC, IDB_WARRIOR_RW1, hInstance);
		warriorRW2		[0] =			new Bitmap(hDC, IDB_WARRIOR_RW2, hInstance);
		warriorRA1		[0] =			new Bitmap(hDC, IDB_WARRIOR_RA1, hInstance);
		warriorRA2		[0] =			new Bitmap(hDC, IDB_WARRIOR_RA2, hInstance);
		warriorRMounted	[0] =			new Bitmap(hDC, IDB_WARRIORMOUNTEDR, hInstance);
						 
		warriorL		[0] =			new Bitmap(hDC, IDB_WARRIORL, hInstance);
		warriorLW1		[0] =			new Bitmap(hDC, IDB_WARRIOR_LW1, hInstance);
		warriorLW2		[0] =			new Bitmap(hDC, IDB_WARRIOR_LW2, hInstance);
		warriorLA1		[0] =			new Bitmap(hDC, IDB_WARRIOR_LA1, hInstance);
		warriorLA2		[0] =			new Bitmap(hDC, IDB_WARRIOR_LA2, hInstance);
		warriorLMounted	[0] =			new Bitmap(hDC, IDB_WARRIORMOUNTEDL, hInstance);

		//The 2nd/hot/aggro player
		warriorR		[1] = new Bitmap(hDC, IDB_WARRIOR_RO, hInstance);
		warriorRW1		[1] = new Bitmap(hDC, IDB_WARRIOR_RW1O, hInstance);
		warriorRW2		[1] = new Bitmap(hDC, IDB_WARRIOR_RW2O, hInstance);
		warriorRA1		[1] = new Bitmap(hDC, IDB_WARRIOR_RA1O, hInstance);
		warriorRA2		[1] = new Bitmap(hDC, IDB_WARRIOR_RA2O, hInstance);
		warriorRMounted	[1] = new Bitmap(hDC, IDB_WARRIOR_RHO, hInstance);
						 
		warriorL		[1] = new Bitmap(hDC, IDB_WARRIOR_LO, hInstance);
		warriorLW1		[1] = new Bitmap(hDC, IDB_WARRIOR_LW1O, hInstance);
		warriorLW2		[1] = new Bitmap(hDC, IDB_WARRIOR_LW2O, hInstance);
		warriorLA1		[1] = new Bitmap(hDC, IDB_WARRIOR_LA1O, hInstance);
		warriorLA2		[1] = new Bitmap(hDC, IDB_WARRIOR_LA2O, hInstance);
		warriorLMounted	[1] = new Bitmap(hDC, IDB_WARRIOR_LHO, hInstance);

		//The 3RD/Cool player
		warriorR		[2] = new Bitmap(hDC, IDB_WARRIOR_RC, hInstance);
		warriorRW1		[2] = new Bitmap(hDC, IDB_WARRIOR_RW1C, hInstance);
		warriorRW2		[2] = new Bitmap(hDC, IDB_WARRIOR_RW2C, hInstance);
		warriorRA1		[2] = new Bitmap(hDC, IDB_WARRIOR_RA1C, hInstance);
		warriorRA2		[2] = new Bitmap(hDC, IDB_WARRIOR_RA2C, hInstance);
		warriorRMounted	[2] = new Bitmap(hDC, IDB_WARRIOR_RHC, hInstance);
						 
		warriorL		[2] = new Bitmap(hDC, IDB_WARRIOR_LC, hInstance);
		warriorLW1		[2] = new Bitmap(hDC, IDB_WARRIOR_LW1C, hInstance);
		warriorLW2		[2] = new Bitmap(hDC, IDB_WARRIOR_LW2C, hInstance);
		warriorLA1		[2] = new Bitmap(hDC, IDB_WARRIOR_LA1C, hInstance);
		warriorLA2		[2] = new Bitmap(hDC, IDB_WARRIOR_LA2C, hInstance);
		warriorLMounted	[2] = new Bitmap(hDC, IDB_WARRIOR_LHC, hInstance);

	}
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