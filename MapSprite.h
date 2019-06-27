#pragma once
#include "Sprite.h"
#include "Map.h"
#include <cmath>

class MapSprite : public Sprite
{
public:
	bool isOnMap=true;
	MapSprite(HDC hDC, HINSTANCE hInstance, UINT BITMAP_ID,bool isOnMap=true) :isOnMap(isOnMap),Sprite(hDC, hInstance, BITMAP_ID) {};
	MapSprite(Bitmap* pBitmap, UINT BITMAP_ID, bool isOnMap = true) :Sprite(pBitmap, BITMAP_ID) {};
	MapSprite(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, BITMAP_ID, baBoundsAction) {};
	MapSprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, BITMAP_ID, baBoundsAction) {};

	virtual ~MapSprite() 
	{
		if(isOnMap)
		{
			Map::SetGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), 0);
			Map::SetSpriteGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), NULL);
		}
	};

public:
	int GetXIndex(int mapCellWidth) { return max(0, min(Map::GetWidth() - 1, floor(((m_rcPosition.left + m_rcPosition.right) / 2) / mapCellWidth))); };
	int GetYIndex(int mapCellHeight) { return max(0, min(Map::GetHeight() - 1, floor(((m_rcPosition.top + m_rcPosition.bottom) / 2) / mapCellHeight))); };
	int GetXIndexByRightPosition(int mapCellWidth) { return max(0, min(Map::GetWidth() - 1, floor( m_rcPosition.right / mapCellWidth))); };
	int GetXIndexByLeftPosition(int mapCellWidth) { return max(0,min(Map::GetWidth()-1,floor( m_rcPosition.left / mapCellWidth))); };
	int GetYIndexByBottomPosition(int mapCellHeight) { return max(0, min(Map::GetHeight() - 1, floor(m_rcPosition.bottom / mapCellHeight))); };
	int GetYIndexByTopPosition(int mapCellHeight) { return max(0, min(Map::GetHeight() - 1, floor(m_rcPosition.top/ mapCellHeight))); };
};