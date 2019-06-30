#include "Horse.h"
#include "GameEngine.h"
int Horse::horseCount = 0;
int Horse::minHorseCount = 7;

void Horse::Update()
{
	hasYBouncedThisCycle = false;
	hasXBouncedThisCycle = false;

	if (GetTickCount()>timeLastGivenRandomDirection+rand()%(maxRandomDirectionInterval-minRandomDirectionInterval+1)+minRandomDirectionInterval*1000)
	{
		timeLastGivenRandomDirection = GetTickCount();
		MoveInRandomDirection();
	}

	HandleMounting();


}

void Horse::HandleMounting()
{
	int otherSprite = Map::GetSpriteCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()));

	if ( otherSprite!=-1 && Sprite::IsAllocated(otherSprite))
	{
		Warrior* otherWarrior = dynamic_cast<Warrior*>(Sprite::GetSpriteById(otherSprite));
		if (otherWarrior
			&& !m_deletionPending
			&& !otherWarrior->DeletionIsPending()
			&& !otherWarrior->GetIsMounted())
		{
			//If we're not marked for deletion...
			
			{
				//...have the warrior mount the horse...
				otherWarrior->SetIsMounted(true);

				//... and mark this horse for deletion
				MarkForDeletion();
			}

		}
		else
		{
			std::cout << "Y Index:" << (GetVelocity().y > 0 ? GetYIndexByBottomPosition(Map::GetCellHeight()) : GetVelocity().y == 0 ? GetYIndex(Map::GetCellHeight()) : GetYIndexByTopPosition(Map::GetCellHeight())) << std::endl;
			std::cout << "X Index:" << (GetVelocity().x > 0 ? GetXIndexByRightPosition(Map::GetCellWidth()) : GetVelocity().x == 0 ? GetXIndex(Map::GetCellWidth()) : GetXIndexByLeftPosition(Map::GetCellWidth())) << std::endl;


			int otherCell = Map::GetGridCell(
					GetVelocity().y > 0 ? GetYIndexByBottomPosition(Map::GetCellHeight()) : GetVelocity().y==0 ? GetYIndex(Map::GetCellHeight()) : GetYIndexByTopPosition(Map::GetCellHeight()),
					GetVelocity().x > 0 ? GetXIndexByRightPosition(Map::GetCellWidth()) : GetVelocity().x == 0 ? GetXIndex(Map::GetCellWidth()) : GetXIndexByLeftPosition(Map::GetCellWidth())
				);
			//Bounce, based on how it hit
			if (!hasXBouncedThisCycle && otherCell!=0) //Moving to the left
			{
				hasXBouncedThisCycle = true;
				ReverseDirectionX();
			}
			if (!hasYBouncedThisCycle && otherCell!=0) //Moving to the up
			{
				hasYBouncedThisCycle = true;
				ReverseDirectionY();
			}



		}
	}
	else
	{
		std::cout << "Y Index:" << (GetVelocity().y > 0 ? GetYIndexByBottomPosition(Map::GetCellHeight()) : GetVelocity().y == 0 ? GetYIndex(Map::GetCellHeight()) : GetYIndexByTopPosition(Map::GetCellHeight())) << std::endl;
		std::cout << "X Index:" << (GetVelocity().x > 0 ? GetXIndexByRightPosition(Map::GetCellWidth()) : GetVelocity().x == 0 ? GetXIndex(Map::GetCellWidth()) : GetXIndexByLeftPosition(Map::GetCellWidth())) << std::endl;

		int otherCell = Map::GetGridCell(
			GetVelocity().y > 0 ? GetYIndexByBottomPosition(Map::GetCellHeight()) : GetVelocity().y == 0 ? GetYIndex(Map::GetCellHeight()) : GetYIndexByTopPosition(Map::GetCellHeight()),
			GetVelocity().x > 0 ? GetXIndexByRightPosition(Map::GetCellWidth()) : GetVelocity().x == 0 ? GetXIndex(Map::GetCellWidth()) : GetXIndexByLeftPosition(Map::GetCellWidth())
		);
		//Bounce, based on how it hit
		if (!hasXBouncedThisCycle && otherCell != 0) //Moving to the left
		{
			hasXBouncedThisCycle = true;
			ReverseDirectionX();
		}
		if (!hasYBouncedThisCycle && otherCell != 0) //Moving to the up
		{
			hasYBouncedThisCycle = true;
			ReverseDirectionY();
		}



	}
}

void Horse::OnCollisionEnter(Sprite * otherSprite)
{
	

}

void Horse::MoveInRandomDirection()
{
	SetVelocity(rand() % 3 - 1, rand() % 3 - 1);
}

void Horse::ReverseDirectionX()
{
	SetVelocity(-GetVelocity().x, GetVelocity().y);
}

void Horse::ReverseDirectionY()
{
	SetVelocity(GetVelocity().x, -GetVelocity().y);
}

void Horse::HandleHorseSpawnBalance(HDC hDC)
{
	if (horseCount < minHorseCount)
	{
		//Spawn at LEAST horseCount-minHorseCount amount of horses
		int randYIndex = rand() % Map::GetHeight();
		int randXIndex = rand() % Map::GetWidth();
		if (Map::GetSpriteCell(randYIndex, randXIndex) == -1)
		{
			Horse* newHorse = (Horse*)GameEngine::GetEngine()->CreateSprite<Horse>(hDC);
			newHorse->SetPosition(Map::GetCellWidth()*randXIndex, Map::GetCellHeight()*randYIndex);
			
		}
	}

}

void Horse::InitializeHorse()
{
	MoveInRandomDirection();
	Horse::horseCount++;
}