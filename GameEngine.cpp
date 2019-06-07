//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - GameEngine.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
GameEngine *GameEngine::m_pGameEngine = NULL;

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  PSTR szCmdLine, int iCmdShow)
{
  MSG         msg;
  static int  iTickTrigger = 0;
  int         iTickCount;
  bool debugConsole = true;//Used to provide a console for debugging.

  if (GameInitialize(hInstance))
  {
	  //Open up a debug console
	  if (debugConsole)
	  {
		  AllocConsole();
		  freopen("CONOUT$", "w+", stdout);
	  }

    // Initialize the game engine
    if (!GameEngine::GetEngine()->Initialize(iCmdShow))
      return FALSE;

    // Enter the main message loop
    while (TRUE)
    {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        // Process the message
        if (msg.message == WM_QUIT)
          break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else
      {
        // Make sure the game engine isn't sleeping
        if (!GameEngine::GetEngine()->GetSleep())
        {
          // Check the tick count to see if a game cycle has elapsed
          iTickCount = GetTickCount();
          if (iTickCount > iTickTrigger)
          {
            iTickTrigger = iTickCount +
              GameEngine::GetEngine()->GetFrameDelay();
            HandleKeys();
            GameEngine::GetEngine()->CheckJoystick();
            GameCycle();
          }
        }
      }
    }
    return (int)msg.wParam;
  }

  // End the game
  GameEnd();

  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // Route all Windows messages to the game engine
  return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// GameEngine Constructor(s)/Destructor
//-----------------------------------------------------------------
GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
  LPTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
  // Set the member variables for the game engine
  m_pGameEngine = this;
  m_hInstance = hInstance;
  m_hWindow = NULL;
  if (lstrlen(szWindowClass) > 0)
    lstrcpy(m_szWindowClass, szWindowClass);
  if (lstrlen(szTitle) > 0)
    lstrcpy(m_szTitle, szTitle);
  m_wIcon = wIcon;
  m_wSmallIcon = wSmallIcon;
  m_iWidth = iWidth;
  m_iHeight = iHeight;
  m_iFrameDelay = 50;   // 20 FPS default
  m_bSleep = TRUE;
  m_uiJoystickID = 0;
  m_vSprites.reserve(50);
}

GameEngine::~GameEngine()
{
}

//-----------------------------------------------------------------
// Game Engine General Methods
//-----------------------------------------------------------------
BOOL GameEngine::Initialize(int iCmdShow)
{
  WNDCLASSEX    wndclass;

  // Create the window class for the main window
  wndclass.cbSize         = sizeof(wndclass);
  wndclass.style          = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc    = WndProc;
  wndclass.cbClsExtra     = 0;
  wndclass.cbWndExtra     = 0;
  wndclass.hInstance      = m_hInstance;
  wndclass.hIcon          = LoadIcon(m_hInstance,
    MAKEINTRESOURCE(GetIcon()));
  wndclass.hIconSm        = LoadIcon(m_hInstance,
    MAKEINTRESOURCE(GetSmallIcon()));
  wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
  wndclass.lpszMenuName   = NULL;
  wndclass.lpszClassName  = m_szWindowClass;

  // Register the window class
  if (!RegisterClassEx(&wndclass))
    return FALSE;

  // Calculate the window size and position based upon the game size
  int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
      iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
        GetSystemMetrics(SM_CYCAPTION);
  if (wndclass.lpszMenuName != NULL)
    iWindowHeight += GetSystemMetrics(SM_CYMENU);
  int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
      iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

  // Create the window
  m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
    WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
    iWindowHeight, NULL, NULL, m_hInstance, NULL);
  if (!m_hWindow)
    return FALSE;

  // Show and update the window
  ShowWindow(m_hWindow, iCmdShow);
  UpdateWindow(m_hWindow);

  return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // Route Windows messages to game engine member functions
  switch (msg)
  {
    case WM_CREATE:
      // Set the game window and start the game
      SetWindow(hWindow);
      GameStart(hWindow);
      return 0;

    case WM_ACTIVATE:
      // Activate/deactivate the game and update the Sleep status
      if (wParam != WA_INACTIVE)
      {
        GameActivate(hWindow);
        SetSleep(FALSE);
      }
      else
      {
        GameDeactivate(hWindow);
        SetSleep(TRUE);
      }
      return 0;

    case WM_PAINT:
      HDC         hDC;
      PAINTSTRUCT ps;
	  RECT windowRect;
	  GetWindowRect(hWindow, &windowRect);
      hDC = BeginPaint(hWindow, &ps);
	  
	  //InvalidateRect(NULL, NULL, FALSE);
      // Paint the game
      GamePaint(hDC);

	  ps.rcPaint.top = 0;
	  ps.rcPaint.left = 0;
	  ps.rcPaint.bottom = windowRect.bottom;
	  ps.rcPaint.right = windowRect.right;

      EndPaint(hWindow, &ps);
      return 0;

    case WM_LBUTTONDOWN:
      // Handle left mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

    case WM_LBUTTONUP:
      // Handle left mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
      return 0;

    case WM_RBUTTONDOWN:
      // Handle right mouse button press
      MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

    case WM_RBUTTONUP:
      // Handle right mouse button release
      MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
      return 0;

    case WM_MOUSEMOVE:
      // Handle mouse movement
      MouseMove(LOWORD(lParam), HIWORD(lParam));
	  //Input::UpdateMousePosition(LOWORD(lParam), HIWORD(lParam));

      return 0;

    case WM_DESTROY:
      // End the game and exit the application
      GameEnd();
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
  MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
  PostQuitMessage(0);
}

