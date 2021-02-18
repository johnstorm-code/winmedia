#pragma once

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                MyInitInstance(HINSTANCE, int);
LRESULT CALLBACK    MyWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MyAbout(HWND, UINT, WPARAM, LPARAM);

// Basic WaveFormat structure
struct MYWAVEFORMAT
{
	// RIFF CHUNK
	BYTE riff[4];
	DWORD riffsize;

	// WAVEID
	BYTE riffwave[4];

	// FMT CHUNK
	BYTE fmt[4];
	DWORD fmtsize;
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD nBitsPerSample;

	// TODO: FACT, CUE, PLAYLIST, ASSOCIATED DATA LIST

	// DATA CHUNK
	BYTE data[4];
	DWORD datasize;
}TWAVEFORMAT;