#pragma once
#include "LivelySprite.h"

std::list<LivelySprite::PlayerScoreBoard*> LivelySprite::playerScoreBoard=std::list<LivelySprite::PlayerScoreBoard*>();
LivelySprite::PlayerScoreBoard* LivelySprite::occupyingPlayerScoreBoard = NULL;
//DEBUG
void LivelySprite::Update()
{
	
}

void LivelySprite::increasePlayerCount(Player* myPlayer) {

	std::list<PlayerScoreBoard*>::iterator it;
	for (it = LivelySprite::playerScoreBoard.begin(); it != LivelySprite::playerScoreBoard.end(); it++)
	{
		if ((*it)->player == myPlayer)
		{
			(*it)->score++;
		}
	}
	HandleOccupyingPlayer();
}

void LivelySprite::decreasePlayerCount(Player* myPlayer) {

	std::list<PlayerScoreBoard*>::iterator it;
	for (it = LivelySprite::playerScoreBoard.begin(); it != LivelySprite::playerScoreBoard.end(); it++)
	{
		if ((*it)->player == myPlayer)
		{
			(*it)->score--;
		}
	}

	HandleOccupyingPlayer();
}

void LivelySprite::HandleOccupyingPlayer()
{
	int maxPlayerScore = 0;
	occupyingPlayerScoreBoard = NULL;
	std::list<PlayerScoreBoard*>::iterator it;
	for (it = LivelySprite::playerScoreBoard.begin(); it != LivelySprite::playerScoreBoard.end(); it++)
	{
		if ((*it)->score > maxPlayerScore)
		{
			occupyingPlayerScoreBoard = (*it);
			maxPlayerScore = (*it)->score;
		}
	}
}

void LivelySprite::HandleDisplay(HDC hDC)
{
	//Only display the text if we have a capturing player
	if(occupyingPlayerScoreBoard)
	{
		RECT rect = RECT{ 0,0,500,500 };
		std::ostringstream oss;
		oss << "Occupying Player: " << occupyingPlayerScoreBoard->player->m_Name << " Score: " << occupyingPlayerScoreBoard->score;
		std::string stringToDisplay = oss.str();
		DrawText(hDC, TEXT(stringToDisplay.c_str()), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	
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
	if (otherSprite)
	{
		Unit* otherUnit = dynamic_cast<Unit*> (otherSprite);

		if (otherUnit != NULL) {

			decreasePlayerCount(otherUnit->GetPlayer());
		}
	}
}

void LivelySprite::OnCollisionStay(Sprite* otherSprite)
{
}