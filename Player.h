#pragma once
#include <string>
class Unit;
#include "Unit.h"
#include "Windows.h"
#include <queue>
class GameEngine;
#include <vector>
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
	template <typename T>
	void SpawnUnit(HDC hDC,GameEngine* pGame, int x, int y)
	{
		T* newUnit = pGame->CreateSprite<T>(hDC);
		m_Units.push_back(newUnit);
		RECT newPosition = { x,y,x+ newUnit->GetWidth(),y+ newUnit->GetHeight() };
		newUnit->SetPosition(newPosition);
		newUnit->SetPlayer(this);

		if (std::is_same_v<T, Gatherer>)
			m_gathererCount++;

		if (std::is_same_v<T, Warrior>)
			m_warriorCount++;

	}

	void RequestUnitSpawn(UNIT_TYPE typeOfUnitRequested, int xPosition = 0, int yPosition = 0);
	void HandleRequestedUnitSpawns(HDC hDC, GameEngine* pGame);

};