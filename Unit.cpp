#include "Unit.h"

bool Unit::pathfindingPerformedThisCycle=false;


Unit::~Unit(void)
{
	//Remove it from the player list
	if (m_player)
	{
		for (std::list<Unit*>::reverse_iterator siUnit = m_player->m_Units.rbegin(); siUnit != m_player->m_Units.rend(); ++siUnit)
		{
			if ((*siUnit)->DeletionIsPending())
			{
				m_player->m_Units.erase(std::next(siUnit).base());
				break;
			}
		}
	}

	//Remove it from the map
	Map::SetGridCell(yIndex, xIndex,0);
	Map::RemoveSpriteGridCell(yIndex,xIndex);

	if (Map::GetSpriteCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth())) == NULL)
	{
		std::cout << "NULLS!" << std::endl;
	}
	std::cout << "Unit destruct" << std::endl;
}

void Unit::Update()
{
	MoveToPoint();
	HandlePathTraversal();
}

void Unit::PreventOverlap(Sprite* otherSprite)
{

	Unit* unit = dynamic_cast<Unit*>(otherSprite);

	//TODO->Inline this badboy so you don't have to fill the stack up with it
	auto Collision = [this, &unit]()
	{
		//std::cout << "Collision Overlap" << std::endl;
		CalcCollisionRect();
		unit->CalcCollisionRect();
		ResetCollisionList();
		unit->ResetCollisionList();
	};

	auto XCollision = [this, &unit, &Collision]()
	{
		Collision();
		m_ptVelocity.x = 0;
	};

	auto YCollision = [this, &unit, &Collision]()
	{
		Collision();
		m_ptVelocity.y = 0;
	};
	/*
	if (unit
		&&
		(
		(m_ptVelocity.x < 0
			&& unit->GetVelocity().x > 0)
			||
			(
				m_ptVelocity.x > 0
				&& unit->GetVelocity().x < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().right > m_rcPosition.left
			|| otherSprite->GetPosition().right < m_rcPosition.left
			)
		)
	{
		std::cout << "SET POS" << std::endl;
		m_rcPosition = { GetPosition().left - m_ptVelocity.x, m_rcPosition.top, GetPosition().right - m_ptVelocity.x, m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&&
		(
		(m_ptVelocity.y < 0
			&& unit->GetVelocity().y > 0)
			||
			(
				m_ptVelocity.y > 0
				&& unit->GetVelocity().y < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().bottom > m_rcPosition.top
			|| otherSprite->GetPosition().bottom < m_rcPosition.top
			)
		)
	{
		m_rcPosition = { m_rcPosition.left, GetPosition().top - m_ptVelocity.y,  m_rcPosition.right,GetPosition().bottom - m_ptVelocity.y };
		YCollision();
	}
	*/
	//VELOCITY COLLISIONS
	/*
	if (unit
		&& m_ptVelocity.x > 0
		&& unit->GetVelocity().x == 0
		&& otherSprite->GetPosition().left < m_rcPosition.right)
	{
		std::cout << "SET POS" << std::endl;
		m_rcPosition = { otherSprite->GetPosition().left - GetWidth() , m_rcPosition.top, otherSprite->GetPosition().left, m_rcPosition.bottom };
		XCollision();
	}
	if (unit
		&& m_ptVelocity.x < 0
		&& unit->GetVelocity().x == 0
		&& otherSprite->GetPosition().right > m_rcPosition.left)
	{
		std::cout << "SET POS" << std::endl;
		m_rcPosition = { otherSprite->GetPosition().right, m_rcPosition.top, otherSprite->GetPosition().right + GetWidth(), m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&& m_ptVelocity.y > 0
		&& unit->GetVelocity().y == 0
		&& otherSprite->GetPosition().top < m_rcPosition.bottom)
	{

		m_rcPosition = { m_rcPosition.left ,otherSprite->GetPosition().top - GetHeight(), m_rcPosition.right,otherSprite->GetPosition().top };
		YCollision();
	}
	if (unit
		&& m_ptVelocity.y < 0
		&& unit->GetVelocity().y == 0
		&& otherSprite->GetPosition().bottom > m_rcPosition.top)
	{
		std::cout << "SET POS" << std::endl;
		m_rcPosition = { m_rcPosition.left,otherSprite->GetPosition().bottom , m_rcPosition.right,otherSprite->GetPosition().bottom + GetHeight()};
		YCollision();
	}*/

	//No velocity collisions
	if (unit
		&& otherSprite->GetPosition().left < m_rcPosition.right)
	{
		m_rcPosition = { otherSprite->GetPosition().left - GetWidth() , m_rcPosition.top, otherSprite->GetPosition().left, m_rcPosition.bottom };
		XCollision();
	}
	else if (unit
		&& otherSprite->GetPosition().right > m_rcPosition.left)
	{
		m_rcPosition = { otherSprite->GetPosition().right, m_rcPosition.top, otherSprite->GetPosition().right + GetWidth(), m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&& otherSprite->GetPosition().top < m_rcPosition.bottom)
	{

		m_rcPosition = { m_rcPosition.left ,otherSprite->GetPosition().top - GetHeight(), m_rcPosition.right,otherSprite->GetPosition().top };
		YCollision();
	}
	else if (unit
		&& otherSprite->GetPosition().bottom > m_rcPosition.top)
	{
		//std::cout << "SET POS" << std::endl;
		m_rcPosition = { m_rcPosition.left,otherSprite->GetPosition().bottom , m_rcPosition.right,otherSprite->GetPosition().bottom + GetHeight() };
		YCollision();
	}

}

void Unit::OnCollisionEnter(Sprite * otherSprite)
{
	Unit* unit = dynamic_cast<Unit*>(otherSprite);

	//TODO->Inline this badboy so you don't have to fill the stack up with it
	auto Collision = [this, &unit]()
	{
		CalcCollisionRect();
		unit->CalcCollisionRect();
		ResetCollisionList();
		unit->ResetCollisionList();
	};

	auto XCollision = [this, &unit, &Collision]()
	{
		Collision();
		//m_ptVelocity.x = 0;
	};

	auto YCollision = [this, &unit, Collision]()
	{
		Collision();
		//m_ptVelocity.y = 0;
	};

	if (unit
		&&
		(
		(m_ptVelocity.x < 0
			&& unit->GetVelocity().x > 0)
			||
			(
				m_ptVelocity.x > 0
				&& unit->GetVelocity().x < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().right > m_rcPosition.left
			|| otherSprite->GetPosition().right < m_rcPosition.left
			)
		)
	{
		//m_rcPosition = { GetPosition().left - m_ptVelocity.x, m_rcPosition.top, GetPosition().right - m_ptVelocity.x, m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&&
		(
		(m_ptVelocity.y < 0
			&& unit->GetVelocity().y > 0)
			||
			(
				m_ptVelocity.y > 0
				&& unit->GetVelocity().y < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().bottom > m_rcPosition.top
			|| otherSprite->GetPosition().bottom < m_rcPosition.top
			)
		)
	{
		//m_rcPosition = { m_rcPosition.left, GetPosition().top - m_ptVelocity.y,  m_rcPosition.right,GetPosition().bottom - m_ptVelocity.y };
		YCollision();
	}

	if (unit
		&& m_ptVelocity.x > 0
		&& unit->GetVelocity().x >= 0
		&& otherSprite->GetPosition().left < m_rcPosition.right)
	{
		//m_rcPosition = { otherSprite->GetPosition().left - GetWidth() - 1, m_rcPosition.top, otherSprite->GetPosition().left - 1, m_rcPosition.bottom };
		XCollision();
	}
	if (unit
		&& m_ptVelocity.x < 0
		&& unit->GetVelocity().x <= 0
		&& otherSprite->GetPosition().right > m_rcPosition.left)
	{
		//m_rcPosition = { otherSprite->GetPosition().right + 1, m_rcPosition.top, otherSprite->GetPosition().right + GetWidth() + 1, m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&& m_ptVelocity.y > 0
		&& unit->GetVelocity().y >= 0
		&& otherSprite->GetPosition().top < m_rcPosition.bottom)
	{

		//m_rcPosition = { m_rcPosition.left ,otherSprite->GetPosition().top - GetHeight() - 1, m_rcPosition.right,otherSprite->GetPosition().top - 1 };
		YCollision();
	}
	if (unit
		&& m_ptVelocity.y < 0
		&& unit->GetVelocity().y <= 0
		&& otherSprite->GetPosition().bottom > m_rcPosition.top)
	{
		//m_rcPosition = { m_rcPosition.left,otherSprite->GetPosition().bottom + 1, m_rcPosition.right,otherSprite->GetPosition().bottom + GetHeight() + 1 };
		YCollision();
	}
}

void Unit::OnCollisionStay(Sprite * otherSprite)
{
	Unit* unit = dynamic_cast<Unit*>(otherSprite);

	//TODO->Inline this badboy so you don't have to fill the stack up with it
	auto Collision = [this, &unit]()
	{
		CalcCollisionRect();
		unit->CalcCollisionRect();
		ResetCollisionList();
		unit->ResetCollisionList();
	};

	auto XCollision = [this, &unit, &Collision]()
	{
		Collision();
		//m_ptVelocity.x = 0;
	};

	auto YCollision = [this, &unit, Collision]()
	{
		Collision();
		//m_ptVelocity.y = 0;
	};

	if (unit
		&&
		(
		(m_ptVelocity.x < 0
			&& unit->GetVelocity().x > 0)
			||
			(
				m_ptVelocity.x > 0
				&& unit->GetVelocity().x < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().right > m_rcPosition.left
			|| otherSprite->GetPosition().right < m_rcPosition.left
			)
		)
	{
		//std::cout << "SET POS" << std::endl;
		//m_rcPosition = { GetPosition().left - m_ptVelocity.x, m_rcPosition.top, GetPosition().right - m_ptVelocity.x, m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&&
		(
		(m_ptVelocity.y < 0
			&& unit->GetVelocity().y > 0)
			||
			(
				m_ptVelocity.y > 0
				&& unit->GetVelocity().y < 0
				)
			)
		&&
		(
			otherSprite->GetPosition().bottom > m_rcPosition.top
			|| otherSprite->GetPosition().bottom < m_rcPosition.top
			)
		)
	{
		//m_rcPosition = { m_rcPosition.left, GetPosition().top - m_ptVelocity.y,  m_rcPosition.right,GetPosition().bottom - m_ptVelocity.y };
		YCollision();
	}

	if (unit
		&& m_ptVelocity.x > 0
		&& unit->GetVelocity().x == 0
		&& otherSprite->GetPosition().left < m_rcPosition.right)
	{
		//std::cout << "SET POS" << std::endl;
		m_rcPosition = { otherSprite->GetPosition().left - GetWidth() - 1, m_rcPosition.top, otherSprite->GetPosition().left - 1, m_rcPosition.bottom };
		XCollision();
	}
	if (unit
		&& m_ptVelocity.x < 0
		&& unit->GetVelocity().x == 0
		&& otherSprite->GetPosition().right > m_rcPosition.left)
	{
		//std::cout << "SET POS" << std::endl;
		//m_rcPosition = { otherSprite->GetPosition().right + 1, m_rcPosition.top, otherSprite->GetPosition().right + GetWidth() + 1, m_rcPosition.bottom };
		XCollision();
	}

	if (unit
		&& m_ptVelocity.y > 0
		&& unit->GetVelocity().y == 0
		&& otherSprite->GetPosition().top < m_rcPosition.bottom)
	{

		//m_rcPosition = { m_rcPosition.left ,otherSprite->GetPosition().top - GetHeight() - 1, m_rcPosition.right,otherSprite->GetPosition().top - 1 };
		YCollision();
	}
	if (unit
		&& m_ptVelocity.y < 0
		&& unit->GetVelocity().y == 0
		&& otherSprite->GetPosition().bottom > m_rcPosition.top)
	{
		//std::cout << "SET POS" << std::endl;
		//m_rcPosition = { m_rcPosition.left,otherSprite->GetPosition().bottom + 1, m_rcPosition.right,otherSprite->GetPosition().bottom + GetHeight() + 1 };
		YCollision();
	}
}

void Unit::Draw(HDC hDC, Camera* cam)
{
	Sprite::Draw(hDC, cam);
	//Apply text drawing here
	int rectWidth = GetWidth() + 30;
	RECT rect = RECT{ GetPosition().left - cam->GetPosition().x-30,GetPosition().top - 70 - cam->GetPosition().y, GetPosition().right - cam->GetPosition().x,GetPosition().bottom - cam->GetPosition().y-70 };
	int rectHeight = DrawText(hDC, TEXT(std::string("HP: " + std::to_string(GetHealth()) + "\nPlayer: " + m_player->m_Name).c_str()), -1, &rect, DT_CALCRECT);
	rect = RECT{ rect.left,rect.bottom,rect.right,rect.bottom + rectHeight };
	DrawText(hDC, TEXT(std::string("HP: " + std::to_string(GetHealth()) + "\nPlayer: " + m_player->m_Name).c_str()), -1, &rect, DT_CENTER);
};

//Functions
void Unit::SetDestination(int x, int y, int cellWidth, int cellHeight)
{
	int destinationX = floor(x / cellWidth);
	int destinationY = floor(y / cellHeight);

	//std::cout << "Selected Cell Value: " << Map::GetGridCell(destinationY, destinationX) << std::endl;

	if (Map::GetGridCell(destinationY, destinationX) != 0
	&& Map::GetGridCell(destinationY, destinationX) != 3)
	{
		//std::cout << "ERROR IN UNIT.CPP - SETDESTINATION: The selected destination is already filled." << std::endl;
		return;
	}	

	//std::cout << "X: " << destinationX << " Y: " << destinationY << std::endl;
	m_destinationIndex = POINT{ destinationX,destinationY};
	m_destination = POINT{ long(destinationX*cellWidth)+long(floor(cellWidth/2))-long(GetWidth()/2),long(destinationY*cellHeight) + long(floor(cellHeight / 2))-long(GetHeight()/2)};

	

}

SPRITEACTION Unit::UpdatePosition()
{
	
	// Update the position
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	//TODO-> Change ptNewPosition so that it never
	//intersects another collider

	// Check the bounds
	// Wrap?
	if (m_baBoundsAction == BA_WRAP)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
			ptNewPosition.x = m_rcBounds.right;
		else if (ptNewPosition.x > m_rcBounds.right)
			ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
			ptNewPosition.y = m_rcBounds.bottom;
		else if (ptNewPosition.y > m_rcBounds.bottom)
			ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;

	}
	// Bounce?
	else if (m_baBoundsAction == BA_BOUNCE)
	{
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if (ptNewPosition.x < m_rcBounds.left)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.left;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		if (ptNewPosition.y < m_rcBounds.top)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.top;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		if (bBounce)
			SetVelocity(ptNewVelocity);

	}
	// Die?
	else if (m_baBoundsAction == BA_DIE)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
			ptNewPosition.x > m_rcBounds.right ||
			(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
			ptNewPosition.y > m_rcBounds.bottom)
		{
			return SA_KILL;
		}
			
	}
	// Stop? 
	else if (m_baBoundsAction == BA_CONTINUE)
	{
		if (ptNewPosition.x  < m_rcBounds.left ||
			ptNewPosition.x >(m_rcBounds.right - ptSpriteSize.x))
		{
			ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
				m_rcBounds.right - ptSpriteSize.x));
			SetVelocity(0, 0);
		}
		if (ptNewPosition.y  < m_rcBounds.top ||
			ptNewPosition.y >(m_rcBounds.bottom - ptSpriteSize.y))
		{
			ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
				m_rcBounds.bottom - ptSpriteSize.y));
			SetVelocity(0, 0);
		}
	}
	// Continue? (Default)
	else //Do nothing, let it go
	{
	}


	SetPosition(ptNewPosition);

	return SA_NONE;
}

