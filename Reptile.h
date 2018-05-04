#include "CompositeImage.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;


#ifndef __REPTILE_H__
#define __REPTILE_H__


/**
\class		Reptile
\author		Tom Bisch
\date		Mar 1, 2016
\brief		Represents and keeps track of a flying reptile object.
\details	Contains methods for checking for boundary collisions, updating the position on the screen, 
			getting and setting the state, getting the name of the current bitmap representing the reptile, 
			resetting the reptile to an initial starting state and getting and setting various attributes.
			Inherits from the CompositeImage class.
*/
class Reptile : public CompositeImage
{

private:
	int xPrevPos;						// X coordinate of previous reptile position
	int yPrevPos;						// Y coordinate of previous reptile position
	float xVel;							// the X velocity of the reptile
	float yVel;							// the Y velocity of the reptile	
	int state;							// reptiles current state
	int flyState;						// reptiles current flying state
	int resetTimeCount;					// counts up to RESET_TIME
	int flyTime;						// time until reptile changes flight direction
	int flyTimeCount;					// counts up to flyTime
	int imageIndex;						// the current image/frame of the reptile
	bool isHit;							// indicates if reptile has been hit this frame

public:
	static const float FRICTION;		// force of friction on reptile
	static const int RESET_TIME;		// time before reptile resets after hitting ground
	static const int STATE_FLYING;		// reptile is flying
	static const int STATE_FALLING;		// reptile has been hit and is now falling
	static const int STATE_GROUNDED;	// reptile has hit the ground
	static const int FLYSTATE_UP;		// reptile is flying up
	static const int FLYSTATE_DOWN;		// reptile is flying down
	static const int FLYTIME_MAX;		// fly interval max time
	static const int FLYTIME_MIN;		// fly interval min time

	Reptile(void);
	~Reptile(void);

	bool CheckCollision(int x, int y);
	bool Update(int windowWidth, int windowHeight);
	void SetState(int newState);
	int GetState(void);
	wstring GetNameOfImageToDraw(void);
	void Reptile::SetIsHit(bool isHit);
	bool GetIsHit(void);
	float GetXVel(void);
	float GetYVel(void);
	void SetXVel(float xVel);
	void SetYVel(float yVel);
	int GetXPrevPos(void);
	int GetYPrevPos(void);
	void SetXPrevPos(int xPrevPos);
	void SetYPrevPos(int yPrevPos);
	void Reset(int windowWidth, int windowHeight);

};


#endif