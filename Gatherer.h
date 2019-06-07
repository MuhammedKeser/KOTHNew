#pragma once
#include "Unit.h"
#include <iostream>
#include "Resource.h"
#include "TreeSprite.h"
#include "Warrior.h"
class Gatherer : public Unit
{
public:
	// Constructor(s)/Destructor
	Gatherer(HDC hDC, HINSTANCE hInstance) :Unit(hDC, hInstance, IDB_GATHERERL) { isStatic = true; };
	Gatherer(Bitmap* pBitmap) :Unit(pBitmap, IDB_GATHERERL) { };
	Gatherer(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) : Unit(pBitmap, rcBounds, IDB_GATHERERL,baBoundsAction) {};
	Gatherer(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Unit(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, IDB_GATHERERL,baBoundsAction) {
	};
	virtual ~Gatherer() {};

	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void OnCollisionExit(Sprite* otherSprite) override;
	virtual void OnCollisionStay(Sprite* otherSprite) override;
	void ChangePlayer(Player* player);
	void Update() override;
	void RequestWinOver(Warrior* warrior);
	static int GetFoodNeededForBirth() { return m_foodNeededForBirth; };
	int GetTimeLastBirthed() { return m_timeLastBirthed; };
	int SetTimeLastBirthed(int timeBirthed) { m_timeLastBirthed = timeBirthed; };
private:
	void SapTree(TreeSprite* tree);
	void GiveBirth();
	void HandleBirth();

	//Members
	static int m_foodNeededForBirth;
	static int m_birthCooldown;
	static int m_timeTakenForBirth;
	int m_timeLastBirthed = -1;
	int m_timeToBirth=-1;
private:
	int sapSpeed = 1;
}; 