BOOL GameEngine::InitJoystick()
{
  // Make sure joystick driver is present
  UINT uiNumJoysticks;
  if ((uiNumJoysticks = joyGetNumDevs()) == 0)
    return FALSE;

  // Make sure the joystick is attached
  JOYINFO jiInfo;
  if (joyGetPos(JOYSTICKID1, &jiInfo) != JOYERR_UNPLUGGED)
    m_uiJoystickID = JOYSTICKID1;
  else
    return FALSE;

  // Calculate the trip values
  JOYCAPS jcCaps;
  joyGetDevCaps(m_uiJoystickID, &jcCaps, sizeof(JOYCAPS));
  DWORD dwXCenter = ((DWORD)jcCaps.wXmin + jcCaps.wXmax) / 2;
  DWORD dwYCenter = ((DWORD)jcCaps.wYmin + jcCaps.wYmax) / 2;
  m_rcJoystickTrip.left = (jcCaps.wXmin + (WORD)dwXCenter) / 2;
  m_rcJoystickTrip.right = (jcCaps.wXmax + (WORD)dwXCenter) / 2;
  m_rcJoystickTrip.top = (jcCaps.wYmin + (WORD)dwYCenter) / 2;
  m_rcJoystickTrip.bottom = (jcCaps.wYmax + (WORD)dwYCenter) / 2;

  return TRUE;
}

void GameEngine::HandleCameraMovement(Camera* camera)
{

	POINT cameraOldPosition = {camera->GetPosition().x, camera->GetPosition().y };
	POINT increment = { 0,0 };
	bool inputSent = false;
	if (Input::KeyHeld(InputKeys::KEY::LEFT) || Input::KeyHeld(InputKeys::KEY::A))
	{
		inputSent = true;
		increment.x -= 10;
	}
	if (Input::KeyHeld(InputKeys::KEY::RIGHT)|| Input::KeyHeld(InputKeys::KEY::D))
	{
		inputSent = true;
		increment.x += 10;
	}
	if (Input::KeyHeld(InputKeys::KEY::DOWN) || Input::KeyHeld(InputKeys::KEY::S))
	{
		inputSent = true;
		increment.y += 10;
	}
	if (Input::KeyHeld(InputKeys::KEY::UP) || Input::KeyHeld(InputKeys::KEY::W))
	{
		inputSent = true;
		increment.y -= 10;
	}

	if (inputSent)
	{
		camera->SetPosition(cameraOldPosition.x + increment.x, cameraOldPosition.y + increment.y);
		// Track the mouse with the target sprite
		//Fore._pTargetSprite->SetPosition(x - (_pTargetSprite->GetWidth() / 2), y - (_pTargetSprite->GetHeight() / 2));
	}

}