bool Unit::cmp ( Unit::PathfindingNode* a, Unit::PathfindingNode* b)
{
	return a->totalPathCost > b->totalPathCost;
};

void Unit::UpdateMapPosition()
{

	Map::SetSpriteGridCell(yIndex, xIndex, this);
	Map::SetGridCell(yIndex, xIndex, 4);


}

void Unit::Pathfind()
{
	while (!path.empty())
	{
		path.pop();
	}

	xIndex = GetXIndex(Map::GetCellWidth());
	yIndex = GetYIndex(Map::GetCellHeight());

	if (pathfindingPerformedThisCycle)
		return;

	//Create a pathfinding map
	//TODO-> This is created on the heap. This is bleugh. Create the objects/structs on the stack, but IDK how?
	//TODO-> EVERYTHING IS EQUAL WEIGHT RIGHT NOW. Give diagonals a weight of 1.5, and horizontals and verticals a weight of 1
	PathfindingNode*** pathfindingMap;
	
	pathfindingMap = (PathfindingNode***)calloc(Map::GetHeight(), sizeof(PathfindingNode**));
	for (int i = 0; i < Map::GetHeight(); i++)
	{
		pathfindingMap[i] = (PathfindingNode**)calloc(Map::GetWidth(), sizeof(PathfindingNode*));
		for (int j = 0; j < Map::GetWidth(); j++)
		{
			PathfindingNode* bluh = new PathfindingNode(i, j);
			pathfindingMap[i][j] = bluh;
		}
	}

	//Make sure that our destination is set
	if (m_destinationIndex.x != -1 && m_destinationIndex.y != -1)
	{
		


		//PERFORM PATHFINDING
		std::vector<PathfindingNode*> nodesToCheck;//The list of nodes to check
		std::make_heap(nodesToCheck.begin(), nodesToCheck.end(),cmp);
		PathfindingNode* startingNode = pathfindingMap[GetYIndex(Map::GetCellHeight())][GetXIndex(Map::GetCellWidth())];//The starting node
		startingNode->totalPathCost = 0;
		startingNode->wasChecked = true;
		nodesToCheck.push_back(startingNode);//Push the node you're currently on into the queue
		PathfindingNode* destinationNode=pathfindingMap[m_destinationIndex.y][m_destinationIndex.x];//The destination node

		bool destinationReached = false;
		while (nodesToCheck.size()>0)
		{
			//std::cout << "BOI";
			PathfindingNode* curNode =nodesToCheck.front();
			
			//Nodes can be added to the list prior to being checked.
			//Instead of iterating through every node, you can pop it off if it's been checked.
			if (curNode!=startingNode && curNode->wasChecked)
			{
				nodesToCheck.erase(nodesToCheck.begin(), nodesToCheck.begin()+1);
				std::make_heap(nodesToCheck.begin(), nodesToCheck.end(), cmp);

			}
			else
			{
				for (int i = -1; i <= 1; i++)
				{
					if (destinationReached)
						break;
					for (int j = -1; j <= 1; j++)
					{
						if (destinationReached)
							break;

						int neighborXIndex = j + curNode->xIndex;
						int neighborYIndex = i + curNode->yIndex;
						float neighborPathCost = ((abs(i) + abs(j) > 1.0f) ? 1.5f : 1.0f);
						PathfindingNode* neighborNode;
						if (
							!(i == 0 && j == 0)
							&& neighborYIndex < Map::GetHeight()
							&& neighborYIndex >= 0
							&& neighborXIndex < Map::GetWidth()
							&& neighborXIndex >= 0
							&& 
							
							(pathfindingMap[neighborYIndex][neighborXIndex]->totalPathCost>neighborPathCost+curNode->totalPathCost)
							
							&&  
							(
								Map::GetGridCell(neighborYIndex, neighborXIndex) == 0
								|| Map::GetGridCell(neighborYIndex, neighborXIndex) == 3
							)

							)
						{
							if (Map::GetGridCell(neighborYIndex, neighborXIndex) == 0 || Map::GetGridCell(neighborYIndex, neighborXIndex) == 3)
							{
								neighborNode = pathfindingMap[neighborYIndex][neighborXIndex];
								neighborNode->parentNode = curNode;


								neighborNode->totalPathCost = curNode->totalPathCost + neighborPathCost;

								if (neighborNode == destinationNode)
								{
									//DEBUG
									std::stack<PathfindingNode*> tempStack;//DEBUG
									std::cout << "Destination Found!" << std::endl;
									while (neighborNode->parentNode != NULL)
									{
										path.push(neighborNode);
										tempStack.push(neighborNode);//DEBUG
										neighborNode = neighborNode->parentNode;
									}

									//DEBUG
									while (!tempStack.empty())
									{
										std::cout << "X: " << tempStack.top()->xIndex<< " Y:"<< tempStack.top()->yIndex << std::endl;
										tempStack.pop();
									}


									//We're done checking nodes
									while (nodesToCheck.size() > 0)
									{
										nodesToCheck.erase(nodesToCheck.begin(),nodesToCheck.begin()+1);
									}

									destinationReached = true;

									break;

								}
								else
								{
									neighborNode->wasAddedToList=true;
									nodesToCheck.push_back(neighborNode);
									std::push_heap(nodesToCheck.begin(), nodesToCheck.end(), cmp);
								}
							}
						}
					}
				}//for
			}
			

			curNode->wasChecked = true;
			if(nodesToCheck.size()>0)
				nodesToCheck.erase(nodesToCheck.begin(), nodesToCheck.begin()+1);
			}
		}

		/*
	for (int i = 0; i < Map::GetCellHeight(); i++)
	{
		for (int j = 0; j < Map::GetCellWidth(); j++)
		{
			std::cout << Map::GetGridCell(i, j);
		}
		std::cout << std::endl;
	}
	*/

		//UPDATE THE MAP WITH YOUR CURRENT POSITION
	
}

