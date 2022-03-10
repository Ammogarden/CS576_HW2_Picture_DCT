//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>
#include <vector>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage;						// image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

MyImage		outImage;						// image object for output
int QUANTIZATION_LEVEL = 0;						// Quantization level
int DELIVERY_MODE = 0;							// Delivery mode
int LATENCY = 0;								// Latency

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	//Different way to parse the arguments from single string into vector of strings
	std::vector<std::string> argv;
	int itr = 0;
	std::string curr = "";
	while (lpCmdLine[itr] != '\0') {
		printf("%c ", lpCmdLine[itr]);
		if (lpCmdLine[itr] != ' ') {
			curr.push_back((char)lpCmdLine[itr]);
		}
		if ((lpCmdLine[itr] == ' ' || lpCmdLine[itr + 1] == '\0') && curr.length() > 0) {
			argv.push_back(curr);
			curr = "";
		}
		itr++;
	}

	for (int i = 0; i < argv.size(); i++) {
		printf("The %d-th parameter was: %s\n", i, argv[i].c_str());
	}

	// Verify parameters
	if (argv.size() < 4) {
		wprintf(L"Insufficient command line arguement. Usage: Image.exe {file path} {quantizationLevel} {DeliveryMode} {Latency}");
		return -1;
	}

	QUANTIZATION_LEVEL = stoi(argv[1]);
	DELIVERY_MODE = stoi(argv[2]);
	LATENCY = stoi(argv[3]);
	
	if (QUANTIZATION_LEVEL < 0 || QUANTIZATION_LEVEL > 7) {
		wprintf(L"{quantizationLevel}: quantizationLevel has range 0 ~ 7");
		return -1;
	}
	if (DELIVERY_MODE <= 0 || DELIVERY_MODE > 3) {
		wprintf(L"{DeliveryMode}: DeliveryMode has to be 1, 2, or 3");
		return -1;
	}
	if (LATENCY < 0) {
		wprintf(L"{Latency}: Latency has to greater or equal to 0");
		return -1;
	}



	// Set up the images
	int w = 352;
	int h = 288;
	inImage.setWidth(w);
	inImage.setHeight(h);

	inImage.setImagePath(argv[0].c_str());
	inImage.ReadImage();

	outImage.setWidth(w);
	outImage.setHeight(h);

	outImage.setImagePath(argv[0].c_str());
	outImage.ReadImage();
	/*
	outImage = MyEncodeImage(w, h, QUANTIZATION_LEVEL);
	outImage.setImagePath(lpCmdLine);
	outImage.ReadImage();
	*/


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);

				//print input image
				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = inImage.getWidth();
				bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetDIBitsToDevice(hdc,
								  0,100,inImage.getWidth(),inImage.getHeight(),
								  0,0,0,inImage.getHeight(),
								  inImage.getImageData(),&bmi,DIB_RGB_COLORS);
				
				
				//paint output image
				BITMAPINFO bmiOut;
				CBitmap bitmapOut;
				memset(&bmiOut, 0, sizeof(bmiOut));
				bmiOut.bmiHeader.biSize = sizeof(bmiOut.bmiHeader);
				bmiOut.bmiHeader.biWidth = outImage.getWidth();
				bmiOut.bmiHeader.biHeight = -outImage.getHeight();  // Use negative height.  DIB is top-down.
				bmiOut.bmiHeader.biPlanes = 1;
				bmiOut.bmiHeader.biBitCount = 24;
				bmiOut.bmiHeader.biCompression = BI_RGB;
				bmiOut.bmiHeader.biSizeImage = outImage.getWidth() * outImage.getHeight();

				SetDIBitsToDevice(hdc,
					700, 100, outImage.getWidth(), outImage.getHeight(),
					0, 0, 0, outImage.getHeight(),
					outImage.getImageData(), &bmiOut, DIB_RGB_COLORS);
				

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


