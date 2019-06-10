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
	std::list<Sprite*> neighborSprites = GetNeighboringCells();

	std::list<Sprite*>::iterator siSprite;
	for (siSprite = neighborSprites.begin(); siSprite != neighborSprites.end(); siSprite++)
	{
		if ((*siSprite) == NULL)
			continue;
		if (Unit* neighborUnit = dynamic_cast<Unit*>(*siSprite))
		{
			Fight(neighborUnit);
		}
	}
	handleBitmaps();
	HandleDeath();
}

//TODO
//1.We're going to be using a grid-based map. Turn this from collision-based attacking to neighboring cell attacking
//ALSO perform a player check to make sure you're not attacking your own units
//2.If it's a gatherer, win it over instead of fighting it
void Warrior::Fight(Unit* otherUnit)
{

	if (otherUnit)
	{
		if (otherUnit->GetStatus() != UNIT_STATUS::DEAD
			&& strcmp(otherUnit->GetPlayer()->m_Name.c_str(),m_player->m_Name.c_str())!=0
			&& m_timeOfLastAttack+m_attackInterval*1000<GetTickCount())
		{
			SetStatus(UNIT_STATUS::ATTACKING);
			std::cout << "ATTACK!" << std::endl;
			//Lower the other unit's health
			otherUnit->SetHealth(otherUnit->GetHealth() - m_damage);

			//Set the time of attack
			m_timeOfLastAttack = GetTickCount();
		}
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

		if (m_pBitmap == m_warriorLW1 || m_pBitmap == m_warriorLW2 || m_pBitmap == m_warriorL) {
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
		m_timeOfLastAnimation = GetTickCount();

	}
}
