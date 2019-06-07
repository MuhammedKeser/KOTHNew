#pragma once
#include "Sprite.h"
#include "Map.h"
#include <cmath>

class MapSprite : public Sprite
{
public:
	MapSprite(HDC hDC, HINSTANCE hInstance, UINT BITMAP_ID) :Sprite(hDC, hInstance, BITMAP_ID) {};
	MapSprite(Bitmap* pBitmap, UINT BITMAP_ID) :Sprite(pBitmap, BITMAP_ID) {};
	MapSprite(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, BITMAP_ID, baBoundsAction) {};
	MapSprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, BITMAP_ID, baBoundsAction) {};

	virtual ~MapSprite() 
	{
		Map::SetGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), 0);
		Map::SetSpriteGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), NULL);
	};

public:
	int GetXIndex(int mapCellWidth) { return floor(((m_rcPosition.left + m_rcPosition.right) / 2) / mapCellWidth); };
	int GetYIndex(int mapCellHeight) { return floor(((m_rcPosition.top + m_rcPosition.bottom) / 2) / mapCellHeight); };
};