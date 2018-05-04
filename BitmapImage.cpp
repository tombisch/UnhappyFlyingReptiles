#include "BitmapImage.h"


/**
\brief		Constructs a BitmapImage object and sets default values for all members.
*/
BitmapImage::BitmapImage(void)
{
	original = nullptr;
	bitmap = nullptr;
	path = L"";
	name = L"";
	xPos = 0;
	yPos = 0;
	rotation = 0;
}


/**
\brief		Constructs a BitmapImage object and sets member values from parameters.
\param[in]	bitmapPath The filepath of the image to keep track of.
\param[in]	bitmapName The name of the image to keep track of.
*/
BitmapImage::BitmapImage(wstring bitmapPath, wstring bitmapName)
{
	original = new Gdiplus::Bitmap(bitmapPath.c_str());
	bitmap = new Gdiplus::Bitmap(bitmapPath.c_str());
	path = bitmapPath;
	name = bitmapName;
	xPos = 0;
	yPos = 0;
	rotation = 0;
}


/**
\brief		Destructor for a BitmapImage. Currently does nothing.
*/
BitmapImage::~BitmapImage(void)
{
}


/**
\brief		Draws the BitmapImage to the window using the passed Gdiplus::Graphics pointer.
\param[in]	g The pointer to the graphics object used to draw to the window.
\param[in]	ia The pointer to the image attributes object used to specify draw attributes.
*/
void BitmapImage::Draw(Gdiplus::Graphics* g, Gdiplus::ImageAttributes* ia)
{
	// check if image requires rotation
	if (rotation > 0)
	{
		// find origin point - center of the bitmap
		Gdiplus::PointF origin((Gdiplus::REAL)xPos + (GetWidth() / 2), (Gdiplus::REAL)yPos + (GetHeight() / 2));
		// matrix object used to rotate at bitmap origin point
		Gdiplus::Matrix matrix;
		// update matrix object as product of itself and matrix that represents rotation about origin point
		matrix.RotateAt((Gdiplus::REAL)rotation, origin);
		// update graphics object as product of itself and matrix object
		g->MultiplyTransform(&matrix);		
	}
	// draw image to window
	// check if image attributes are required
	if (ia == NULL)
	{
		g->DrawImage(bitmap, xPos, yPos);
	}
	else
	{
		g->DrawImage(bitmap, RectF(0, 0, (Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight()), 0, 0,
			(Gdiplus::REAL)GetWidth(), (Gdiplus::REAL)GetHeight(), UnitPixel, ia);
	}	
	// reset the graphics transformation
	g->ResetTransform();	
}


/**
\brief		Resizes the bitmap using the width and height parameter values.
\details	If the bool scaleDimensions parameter is false, the passed width and height
			parameters will be the resized bitmap's width and height. If scaleDimentions
			is true, the passed width and height parameters are used as scalars for the
			original bitmap dimensions to calculate the resized bitmap's width and height.
\param[in]	width Either the new width or the scaled width depending on scaleDimensions.
\param[in]	height Either the new height or the scaled height depending on scaleDimensions.
\param[in]	scaleDimensions Indicates if the width and height parameters are exact or scalar values.
*/
void BitmapImage::Resize(double width, double height, bool scaleDimensions)
{
	// default new dimentions as fixed pixel measurements
	int newWidth = (int)width;
	int newHeight = (int)height;
	// set new dimentions as scalars of the original image measurements
	if (scaleDimensions == true)
	{
		newWidth = (int)(original->GetWidth() * width);
		newHeight = (int)(original->GetHeight() * height);
	}
	// set the bounds for the resized image
	Gdiplus::Rect dest(0, 0, newWidth, newHeight);
	// set the temporary drawn bitmap to a new bitmap with the new width and height
	// also specify that the new bitmap supports an alpha channel (32-bit)
	delete bitmap;
	bitmap = new Gdiplus::Bitmap(newWidth, newHeight, PixelFormat32bppARGB);
	// create a graphics object that will draw onto the temporary bitmap
	Gdiplus::Graphics graphics(bitmap);
	// use the graphics object to draw the original bitmap onto the scaled temporary bitmap
	graphics.DrawImage(original, dest);
}


/**
\brief		Sets the rotation of the temporary bitmap to the passed parameter value.
\details	Uses the mod operator to truncate the parameter value because degrees range 
			from 0 - 360 and also accounts for negative degrees.
*/
void BitmapImage::Rotate(int degrees)
{
	// check for negative degrees
	if (degrees < 0)
	{
		// make degrees a positive value from 0 - 360,
		// subtract the result from 360 to get the inverse degrees
		rotation = 360 - ((degrees *= -1) % 360);
	}
	else
	{
		// make degrees a positive value from 0 - 360 
		rotation = degrees % 360;
	}
}


/**
\brief		Translates the origin point of temporary bitmap to the passed X and Y coordinates.
*/
void BitmapImage::MoveTo(int x, int y)
{
	xPos = x;
	yPos = y;
}


/**
\brief		Removes the color indicated by the color parameter from the BitmapImage.
\param[in]	color The color of the pixels to be set as fully transparent.
*/
void BitmapImage::RemoveChromaKey(Gdiplus::Color color)
{
	Gdiplus::Color pixelColor;
	Gdiplus::Color transparent(0, 255, 255, 255);
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int colorR = color.GetR();
	int colorG = color.GetG();
	int colorB = color.GetB();
	// check each pixel in the bitmap and if its color matches the passed parameter's color,
	// set the pixel alpha channel to 0 (fully transparent)
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			bitmap->GetPixel(x, y, &pixelColor);
			// compare the pixel's rgb values to the parameter color's rgb values
			if (pixelColor.GetR() == colorR && pixelColor.GetG() == colorG && pixelColor.GetB() == colorB)
			{
				bitmap->SetPixel(x, y, transparent);
			}
		}
	}
}


