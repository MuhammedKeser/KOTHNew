#pragma once
#include <windows.h>
#include "Bitmap.h"
#include "Camera.h"
#include "Sprite.h"
#include "Player.h"

class LivelySprite : public Sprite
{
public:
	struct PlayerScoreBoard
	{
		Player* player;
		int score;

		PlayerScoreBoard(Player* player)//standard CTOR
		{
			this->player = player;
			score = 0;
		}
		PlayerScoreBoard(PlayerScoreBoard* playerScoreBoardToCopy)//Copy CTOR
		{
			this->player = playerScoreBoardToCopy->player;
			this->score = playerScoreBoardToCopy->score;
		}
	};
	//TODO->Make these private, and add getters and setters
	RECT m_position;
	Bitmap* m_bitmap;

	int timeCountdownStarted = -1;
	int countdownInterval = 20;//Shows the countdown interval in seconds.
	//CTOR
	LivelySprite(HDC hDC, HINSTANCE hInstance) :Sprite(hDC, hInstance, IDB_LIVELY) {
		std::list<Player*>::iterator it;
		for (it = Player::playerList.begin(); it != Player::playerList.end(); it++)
		{
			LivelySprite::playerScoreBoard.push_back(new PlayerScoreBoard((*it)));
		}
	};
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

	void increasePlayerCount(Player* otherPlayer);
	void decreasePlayerCount(Player* otherPlayer);
	void HandleDisplay(HDC hDC);
	void HandleOccupyingPlayer();
	void startCountdown()
	{
		timeCountdownStarted = GetTickCount();
	}
	void stopCountdown()
	{
		timeCountdownStarted = -1;
	}

	void HandleCountdown();

	static std::list<PlayerScoreBoard*> playerScoreBoard;
	static PlayerScoreBoard* occupyingPlayerScoreBoard;

};