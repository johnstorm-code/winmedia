#pragma once

#include "Controls.h"

void LoadUIControls(HWND hWnd) 
{
	// Buttons
	createButtonWindow(hWnd, L"INFO", 10, 10, IDC_BUTTON1,0);
	createButtonWindow(hWnd, L"OPEN", 10, 60, IDC_BUTTON2,0);
	createButtonWindow(hWnd, L"PLAY", 10, 110, IDC_BUTTON3, WS_DISABLED);
	createButtonWindow(hWnd, L"STOP", 10, 160, IDC_BUTTON4, 0);
	// Button is also checkbox
	HWND hwndcb = createButtonWindow(hWnd, L"LOOP", 10, 210, IDC_CHECKBOX1, BS_CHECKBOX|BS_AUTOCHECKBOX);
	SendMessage(hwndcb, BM_SETCHECK, BST_CHECKED, 0);
	// Static control
	createStaticControlWindow(hWnd, L"0", 10, 260, 100, 20, IDC_STATIC1, 0);
	// Combobox (selection)
	createComboBox(hWnd, 150, 10, 800, 200, IDC_COMBOBOX1, 0, 0);
	// Edit controls
	HWND hwndEdit = createEditWindow(hWnd, 150, 40, 1200, 300, IDC_EDIT1, 0, 0);
	SetFocus(hwndEdit);
	createEditWindow(hWnd, 150, 350, 1200, 150, IDC_EDIT2, 0, ES_READONLY);
	// Trackbar control
	HWND trackbar = createTrackbarControlWindow(hWnd, 10, 290, 100, 30, IDC_TRACKBAR1, TBS_AUTOTICKS|TBS_TOOLTIPS);
	DWORD msg; 	WORD l = 0; 	WORD h = 100; msg = MAKELONG(l, h);
	SendMessage(trackbar, TBM_SETRANGE, FALSE, (LPARAM)msg);
	SendMessage(trackbar, TBM_SETTICFREQ, 25, 0);
	SendMessage(trackbar, TBM_SETPOS, TRUE, 75);
}