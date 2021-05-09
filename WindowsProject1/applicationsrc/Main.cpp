// WindowsProject1.cpp : Defines the entry point for the application.
// https://docs.microsoft.com/en-us/windows/win32/audio-and-video
// https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
// John Storm 1/27/2021
// Testing Windows Application
// Learning windows/GUI-based programming

#include "../application/framework.h"
#include "../resource.h"
#include "../application/Constants.h"
#include "../application/Declarations.h"
#include "../application/RegisterWindow.h"
#include "../application/Application.h"
#include "../application/UI.h"
#include "../immdevicesapi/IMMDevicesAPI.h"
#include "../application/WinProc.h"

// Main Entry
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow){
	// Unused
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);

	// Register CLASS
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!MyInitInstance(hInstance, nCmdShow)){
		return FALSE;
	}

	// Load Accelerator table
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// Enter Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Return on Exit
	return (int)msg.wParam;
}