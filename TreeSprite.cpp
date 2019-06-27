#pragma once
#include "TreeSprite.h"
#include "GameEngine.h"

int TreeSprite::treeCount = 0;
int TreeSprite::minTreeCount = 40;

void TreeSprite::Update()
{
	if (xScale <= 0 || yScale <= 0)
	{
		MarkForDeletion();
	}
}

void TreeSprite::OnCollisionEnter(Sprite * otherSprite)
{
	
}

void TreeSprite::OnCollisionExit(Sprite * otherSprite)
{
}

void TreeSprite::OnCollisionStay(Sprite * otherSprite)
{
}

void TreeSprite::HandleTreeSpawnBalance(HDC hDC)
{
	if (treeCount < minTreeCount)
	{
		//Spawn at LEAST horseCount-minHorseCount amount of horses
		int randYIndex = rand() % Map::GetHeight();
		int randXIndex = rand() % Map::GetWidth();
		if (Map::GetSpriteCell(randYIndex, randXIndex) == NULL && Map::GetGridCell(randYIndex, randXIndex)==0)
		{
			TreeSprite* newTree = (TreeSprite*)GameEngine::GetEngine()->CreateSprite<TreeSprite>(hDC);
			RECT rect = { newTree->GetWidth()*randXIndex,newTree->GetHeight()*randYIndex,newTree->GetWidth()*(randXIndex + 1),newTree->GetHeight()*(randYIndex + 1) };
			newTree->SetPosition(rect);
			newTree->RecalculateColliderRect();
			newTree->isStatic = true;
			Map::SetGridCell(randYIndex, randXIndex, 2);
			Map::SetSpriteGridCell(randYIndex, randXIndex, newTree);

		}
	}
}
