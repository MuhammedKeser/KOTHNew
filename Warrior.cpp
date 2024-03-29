#include "Warrior.h"
#include "Horse.h"



void Warrior::OnCollisionEnter(Sprite * otherSprite)
{
	Unit::OnCollisionEnter(otherSprite);
}

void Warrior::OnCollisionExit(Sprite * otherSprite)
{
}

void Warrior::OnCollisionStay(Sprite * otherSprite)
{
	if (Unit* otherUnit = dynamic_cast<Unit*>(otherSprite))
	{
		//Fight(otherUnit);
	}
}

void Warrior::Update()
{
	Unit::Update();
	std::list<int> neighborSprites = GetNeighboringCells();

	std::list<int>::iterator siSprite;
	for (siSprite = neighborSprites.begin(); siSprite != neighborSprites.end(); siSprite++)
	{
		if (!Sprite::IsAllocated(*siSprite))//Deallocation error
			continue;
		if (Warrior* neighborUnit = dynamic_cast<Warrior*>(Sprite::GetSpriteById(*siSprite)))
		{
			Fight(neighborUnit);
		}
	}
	handleBitmaps();
	
	if (!m_player->isMainPlayer)
		HandleWandering();
	
	HandleDeath();
}

//TODO
//1.We're going to be using a grid-based map. Turn this from collision-based attacking to neighboring cell attacking
//ALSO perform a player check to make sure you're not attacking your own units
//2.If it's a gatherer, win it over instead of fighting it
void Warrior::Fight(Warrior* otherUnit)
{

	if (otherUnit)
	{
		
		if (otherUnit->GetStatus() != UNIT_STATUS::DEAD
			&& strcmp(otherUnit->GetPlayer()->m_Name.c_str(),m_player->m_Name.c_str())!=0
			&& m_timeOfLastAttack+m_attackInterval*1000<GetTickCount())
		{
			SetStatus(UNIT_STATUS::ATTACKING);
			//std::cout << "ATTACK!" << std::endl;
			//Lower the other unit's health
			otherUnit->SetHealth(otherUnit->GetHealth() - m_damage);

			//Set the time of attack
			m_timeOfLastAttack = GetTickCount();
		}
		
	
	}
	else {
		SetStatus(UNIT_STATUS::ALIVE);
	}
	
}

void Warrior::HandleDeath()
{
	//If your health is below a certain threshold...
	if (m_health <= 0)
	{
		std::cout << "DEAD-O" << std::endl;
		//1.Mark your status for deletion.
		MarkForDeletion();

		//2.Perhaps play a death animation.
		//3. Upon the end of the death animation, Remove yourself from the player's list of units, and the gameEngine's sprite list

		//4. If you were mounted, decrease the horse count
		if (m_isMounted)
			Horse::horseCount--;
	}
}

void Warrior::handleBitmaps()
{
	if (GetStatus() == UNIT_STATUS::ATTACKING && m_timeOfLastAnimation +m_timeIntervalOfAnimation <= GetTickCount()) {

		if (m_pBitmap == m_warriorLW1 || m_pBitmap == m_warriorLW2 || m_pBitmap == m_warriorL ) {
			m_pBitmap = m_warriorLA1;
		}
		else if (m_pBitmap == m_warriorRW1 || m_pBitmap == m_warriorRW2 || m_pBitmap == m_warriorR) {
			m_pBitmap = m_warriorRA1;
		}
		else if (m_pBitmap == m_warriorLA1) {
			m_pBitmap = m_warriorLA2;
		}
		else if (m_pBitmap == m_warriorLA2) {
			m_pBitmap = m_warriorRA1;
		}
		else if (m_pBitmap == m_warriorRA1) {
			m_pBitmap = m_warriorRA2;
		}
		else if (m_pBitmap == m_warriorRA2)
		{
			m_pBitmap = m_warriorRA1;
		}
		else {
			m_pBitmap = m_warriorRA1;
		}
		if (sword) {
			PlaySound(TEXT("sword1.wav"), NULL, SND_ASYNC);
			sword = false;
		}
		else if (!sword) {
			PlaySound(TEXT("sword2.wav"), NULL, SND_ASYNC);
			sword = true;
		}
		m_timeOfLastAnimation = GetTickCount();

	}
	if ((GetStatus() == UNIT_STATUS::WALKING || GetStatus() == UNIT_STATUS::COMMANDED) && (m_timeOfLastAnimation + m_timeIntervalOfWalking <= GetTickCount())) {
		if (GetVelocity().x <= 0) {
			if(!m_isMounted){
				if (m_pBitmap == m_warriorLW1 || m_pBitmap == m_warriorL) {
					m_pBitmap = m_warriorLW2;
				}
				else {
					m_pBitmap = m_warriorLW1;
				}
			}
			else if (m_isMounted) {
				m_pBitmap = m_pMountedBitmapL;
			}
		}
		else if (GetVelocity().x > 0) {
			if (!m_isMounted) {
				if (m_pBitmap == m_warriorRW1 || m_pBitmap == m_warriorR) {
					m_pBitmap = m_warriorRW2;
				}
				else {
					m_pBitmap = m_warriorRW1;
				}
			}
			else if (m_isMounted) {
				m_pBitmap = m_pMountedBitmapR;
			}
		}
		m_timeOfLastAnimation = GetTickCount();
	}
	if ((GetStatus() == UNIT_STATUS::ALIVE && (m_timeOfLastAnimation + m_timeIntervalOfWaiting <= GetTickCount()))) {
		if (m_pBitmap == m_warriorL) {
			m_pBitmap = m_warriorL;
		}
		else if (m_pBitmap == m_warriorL) {
			m_pBitmap = m_warriorR;
		}
		m_timeOfLastAnimation = GetTickCount();
	}
}