std::list<Sprite*> Unit::GetNeighboringCells()
{
	std::list<Sprite*> ret;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			//Only allow up, down, left and right
			if (abs(i + j) != 1)
				continue;
			//Only Look for positions that are in bounds
			int neighboringYIndex = i + GetYIndex(Map::GetCellHeight());
			int neighboringXIndex = j + GetXIndex(Map::GetCellWidth());
			if (
				!((neighboringYIndex < 0 || neighboringYIndex >= Map::GetCellHeight()
				|| neighboringXIndex < 0 || neighboringXIndex >= Map::GetCellWidth())
				||Map::GetSpriteCell(neighboringYIndex, neighboringXIndex)==NULL))
				ret.push_back(Map::GetSpriteCell(neighboringYIndex, neighboringXIndex));
		}
	}

	return ret;
	
}

void Unit::HandlePathTraversal()
{
	//As long as the path is not empty, move towards the 
	if (!path.empty())
	{
		PathfindingNode* nextNode = path.top();

		//Set your position

		//TODO, RIGHT HERE->If the next node is full, STOP. Re-pathfind
		if (Map::GetGridCell(nextNode->xIndex, nextNode->yIndex) != 0
			&& Map::GetGridCell(nextNode->xIndex, nextNode->yIndex) != 3)
		{
			//while (path.size() > 1)
			//{
				//path.pop();
			//}
			/*m_destinationIndex.x = path.top()->xIndex;
			m_destinationIndex.y = path.top()->yIndex;
			Pathfind();*/
			
		}
		//If you're on the nextnode, pull off the node.

		//TODO->Fix the velocity positioning
		if (nextNode == NULL)
			return;

		int cellWidth = Map::GetCellWidth();
		int cellHeight = Map::GetCellHeight();
		int destinationX = nextNode->xIndex;
		int destinationY = nextNode->yIndex;
		POINT destination = POINT{ long(destinationX*cellWidth) + long(floor(cellWidth / 2)) - long(GetWidth() / 2),
			long(destinationY*cellHeight) + long(floor(cellHeight / 2)) - long(GetHeight() / 2) };
		int xVel = nextNode->xIndex - xIndex;
		int yVel = nextNode->yIndex - yIndex;
		//Get their signs
		xVel = xVel / (xVel == 0 ? 1 : abs(xVel));
		yVel = yVel / (yVel == 0 ? 1 : abs(yVel));
		float normalizedXVel = float(xVel) / sqrt(pow(xVel, 2) + pow(yVel, 2));
		float normalizedYVel = float(yVel) / sqrt(pow(xVel, 2) + pow(yVel, 2));
		//if(xVel==0)
		//	SetPosition(floor((nextNode->xIndex)*Map::GetCellWidth()) + GetWidth() / 2, (GetPosition().top+GetPosition().bottom)/2);
		//if (yVel == 0)
		//	SetPosition((GetPosition().left+ GetPosition().right)/2, floor((nextNode->yIndex)*Map::GetCellHeight()) + GetHeight() / 2);

		//SetPosition(floor((nextNode->xIndex)*Map::GetCellWidth()), floor((nextNode->yIndex)*Map::GetCellHeight()));
		SetVelocity(POINT{ long(normalizedXVel*m_movementSpeed),long(normalizedYVel*m_movementSpeed) });

		if (
				(
				GetXIndex(Map::GetCellWidth()) == nextNode->xIndex
				&& GetYIndex(Map::GetCellHeight()) == nextNode->yIndex
				)
				||
				(//Overshoot
					(GetVelocity().x > 0 && floor((nextNode->xIndex)*Map::GetCellWidth()) < (GetPosition().left))
				||	(GetVelocity().x < 0 && floor((nextNode->xIndex)*Map::GetCellWidth()) > (GetPosition().left))
				||	(GetVelocity().y > 0 && floor((nextNode->yIndex)*Map::GetCellHeight()) <(GetPosition().top))
				||	(GetVelocity().y < 0 && floor((nextNode->yIndex)*Map::GetCellHeight()) >(GetPosition().top))
				)
			)
		{

			Map::SetGridCell(yIndex, xIndex, 0);
			Map::RemoveSpriteGridCell(yIndex, xIndex);

			//SetPosition(floor((nextNode->xIndex)*Map::GetCellWidth()), floor((nextNode->yIndex)*Map::GetCellHeight()));
			
			xIndex = nextNode->xIndex;
			yIndex = nextNode->yIndex;

			
			//TODO->Remove the old gridcells properly. Do this where velocity is applied.
			if (path.size() == 1)
			{
				SetPosition(floor((nextNode->xIndex)*Map::GetCellWidth()) + GetWidth() / 2, floor((nextNode->yIndex)*Map::GetCellHeight()));
				SetVelocity(POINT{ 0,0 });
			}

			UpdateMapPosition();
			
			path.pop();
			if (path.size() > 0)
				nextNode = path.top();
			else
			{
				nextNode = NULL;
			}
		}




		
	
	

	}
	else
	{

	}



}

