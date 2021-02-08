#pragma once

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL MyInitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(
		szWindowClass,				//class name
		szTitle,					//title
		WS_OVERLAPPEDWINDOW,		//style
		CW_USEDEFAULT,				//x
		0,							//y
		CW_USEDEFAULT,				//width
		0,							//height
		nullptr,					//parent handle
		nullptr,					//menu
		hInstance,					//instance
		nullptr);					//lpparam

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}