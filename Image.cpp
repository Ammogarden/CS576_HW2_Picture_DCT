//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"


// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student
	
	// sample operation
	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;

	}

	return false;
}

MyEncodeImage::MyEncodeImage(int width, int height, int N) {
	setWidth(width);
	setHeight(height);
	quantizationLevel = N;
}


//helper function used for DCT & IDCT
double inline C(int a) {
	if (a == 0) {
		return 1.0 / sqrt(2);
	}
	return 1;
}
// MyEncodeImage::ReadImage
// Function to read the image given a path, and encode it using DCT
bool MyEncodeImage::ReadImage()
{
	char* ImagePath = getImagePath();
	int Width = getWidth();
	int Height = getHeight();
	char* Data = getImageData();

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	std::vector<std::vector<UINT8>> Rbuf(Height, std::vector<UINT8>(Width));
	std::vector<std::vector<UINT8>> Gbuf(Height, std::vector<UINT8>(Width));
	std::vector<std::vector<UINT8>> Bbuf(Height, std::vector<UINT8>(Width));

	for (int i = 0; i < Width*Height; i ++)
	{
		Rbuf[i / Width][i % Width] = fgetc(IN_FILE);
	}
	for (int i = 0; i < Width*Height; i ++)
	{
		Gbuf[i / Width][i % Width] = fgetc(IN_FILE);
	}
	for (int i = 0; i < Width*Height; i ++)
	{
		Bbuf[i / Width][i % Width] = fgetc(IN_FILE);
	}

	//DO DCT for each 8x8 block and store coefficient
	for (int i = 0; i + 8 <= Height; i += 8) {
		RDCT.push_back(std::vector<std::vector<double>>());
		GDCT.push_back(std::vector<std::vector<double>>());
		BDCT.push_back(std::vector<std::vector<double>>());
		for (int j = 0; j + 8 <= Width; j += 8) {
			//the 8x8 block is from top left corner [i, j] to bottom right corner [i + 7, j + 7]
			std::vector<double> rDctCoeficient(64);
			std::vector<double> gDctCoeficient(64);
			std::vector<double> bDctCoeficient(64);

			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					int index = u * 8 + v;
					double rCoef = 0;
					double gCoef = 0;
					double bCoef = 0;
					for (int r = 0; r < 8; r++) {
						for (int c = 0; c < 8; c++) {
							int x = i + r;
							int y = j + c;
							rCoef += Rbuf[x][y] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
							gCoef += Gbuf[x][y] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
							bCoef += Bbuf[x][y] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
						}
					}
					rCoef *= C(u) * C(v) / 4.0;
					gCoef *= C(u) * C(v) / 4.0;
					bCoef *= C(u) * C(v) / 4.0;

					rDctCoeficient[index] = rCoef;
					gDctCoeficient[index] = gCoef;
					bDctCoeficient[index] = bCoef;
				}
			}

			RDCT.back().push_back(rDctCoeficient);
			GDCT.back().push_back(gDctCoeficient);
			BDCT.back().push_back(bDctCoeficient);
		}
	}
	//TODO: for test only: do IDCT to reconstruct data
	std::fill(Rbuf.begin(), Rbuf.end(), std::vector<UINT8>(Width, 0));
	std::fill(Gbuf.begin(), Gbuf.end(), std::vector<UINT8>(Width, 0));
	std::fill(Bbuf.begin(), Bbuf.end(), std::vector<UINT8>(Width, 0));

	for (int x = 0; x < Height; x++) {
		for (int y = 0; y < Width; y++) {
			int r = x / 8;
			int c = y / 8;

			double rVal = 0;
			double gVal = 0;
			double bVal = 0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					int index = u * 8 + v;
					rVal += C(u) * C(v) * RDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
					gVal += C(u) * C(v) * GDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
					bVal += C(u) * C(v) * BDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
				}
			}

			rVal /= 4.0;
			gVal /= 4.0;
			bVal /= 4.0;
			Rbuf[x][y] = (UINT8)rVal;
			Gbuf[x][y] = (UINT8)gVal;
			Bbuf[x][y] = (UINT8)bVal;
		}
	}



	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (int i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i / Width][i % Width];
		Data[3*i+1]	= Gbuf[i / Width][i % Width];
		Data[3*i+2]	= Rbuf[i / Width][i % Width];
	}
	setImageData(Data);

	// Clean up and return
	fclose(IN_FILE);

	return true;
}