void Unit::MoveToPoint()
{
	if (m_destinationIndex.x != -1 && m_destinationIndex.y != -1)
	{
		//DEBUG
		//TODO-> This is DIRTY. Clean this up, and implement it AFTER movement (it's just teleportation right now.)
		Map::RemoveSpriteGridCell(GetYIndex(Map::GetCellHeight()), GetXIndex(Map::GetCellWidth()));
		
		Pathfind();
		
		m_destination.x = -1;
		m_destination.y = -1;
		m_destinationIndex.x = -1;
		m_destinationIndex.y = -1;
		//Pull the next destination from your pathfinding Queue
		//Find the velocity required to reach it, and apply it here.

		//Replace this with pathfinding movement (one square at a time or something)
		/*
		POINT velocityDir = {
			long(m_destination.x - (GetPosition().left + GetWidth() / 2)),
			long(m_destination.y - (GetPosition().top + GetHeight() / 2))
		};
		long hypot = long(sqrt(pow(velocityDir.x, 2) + pow(velocityDir.y, 2)) / 5);
		POINT normalizedVelocityDir = { m_movementSpeed*velocityDir.x / hypot,m_movementSpeed*velocityDir.y / hypot };
		SetVelocity(normalizedVelocityDir);
		*/

		/*
		// Stop overshoot
		POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
		ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
		ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
		
		//Stop X overshoot
		if (
			(ptNewPosition.x < m_destination.x && m_ptVelocity.x < 0) ||
			(ptNewPosition.x > m_destination.x && m_ptVelocity.x > 0)
			) 
		{
			m_ptVelocity = POINT{ 0,m_ptVelocity.y };
			ptNewPosition = POINT{m_destination.x, ptNewPosition.y};
			SetPosition(ptNewPosition.x, ptNewPosition.y);
		}
		//Stop Y overshoot
		if (
			(ptNewPosition.y < m_destination.y && m_ptVelocity.y < 0) ||
			(ptNewPosition.y > m_destination.y && m_ptVelocity.y>0)
			)
		{
			m_ptVelocity = POINT{ m_ptVelocity.x,0 };
			ptNewPosition = POINT{ptNewPosition.x, m_destination.y };
			SetPosition(ptNewPosition.x, ptNewPosition.y);
		}
		*/

	}
}
