#include "Gatherer.h"
#include "StateHandler_Gatherer.h"

int Gatherer::m_foodNeededForBirth=20;
int Gatherer::m_birthCooldown=10;
int Gatherer::m_timeTakenForBirth=10;


void Gatherer::Draw(HDC hDC, Camera * cam)
{
	Unit::Draw(hDC, cam);
	DrawBirthTimer(hDC, cam);
}

void Gatherer::OnCollisionEnter(Sprite * otherSprite)
{
	Unit::OnCollisionEnter(otherSprite);


}

void Gatherer::OnCollisionExit(Sprite * otherSprite)
{
}

void Gatherer::OnCollisionStay(Sprite * otherSprite)
{
	
}

void Gatherer::HandleWandering()
{
	if (GetStatus() == UNIT_STATUS::BIRTHING)
		return;

	if (GetStatus() == UNIT_STATUS::SAPPING)
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
		int randomXIndex = rand() % 5 - 2 + xIndex;
		int randomYIndex = rand() % 5 - 2 + yIndex;

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

void Gatherer::SapTree(TreeSprite* tree)
{
	if (UNIT_STATUS::DEAD)
		return;


	if (tree != NULL && tree->m_Food>0)//Type of tree
	{
		if (m_player == NULL)
		{
			//std::cout << "No player attached to gatherer!" << std::endl;
			return;
		}

		m_player->m_food += sapSpeed;
		tree->m_Food -= sapSpeed;
		tree->ScaleTree();


		if(GetStatus()!=UNIT_STATUS::SAPPING
			&& GetStatus()!=UNIT_STATUS::COMMANDED
			&& GetStatus()!=UNIT_STATUS::BIRTHING)
		{
			SetStatus(UNIT_STATUS::SAPPING);
			while (!path.empty())
				path.pop();
		}
		//std::cout << "Sapping!" << std::endl;
	}
	else
	{
		SetStatus(UNIT_STATUS::ALIVE);
	}
}

void Gatherer::GiveBirth()
{
	/*todo->make a list of cell indices n radius around the gatherer.
	Eliminate the full indices, and shuffle the list afterwards.
	Take the first index, if the list isnt empty.
	If it's empty, do nothing. Wait until it empties.
	
	*/
	//DEBUG->This just spawns warriors 2 indices around the birthing gatherer. Do the above, later.

	int randPosX = (GetXIndex(Map::GetCellWidth()) + rand() % 3 - 1);
	int randPosY = (GetYIndex(Map::GetCellHeight()) + rand() % 3 - 1);
	randPosX = randPosX < 0 ? 0 : (randPosX>=Map::GetWidth()? Map::GetWidth()-1 : randPosX);
	randPosY = randPosY < 0 ? 0 : (randPosY >= Map::GetHeight() ? Map::GetHeight() - 1 : randPosY);
	if(Map::GetGridCell(randPosY,randPosX)==0)
	{
			Map::SetGridCell(randPosY, randPosX, 4);
			m_player->RequestUnitSpawn( 
				rand()%100>94 ? UNIT_TYPE::GATHERER :UNIT_TYPE::WARRIOR,
				randPosX*Map::GetCellWidth()+ floor(Map::GetCellWidth()/2) - GetWidth()/2,
				randPosY * Map::GetCellHeight() + floor(Map::GetCellHeight() / 2) - GetHeight() / 2
			);

		status = UNIT_STATUS::ALIVE;
		m_timeToBirth = -1;
	}
}

void Gatherer::DrawBirthTimer(HDC hDC, Camera* cam)
{
	if (m_timeToBirth!=-1)
	{
		int rectWidth = GetWidth() + 1000;
		RECT rect = RECT{ GetPosition().left - cam->GetPosition().x - 300,GetPosition().top - 70 - cam->GetPosition().y, GetPosition().right - cam->GetPosition().x+300,GetPosition().bottom - cam->GetPosition().y - 70 };
		
		DrawText(hDC, TEXT(std::string("Time to Birth: " + std::to_string((m_timeToBirth - GetTickCount())/1000)).c_str()), -1, &rect, DT_CENTER);
	}
}


void Gatherer::HandleBirth()
{
	if (m_player->m_food > Gatherer::GetFoodNeededForBirth()
		&& status != UNIT_STATUS::BIRTHING
		&& status != UNIT_STATUS::DEAD
		&& 
		(m_timeLastBirthed + m_birthCooldown*1000 < GetTickCount() ||
			m_timeLastBirthed==-1))
	{
		status = UNIT_STATUS::BIRTHING;
		m_timeToBirth = GetTickCount() + m_timeTakenForBirth*1000;
		m_player->m_food -= Gatherer::GetFoodNeededForBirth();
	}

	if (status==UNIT_STATUS::BIRTHING
		&& m_timeToBirth<GetTickCount()
		&& m_timeToBirth !=-1)
	{
	
		GiveBirth();
	}
}

void Gatherer::ChangePlayer(Player* player)
{
	if (m_player != player)
	{
		std::cout << "Switched players from: " << m_player->m_Name << " to: " << player->m_Name << std::endl;
		m_player = player;
	}
}

void Gatherer::Update()
{
	Unit::Update();
	std::list<Sprite*> neighborSprites = GetNeighboringCells();

	std::list<Sprite*>::iterator siSprite;
	std::list<Player*> surroundingPlayers;
	for (siSprite = neighborSprites.begin(); siSprite != neighborSprites.end(); siSprite++)
	{
		if ((*siSprite) == NULL)
			continue;
		if (TreeSprite* neighborTree = dynamic_cast<TreeSprite*>(*siSprite))
		{
			SapTree(neighborTree);
		}
		else if (Warrior* neighborWarrior = dynamic_cast<Warrior*>(*siSprite))
		{
			//If you have at least 2 different player types around you, don't do anything
			if (surroundingPlayers.size() >= 2)
				break;
			bool containsPlayer=false;
			for (std::list<Player*>::iterator it = surroundingPlayers.begin(); it != surroundingPlayers.begin(); it++)
			{
				if (strcmp((*it)->m_Name.c_str(), neighborWarrior->GetPlayer()->m_Name.c_str()) == 0)
				{
					containsPlayer = true;
					break;
				}
			}
			if (!containsPlayer)
				surroundingPlayers.push_back(neighborWarrior->GetPlayer());
		}
	}
	if (surroundingPlayers.size() == 1)//If you only have ONE player around you, convert
	{
		ChangePlayer(surroundingPlayers.front());
	}

	HandleWandering();
	HandleBirth();
}

void Gatherer::RequestWinOver(Warrior * warrior)
{
	//Check your surroundings on the map. If there are any of your own warriors around you, don't win yourself over.
	//If not, start a countdown. If your own warriors get to you before the countdown ends, stop being won over.
	//Otherwise, once the timer ends, switch players.
}
