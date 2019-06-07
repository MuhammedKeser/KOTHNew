#pragma once
#include "MapSprite.h"
#include <iostream>
#include <windows.h>
#include "Resource.h"
#include "Map.h"

class TreeSprite : public MapSprite
{
public:
	// Constructor(s)/Destructor
	
	TreeSprite(HDC hDC, HINSTANCE hInstance) :MapSprite(hDC, hInstance, IDB_TREE) { InitializeTree(); };
	TreeSprite(Bitmap* pBitmap) :MapSprite(pBitmap, IDB_TREE) { InitializeTree(); };
	TreeSprite(Bitmap* pBitmap, RECT& rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction = BA_STOP) : MapSprite(pBitmap, rcBounds, IDB_TREE, baBoundsAction) { InitializeTree(); };
	TreeSprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		MapSprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, IDB_TREE, baBoundsAction) {
		InitializeTree();
	};
	virtual void Update() override;
	virtual ~TreeSprite() 
	{
	};

	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void OnCollisionExit(Sprite* otherSprite) override;
	virtual void OnCollisionStay(Sprite* otherSprite) override;

	//members
	int m_Food = 0;

	//class members
	const int maxFoodOnSpawn=400;
	const int minFoodOnSpawn=300;

public:
	void ScaleTree()
	{
		Scale((float)((float)m_Food / (float)minFoodOnSpawn), (float)((float)m_Food /(float)minFoodOnSpawn));
		//std::cout << "Scale: " << (float)m_Food / (float)minFoodOnSpawn << std::endl;
	}

private:
	void InitializeTree()
	{
		m_Food = rand() % (maxFoodOnSpawn - minFoodOnSpawn + 1) + minFoodOnSpawn;
		//Scale(static_cast<float>(m_Food/((maxFoodOnSpawn+minFoodOnSpawn)/2)), static_cast<float>(m_Food / ((maxFoodOnSpawn + minFoodOnSpawn) / 2)));
		ScaleTree();
	}

};