void GameEngine::CaptureJoystick()
{
  // Capture the joystick
  if (m_uiJoystickID == JOYSTICKID1)
    joySetCapture(m_hWindow, m_uiJoystickID, NULL, TRUE);
}

void GameEngine::ReleaseJoystick()
{
  // Release the joystick
  if (m_uiJoystickID == JOYSTICKID1)
    joyReleaseCapture(m_uiJoystickID);
}

void GameEngine::CheckJoystick()
{
  if (m_uiJoystickID == JOYSTICKID1)
  {
    JOYINFO jiInfo;
    JOYSTATE jsJoystickState = 0;
    if (joyGetPos(m_uiJoystickID, &jiInfo) == JOYERR_NOERROR)
    {
      // Check horizontal movement
      if (jiInfo.wXpos < (WORD)m_rcJoystickTrip.left)
        jsJoystickState |= JOY_LEFT;
      else if (jiInfo.wXpos > (WORD)m_rcJoystickTrip.right)
        jsJoystickState |= JOY_RIGHT;

      // Check vertical movement
      if (jiInfo.wYpos < (WORD)m_rcJoystickTrip.top)
        jsJoystickState |= JOY_UP;
      else if (jiInfo.wYpos > (WORD)m_rcJoystickTrip.bottom)
        jsJoystickState |= JOY_DOWN;

      // Check buttons
      if(jiInfo.wButtons & JOY_BUTTON1)
        jsJoystickState |= JOY_FIRE1;
      if(jiInfo.wButtons & JOY_BUTTON2)
        jsJoystickState |= JOY_FIRE2;
    }

    // Allow the game to handle the joystick
    HandleJoystick(jsJoystickState);
  }
}

void GameEngine::AddSprite(Sprite* pSprite)
{
  // Add a sprite to the sprite vector
  if (pSprite != NULL)
  {
    // See if there are sprites already in the sprite vector
    if (m_vSprites.size() > 0)
    {
      // Find a spot in the sprite vector to insert the sprite by its z-order
      vector<Sprite*>::iterator siSprite;
      for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
        if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
        {
          // Insert the sprite into the sprite vector
          m_vSprites.insert(siSprite, pSprite);
          return;
        }
    }

    // The sprite's z-order is highest, so add it to the end of the vector
    m_vSprites.push_back(pSprite);
  }
}

void GameEngine::DrawBackground(HDC hDC, Bitmap* backgroundBM, RECT backgroundRect)
{
	backgroundBM->Draw(hDC,backgroundRect.left, backgroundRect.top , 1, 1,TRUE);
}

void GameEngine::DrawSprites(HDC hDC,Camera* camera)
{
  // Draw the sprites in the sprite vector
  vector<Sprite*>::iterator siSprite;
  for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
  {
	  if (
		  (*siSprite)->GetPosition().left <= camera->GetPosition().x + GetWidth() &&
		  camera->GetPosition().x <= (*siSprite)->GetPosition().right &&
		  (*siSprite)->GetPosition().top <= camera->GetPosition().y + GetHeight() &&
		  camera->GetPosition().y <= (*siSprite)->GetPosition().bottom
		  )
		  (*siSprite)->Draw(hDC, camera);
  }
}

void GameEngine::UpdateSprites()
{
  // Update the sprites in the sprite vector
  RECT          rcOldSpritePos;
  SPRITEACTION  saSpriteAction;
  vector<Sprite*>::iterator siSprite;
  for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
  {
	  //Update the sprites' positions
	  saSpriteAction = (*siSprite)->UpdatePosition();
	  // Handle the SA_KILL sprite action
	  if (saSpriteAction & SA_KILL)
	  {
		  delete (*siSprite);
		  m_vSprites.erase(siSprite);
		  siSprite--;
		  continue;
	  }

	  // Save the old sprite position in case we need to restore it
	  rcOldSpritePos = (*siSprite)->GetPosition();

  }

 
  
}

void GameEngine::UpdateCollisions()
{
	//Update codes

	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Update the sprite
		(*siSprite)->Update();

		// See if the sprite collided with any others
		CheckSpriteCollision(*siSprite);

		// Restore the old sprite position
		//(*siSprite)->SetPosition(rcOldSpritePos);
	}

	//Exit collisions, and collision list resetting
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		if ((*siSprite)->isStatic)
			continue;

		(*siSprite)->ResetCollisionList();

	}
}

