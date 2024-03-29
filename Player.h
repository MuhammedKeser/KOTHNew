#pragma once
#include <string>
class Unit;
#include "Unit.h"
#include "Windows.h"
#include <queue>
class GameEngine;
#include <vector>
#include <list>
#include <iostream>
class Warrior;
class Gatherer;
enum UNIT_TYPE
{
	GATHERER,
	WARRIOR

};

class Player
{
	
public:
	struct UnitSpawnRequest
	{
		UNIT_TYPE type;
		int xPosition;
		int yPosition;
		UnitSpawnRequest(UNIT_TYPE type, int xPosition, int yPosition) :
			type(type), xPosition(xPosition), yPosition(yPosition) {};
	};
	//Members
	bool isMainPlayer = false;
	int playerIndex = -1;
	static std::list<Player*> playerList;
	std::string m_Name;
	std::list<Unit*> m_Units;
	int m_food=0;
	int m_gathererCount = 0;
	int m_warriorCount = 0;
	//CTOR
	Player(std::string name);

protected:
	std::queue<UnitSpawnRequest*> m_requestedUnitsToSpawn;

//Functions
public:

	static void InitializeUnitBitmaps(HDC hDC, HINSTANCE hInstance);

	static Bitmap* warriorR[3];
	static Bitmap* warriorRW1[3];
	static Bitmap* warriorRW2[3];
	static Bitmap* warriorRA1[3];
	static Bitmap* warriorRA2[3];
	static Bitmap* warriorRMounted[3];
				   
	static Bitmap* warriorL[3];
	static Bitmap* warriorLW1[3];
	static Bitmap* warriorLW2[3];
	static Bitmap* warriorLA1[3];
	static Bitmap* warriorLA2[3];
	static Bitmap* warriorLMounted[3];


	template <typename T>
	void SpawnUnit(HDC hDC,GameEngine* pGame, int x, int y)
	{
		T* newUnit = pGame->CreateSprite<T>(hDC);
		m_Units.push_back(newUnit);
		RECT newPosition = { x,y,x+ newUnit->GetWidth(),y+ newUnit->GetHeight() };
		newUnit->SetPosition(newPosition);
		newUnit->SetPlayer(this);
		newUnit->SetupBitmaps();
		int xIndex = newUnit->GetXIndex(Map::GetCellWidth());
		int yIndex = newUnit->GetYIndex(Map::GetCellHeight());
		Map::SetGridCell(yIndex,xIndex,4);
		Map::SetSpriteGridCell(yIndex, xIndex, newUnit);



		if (std::is_same_v<T, Gatherer>)
		{
			m_gathererCount++;
		}

		if (std::is_same_v<T, Warrior>)
		{
			m_warriorCount++;
			//TEST THE BITMAPS BY SWITCHING THEIR INDICES
			((Warrior*)newUnit)->m_pMountedBitmapR	= Player::warriorRMounted	[playerIndex];
			((Warrior*)newUnit)->m_pMountedBitmapL	= Player::warriorLMounted	[playerIndex];
			((Warrior*)newUnit)->m_warriorLA1		= Player::warriorLA1		[playerIndex];
			((Warrior*)newUnit)->m_warriorLA2		= Player::warriorLA2		[playerIndex];
			((Warrior*)newUnit)->m_warriorLW1		= Player::warriorLW1		[playerIndex];
			((Warrior*)newUnit)->m_warriorLW2		= Player::warriorLW2		[playerIndex];
			((Warrior*)newUnit)->m_warriorRA1		= Player::warriorRA1		[playerIndex];
			((Warrior*)newUnit)->m_warriorRA2		= Player::warriorRA2		[playerIndex];
			((Warrior*)newUnit)->m_warriorRW1		= Player::warriorRW1		[playerIndex];
			((Warrior*)newUnit)->m_warriorRW2		= Player::warriorRW2		[playerIndex];
			((Warrior*)newUnit)->m_warriorL			= Player::warriorL			[playerIndex];
			((Warrior*)newUnit)->m_warriorR			= Player::warriorR			[playerIndex];
		}

	}

	void RequestUnitSpawn(UNIT_TYPE typeOfUnitRequested, int xPosition = 0, int yPosition = 0);
	void HandleRequestedUnitSpawns(HDC hDC, GameEngine* pGame);

};