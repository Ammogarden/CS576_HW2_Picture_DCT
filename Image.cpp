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

MyEncodeImage::MyEncodeImage(int width, int height, int N, int delivery) {
	setWidth(width);
	setHeight(height);
	quantizationLevel = N;
	deliveryMode = delivery;
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

	//create buffer in heap used for output
	outRbuf = std::vector<std::vector<UINT8>>(Height, std::vector<UINT8>(Width));
	outGbuf = std::vector<std::vector<UINT8>>(Height, std::vector<UINT8>(Width));
	outBbuf = std::vector<std::vector<UINT8>>(Height, std::vector<UINT8>(Width));

	doubleRbuf = std::vector<std::vector<double>>(Height, std::vector<double>(Width));
	doubleGbuf = std::vector<std::vector<double>>(Height, std::vector<double>(Width));
	doubleBbuf = std::vector<std::vector<double>>(Height, std::vector<double>(Width));

	Data = new char[Width * Height * 3];
	setImageData(Data);

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
			std::vector<double> rDctCoefficient(64);
			std::vector<double> gDctCoefficient(64);
			std::vector<double> bDctCoefficient(64);

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

					rDctCoefficient[index] = rCoef;
					gDctCoefficient[index] = gCoef;
					bDctCoefficient[index] = bCoef;
				}
			}

			RDCT.back().push_back(rDctCoefficient);
			GDCT.back().push_back(gDctCoefficient);
			BDCT.back().push_back(bDctCoefficient);
		}
	}

	//quantize all coefficient
	//immediate resize back instead of doing it at decoder side for simplicity
	if (quantizationLevel > 0) {
		for (int i = 0; i < RDCT.size(); i++) {
			for (int j = 0; j < RDCT[0].size(); j++) {
				for (int k = 0; k < RDCT[0][0].size(); k++) {
					RDCT[i][j][k] = round(RDCT[i][j][k] / pow(2.0, quantizationLevel));
					RDCT[i][j][k] *= pow(2.0, quantizationLevel);
					GDCT[i][j][k] = round(GDCT[i][j][k] / pow(2.0, quantizationLevel));
					GDCT[i][j][k] *= pow(2.0, quantizationLevel);
					BDCT[i][j][k] = round(BDCT[i][j][k] / pow(2.0, quantizationLevel));
					BDCT[i][j][k] *= pow(2.0, quantizationLevel);
				}
			}
		}
	}
	

	// Clean up and return
	fclose(IN_FILE);

	return true;
}

bool    MyEncodeImage::decode() {
	int Width = getWidth();
	int Height = getHeight();
	char* Data = getImageData();

	if (deliveryMode == 1) {
		if (seqRow >= Height || seqCol >= Width) {
			return true;
		}
		//sequential mode, for each time decode one block only
		//process block from top-left corner [seqRow, seqCol] to [seqRow + 7, seqCol + 7]
		for (int x = seqRow; x < seqRow + 8; x++) {
			for (int y = seqCol; y < seqCol + 8; y++) {
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

				doubleRbuf[x][y] = rVal;
				doubleGbuf[x][y] = gVal;
				doubleBbuf[x][y] = bVal;

				rVal = min(rVal, 255);
				rVal = max(rVal, 0);
				gVal = min(gVal, 255);
				gVal = max(gVal, 0);
				bVal = min(bVal, 255);
				bVal = max(bVal, 0);
				outRbuf[x][y] = (UINT8)rVal;
				outGbuf[x][y] = (UINT8)gVal;
				outBbuf[x][y] = (UINT8)bVal;
			}
		}

		//advance seqRow and seqCol
		if (seqCol + 8 >= Width) {
			seqRow += 8;
			seqCol = 0;
		}
		else {
			seqCol += 8;
		}
	}
	else if (deliveryMode == 2) {
		//progressive mode, for each time decode the next DCT coefficient for all block
		//will only process coefficient within [0:index]
		if (progressIndex >= 64) {
			return true;
		}
		for (int x = 0; x < Height; x++) {
			for (int y = 0; y < Width; y++) {
				int r = x / 8;
				int c = y / 8;

				double rVal = doubleRbuf[x][y] * 4;
				double gVal = doubleGbuf[x][y] * 4;
				double bVal = doubleBbuf[x][y] * 4;
				//determine u and v based on progressIndex
				int u = zigZagOrder[progressIndex][0];
				int v = zigZagOrder[progressIndex][1];
				int index = u * 8 + v;
				rVal += C(u) * C(v) * RDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
				gVal += C(u) * C(v) * GDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);
				bVal += C(u) * C(v) * BDCT[r][c][index] * cos((2 * x + 1) * u * M_PI / 16.0) * cos((2 * y + 1) * v * M_PI / 16.0);

				rVal /= 4.0;
				gVal /= 4.0;
				bVal /= 4.0;

				doubleRbuf[x][y] = rVal;
				doubleGbuf[x][y] = gVal;
				doubleBbuf[x][y] = bVal;

				rVal = min(rVal, 255);
				rVal = max(rVal, 0);
				gVal = min(gVal, 255);
				gVal = max(gVal, 0);
				bVal = min(bVal, 255);
				bVal = max(bVal, 0);
				outRbuf[x][y] = (UINT8)rVal;
				outGbuf[x][y] = (UINT8)gVal;
				outBbuf[x][y] = (UINT8)bVal;
			}
		}
		progressIndex++;
	}
	else if (deliveryMode == 3) {
	
	
	}
	else {
		//unknown delivery mode, deliver all content at once
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
				outRbuf[x][y] = (UINT8)rVal;
				outGbuf[x][y] = (UINT8)gVal;
				outBbuf[x][y] = (UINT8)bVal;
			}
		}
	}
	
	// Allocate Data structure and copy
	
	for (int i = 0; i < Height * Width; i++)
	{
		Data[3 * i] = outBbuf[i / Width][i % Width];
		Data[3 * i + 1] = outGbuf[i / Width][i % Width];
		Data[3 * i + 2] = outRbuf[i / Width][i % Width];
	}
	setImageData(Data);



	switch (deliveryMode)
	{
	case 1:
		return seqRow >= Height;
		break;
	case 2:
		return progressIndex >= 64;
		break;
	case 3:
		break;
	default:
		break;
	}

	return true;
}
