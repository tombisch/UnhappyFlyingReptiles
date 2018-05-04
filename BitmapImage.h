#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;


#ifndef __BITMAP_IMAGE_H__
#define __BITMAP_IMAGE_H__


/**
\class		BitmapImage
\author		Tom Bisch
\date		Feb 2, 2016
\brief		A bitmap image object that keeps track of and can modify a bitmap image from a file.
\details	A BitmapImage instance is essentially an image object that can be scaled, positioned 
			and modified without changing the original image file located on the disk or sacrificing 
			the original image quality. A BitmapImage is also constructed with a name to distinguish
			it from other BitmapImages that may be stored in a data stucture or CompositeImage.
*/
class BitmapImage
{

private:
	Gdiplus::Bitmap* original;	// used to maintain original image quality
	Gdiplus::Bitmap* bitmap;	// the resized/modified bitmap drawn to the window
	wstring name;				// the name of the bitmap image
	wstring path;				// the path to the bitmap's file location
	int xPos;					// the X coordinate to draw the bitmap
	int yPos;					// the Y coordinate to draw the bitmap
	int rotation;				// the rotation of the bitmap to draw

public:
	BitmapImage(void);
	BitmapImage(wstring bitmapPath, wstring bitmapName);
	~BitmapImage(void);

	void Draw(Gdiplus::Graphics* g, Gdiplus::ImageAttributes* ia = NULL);
	void Resize(double width, double height, bool scaleDimensions = false);
	void Rotate(int degrees);
	void MoveTo(int x, int y);
	void RemoveChromaKey(Gdiplus::Color color);
	void SaveBitmapAsOriginal(void);
	
	void SetPath(wstring bitmapPath);
	void SetName(wstring bitmapName);
	wstring GetPath(void);
	wstring GetName(void);
	int GetXPos(void);
	int GetYPos(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetRotation(void);
	Gdiplus::Bitmap* GetBitmap(void);

};


#endif