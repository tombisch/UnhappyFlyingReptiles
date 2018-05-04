#include "Reptile.h"
#include "Scoreboard.h"


// class constants
const float Reptile::FRICTION = 0.05f;
const int Reptile::RESET_TIME = 50;
const int Reptile::STATE_FLYING = 0;
const int Reptile::STATE_FALLING = 1;
const int Reptile::STATE_GROUNDED = 2;
const int Reptile::FLYSTATE_UP = 0;
const int Reptile::FLYSTATE_DOWN = 1;
const int Reptile::FLYTIME_MAX = 40;
const int Reptile::FLYTIME_MIN = 20;


/**
\brief		Constructs a Reptile object and sets default values for members.
*/
Reptile::Reptile(void) : CompositeImage()
{
	xVel = 8;
	yVel = 0;
	state = STATE_FLYING;
	resetTimeCount = 0;
	flyTime = rand() % (FLYTIME_MAX - FLYTIME_MIN + 1) + FLYTIME_MIN;
	flyTimeCount = 0;
	imageIndex = 0;
	isHit = false;
}


/**
\brief		Destructor for a Reptile. Currently does nothing.
*/
Reptile::~Reptile(void)
{
}


/**
\brief		Checks for a boundary collision with the Reptile object.
\details	The point created from the passed X and Y coordinates is compared to the 
			boundaries of the Reptile bitmap to determine if the point is in contact 
			with the Reptile object.
\param[in]	x The X coordinate combined with the Y coordinate to form a point.
\param[in]	y The Y coordinate combined with the X coordinate to form a point.
*/
bool Reptile::CheckCollision(int x, int y)
{	
	if (x > GetXPos() &&
		x < (GetXPos() + GetWidth()) &&
		y > GetYPos() &&
		y < (GetYPos() + GetHeight()))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/**
\brief		Sets the state member of the Reptile object to the passed parameter value.
\param[in]	newState The new state of the Reptile object.
*/
void Reptile::SetState(int newState)
{
	state = newState;
}


/**
\brief		Returns the state member of the Reptile object.
\return		int - The current state of the Reptile.
*/
int Reptile::GetState(void)
{
	return state;
}


/**
\brief		Returns the name of the bitmap to draw for the reptile.
\return		The name of the BitmapImage whose bitmap should be drawn to represent the Reptile.
*/
wstring Reptile::GetNameOfImageToDraw(void)
{
	wstring imageName;
	switch (imageIndex)
	{
		case 0: imageName = L"flap0"; break;
		case 1: imageName = L"flap1"; break;
		case 2: imageName = L"flap2"; break;
		case 3: imageName = L"flap3"; break;
		case 4: imageName = L"flap4"; break;
		case 5: imageName = L"flap5"; break;
		case 6: imageName = L"flap4"; break;
		case 7: imageName = L"flap3"; break;
		case 8: imageName = L"flap2"; break;
		case 9: imageName = L"flap1"; break;
		case 10: imageName = L"dead"; break;
		default: imageName = L"flap0"; break;
	}
	return imageName;
}


/**
\brief		Updates the position of the Reptile object for the next frame.
\details	Checks the state of the Reptile object and updates the position of the Reptile
			accordingly:
			- In flying mode, the Reptile flys in pseudo-random zigzag pattern across the top portion of the screen
			- In falling mode, the Reptile falls rotating 5 degrees per frame downwards
			- In grounded mode, the Reptile rolls before slowing to a stop
\param[in]	windowWidth The width of the window to draw the reptile on.
\param[in]	windowHeight The height of the window to draw the reptile on.
\return		bool - Indicates if the reptile was reset.
*/
bool Reptile::Update(int windowWidth, int windowHeight)
{
	bool resetReptile = false;

	// set previous position coordinates
	xPrevPos = GetXPos();
	yPrevPos = GetYPos();

	// reptile flying
	if (state == STATE_FLYING)
	{
		// increment reptile flap image\frame
		imageIndex++;

		// reset to first image\frame of sequence
		if (imageIndex >= 10)
		{
			imageIndex = 0;
		}

		// fly up
		if (flyState == FLYSTATE_UP)
		{
			// calculate random vertical velocity between 1 - 5,
			// then take the negative of that number
			yVel = (float)-(rand() % 5 + 1);
			// check if going to high
			if (GetYPos() < 0)
			{
				flyState = !flyState;
			}
		}
		// fly down
		else
		{
			// calculate random vertical velocity between 1 - 5
			yVel = (float)(rand() % 5 + 1);
			// check if going to low
			if (GetYPos() > windowHeight * 0.35)
			{
				flyState = !flyState;
			}
		}

		// increment flyTimeCount
		flyTimeCount++;

		// update position of Reptile bitmap
		MoveTo(GetXPos() + (int)xVel, GetYPos() + (int)yVel);

		// change direction reptile is flying if flyTimeCount hits flyTime
		if (flyTimeCount >= flyTime)
		{
			// calculate random time to fly in a direction
			flyTime = rand() % (FLYTIME_MAX - FLYTIME_MIN + 1) + FLYTIME_MIN;
			flyTimeCount = 0;

			// calculate random horizontal velocity between 5 - 8
			xVel = (float)(rand() % 4 + 5);

			// randomly invert X and Y velocity
 			if (rand() % 2 == 0)
			{
				// invert horizontal velocity
				xVel *= -1;
			}
			if (rand() % 2 == 0)
			{
				// invert vertical velocity
				flyState = !flyState;
			}		
		}
	}

	// reptile falling
	else if (state == STATE_FALLING)
	{
		// set reptile image/frame to dead image
		imageIndex = 10;

		// update X velocity and rotation
		if (xVel > 0)
		{
			if (xVel - FRICTION > 0)
			{
				xVel -= FRICTION;
			}
			Rotate(GetRotation() + 5);
		}
		else
		{
			if (xVel + FRICTION < 0)
			{
				xVel += FRICTION;
			}
			Rotate(GetRotation() - 5);
		}

		// update Y velocity
		yVel = 5;

		// update position of Reptile bitmap
		MoveTo(GetXPos() + (int)xVel, GetYPos() + (int)yVel);

		// check if reptile hit ground
		if (GetYPos() > (int)(windowHeight * 0.82))
		{
			state = STATE_GROUNDED;
		}
	}

	// reptile grounded
	else if (state = STATE_GROUNDED)
	{
		// update X velocity and rotation
		if (xVel > 0)
		{
			if (xVel - FRICTION > 0)
			{
				xVel -= FRICTION;
			}		
			Rotate(GetRotation() + (int)xVel);			
		}
		else
		{
			if (xVel + FRICTION < 0)
			{
				xVel += FRICTION;
			}
			Rotate(GetRotation() + (int)xVel);
		}

		// update position of Reptile bitmap
		MoveTo(GetXPos() + (int)xVel, GetYPos() + (int)yVel);

		// increment timeCount
		resetTimeCount++;

		// reset Reptile object back to flying mode
		if (resetTimeCount >= RESET_TIME)
		{			
			Reset(windowWidth, windowHeight);
			resetReptile = true;
		}
	}

	// check if reptile goes beyond the window
	if (GetXPos() + GetWidth() > windowWidth)
	{
		// invert horizontal velocity to bounce of wall
		xVel *= -1;
	}
	if (GetXPos() < 0)
	{
		// invert horizontal velocity to bounce of wall
		xVel *= -1;
	}
	if (GetYPos() > (int)(windowHeight * 0.82))
	{
		// restrict the minimum reptile vertical position to the ground
		MoveTo(GetXPos(), (int)(windowHeight * 0.82));
	}

	return resetReptile;
}


/**
\brief		Sets the isHit member of the Reptile object to the passed parameter value.
\param[in]	isHit Indicates if the reptile has just been hit.
*/
void Reptile::SetIsHit(bool isHit)
{
	Reptile::isHit = isHit;
}


/**
\brief		Returns the isHit member of the Reptile object.
\return		A value indicating if the reptile has just been hit.
*/
bool Reptile::GetIsHit(void)
{
	return isHit;
}


/**
\brief		Returns the xVel member of the Reptile object.
\return		The horizontal velocity of the reptile.
*/
float Reptile::GetXVel(void)
{
	return xVel;
}


/**
\brief		Returns the yVel member of the Reptile object.
\return		The vertical velocity of the reptile.
*/
float Reptile::GetYVel(void)
{
	return yVel;
}


/**
\brief		Sets the xVel member of the Reptile object to the passed parameter value.
\param[in]	xVel The new horizontal velocity of the reptile.
*/
void Reptile::SetXVel(float xVel)
{
	Reptile::xVel = xVel;
}


/**
\brief		Sets the yVel member of the Reptile object to the passed parameter value.
\param[in]	yVel The new vertical velocity of the reptile.
*/
void Reptile::SetYVel(float yVel)
{
	Reptile::yVel = yVel;
}


/**
\brief		Returns the xPrevPos member of the Reptile object.
\return		The previous X position coordinate of the reptile.
*/
int Reptile::GetXPrevPos(void)
{
	return xPrevPos;
}


/**
\brief		Returns the yPrevPos member of the Reptile object.
\return		The previous Y position coordinate of the reptile.
*/
int Reptile::GetYPrevPos(void)
{
	return yPrevPos;
}


/**
\brief		Sets the xPrevPos member of the Reptile object to the passed parameter value.
\param[in]	xPrevPos The new previous X position coordinate of the reptile.
*/
void Reptile::SetXPrevPos(int xPrevPos)
{
	Reptile::xPrevPos = xPrevPos;
}


/**
\brief		Sets the yPrevPos member of the Reptile object to the passed parameter value.
\param[in]	yPrevPos The new previous Y position coordinate of the reptile.
*/
void Reptile::SetYPrevPos(int yPrevPos)
{
	Reptile::yPrevPos = yPrevPos;
}


/**
\brief		Resets the position of the Reptile object for the next round.
\param[in]	windowWidth The width of the window to draw the reptile on.
\param[in]	windowHeight The height of the window to draw the reptile on.
*/
void Reptile::Reset(int windowWidth, int windowHeight)
{
	// update scoreboard for next round
	Scoreboard::StartNextRound();
	// calculate random X velocity
	xVel = (float)(rand() % 4 + 5);
	if (rand() % 2 == 0)
	{
		// invert horizontal velocity
		xVel *= -1;
	}
	yVel = 0;
	state = STATE_FLYING;
	resetTimeCount = 0;
	Rotate(0);
	// start reptile at random X postion at top of window
	MoveTo((int)(windowWidth * (rand() % 9) * 0.1), 0);
	imageIndex = 0;
	isHit = false;
}