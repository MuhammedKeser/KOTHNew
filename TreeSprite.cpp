#pragma once
#include "TreeSprite.h"


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
