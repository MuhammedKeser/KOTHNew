#pragma once
#include "LivelySprite.h"

std::list<LivelySprite::PlayerScoreBoard*> LivelySprite::playerScoreBoard=std::list<LivelySprite::PlayerScoreBoard*>();
LivelySprite::PlayerScoreBoard* LivelySprite::occupyingPlayerScoreBoard = NULL;
LivelySprite::PlayerScoreBoard* LivelySprite::winnerPlayerScoreBoard = NULL;
//DEBUG
void LivelySprite::Update()
{
	HandleCountdown();
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
	PlayerScoreBoard* oldOccupyingPlayerScoreBoard = NULL;
	if (occupyingPlayerScoreBoard != NULL)
		oldOccupyingPlayerScoreBoard = new PlayerScoreBoard(occupyingPlayerScoreBoard);
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

	if (
		occupyingPlayerScoreBoard!=NULL  &&
		((oldOccupyingPlayerScoreBoard == NULL)
		||
		(oldOccupyingPlayerScoreBoard->player != occupyingPlayerScoreBoard->player)
		))
	{
		startCountdown();
	}


	delete(oldOccupyingPlayerScoreBoard);
}

void LivelySprite::HandleCountdown()
{
	if (timeCountdownStarted != -1
		&& timeCountdownStarted + countdownInterval*1000 < GetTickCount())
	{
		//End the game
		std::cout << "Endgame" << std::endl;
		winnerPlayerScoreBoard = occupyingPlayerScoreBoard;
	}
}

void LivelySprite::HandleDisplay(HDC hDC)
{
	//Only display the text if we have a capturing player
	if(occupyingPlayerScoreBoard)
	{
		RECT rect = RECT{ 0,0,500,500 };
		std::ostringstream oss;
		
		oss << "Occupying Player: " << occupyingPlayerScoreBoard->player->m_Name << "\nScore: " << occupyingPlayerScoreBoard->score << "\nTime left (sec): "<<floor((countdownInterval*1000+timeCountdownStarted-GetTickCount())/1000);
		std::string stringToDisplay = oss.str();
		DrawText(hDC, TEXT(stringToDisplay.c_str()), -1, &rect, DT_CENTER | DT_VCENTER);
	}
	
}

void LivelySprite::OnCollisionEnter(Sprite* otherSprite)
{

	Unit* otherUnit = dynamic_cast<Unit*> (otherSprite);

	if (otherUnit != NULL) {
		otherUnit->isOnLivelySprite = TRUE;
		increasePlayerCount(otherUnit->GetPlayer());
	}

}

void LivelySprite::OnCollisionExit(Sprite* otherSprite)
{
	if (otherSprite)
	{
		Unit* otherUnit = dynamic_cast<Unit*> (otherSprite);//Deallocation error

		if (otherUnit != NULL) {
			otherUnit->isOnLivelySprite = FALSE;
			decreasePlayerCount(otherUnit->GetPlayer());
		}
	}
}


void LivelySprite::OnCollisionStay(Sprite* otherSprite)
{
}