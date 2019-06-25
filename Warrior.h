#pragma once
#include "Unit.h"
#include <iostream>
#include "Resource.h"
#include "Windows.h"
#include "MMSystem.h"

class Warrior: public Unit
{
public:
	// Constructor(s)/Destructor
	Warrior(HDC hDC, HINSTANCE hInstance) :Unit(hDC, hInstance, IDB_WARRIORR) 
	{ 
		if (!m_pMountedBitmapR || !m_pMountedBitmapL)
		{
			m_pMountedBitmapR = new Bitmap(hDC, IDB_WARRIORMOUNTEDR, hInstance);
			m_warriorLA1 = new Bitmap(hDC, IDB_WARRIOR_LA1, hInstance);
			m_warriorLA2 = new Bitmap(hDC, IDB_WARRIOR_LA2, hInstance);
			m_warriorLW1 = new Bitmap(hDC, IDB_WARRIOR_LW1, hInstance);
			m_warriorLW2 = new Bitmap(hDC, IDB_WARRIOR_LW2, hInstance);
			m_warriorRA1 = new Bitmap(hDC, IDB_WARRIOR_RA1, hInstance);
			m_warriorRA2 = new Bitmap(hDC, IDB_WARRIOR_RA2, hInstance);
			m_warriorRW1 = new Bitmap(hDC, IDB_WARRIOR_RW1, hInstance);
			m_warriorRW2 = new Bitmap(hDC, IDB_WARRIOR_RW2, hInstance);
			m_warriorL = new Bitmap(hDC, IDB_WARRIORL, hInstance);
			m_warriorR = new Bitmap(hDC, IDB_WARRIORR, hInstance);
			m_pMountedBitmapL = new Bitmap(hDC, IDB_WARRIORMOUNTEDL, hInstance);
			
		}
	};
	Warrior(Bitmap* pBitmap) :Unit(pBitmap, IDB_WARRIORR) { };
	Warrior(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) : Unit(pBitmap, rcBounds, IDB_WARRIORR, baBoundsAction) {};
	Warrior(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Unit(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, IDB_WARRIORR, baBoundsAction) {
		

	};
	virtual ~Warrior() {
		std::cout << "Warrior destruct" << std::endl;
	};

	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void OnCollisionExit(Sprite* otherSprite) override;
	virtual void OnCollisionStay(Sprite* otherSprite) override;
	void Update() override;
	void Fight(Warrior* otherUnit);
	void HandleDeath();
	void handleBitmaps();

	//Getters/Setters
	int GetDamage() { return m_damage; };
	float GetAttackInterval() { return m_attackInterval; };
	float GetTimeOfLastAttack() { return m_timeOfLastAttack; };
	bool GetIsMounted() { return m_isMounted; };

	void SetDamage(int damage) { m_damage=damage; };
	void SetAttackInterval(float attackInterval) { m_attackInterval=attackInterval; };
	void SetTimeOfLastAttack(int timeOfLastAttack) { m_timeOfLastAttack=timeOfLastAttack; };
	void SetIsMounted(bool isMounted) { m_isMounted = isMounted; if (m_isMounted) { SetBitmap(m_pMountedBitmapR); m_movementSpeed = m_mountedMovementSpeed; } };
	void HandleWandering();
	
protected:
	//members
	int m_damage = 20;
	float m_attackInterval = 1.0f;//The time (in seconds) between attacks
	int m_timeOfLastAttack = -1;
	int m_timeOfLastAnimation = -1;
	int m_timeIntervalOfWaiting = 500;
	int m_timeIntervalOfAnimation = 1000;
	int m_timeIntervalOfWalking = 333;
	float m_mountedMovementSpeed = 10.0f;
	bool m_isMounted = false;
	bool sword = false;
	static Bitmap*       m_pMountedBitmapR;
	static Bitmap* m_pMountedBitmapL;
	static Bitmap* m_warriorLA1;
	static Bitmap* m_warriorLA2;
	static Bitmap* m_warriorLW1;
	static Bitmap* m_warriorLW2;
	static Bitmap* m_warriorRA1;
	static Bitmap* m_warriorRA2;
	static Bitmap* m_warriorRW1;
	static Bitmap* m_warriorRW2;
	static Bitmap* m_warriorL;
	static Bitmap* m_warriorR;
	

};
