#include "BitmapImage.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <list>
using namespace Gdiplus;
using namespace std;


#ifndef __COMPOSITE_IMAGE_H__
#define __COMPOSITE_IMAGE_H__


/**
\class		CompositeImage
\author		Tom Bisch
\date		Feb 2, 2016
\brief		A composite image object combines one or more bitmaps from BitmapImage objects.
\details	A CompositeImage instance can contain any number of BitmapImage objects that will be
			combined into a single image when drawn onto a device context. This class contains methods 
			to add and remove BitmapImage objects from a CompositeImage objects as well as methods that 
			allow all the bitmaps contained within to be repositioned and scaled. 
*/
class CompositeImage 
{

private:
	std::list<BitmapImage> bitmaps;		// the bitmaps combined to make a composite image
	int xPos;							// the X coordinate to draw all the bitmaps
	int yPos;							// the Y coordinate to draw all the bitmaps
	int width;							// the width of the widest bitmap
	int height;							// the height of the highest bitmap
	int rotation;						// the rotation at which to draw all the bitmaps

	void CalculateDimensions(void);

public:
	CompositeImage(void);
	~CompositeImage(void);

	void AddImage(BitmapImage bitmap);
	bool RemoveImage(wstring bitmapName);
	void Draw(Gdiplus::Graphics* g, Gdiplus::ImageAttributes* ia = NULL);
	bool DrawSingle(Gdiplus::Graphics* g, wstring bitmapName, Gdiplus::ImageAttributes* ia = NULL);
	void Resize(double width, double height, bool scaleDimensions = false);
	void Rotate(int degrees);
	void MoveTo(int x, int y);
	int BitmapImageCount(void);

	int GetXPos(void);
	int GetYPos(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetRotation(void);
	BitmapImage* GetBitmapImage(wstring bitmapName);

};


#endif