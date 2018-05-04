/**
\file		Main.cpp
\author		Tom Bisch
\date		Apr 5, 2016
\brief		This file contains the main window and UI components for the Unhappy Flying Reptiles game.
\details	Creates a window using win32 and then loads gdi+ and image resources before animating an
			unhappy flying reptile across the screen. In loading the resources, BitmapImage and CompositeImage 
			objects and inherited versions are used to store image resources to be drawn onto the window. 
			The player can click on the window in an attempt to shoot the flying reptile and if successful, 
			will see the reptile fall rotating to the ground. The player earns points based on how quickly the 
			reptile is shot down. Extra points are awarded if the reptile happens to knock one of the top 3 boxes
			onto the ground. The game currently does not end.
*/


// include files
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")
#include "CompositeImage.h"
#include "BitmapImage.h"
#include "Reptile.h"
#include "Box.h"
#include "Scoreboard.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <ctime>
using namespace Gdiplus;
using namespace std;


// prototypes
void LoadResources(HWND hWnd);
void UnloadResources(void);
void Draw(HWND hWnd);
void Update(void);
void WindowResize(int width, int height);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// resource and window objects
ULONG_PTR gdiplusToken;
int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 400;


// game objects
CompositeImage* background;
CompositeImage* slingshot;
Reptile* reptile;
Box* boxes[6];
BitmapImage* flash;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd) 
{
	// seed random number generator
	srand((unsigned)time(0));

	// declare window objects
	WNDCLASSEX window;
	HWND windowHandle;
	MSG message;
	ZeroMemory(&window, sizeof(WNDCLASSEX));
	ZeroMemory(&message, sizeof(MSG));
	
	// build window structure	
	window.cbClsExtra = NULL;
	window.cbSize = sizeof(WNDCLASSEX);
	window.cbWndExtra = NULL;
	window.hbrBackground = (HBRUSH)COLOR_WINDOW;
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hIcon = NULL;
	window.hIconSm = NULL;
	window.hInstance = hInst;
	window.lpfnWndProc = (WNDPROC)WinProc;
	window.lpszClassName = L"Window Class";
	window.lpszMenuName = NULL;
	window.style = CS_HREDRAW | CS_VREDRAW;

	// register window class
	if (RegisterClassEx(&window) == NULL) 
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Window Class Creation Failed", L"Window Class Failed", MB_ICONERROR);
	}

	// create window
	windowHandle = CreateWindowEx(
		NULL, L"Window Class",
		L"Unhappy Flying Reptiles",
		WS_OVERLAPPEDWINDOW,
		200, 200, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInst, NULL);

	// check for error creating window
	if (windowHandle == NULL) 
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Window Creation Failed", L"Window Creation Failed", MB_ICONERROR);
	}

	// load resources before showing window
	LoadResources(windowHandle);

	// show window
	ShowWindow(windowHandle, nShowCmd);

	// GetMessage returns all available messages (no filtering performed)
	while (GetMessage(&message, NULL, 0, 0)) 
	{
		TranslateMessage(&message); // translates virtual-key messages into character messages
		DispatchMessage(&message); // dispatches a message to a window procedure
	}

	// release resources
	UnloadResources();

	return 0;
}


/**
\brief		Updates the position of the Reptile object for the next frame.
\details	An application-defined function that processes messages sent to a window.
\param[in]	hWnd The handle to the window.
\param[in]	message The message of what event happened on the window.
\param[in]	wParam Additional message information that depends on the message parameter.
\param[in]	lParam Additional message information that depends on the message parameter.
/return		The result returned depends on the message parameter.
*/
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		// hide cursor for client area of window
		case WM_SETCURSOR:		
			if (message == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(NULL);
				return TRUE;
			}
			break;

		// window resized
		case WM_SIZE:	
			WindowResize(LOWORD(lParam), HIWORD(lParam));
			break;

		// timer event handler (game loop)
		case WM_TIMER:
			Update();
			Draw(hWnd);		
			break;

		// mouse move
		case WM_MOUSEMOVE:
			slingshot->MoveTo(LOWORD(lParam) - slingshot->GetWidth() / 2, HIWORD(lParam) - 15);
			break;

		// click event
		case WM_LBUTTONDOWN:
			// check for collision with reptile
			if (reptile->CheckCollision(LOWORD(lParam), HIWORD(lParam)) == true)
			{			
				if (reptile->GetState() == Reptile::STATE_FLYING)
				{
					// set is hit for screen flash
					reptile->SetIsHit(true);
					// end the round to stop round score from decreasing
					Scoreboard::EndRound();
					// play reptile shot sound
					PlaySound(L"Sounds\\reptileshot.wav", NULL, SND_ASYNC | SND_FILENAME);
					// set the new state for reptile
					reptile->SetState(Reptile::STATE_FALLING);
				}					
			}
			if (reptile->GetState() == Reptile::STATE_FLYING)
			{
				// reptile missed so play gunshot sound
				PlaySound(L"Sounds\\gunshot.wav", NULL, SND_ASYNC | SND_FILENAME);
			}
			break;

		// window closed
		case WM_DESTROY:		
			PostQuitMessage(0);
			return 0;
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