void GameEngine::DeleteSprites()
{
	for (std::vector<Sprite*>::reverse_iterator siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); ++siSprite)
	{
		if ((*siSprite)->DeletionIsPending())
		{
			delete (*siSprite);
			m_vSprites.erase(std::next(siSprite).base());
			break;
		}
	}
}

//-----------------------------------------------------------------
// Game Engine Helper Methods
//-----------------------------------------------------------------
void GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
	//Static sprites shouldn't check for collision
	if (pTestSprite->isStatic)
		return;

  // See if the sprite has collided with any other sprites
  vector<Sprite*>::iterator siSprite;
  int debug = 0;
  for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
  {
    // Make sure not to check for collision with itself
    if (pTestSprite == (*siSprite))
      continue;

    // Test the collision
	if (pTestSprite->TestCollision(*siSprite))
	{
		//Run each sprites' collision code, depending on whether or not this is the first frame of their collision or not
		//Used to check if this collision is a 'stay' or 'enter' collision
		bool isOnStayCollision = false;

		const std::list<Sprite*>& collisionList = pTestSprite->GetCollisionList();
		
		//Check if they exist in each others' collision lists - i.e., were already in a collision
		std::list<Sprite*>::const_iterator it = collisionList.begin();
		for (int i = 0; i < collisionList.size(); i++)
		{
			if ((*it) == (*siSprite))
			{
				isOnStayCollision = true;
				break;
			}
			std::advance(it, 1);
		}
		
		//Run the appropriate event
		if (isOnStayCollision)
			pTestSprite->OnCollisionStay(*siSprite);
		else
		{
			pTestSprite->AddSpriteToCollisionList(*siSprite);
			pTestSprite->OnCollisionEnter(*siSprite);
		}



		// Return the detected collision, and set their velocities
		//bool returnValue = CheckCollision((*siSprite), pTestSprite);
		//return true;
	}



  }

  // No collision
  //return FALSE;
}
/*

//-----------------------------------------------------------------
// Game Engine Helper Methods
//-----------------------------------------------------------------
void GameEngine::AI()
{

	// See if the sprite has collided with any other sprites
	vector<Sprite*>::iterator siSprite;
	int debug = 0;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite))
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))
		{
			//Run each sprites' collision code, depending on whether or not this is the first frame of their collision or not
			//Used to check if this collision is a 'stay' or 'enter' collision
			bool isOnStayCollision = false;

			const std::list<Sprite*>& collisionList = pTestSprite->GetCollisionList();

			//Check if they exist in each others' collision lists - i.e., were already in a collision
			std::list<Sprite*>::const_iterator it = collisionList.begin();
			for (int i = 0; i < collisionList.size(); i++)
			{
				if ((*it) == (*siSprite))
				{
					isOnStayCollision = true;
					break;
				}
				std::advance(it, 1);
			}



			//Run the appropriate event
			if (isOnStayCollision)
				pTestSprite->OnCollisionStay(*siSprite);
			else
			{
				pTestSprite->AddSpriteToCollisionList(*siSprite);
				pTestSprite->OnCollisionEnter(*siSprite);
			}



			// Return the detected collision, and set their velocities
			//bool returnValue = CheckCollision((*siSprite), pTestSprite);
			//return true;
		}



	}

	// No collision
	//return FALSE;
}*/

void GameEngine::CleanupSprites()
{
  // Delete and remove the sprites in the sprite vector
  vector<Sprite*>::iterator siSprite;
  for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
  {
	  if(*siSprite!=NULL)
		delete (*siSprite);
    //m_vSprites.erase(siSprite);
    //siSprite--;
  }
}

void GameEngine::DeleteSprite(Sprite* spriteToDelete)
{
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		if (*siSprite == spriteToDelete)
		{
			m_vSprites.erase(siSprite);
			break;
		}
	}
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
  // See if the point is in a sprite in the sprite vector
  vector<Sprite*>::reverse_iterator siSprite;
  for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
    if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
      return (*siSprite);

  // The point is not in a sprite
  return NULL;
}

