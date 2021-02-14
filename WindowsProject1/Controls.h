#pragma once

#include "framework.h"

/* Define all control creation functions here */

// Windows Related
HWND createButtonWindow(HWND parent, LPCWSTR bText, int x, int y, int ctrlId, int xtraWs);
HWND createEditWindow(HWND parent, int x, int y, int width, int height, int ctrlId, int xtra, int stra);
HWND createComboBox(HWND parent, int x, int y, int width, int height, int ctrlId, int xtra, int stra);
HWND createStaticControlWindow(HWND parent, LPCWSTR bText, int x, int y, int width, int height, int ctrlId, int xtraWs);