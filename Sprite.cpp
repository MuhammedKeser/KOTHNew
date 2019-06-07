//-----------------------------------------------------------------
// Sprite Object
// C++ Source - Sprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Sprite.h"

//-----------------------------------------------------------------
// Sprite Constructor(s)/Destructor
//-----------------------------------------------------------------
Sprite::Sprite(HDC hDC, HINSTANCE hInstance)
{
	//m_pBitmap = (Bitmap*)malloc(sizeof(Bitmap*));
	m_pBitmap = new Bitmap(hDC, BITMAP_ID, hInstance);
	SetRect(&m_rcPosition, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap)
{
  // Initialize the member variables
  m_pBitmap = pBitmap;
  SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
  CalcCollisionRect();
  m_ptVelocity.x = m_ptVelocity.y = 0;
  m_iZOrder = 0;
  SetRect(&m_rcBounds, 0, 0, 640, 480);
  m_baBoundsAction = BA_STOP;
  m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
  // Calculate a random position
  int iXPos = rand() % (rcBounds.right - rcBounds.left);
  int iYPos = rand() % (rcBounds.bottom - rcBounds.top);

  // Initialize the member variables
  m_pBitmap = pBitmap;
  SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
    iYPos + pBitmap->GetHeight());
  CalcCollisionRect();
  m_ptVelocity.x = m_ptVelocity.y = 0;
  m_iZOrder = 0;
  CopyRect(&m_rcBounds, &rcBounds);
  m_baBoundsAction = baBoundsAction;
  m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
    RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
  // Initialize the member variables
  m_pBitmap = pBitmap;
  SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, pBitmap->GetWidth(),
    pBitmap->GetHeight());
  CalcCollisionRect();
  m_ptVelocity = ptPosition;
  m_iZOrder = iZOrder;
  CopyRect(&m_rcBounds, &rcBounds);
  m_baBoundsAction = baBoundsAction;
  m_bHidden = FALSE;
}

Sprite::Sprite(HDC hDC, HINSTANCE hInstance, UINT BITMAP_ID)
{
	this->BITMAP_ID = BITMAP_ID;
	m_pBitmap = new Bitmap(hDC, BITMAP_ID, hInstance);
	SetRect(&m_rcPosition, 0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap * pBitmap, UINT BITMAP_ID)
{
	this->BITMAP_ID = BITMAP_ID;
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap * pBitmap, RECT & rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction)
{
	this->BITMAP_ID = BITMAP_ID;
	// Calculate a random position
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);

	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
		iYPos + pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap * pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT & rcBounds, UINT BITMAP_ID, BOUNDSACTION baBoundsAction)
{
	this->BITMAP_ID = BITMAP_ID;
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, pBitmap->GetWidth(),
		pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity = ptPosition;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
}

Sprite::~Sprite()
{
}

//-----------------------------------------------------------------
// Sprite General Methods
//-----------------------------------------------------------------
void Sprite::Scale(float x,float y)
{
	//Set the actual new values
	xScale = x;
	yScale = y;

	//std::cout << "New x scale: " << xScale<< std::endl;

	int widthOffset = floor((float)GetWidth() * xScale);
	int heightOffset = floor((float)GetHeight() * yScale);


	SetRect(&m_rcPosition,
		widthOffset >= 0 ? m_rcPosition.left : m_rcPosition.right + widthOffset,
		heightOffset >= 0 ? m_rcPosition.top : m_rcPosition.bottom + heightOffset,
		widthOffset >= 0 ? (m_rcPosition.left + widthOffset) : m_rcPosition.right,
		heightOffset >= 0 ? (m_rcPosition.top + heightOffset) : m_rcPosition.bottom);
	
	CopyRect(&m_rcCollision, &m_rcPosition);
	CalcCollisionRect();
}

void Sprite::MarkForDeletion()
{
	m_deletionPending = true;
}

SPRITEACTION Sprite::UpdatePosition()
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
			return SA_KILL;
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

void Sprite::Draw(HDC hDC,Camera* cam)
{
  // Draw the sprite if it isn't hidden
  if (m_pBitmap != NULL && !m_bHidden)
    m_pBitmap->Draw(hDC,
		xScale>=0? m_rcPosition.left- cam->GetPosition().x : m_rcPosition.right - cam->GetPosition().x,
		yScale>=0? m_rcPosition.top- cam->GetPosition().y : m_rcPosition.bottom - cam->GetPosition().y,
		xScale, 
		yScale,
		TRUE);
    //m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
}

void Sprite::ResetCollisionList()
{
	
	std::list<Sprite*>::const_iterator it = collisionList.begin();
	
	while (it != collisionList.end())
	{
		std::list<Sprite*>::const_iterator curr = it++;
		if (!(this->TestCollision(*curr)))
		{
			std::cout << "REMOVED FROM COLLISION LIST" << std::endl;
			//Run the oncollision exit code
			OnCollisionExit(*curr);
			//Remove the sprite from the list
			collisionList.erase(curr);
		}

	}
}
