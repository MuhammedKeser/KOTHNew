#include "Gatherer.h"

int Gatherer::m_foodNeededForBirth=20;
int Gatherer::m_birthCooldown=10;
int Gatherer::m_timeTakenForBirth=10;


void Gatherer::OnCollisionEnter(Sprite * otherSprite)
{
	Unit::OnCollisionEnter(otherSprite);


}

void Gatherer::OnCollisionExit(Sprite * otherSprite)
{
}

void Gatherer::OnCollisionStay(Sprite * otherSprite)
{
	//Checking the sprite type
	TreeSprite* tree = dynamic_cast<TreeSprite*>(otherSprite);
	SapTree(tree);
}

void Gatherer::SapTree(TreeSprite* tree)
{
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
		//std::cout << "Sapping!" << std::endl;
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
	m_player->RequestUnitSpawn( rand()%100>94 ? UNIT_TYPE::GATHERER :UNIT_TYPE::WARRIOR,
		(GetXIndex(Map::GetCellWidth())+rand() % 5 - 2)*Map::GetCellWidth()+ floor(Map::GetCellWidth()/2) - GetWidth()/2,
		(GetYIndex(Map::GetCellHeight()) + rand() % 5 - 2) * Map::GetCellHeight() + floor(Map::GetCellHeight() / 2) - GetHeight() / 2
	);
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
		status = UNIT_STATUS::ALIVE;
		m_timeToBirth = -1;
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

	HandleBirth();
}

void Gatherer::RequestWinOver(Warrior * warrior)
{
	//Check your surroundings on the map. If there are any of your own warriors around you, don't win yourself over.
	//If not, start a countdown. If your own warriors get to you before the countdown ends, stop being won over.
	//Otherwise, once the timer ends, switch players.
}
