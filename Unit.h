#pragma once
#include "Sprite.h"
class Player;
#include "Player.h"
#include <vector>
#include "Map.h"
#include "MapSprite.h"
#include <cmath>
#include <sstream>

/*
	TODO:
	Moving the position of a sprite requires the map to be updated.



*/

enum UNIT_STATUS 
{
	DEAD,
	ALIVE,
	BIRTHING,
	ATTACKING,
	WALKING
};

class Unit : public MapSprite
{
public:
	// Constructor(s)/Destructor
	Unit(HDC hDC, HINSTANCE hInstance,UINT BITMAP_ID) :MapSprite(hDC, hInstance, BITMAP_ID) {};
	Unit(Bitmap* pBitmap, UINT BITMAP_ID) :MapSprite(pBitmap, BITMAP_ID) {};
	Unit(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) : MapSprite(pBitmap, rcBounds, BITMAP_ID, baBoundsAction) {};
	Unit(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		MapSprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, BITMAP_ID, baBoundsAction) {};
	virtual ~Unit();
	BOOL isSelected = false;

	//Functions
	void Unit::SetDestination(int x, int y, int cellWidth, int cellHeight);
	void Pathfind(int **map);
	void MoveToPoint();

//Members
protected:
	Player *m_player;
	float m_movementSpeed = 1.0f;
	POINT m_destinationIndex = POINT{-1,-1};
	POINT m_destination = POINT{-1,-1};
	int status = UNIT_STATUS::ALIVE;
	int m_health=100;

//Methods
public:
	//GETTERS/SETTERS
	Player* GetPlayer() { return m_player; }
	void SetPlayer(Player* player) { m_player = player; }
	const float& GetMovementSpeed() { return m_movementSpeed; }
	void SetMovementSpeed(float movementSpeed) { m_movementSpeed = movementSpeed; }
	void Update() override;
	void OnCollisionEnter(Sprite* otherSprite) override;
	void OnCollisionStay(Sprite* otherSprite) override;
	void PreventOverlap(Sprite* otherSprite);
	virtual void          Draw(HDC hDC, Camera* cam) override
	{
		Sprite::Draw(hDC, cam);
		//Apply text drawing here
		int rectWidth = GetWidth() + 30;
		RECT rect = RECT{ GetPosition().left - cam->GetPosition().x,GetPosition().top-35 - cam->GetPosition().y, GetPosition().right - cam->GetPosition().x,GetPosition().bottom - cam->GetPosition().y };
		int rectHeight=DrawText(hDC, TEXT(std::string("HP: "+std::to_string(GetHealth())).c_str()), -1, &rect, DT_CALCRECT);
		rect = RECT{ rect.left,rect.bottom,rect.right,rect.bottom + rectHeight };
		DrawText(hDC, TEXT(std::string("HP: " + std::to_string(GetHealth())).c_str()), -1, &rect,  DT_CENTER);
	};
	int GetStatus() { return status; };
	void SetStatus(UNIT_STATUS statusToSet) { status = statusToSet; };
	int GetHealth() { return m_health; };
	void SetHealth(int newHealth) { m_health = newHealth; };
	std::list<Sprite*> GetNeighboringCells();
};