/**
\brief		Resizes all the bitmap images to fit the new window size.
\param[in]	width The new width of the window.
\param[in]	height The new height of the window.
*/
void WindowResize(int width, int height)
{
	// set new widow dimensions
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	// background
	background->Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// slingshot
	slingshot->Resize(1.0, 0.7, true);
	// reptile
	reptile->Resize((int)(WINDOW_WIDTH / 9.6), WINDOW_HEIGHT / 6);
}


/**
\brief		Updates the game objects for the next frame.
\details	Updates the position of the reptile and all boxes. Updates the scoreboard to
			so that the round score can be decremented. Checks the reptile for collisions
			against boxes. Checks boxes for collisions against other boxes. Adds bonus
			points to the total score if one of the 3 top boxes falls down.
*/
void Update(void)
{
	// update the reptile position
	if (reptile->Update(WINDOW_WIDTH, WINDOW_HEIGHT) == true)
	{
		// if reptile update method returns true, 
		// the next round is starting so the boxes should be reset

		// reset boxes
		for (int i = 0; i < 6; i++)
		{
			boxes[i]->Reset();
		}
	}

	// check boxes for collisions if reptile is falling or grounded
	if (reptile->GetState() != reptile->STATE_FLYING)
	{
		// check all boxes against reptile
		bool collision = false;
		for (int i = 0; i < 6; i++)
		{
			if (boxes[i]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, reptile) == true)
			{
				collision = true;
			}
		}
		// if no boxes are colliding with reptile, set reptile vertical velocity
		if (collision == false)
		{
			reptile->SetYVel(5);
		}

		// after one of the top 3 boxes has fallen, add 1 bonus point to total score each frame until the round restarts

		// check top box against boxes below
		if (boxes[0]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[1]) == false &&
			boxes[0]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[2]) == false)
		{
			// top box knocked onto ground
			Scoreboard::AddPoints(1);
			boxes[0]->SetYVel(5);
		}
		// check midleft against boxes below and to right
		if (boxes[1]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[2], true) == false &&
			boxes[1]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[3]) == false &&
			boxes[1]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[4]) == false)
		{
			// midleft box knocked onto ground
			Scoreboard::AddPoints(1);
			boxes[1]->SetYVel(5);
		}
		// check midright against boxes below and to left
		if (boxes[2]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[1], true) == false &&
			boxes[2]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[4]) == false &&
			boxes[2]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[5]) == false)
		{
			// midright box knocked onto ground
			Scoreboard::AddPoints(1);
			boxes[2]->SetYVel(5);
		}
		// check botleft against botcenter
		boxes[3]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[4], true);
		// check botcenter against botleft and botright
		boxes[4]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[3], true);
		boxes[4]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[5], true);
		// check botright against botcenter
		boxes[5]->CheckCollision(WINDOW_WIDTH, WINDOW_HEIGHT, boxes[4], true);

		// update position of each box
		for (int i = 0; i < 6; i++)
		{
			boxes[i]->Update(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}

	// update scoreboard
	if (Scoreboard::Update(WINDOW_WIDTH, WINDOW_HEIGHT) == true)
	{
		// if the scoreboard update method returns true, 
		// the round score for that round hit zero and the next round needs to start by resetting the reptile
		reptile->Reset(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}


/**
\brief		Draws all the game objects to the window.
\param[in]	hWnd The handle to the window.
*/
void Draw(HWND hWnd)
{
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;

	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	const int nMemDC = SaveDC(hdcMem);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMem, hBitmap);

	// Graphics object used for drawing to window
	Gdiplus::Graphics graphics(hdcMem);
	// ImageAttributes object used to draw primarily green pixels as transparent
	Gdiplus::ImageAttributes imgAttr;
	imgAttr.SetColorKey(Gdiplus::Color(0, 155, 0), Gdiplus::Color(100, 255, 100));

	// draw game objects
		
	// background
	background->Draw(&graphics, &imgAttr);
	// boxes
	for (int i = 0; i < 6; i++)
	{
		boxes[i]->Draw(&graphics);
	}
	// reptile
	reptile->DrawSingle(&graphics, reptile->GetNameOfImageToDraw());
	// slingshot cursor
	slingshot->Draw(&graphics);
	// screen flash
	if (reptile->GetIsHit() == true)
	{
		reptile->SetIsHit(false);
		flash->Draw(&graphics);
	}
	// scoreboard
	Scoreboard::Draw(&graphics, WINDOW_WIDTH, WINDOW_HEIGHT);

	RECT rcClip;
	GetClipBox(hdc, &rcClip);
	left = rcClip.left;
	top = rcClip.top;
	width = rcClip.right - rcClip.left;
	height = rcClip.bottom - rcClip.top;
	BitBlt(hdc, left, top, width, height, hdcMem, left, top, SRCCOPY);

	RestoreDC(hdcMem, nMemDC);
	DeleteObject(hBitmap);
}


/**
\brief		Initializes GDI+ and loads resources into memory for later use.
\param[in]	hWnd The handle to the window.
*/
void LoadResources(HWND hWnd)
{
	// start gdi+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// send WM_TIMER signal to window every 30 ms
	SetTimer(hWnd, WM_TIMER, 30, NULL);

	// create background composite image with background, midground, and foreground images
	background = new CompositeImage();
	background->AddImage(BitmapImage(L"Images\\background.bmp", L"back"));
	background->AddImage(BitmapImage(L"Images\\midground.bmp", L"mid"));
	background->AddImage(BitmapImage(L"Images\\foreground.bmp", L"fore"));

	// create slingshot cursor
	slingshot = new CompositeImage();
	slingshot->AddImage(BitmapImage(L"Images\\slingshot2.png", L"slingBack"));	
	slingshot->AddImage(BitmapImage(L"Images\\slingshot1.png", L"slingFore"));
	slingshot->AddImage(BitmapImage(L"Images\\cross.png", L"cross"));

	// create reptile
	reptile = new Reptile();
	reptile->AddImage(BitmapImage(L"Images\\flap0.png", L"flap0"));
	reptile->AddImage(BitmapImage(L"Images\\flap1.png", L"flap1"));
	reptile->AddImage(BitmapImage(L"Images\\flap2.png", L"flap2"));
	reptile->AddImage(BitmapImage(L"Images\\flap3.png", L"flap3"));
	reptile->AddImage(BitmapImage(L"Images\\flap4.png", L"flap4"));
	reptile->AddImage(BitmapImage(L"Images\\flap5.png", L"flap5"));
	reptile->AddImage(BitmapImage(L"Images\\dead.png", L"dead"));

	// create box objects
	boxes[0] = new Box(L"Images\\box.png", L"topBox", (int)(WINDOW_WIDTH * 0.4), (int)(WINDOW_HEIGHT * 0.53));
	boxes[1] = new Box(L"Images\\box.png", L"midLeftBox", (int)(WINDOW_WIDTH * 0.35), (int)(WINDOW_HEIGHT * 0.645));
	boxes[2] = new Box(L"Images\\box.png", L"midRightBox", (int)(WINDOW_WIDTH * 0.45), (int)(WINDOW_HEIGHT * 0.645));
	boxes[3] = new Box(L"Images\\box.png", L"botLeftBox", (int)(WINDOW_WIDTH * 0.3), (int)(WINDOW_HEIGHT * 0.76));
	boxes[4] = new Box(L"Images\\box.png", L"botCenterBox", (int)(WINDOW_WIDTH * 0.4), (int)(WINDOW_HEIGHT * 0.76));
	boxes[5] = new Box(L"Images\\box.png", L"botRightBox", (int)(WINDOW_WIDTH * 0.5), (int)(WINDOW_HEIGHT * 0.76));

	// create flash BitmapImage object displayed for a frame when the reptile is hit
	flash = new BitmapImage(L"Images\\flash.png", L"flash");

	// set interval for scoreboard to refresh
	Scoreboard::SECOND_LIMIT = 33;
}


/**
\brief		Shuts down GDI+ and unloads resources from memory.
\param[in]	hWnd The handle to the window.
*/
void UnloadResources(void) 
{
	// free game object pointers
	delete background;
	delete slingshot;
	delete reptile;
	for (int i = 0; i < 6; i++)
	{
		delete boxes[i];
	}
	delete flash;

	// stop gdi+
	GdiplusShutdown(gdiplusToken);	
}
