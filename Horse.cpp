#include "Horse.h"

void Horse::Update()
{
	hasYBouncedThisCycle = false;
	hasXBouncedThisCycle = false;

	if (GetTickCount()>timeLastGivenRandomDirection+rand()%(maxRandomDirectionInterval-minRandomDirectionInterval+1)+minRandomDirectionInterval*1000)
	{
		timeLastGivenRandomDirection = GetTickCount();
		MoveInRandomDirection();
	}

}

void Horse::OnCollisionEnter(Sprite * otherSprite)
{
	if (otherSprite->name=="WALL")
	{
		//Bounce, based on how it hit
		if (!hasXBouncedThisCycle && otherSprite->GetPosition().right < m_rcPosition.right && GetVelocity().x<0) //Moving to the left
		{
			hasXBouncedThisCycle = true;
			ReverseDirectionX();
		}
		else if (!hasXBouncedThisCycle && otherSprite->GetPosition().left > m_rcPosition.left&& GetVelocity().x > 0)
		{
			hasXBouncedThisCycle = true;
			ReverseDirectionX();
		}
		if (!hasYBouncedThisCycle && otherSprite->GetPosition().bottom < m_rcPosition.bottom && GetVelocity().y < 0) //Moving to the up
		{
			hasYBouncedThisCycle = true;
			ReverseDirectionY();
		}
		else if (!hasYBouncedThisCycle && otherSprite->GetPosition().top > m_rcPosition.top && GetVelocity().y > 0)
		{
			hasYBouncedThisCycle = true;
			ReverseDirectionY();
		}



	}

	if (Warrior* otherWarrior = dynamic_cast<Warrior*>(otherSprite))
	{
		//If we're not marked for deletion...
		if (!m_deletionPending)
		{
			//...have the warrior mount the horse...
			otherWarrior->SetIsMounted(true);

			//... and mark this horse for deletion
			MarkForDeletion();
		}
		
	}

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

void Horse::InitializeHorse()
{
	MoveInRandomDirection();
}