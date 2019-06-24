#pragma once
#include "Sprite.h"
class Player;
#include "Player.h"
#include <vector>
#include "Map.h"
#include "MapSprite.h"
#include <cmath>
#include <sstream>
#include <stack>
#include <algorithm>
#include <iostream>
/*
	TODO:
	Moving the position of a sprite requires the map to be updated.



*/
class StateHandler;

enum UNIT_STATUS 
{
	DEAD,
	ALIVE,
	BIRTHING,
	SAPPING,
	ATTACKING,
	WALKING,
	COMMANDED
};

class Unit : public MapSprite
{
public:
	Bitmap* healthBarRed	;
	Bitmap* healthBarGreen	;
	// Constructor(s)/Destructor
	Unit(HDC hDC, HINSTANCE hInstance,UINT BITMAP_ID) ;
	Unit(Bitmap* pBitmap, UINT BITMAP_ID) :MapSprite(pBitmap, BITMAP_ID) {};
	Unit(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) : MapSprite(pBitmap, rcBounds, BITMAP_ID, baBoundsAction) {};
	Unit(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		MapSprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, BITMAP_ID, baBoundsAction) {};
	virtual ~Unit();
	BOOL isSelected = false;

	//Functions
	void Unit::SetDestination(int x, int y, int cellWidth, int cellHeight);
	void Pathfind();
	void MoveToPoint();
	void LoseHealthOverTime();

	void InitializeHealthBarBitmaps(HDC hDC,HINSTANCE hInstance);
//Members
protected:
	struct PathfindingNode
	{
		PathfindingNode* parentNode = NULL;
		int xIndex = -1;
		int yIndex = -1;
		float totalPathCost = std::numeric_limits<float>::infinity();
		bool wasChecked = false;
		bool wasAddedToList = false;
		PathfindingNode(int yIndex, int xIndex)
		{
			this->xIndex = xIndex;
			this->yIndex = yIndex;
		}
	};
	std::stack<PathfindingNode*> path;//The path to take (for pathfinding)
	Player *m_player;
	int m_nextTimeToLoseHealth = -1;
	int m_secondsBetweenHealthLoss = 3;
	float m_movementSpeed = 5.0f;
	POINT m_destinationIndex = POINT{-1,-1};
	POINT m_destination = POINT{-1,-1};
	int status = UNIT_STATUS::ALIVE;
	int m_health=100;
	void HandlePathTraversal();
//Methods
public:
	StateHandler* stateHandler;
	int xIndex = 0;
	int yIndex = 0;
	void UpdateMapPosition();
	SPRITEACTION UpdatePosition()override;
	static bool pathfindingPerformedThisCycle;
	static bool cmp(PathfindingNode* a, PathfindingNode* b);
	//GETTERS/SETTERS
	Player* GetPlayer() { return m_player; }
	void SetPlayer(Player* player) { m_player = player; }
	const float& GetMovementSpeed() { return m_movementSpeed; }
	void SetMovementSpeed(float movementSpeed) { m_movementSpeed = movementSpeed; }
	void Update() override;
	void OnCollisionEnter(Sprite* otherSprite) override;
	void OnCollisionStay(Sprite* otherSprite) override;
	void PreventOverlap(Sprite* otherSprite);
	virtual void          Draw(HDC hDC, Camera* cam) override;
	void DrawHealthBar(HDC hDC, Camera* cam);
	int GetStatus() { return status; }; 
	void SetStatus(UNIT_STATUS statusToSet) { status = statusToSet; };
	int GetHealth() { return m_health; };
	void SetHealth(int newHealth) { m_health = newHealth; };
	std::list<Sprite*> GetNeighboringCells();
};