/**
\brief		Saves the temporary bitmap drawn to the window as the original bitmap.
\details	Copies and sets the temporary bitmap drawn to the current window size as 
			the original bitmap that is used to maintain image quality.
*/
void BitmapImage::SaveBitmapAsOriginal(void)
{
	Gdiplus::Rect dest(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	delete original;
	original = new Gdiplus::Bitmap(bitmap->GetWidth(), bitmap->GetHeight(), PixelFormat32bppARGB);
	Gdiplus::Graphics graphics(original);
	graphics.DrawImage(bitmap, dest);
}


/**
\brief		Sets the path of the BitmapImage object.
\param[in]	bitmapPath The new filepath of the image to keep track of.
*/
void BitmapImage::SetPath(wstring bitmapPath)
{
	path = bitmapPath;
	delete original;
	delete bitmap;
	original = new Gdiplus::Bitmap(bitmapPath.c_str());
	bitmap = new Gdiplus::Bitmap(bitmapPath.c_str());
}


/**
\brief		Sets the name of the BitmapImage object.
\param[in]	bitmapName The new name of the BitmapImage.
*/
void BitmapImage::SetName(wstring bitmapName)
{
	name = bitmapName;
}


/**
\brief		Returns the filepath of the BitmapImage object.
\return		The filepath of the BitmapImage.
*/
wstring BitmapImage::GetPath(void)
{
	return path;
}


/**
\brief		Returns the name identifier of the BitmapImage object.
\return		The name of the BitmapImage.
*/
wstring BitmapImage::GetName(void)
{
	return name;
}


/**
\brief		Returns the X coordinate of where to draw the BitmapImage object.
\return		The X coordinate to draw the BitmapImage.
*/
int BitmapImage::GetXPos(void)
{
	return xPos;
}


/**
\brief		Returns the Y coordinate of where to draw the BitmapImage object.
\return		The Y coordinate to draw the BitmapImage.
*/
int BitmapImage::GetYPos(void)
{
	return yPos;
}


/**
\brief		Returns the width of the BitmapImage object.
\return		The width of the BitmapImage.
*/
int BitmapImage::GetWidth(void)
{
	return bitmap->GetWidth();
}


/**
\brief		Returns the height of the BitmapImage object.
\return		The height of the BitmapImage.
*/
int BitmapImage::GetHeight(void)
{
	return bitmap->GetHeight();
}


/**
\brief		Returns the rotation of the BitmapImage object.
\return		The rotation of the BitmapImage.
*/
int BitmapImage::GetRotation(void)
{
	return rotation;
}


/**
\brief		Returns a pointer to the Gdiplus::Bitmap object.
\return		A pointer to the Gdiplus::Bitmap object.
*/
Gdiplus::Bitmap* BitmapImage::GetBitmap(void)
{
	return bitmap;
}