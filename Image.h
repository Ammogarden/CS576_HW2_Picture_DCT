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
	MyEncodeImage(int width, int height, int N, int delivery);
	void setDeliveryMode(int delivery) { deliveryMode = delivery; }
	void setQuantizationLevel(int N) { quantizationLevel = N; }
	~MyEncodeImage() {};

	//Instead of reading the input image directly to data buffer, do DCT and store the coefficient
	bool	ReadImage();
	
	//decode data from DCT coefficient, behavior depends on delivery mode
	//return true if all data has finished decoding, false if there are still remaining data for decode
	bool    decode();

	void quantize();
	void dequantize();

private:
	//3-d vector, [row][col] stores the 64 DCT coefficient for the corresponding 8x8 block
	std::vector<std::vector<std::vector<double>>> RDCT;
	std::vector<std::vector<std::vector<double>>> GDCT;
	std::vector<std::vector<std::vector<double>>> BDCT;
	int quantizationLevel = 0;
	int deliveryMode = 1;

	bool isDequantized = false;
	
	//output buffer
	std::vector<std::vector<UINT8>> outRbuf;
	std::vector<std::vector<UINT8>> outGbuf;
	std::vector<std::vector<UINT8>> outBbuf;

	//value of output buffer before quantization
	std::vector<std::vector<double>> doubleRbuf;
	std::vector<std::vector<double>> doubleGbuf;
	std::vector<std::vector<double>> doubleBbuf;

	//index used by Sequential delivery mode 1:
	int seqRow = 0;
	int seqCol = 0;

	//index used by Progressive Spectral Selection delivery mode 2:
	int progressIndex = 0;
	const std::vector<std::vector<int>> zigZagOrder = {
		{ 0, 0 }, { 0, 1 }, { 1, 0 }, { 2, 0 }, { 1, 1 }, { 0, 2 }, { 0, 3 }, { 1, 2 },
		{ 2, 1 }, { 3, 0 }, { 4, 0 }, { 3, 1 }, { 2, 2 }, { 1, 3 }, { 0, 4 }, { 0, 5 },
		{ 1, 4 }, { 2, 3 }, { 3, 2 }, { 4, 1 }, { 5, 0 }, { 6, 0 }, { 5, 1 }, { 4, 2 },
		{ 3, 3 }, { 2, 4 }, { 1, 5 }, { 0, 6 }, { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 },
		{ 4, 3 }, { 5, 2 }, { 6, 1 }, { 7, 0 }, { 7, 1 }, { 6, 2 }, { 5, 3 }, { 4, 4 },
		{ 3, 5 }, { 2, 6 }, { 1, 7 }, { 2, 7 }, { 3, 6 }, { 4, 5 }, { 5, 4 }, { 6, 3 },
		{ 7, 2 }, { 7, 3 }, { 6, 4 }, { 5, 5 }, { 4, 6 }, { 3, 7 }, { 4, 7 }, { 5, 6 },
		{ 6, 5 }, { 7, 4 }, { 7, 5 }, { 6, 6 }, { 5, 7 }, { 6, 7 }, { 7, 6 }, { 7, 7 },
	};

	//index used by progressive successive bit delivery mode 3:
	UINT8 mask = 0;
	bool isDecoded = false;
	int order = 7;
};

#endif //IMAGE_DISPLAY
