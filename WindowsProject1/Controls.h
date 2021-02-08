#pragma once

#include "framework.h"

/* Define all control creation functions here */

// Windows Related
HWND createButtonWindow(HWND, LPCWSTR, int, int, int);
HWND createEditWindow(HWND, int, int, int, int, int, int, int);
HWND createComboBox(HWND parent, int, int, int, int, int, int, int);