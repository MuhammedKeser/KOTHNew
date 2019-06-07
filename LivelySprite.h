#pragma once
#include <windows.h>
#include "Bitmap.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"

class LivelySprite : public Sprite
{
public:
	//TODO->Make these private, and add getters and setters
	RECT m_position;
	Bitmap* m_bitmap;

	//CTOR
	LivelySprite(HDC hDC, HINSTANCE hInstance) :Sprite(hDC, hInstance, IDB_LIVELY) {  };
	LivelySprite(Bitmap* pBitmap) :Sprite(pBitmap, IDB_LIVELY) { };
	LivelySprite(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) : Sprite(pBitmap, rcBounds, IDB_LIVELY, baBoundsAction) {  };
	LivelySprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, IDB_LIVELY, baBoundsAction) {

	};
	//Methods

	virtual void Update() override;
	virtual ~LivelySprite() {};

	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void OnCollisionExit(Sprite* otherSprite) override;
	virtual void OnCollisionStay(Sprite* otherSprite) override;

	void increasePlayerCount(const Player& otherPlayer);
	void decreasePlayerCount(const Player& otherPlayer);

	static Player* occupyingPlayer;

	//if it is occupied by 1
	//or by player 2
	static int playerOneCount;
	static int playerTwoCount;
};