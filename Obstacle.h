#pragma once
#include "Sprite.h"
#include "Resource.h"

class Obstacle :Sprite
{
public:
	// Constructor(s)/Destructor
	Obstacle(HDC hDC, HINSTANCE hInstance) :Sprite(hDC, hInstance) {};
	Obstacle(Bitmap* pBitmap) :Sprite(pBitmap) {};
	Obstacle(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) : Sprite(pBitmap, rcBounds, baBoundsAction) {};
	Obstacle(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, baBoundsAction) {};
	virtual ~Obstacle() {};

protected:
	UINT BITMAP_ID = IDB_GOLFBALL;

};