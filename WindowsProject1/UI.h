#pragma once

#include "Controls.h"

void LoadUIControls(HWND hWnd) {
	createButtonWindow(hWnd, L"OK", 10, 10, IDC_BUTTON1);
	createButtonWindow(hWnd, L"OPEN", 10, 60, IDC_BUTTON2);
	createComboBox(hWnd, 150, 10, 800, 200, IDC_COMBOBOX1, 0, 0);
	HWND hwndEdit = createEditWindow(hWnd, 150, 40, 1200, 300, IDC_EDIT1, 0, 0);
	createEditWindow(hWnd, 150, 350, 1200, 150, IDC_EDIT2, 0, ES_READONLY);
	SetFocus(hwndEdit);
}