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

		//Remove it from the map
		Map::SetGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), 0);
		//Map::SetSpriteGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), NULL);
	};


	virtual void OnCollisionEnter(Sprite* otherSprite) override;
	virtual void OnCollisionExit(Sprite* otherSprite) override;
	virtual void OnCollisionStay(Sprite* otherSprite) override;

	//members
	int m_Food = 0;
	static int treeCount;
	static int minTreeCount;
	static void HandleTreeSpawnBalance(HDC hDC);
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
		Map::SetGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()), 2);

		//Scale(static_cast<float>(m_Food/((maxFoodOnSpawn+minFoodOnSpawn)/2)), static_cast<float>(m_Food / ((maxFoodOnSpawn + minFoodOnSpawn) / 2)));
		ScaleTree();
		TreeSprite::treeCount++;
		isStatic = true;
	}

};