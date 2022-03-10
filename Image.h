//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <math.h>
#include <iostream>

const double M_PI = 3.14159265358979323846; /* pi */

// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const char *img ) { Data = (char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }

	// Input Output operations
	virtual bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

};

class MyEncodeImage : public MyImage
{
public:
	MyEncodeImage() {};
	MyEncodeImage(int width, int height, int N);
	~MyEncodeImage() {};

	bool	ReadImage();

private:
	//3-d vector, [row][col] stores the 64 DCT coefficient for the corresponding 8x8 block
	std::vector<std::vector<std::vector<double>>> RDCT;
	std::vector<std::vector<std::vector<double>>> GDCT;
	std::vector<std::vector<std::vector<double>>> BDCT;
	int quantizationLevel = 0;
};

#endif //IMAGE_DISPLAY
