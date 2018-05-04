#include "BitmapImage.h"
#include "Reptile.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;


#ifndef __BOX_H__
#define __BOX_H__


/**
\class		Box
\author		Tom Bisch
\date		Mar 24, 2016
\brief		Represents and keeps track of a box object.
\details	Contains methods for checking for boundary collisions with other objects, 
			updating the position of the box on screen, resetting the box to a starting position
			and updating the X and Y velocity of the box. Inherits from the BitmapImage class.
*/
class Box : public BitmapImage
{

private:
	int xStartPos;						// X coordinate of default box position
	int yStartPos;						// Y coordinate of default box position
	int xPrevPos;						// X coordinate of previous box position
	int yPrevPos;						// Y coordinate of previous box position
	float xVel;							// X velocity of the box
	float yVel;							// Y velocity of the box

public:
	static const float FRICTION;		// force of friction on the box

	Box(wstring bitmapPath, wstring bitmapName, int xStart, int yStart);
	~Box(void);

	bool CheckCollision(int windowWidth, int windowHeight, Box* b, bool sideways = false);
	bool CheckCollision(int windowWidth, int windowHeight, Reptile* r);
	void Update(int windowWidth, int windowHeight);
	void Reset(void);
	void SetXVel(float xVel);
	void SetYVel(float yVel);

};


#endif