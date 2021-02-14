#include "Functions.h"

/*
 * Concats strings to a control, essentially "filling" the control with data
 *
 */
void catout(LPCWSTR text, HWND hwnd) {
	int cCount = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
	int tCount = wcslen(text);
	int fCount = cCount + tCount;
	//Allocate buffer to hold new concatenated string
	TCHAR* fText = (TCHAR*)calloc(fCount,  sizeof(TCHAR) + 2);
	if (fText == NULL) return;
	//Fill the buffer with the control's text
	SendMessage(hwnd, WM_GETTEXT, cCount + 1, (LPARAM)fText);
	//Copy new text to buffer beginning where current buffer's text ends, NULL terminate
	memcpy(fText + cCount, text, tCount * sizeof(TCHAR));
	memcpy(fText + fCount, "\0", 1 * sizeof(TCHAR));
	//Set the new text
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)fText);
	//We can now free memory used
	free(fText);
	//Secure
	fText = NULL;
}