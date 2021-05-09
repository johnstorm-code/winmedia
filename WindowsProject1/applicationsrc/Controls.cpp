#include "../application/Controls.h"

/* Use this area to implement functions that create controls */

HWND createButtonWindow(HWND parent, LPCWSTR bText, int x, int y, int ctrlId, int xtraWs)
{
	return CreateWindowW(
		L"BUTTON",  // Predefined class; Unicode assumed 
		bText,      // Button text 
		xtraWs | WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
		x,         // x position 
		y,         // y position 
		100,        // Button width
		40,        // Button height
		parent,     // Parent window
		(HMENU)ctrlId,       // No menu.
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
}
HWND createEditWindow(HWND parent, int x, int y, int width, int height, int ctrlId, int xtra, int stra)
{
	return CreateWindowExW(
		WS_EX_CLIENTEDGE | xtra,
		L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | stra,
		x,
		y,
		width,
		height,
		parent,         // parent window 
		(HMENU)ctrlId,   // edit control ID 
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);        // pointer not needed 
}
HWND createComboBox(HWND parent, int x, int y, int width, int height, int ctrlId, int xtra, int stra)
{
	return CreateWindowExW(
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | xtra,
		L"COMBOBOX",   // predefined class 
		NULL,         // no window title 
		CBS_DROPDOWN | WS_TABSTOP | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | stra,
		x,
		y,
		width,
		height,
		parent,         // parent window 
		(HMENU)ctrlId,   // edit control ID 
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);        // pointer not needed         // pointer not needed 
}
HWND createStaticControlWindow(HWND parent, LPCWSTR bText, int x, int y, int width, int height, int ctrlId, int xtraWs)
{
	return CreateWindowW(
		L"STATIC",  // Predefined class; Unicode assumed 
		bText,      // Button text 
		xtraWs | WS_EX_LEFT | WS_OVERLAPPED | WS_VISIBLE | WS_CHILD,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		parent,     // Parent window
		(HMENU)ctrlId,       // No menu.
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
}
HWND createTrackbarControlWindow(HWND parent, int x, int y, int width, int height, int ctrlId, int xtra)
{
	return CreateWindowW(
		TRACKBAR_CLASS,  // Predefined class; Unicode assumed 
		NULL,      // Button text 
		xtra | WS_EX_LEFT | WS_OVERLAPPED | WS_VISIBLE | WS_CHILD,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		parent,     // Parent window
		(HMENU)ctrlId,       // No menu.
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);      // Pointer not needed.
}