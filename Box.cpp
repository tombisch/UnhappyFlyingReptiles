#include "Box.h"
#include "Scoreboard.h"


// class constants
const float Box::FRICTION = 0.07f;


/**
\brief		Constructs a Box object and sets default values for members.
*/
Box::Box(wstring bitmapPath, wstring bitmapName, int xStart, int yStart) : BitmapImage(bitmapPath, bitmapName)
{
	MoveTo(xStart, yStart); 
	xStartPos = xStart;
	yStartPos = yStart;
	xPrevPos = xStartPos;
	xPrevPos = xStartPos;
	xVel = 0;	
	yVel = 0;		
}


/**
\brief		Destructor for a Box. Currently does nothing.
*/
Box::~Box(void)
{
}


/**
\brief		Checks and accounts for a collision with the passed reptile object.
\details	If a collision is detected, the reptile will transfer its X velocity
			to the box that it collided with. If the reptile hit the top of the box,
			only half of its X velocity is transferred.
\param[in]	windowWidth The width of the window to draw the reptile on.
\param[in]	windowHeight The height of the window to draw the reptile on.
\param[in]  r Reference to the reptile object to check for a collision against.
\return		bool - indicates if there is a collision with the passed reptile
*/
bool Box::CheckCollision(int windowWidth, int windowHeight, Reptile* r)
{
	// assume there is a collision
	bool repCollision = true; 

	// check for collision
	// compare the edges of the reptile object to the box object,
	// look at the 4 situations where the reptile does not come into contact with the box,
	// to left, to right, above or below,
	// if any of them are true, the reptile is not colliding with the box
	if (r->GetXPos() > GetXPos() + GetWidth() ||
		r->GetXPos() + r->GetWidth() < GetXPos() ||
		r->GetYPos() > GetYPos() + GetHeight() ||
		r->GetYPos() + r->GetHeight() < GetYPos())
	{
		repCollision = false;
	}

	// if colliding, adjust velocity of box and reptile accordingly
	if (repCollision == true)
	{
		// reptile on top
		if (r->GetYPrevPos() + r->GetHeight() < GetYPos())
		{
			xVel = r->GetXVel() / 2; // take reptiles velocity divided by 2
			r->SetYVel(0); // set reptile vertical velocity to zero
			r->SetState(r->STATE_GROUNDED);	// set reptiles state to grounded, even though it can still fall		
		}
		// reptile on left or right
		else if (r->GetXPrevPos() + r->GetWidth() < GetXPos() || r->GetXPrevPos() > GetXPos() + GetWidth())
		{
			xVel = r->GetXVel(); // switch velocities
			r->SetXVel(0);	// set reptile horizontal velocity to zero
		}		
	}

	return repCollision;
}


/**
\brief		Checks and accounts for a collision with the passed box object.
\details	If a collision is detected, the box will transfer its X velocity
			to the box that it collided with. The sideways bool parameter indicates 
			if the boxes are known to be beside each other to differentiate between
			boxes resting on top of each other from boxes actually colliding. This way,
			velocity will not be transferred if a box is resting on top of another box.
\param[in]	windowWidth The width of the window to draw the reptile on.
\param[in]	windowHeight The height of the window to draw the reptile on.
\param[in]  b Reference to the box object to check for a collision against.
\param[in]  sideways Indicates if the boxes should have horizontal velocity transferred between them.
\return		bool - indicates if there is a collision with the passed box
*/
bool Box::CheckCollision(int windowWidth, int windowHeight, Box* b, bool sideways)
{
	bool boxCollision = true;

	// first check if collision
	if (b->GetXPos() > GetXPos() + GetWidth() ||
		b->GetXPos() + b->GetWidth() < GetXPos() ||
		b->GetYPos() > GetYPos() + GetHeight() || 
		b->GetYPos() + b->GetHeight() < GetYPos())
	{
		boxCollision = false;
	}

	// if colliding, 
	// set horizontal velocity of resting box to that of moving box,
	// set moving box velocity to zero
	// move the previously moving box to previous position to avoid collision next frame
	if (boxCollision == true && sideways == true)
	{
		// calling box instance is moving and passed box instance is resting
		if (xVel != 0 && b->xVel == 0)
		{
			b->xVel = xVel;
			xVel = 0;
			MoveTo(xPrevPos, yPrevPos);
		}
		// calling box instance is resting and passed box instance is moving
		else if (b->xVel != 0 && xVel == 0)
		{
			xVel = b->xVel;
			b->xVel = 0;
			b->MoveTo(b->xPrevPos, b->yPrevPos);
		}
	}

	return boxCollision;
}


/**
\brief		Updates the position of a box based on its horizontal and vertical velocity.
\details	Subtracts friction to calculate new velocity of box and then moves the box
			to the new position determined by the current position added to the current 
			velocity. Checks box position in regards to window to prevent box from going
			out of bounds.
\param[in]	windowWidth The width of the window to draw the reptile on.
\param[in]	windowHeight The height of the window to draw the reptile on.
*/
void Box::Update(int windowWidth, int windowHeight)
{
	// set previous position coordinates
	xPrevPos = GetXPos();
	yPrevPos = GetYPos();

	// add or subtract friction from horizontal velocity depending on direction
	if (xVel > 0)
	{
		xVel -= FRICTION;
	}
	else if (xVel < 0)
	{
		xVel += FRICTION;
	}

	// move box to new position
	MoveTo(GetXPos() + (int)xVel, GetYPos() + (int)yVel);

	// check if box went outside window bounds
	if (GetXPos() < 0)
	{
		// too far left, inverse horizontal velocity
		xVel *= -1;
	}
	if (GetXPos() + GetWidth() > windowWidth)
	{
		// too far right, inverse horizontal velocity
		xVel *= -1;
	}	
	if (GetYPos() > (int)(windowHeight * 0.84))
	{
		// too far down, restrict Y position to that of ground
		MoveTo(GetXPos(), (int)(windowHeight * 0.84));
	}
}


/**
\brief		Resets the box to be resting at its starting position. 
*/
void Box::Reset(void)
{
	MoveTo(xStartPos, yStartPos);
	xPrevPos = xStartPos;
	xPrevPos = xStartPos;
	xVel = 0;
	yVel = 0;
}


/**
\brief		Sets the horizontal velocity of the box object.
\param[in]	xVel The new horizontal velocity of the box.
*/
void Box::SetXVel(float xVel)
{
	Box::xVel = xVel;
}


/**
\brief		Sets the vertical velocity of the box object.
\param[in]	xVel The new vertical velocity of the box.
*/
void Box::SetYVel(float yVel)
{
	Box::yVel = yVel;
}


