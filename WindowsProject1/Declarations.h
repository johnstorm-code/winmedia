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
	
	// smpl CHUNK
	BYTE smpl[4];
	DWORD smplsize;
	DWORD manf;
	DWORD prod;
	DWORD smplperiod;
	DWORD midiunitynote;
	DWORD midipitchfrac;
	DWORD smptefmt;
	DWORD smpteoffset;
	DWORD nloops;
	DWORD smpldata;
	DWORD smplLoops;
	/* TODO more here may be expected from smpl chunk*/

	// DATA CHUNK
	BYTE data[4];
	DWORD datasize;
}TWAVEFORMAT;