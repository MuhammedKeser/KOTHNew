#pragma once
#include <windows.h>
#include "Bitmap.h"
#include "Camera.h"

class Tile
{
public:
	//TODO->Make these private, and add getters and setters
	RECT m_position;
	Bitmap* m_bitmap;
	
	//CTOR
	Tile(RECT position, Bitmap* bitmap) :m_position(position), m_bitmap(bitmap) {};

	//Methods
	void Draw(HDC hDC, Camera* cam)
	{
		// Draw the sprite if it isn't hidden
		if (m_bitmap != NULL)
			m_bitmap->Draw(hDC,
				m_position.left - cam->GetPosition().x,
				m_position.top - cam->GetPosition().y ,
				1.0f,
				1.0f,
				TRUE);
		//m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
	}
};