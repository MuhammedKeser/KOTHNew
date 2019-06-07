#pragma once
#include "LivelySprite.h"

int LivelySprite::playerOneCount = 0;
int LivelySprite::playerTwoCount = 0;

//DEBUG
void LivelySprite::Update()
{
	if (Input::KeyReleased(InputKeys::KEY::R))
	{
		std::cout << "Player 1 Count: " << LivelySprite::playerOneCount << std::endl;
		std::cout << "Player 2 Count: " << LivelySprite::playerTwoCount << std::endl;
	}
}

void LivelySprite::increasePlayerCount(const Player& myPlayer) {

	if (myPlayer.m_Name.compare("Momo") == 0)
	{
		LivelySprite::playerOneCount++;
	}
	else if (myPlayer.m_Name.compare("ASP") == 0)
	{
		LivelySprite::playerTwoCount++;
	}
	//DrawText(hDC, TEXT("Countdown Started!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void LivelySprite::decreasePlayerCount(const Player& myPlayer) {

	if (myPlayer.m_Name.compare("Momo") == 0)
	{
		LivelySprite::playerOneCount--;
	}
	else if (myPlayer.m_Name.compare("ASP") == 0)
	{
		LivelySprite::playerTwoCount--;
	}
	//DrawText(hDC, TEXT("Countdown Started!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void LivelySprite::OnCollisionEnter(Sprite* otherSprite)
{

	Unit* otherUnit = dynamic_cast<Unit*> (otherSprite);

	if (otherUnit != NULL) {

		increasePlayerCount(otherUnit->GetPlayer());
	}

}

void LivelySprite::OnCollisionExit(Sprite* otherSprite)
{
	Unit* otherUnit = dynamic_cast<Unit*> (otherSprite);

	if (otherUnit != NULL) {

		decreasePlayerCount(otherUnit->GetPlayer());
	}

}

void LivelySprite::OnCollisionStay(Sprite* otherSprite)
{
}