#pragma once
#include "Sprite.h"
#include "Unit.h"
#include "Warrior.h"

class Horse : public MapSprite
{
public:
	static int horseCount;
	static int minHorseCount;
	static void HandleHorseSpawnBalance(HDC hDC);
	Horse(HDC hDC, HINSTANCE hInstance) :MapSprite(hDC, hInstance, IDB_HORSEYR,false) { InitializeHorse(); };
	Horse(Bitmap* pBitmap) :MapSprite(pBitmap, IDB_HORSEYL) { InitializeHorse(); };
	Horse(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) : MapSprite(pBitmap, rcBounds, IDB_HORSEYL, baBoundsAction) { InitializeHorse(); };
	Horse(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		MapSprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, IDB_HORSEYL, baBoundsAction) {
		InitializeHorse();
	};

	void InitializeHorse();
	void MoveInRandomDirection();
	void ReverseDirectionX();
	void ReverseDirectionY();

	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void Update() override;
protected:
	bool hasXBouncedThisCycle = false;
	bool hasYBouncedThisCycle = false;
	int timeLastGivenRandomDirection = -1;
	int minRandomDirectionInterval = 5;
	int maxRandomDirectionInterval = 7;
	void HandleMounting();
};

