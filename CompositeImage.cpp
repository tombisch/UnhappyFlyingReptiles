#include "CompositeImage.h"


/**
\brief		Constructs a CompositeImage object and sets default values for members.
*/
CompositeImage::CompositeImage(void) 
{
	xPos = 0;
	yPos = 0;
	width = 0;
	height = 0;
}


/**
\brief		Destructor for a CompositeImage. Currently does nothing.
*/
CompositeImage::~CompositeImage(void)
{
}


/**
\brief		Adds a BitmapImage object to the CompositeImage instance.
\details	The BitmapImage is added to the list of BitmapImages, whose bitmaps are all combined 
			and drawn to a window when the CompositeImage instance's Draw method is called.
\param[in]	bitmap The BitmapImage to add to the CompositeImage.
*/
void CompositeImage::AddImage(BitmapImage bitmap)
{
	bitmaps.push_back(bitmap);
	CalculateDimensions();
}


/**
\brief		Removes a BitmapImage object from the CompositeImage instance.
\details	The BitmapImage is removed from the list of BitmapImage objects and its bitmap will no longer be drawn.
\param[in]	bitmapName The name identifier of the BitmapImage to remove.
\return		Returns true if the BitmapImage was found, false otherwise.
*/
bool CompositeImage::RemoveImage(wstring bitmapName)
{
	bool isFound = false;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end() && isFound == false)
	{
		if (i->GetName() == bitmapName)
		{
			bitmaps.erase(i);
			isFound = true;
			CalculateDimensions();
		}
		i++;
	}
	return isFound;
}


/**
\brief		Draws the CompositeImage to the window using the passed Gdiplus::Graphics pointer.
\details	Loops through all the BitmapImage objects contained within the CompositeImage and draws
			their bitmaps to the window using the passed pointer to the graphics object.
\param[in]	g The pointer to the graphics object used to draw to the window.
\param[in]	ia The pointer to the image attributes object used to specify draw attributes.
*/
void CompositeImage::Draw(Gdiplus::Graphics* g, Gdiplus::ImageAttributes* ia)
{
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		i->Draw(g, ia);
		i++;
	}
}


/**
\brief		Draws a single bitmap of a CompositeImage to the window using the passed Gdiplus::Graphics pointer.
\details	Loops through all the BitmapImage objects contained within the CompositeImage 
			to find the single bitmap specified by the bitmapName parameter. If the bitmap
			is found, it is drawn to the window using the passed pointer to the graphics object.
\param[in]	g The pointer to the graphics object used to draw to the window.
\param[in]	bitmapName The name of the bitmapImage whose bitmap should be drawn to the window.
\param[in]	ia The pointer to the image attributes object used to specify draw attributes.
*/
bool CompositeImage::DrawSingle(Gdiplus::Graphics* g, wstring bitmapName, Gdiplus::ImageAttributes* ia)
{
	bool isFound = false;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end() && isFound == false)
	{
		if (i->GetName() == bitmapName)
		{
			i->Draw(g, ia);
			isFound = true;
		}
		i++;
	}
	return isFound;
}


/**
\brief		Resizes the all the BitmapImages contained within the CompositeImage to the specified width and height.
\details	If the bool scaleDimensions parameter is false, the passed width and height
			parameters will be each of the resized bitmap's width and height. If scaleDimentions
			is true, the passed width and height parameters are used as scalars for each of the
			original bitmap's dimensions to calculate the resized bitmap's width and height.
\param[in]	width The width of the resized bitmap.
\param[in]	height The height of the resized bitmap.
\param[in]	scaleDimensions Indicates if the width and height parameters are exact or scalar values.
*/
void CompositeImage::Resize(double width, double height, bool scaleDimensions)
{
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		i->Resize(width, height, scaleDimensions);
		i++;
	}
	CalculateDimensions();
}


/**
\brief		Rotates each BitmapImage contained within the CompositeImage the amount of degrees specified by the passed parameter.
\details	Uses the mod operator to truncate the parameter value because degrees range 
			from 0 - 360. Loops through each BitmapImage object and calls its Rotate method.
*/
void CompositeImage::Rotate(int degrees)
{
	// make degrees a positive value from 0 - 360 
	rotation = degrees % 360;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		i->Rotate(rotation);
		i++;
	}
}


/**
\brief		Sets the X and Y coordinates of where to draw all the bitmaps of the BitmapImages.
\param[in]	x The new X coordinate to draw the bitmaps from the BitmapImages.
\param[in]	y The new Y coordinate to draw the bitmaps from the BitmapImages.
*/
void CompositeImage::MoveTo(int x, int y)
{
	xPos = x;
	yPos = y;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		i->MoveTo(x, y);
		i++;
	}
}


/**
\brief		Counts and returns the number of BitmapImages within the CompositeImage instance.
\return		The number of BitmapImages contained in the CompositeImage.
*/
int CompositeImage::BitmapImageCount(void)
{
	return bitmaps.size();
}


/**
\brief		Returns the BitmapImage object specified by the bitmapName parameter value.
\details	Loops through the BitmapImages contained within the CompositeImage and checks if the 
			specified bitmapName belongs to any of its BitmapImages, if so, the BitmapImage is returned.
\param[in]  bitmapName The name identifier of the BitmapImage to return.
\return		Returns the BitmapImage if found, otherwise NULL is returned.
*/
BitmapImage* CompositeImage::GetBitmapImage(wstring bitmapName)
{
	BitmapImage* bitmapImage = NULL;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		if (i->GetName() == bitmapName)
		{
			bitmapImage = &i._Ptr->_Myval;
			break;
		}
		i++;
	}
	return bitmapImage;
}


/**
\brief		Returns the X coordinate of where to draw all the bitmaps of the BitmapImages.
\return		The X coordinate to draw the bitmaps from the BitmapImages.
*/
int CompositeImage::GetXPos(void)
{
	return xPos;
}


/**
\brief		Returns the Y coordinate of where to draw all the bitmaps of the BitmapImages.
\return		The Y coordinate to draw the bitmaps from the BitmapImages.
*/
int CompositeImage::GetYPos(void)
{
	return yPos;
}


/**
\brief		Returns the width of the widest bitmap from all the BitmapImages in the CompositeImage.
\return		The width of the widest bitmap contained.
*/
int CompositeImage::GetWidth(void)
{
	return width;
}


/**
\brief		Returns the height of the highest bitmap from all the BitmapImages in the CompositeImage.
\return		The height of the highest bitmap contained.
*/
int CompositeImage::GetHeight(void)
{
	return height;
}


/**
\brief		Returns the rotation of all the BitmapImages in the CompositeImage.
\return		The rotation of the CompositeImage.
*/
int CompositeImage::GetRotation(void)
{
	return rotation;
}


/**
\brief		Calculates the width and height of the CompositeImage.
\details	Determines the CompositeImage instance's width and height by finding which BitmapImage
			contains the widest and highest bitmaps.
*/
void CompositeImage::CalculateDimensions(void)
{
	int w = 0;
	int h = 0;
	std::list<BitmapImage>::iterator i = bitmaps.begin();
	while (i != bitmaps.end())
	{
		if (i->GetBitmap()->GetWidth() > (unsigned)w)
		{
			w = i->GetBitmap()->GetWidth();
		}
		if (i->GetBitmap()->GetHeight() > (unsigned)h)
		{
			h = i->GetBitmap()->GetHeight();
		}
		i++;
	}
	width = w;
	height = h;
}