void Warrior::HandleWandering()
{
	if (m_player->playerIndex == 2)
	{
		if (GetStatus() == UNIT_STATUS::ATTACKING)
		{

			//do nothing, just keep sapping
			while (!path.empty())
				path.pop();

			SetVelocity(POINT{ 0,0 });
			SetPosition(floor(xIndex*Map::GetCellWidth()) + GetWidth() / 2, floor(yIndex*Map::GetCellHeight()));

		}
		else if (path.empty())//Pathfind, if you haven't already
		{
			//DEBUG -> Just go to a random location around yourself for now
			int randomXIndex = rand() % 9 - 4 + xIndex;
			int randomYIndex = rand() % 9 - 4 + yIndex;

			randomXIndex = randomXIndex < 0 ? 0 : (randomXIndex >= Map::GetWidth() ? Map::GetWidth() - 1 : randomXIndex);
			randomYIndex = randomYIndex < 0 ? 0 : (randomYIndex >= Map::GetHeight() ? Map::GetHeight() - 1 : randomYIndex);

			if (Map::GetGridCell(randomYIndex, randomXIndex) == 0 ||
				Map::GetGridCell(randomYIndex, randomXIndex) == 3)
			{
				m_destinationIndex.x = randomXIndex;
				m_destinationIndex.y = randomYIndex;
				Pathfind();
				m_destinationIndex.x = -1;
				m_destinationIndex.y = -1;
			}

		}
	}
	if (m_player->playerIndex == 1)
	{
		if (GetStatus() == UNIT_STATUS::ATTACKING)
		{
			//do nothing, just keep sapping
			while (!path.empty())
				path.pop();

			SetVelocity(POINT{ 0,0 });
			SetPosition(floor(xIndex*Map::GetCellWidth()) + GetWidth() / 2, floor(yIndex*Map::GetCellHeight()));

		}
		else if (path.empty())//Pathfind, if you haven't already
		{
			//Look for an enemy around yourself. If you can find one, go towards it
			bool enemyFound = false;
			for (int i = -4; i <= 4; i++)
			{
				int neighborIndexY = i+ yIndex;
				for (int j = -4; j <= 4; j++)
				{
					int neighborIndexX = j + xIndex;
					if (
						neighborIndexY>=0&&
						neighborIndexX>=0&&
						neighborIndexY<Map::GetHeight()&&
						neighborIndexX<Map::GetWidth()
						)
					{
						
						if (Map::GetGridCell(neighborIndexY,neighborIndexX)==4
							&& Map::GetSpriteCell(neighborIndexY, neighborIndexX)!=-1
							&& Sprite::IsAllocated(Map::GetSpriteCell(neighborIndexY, neighborIndexX))
							&& dynamic_cast<Warrior*>(Sprite::GetSpriteById(Map::GetSpriteCell(neighborIndexY, neighborIndexX)))
							)
						{
							Warrior* aggroWarrior = dynamic_cast<Warrior*>(Sprite::GetSpriteById(Map::GetSpriteCell(neighborIndexY, neighborIndexX)));
							if (aggroWarrior->m_player->playerIndex != m_player->playerIndex)
							{
								enemyFound = true;
								m_destinationIndex.x = neighborIndexX;
								m_destinationIndex.y = neighborIndexY;
								Pathfind();
								m_destinationIndex.x = -1;
								m_destinationIndex.y = -1;
								break;
							}
						}
					}
				}
				if (enemyFound)
					break;
			}

			if (!enemyFound)
			{
				int randomXIndex = rand() % 9 - 4 + xIndex;
				int randomYIndex = rand() % 9 - 4 + yIndex;

				randomXIndex = randomXIndex < 0 ? 0 : (randomXIndex >= Map::GetWidth() ? Map::GetWidth() - 1 : randomXIndex);
				randomYIndex = randomYIndex < 0 ? 0 : (randomYIndex >= Map::GetHeight() ? Map::GetHeight() - 1 : randomYIndex);

				if (Map::GetGridCell(randomYIndex, randomXIndex) == 0 ||
					Map::GetGridCell(randomYIndex, randomXIndex) == 3)
				{
					m_destinationIndex.x = randomXIndex;
					m_destinationIndex.y = randomYIndex;
					Pathfind();
					m_destinationIndex.x = -1;
					m_destinationIndex.y = -1;
				}
			}
			

		}
	}
}
