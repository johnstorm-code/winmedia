#pragma once

#include "Controls.h"

void LoadUIControls(HWND hWnd) 
{
	// Buttons
	createButtonWindow(hWnd, L"INFO", 10, 10, IDC_BUTTON1,0);
	createButtonWindow(hWnd, L"OPEN", 10, 60, IDC_BUTTON2,0);
	createButtonWindow(hWnd, L"PLAY", 10, 110, IDC_BUTTON3, WS_DISABLED);
	// Button is also checkbox
	createButtonWindow(hWnd, L"LOOP", 10, 160, IDC_CHECKBOX1, BS_CHECKBOX|BS_AUTOCHECKBOX);
	// Static control
	createStaticControlWindow(hWnd, L"0", 10, 210, 100, 20, IDC_STATIC1, 0);
	// Combobox (selection)
	createComboBox(hWnd, 150, 10, 800, 200, IDC_COMBOBOX1, 0, 0);
	// Edit controls
	HWND hwndEdit = createEditWindow(hWnd, 150, 40, 1200, 300, IDC_EDIT1, 0, 0);
	createEditWindow(hWnd, 150, 350, 1200, 150, IDC_EDIT2, 0, ES_READONLY);
	SetFocus(hwndEdit);
}