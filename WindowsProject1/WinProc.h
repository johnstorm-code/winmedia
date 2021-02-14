#pragma once

#include "Functions.h"

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

IMMDevicesApi IMMDevices;

// TODO: these are for testing purposes
DWORD* dat; // raw binary data
DWORD size;	// the size 
WORD align;	// 4, 8, etc
WORD channels;// 1, 2
WORD bps;// bits per second, 8, 16, 32, etc
DWORD sps;//samples per second 44100, 480000
WORD frmat;//waveformat WAVE_FORMAT_PCM, etc
BOOL loop = FALSE;//controls looping 
BOOL play = FALSE;//controls playing
HANDLE h;//handle to thread
UINT timeout = 500;// thread timeout

 // THread function TODO:  not certain if this is where I should handle all
// audio playing.  I would need a mixer to play multiple files at once
// possibly this is simply a set of buffers (2 channels) where data goes in mixed
// before being written to the output buffers allocated by GetRenderClientBuffer()
// To mix the various sources, I would suspect "adding" them to produce an output
// with all frequencies combined.  Problems:  audio becomes louder as multiple
// sounds are added, so a way to normalize is required
// I need to do more reserach in this
DWORD WINAPI ThreadFn(LPVOID lParam) 
{
	HWND hwnd, control;
	hwnd = (HWND)lParam;

	control = GetDlgItem(hwnd, IDC_EDIT2);

	catout(L"ENTERING THREAD--- playing audio.\r\n", control);
	SendMessage(control, EM_LINESCROLL, 0, SendMessage(control, EM_GETLINECOUNT, 0, 0));


	UINT buffersize = IMMDevices.GetAudioClientBufferSizeRaw();
	BYTE* pData;
	// INITIALIZE (essentially silence the start of stream)
	IMMDevices.GetRenderClientBuffer(buffersize, &pData);
	memcpy_s(pData, buffersize, 0, 0);
	IMMDevices.ReleaseRenderClientBuffer(buffersize, 0);
	REFERENCE_TIME hnsActualDuration = (long)10000000 * buffersize / sps;

	// RE-START audio rendering
	IMMDevices.Reset();
	IMMDevices.Start();

	UINT framesProcessed = 0;
	// Start of audio data
	UINT ptr = 0;
	play = TRUE;

	// PLAY THE FILE buffer
	// NOTE:  Some notes on my understanding==
	// Audio doesn't "play" all at one time.  "Fragments" of the audio data, 
	// are processed at a time.  To the listener it seems the
	// audio is playing all at once.  However, just like video is comprised of individual
	// images, this is somewhat a metaphor to how the audio api/card works.
	// In this example here: the audio data is coming from a wav file that has been loaded
	// into a global pointer, which is filled with audio data using calloc(), then
	// this data is read fragment by fragment and copied over via memcpy into the audio api
	// audio buffer.  The api supplies the mechanism of allowing us to write audio data
	// by means of a buffer of its own.
	for (;;) 
	{
		// This STOPS playing audio
		if (play == FALSE) { break; }

		// This LOOPS the sample
		if (framesProcessed > size / 4) {
			if (loop == TRUE){
				framesProcessed = 0;  ptr = 0;
			}
			else {
				break;
			}
		}
		// Per the Example 
		Sleep((DWORD)(hnsActualDuration / 10000 / 2));
		// Get the PAD value, this is to calculate how much we can write to the buffer
		UINT pad = IMMDevices.GetAudioClientCurrentPaddingRaw();
		// Calculate the amount of frames we can write
		UINT framesAvailable = buffersize - pad;
		// Now get buffer to write to
		IMMDevices.GetRenderClientBuffer(framesAvailable, &pData);
		// Copy Data from the audio file into the audio render buffer
		memcpy_s(pData, framesAvailable * align, &dat[ptr], framesAvailable * align);
		// Calculate how much data we have processed
		framesProcessed += framesAvailable;
		// Point to next frame of data from audio file
		ptr += framesAvailable;
		// Once we have written to the buffer, free it, this
		// informs the audio engine to read the written data
		IMMDevices.ReleaseRenderClientBuffer(framesAvailable, 0);
	}
	// Per example (allow enough time for last frame)
	Sleep((DWORD)(hnsActualDuration / 10000 / 2));

	// STOP RENDERING
	IMMDevices.Stop();

	catout(L"LEAVING THREAD--- stopping audio.\r\n", control);

	SendMessage(control, EM_LINESCROLL, 0, SendMessage(control, EM_GETLINECOUNT, 0, 0));
	return 0;
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Use to Instantiate/Create all controls here
	case WM_CREATE:
	{
		LoadUIControls(hWnd);
		IMMDevices.RegisterNotifictionClient(hWnd);
		HWND hwnd = GetDlgItem(hWnd, IDC_COMBOBOX1);
		for (register UINT i = 0; i < IMMDevices.Count(); i++)
		{
			SendMessage(hwnd, CB_INSERTSTRING, i, (LPARAM)IMMDevices.GetDeviceByIndex(i)->GetFriendlyName());
		}
		SendMessage(hwnd, CB_SETCURSEL, 0, 0);
		hwnd = GetDlgItem(hWnd, IDC_EDIT1);
		IMMDevices.DumpDeviceProps(hwnd, 0);

		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);      // Control ID such as 102 or BN_Clicked
		int nCode = HIWORD(wParam);     // Message to this Control like WM_GETTEXT, WM_MOUSEMOVE, etc
										// lParam is Control HANDLE HWND

		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_CHECKBOX1: {
			HWND hwnd = GetDlgItem(hWnd, IDC_CHECKBOX1);
			UINT res = SendMessage(hwnd, BM_GETCHECK, 0, 0);
			if (res == BST_CHECKED) {
				loop = TRUE;
			}
			else {
				loop = FALSE;
			}
		}
		break;
		case IDC_BUTTON3:
		{
			play = FALSE;
			if (h != NULL) {
				WaitForSingleObject(h, timeout);
				CloseHandle(h);
				h = NULL;
			}
			HWND hwnd = GetDlgItem(hWnd, IDC_COMBOBOX1);
			UINT index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
			hwnd = GetDlgItem(hWnd, IDC_EDIT2);

			// Set Device by selection on GUI
			IMMDevices.GetDeviceByIndex(index);

			// NOTE:  We can also get a default device 
			//IMMDevices.GetDefaultDevice(EDataFlow::eRender, ERole::eMultimedia);

			// Process Only eRender
			if (wcscmp(IMMDevices.GetDeviceDataFlow(), L"eRender") == 0)
			{
				// ENSURE IT IS ACTIVE
				if (wcscmp(IMMDevices.GetState(), L"DEVICE_ACTIVE") == 0)
				{
					// Activate the AUDIO Client for the device
					if (IMMDevices.ActivateAudioClient() == S_OK)
					{

						// Initialize the AUDIO engine
						WAVEFORMATEX wf = IMMDevices.BuildWaveFormatEx(frmat, channels, bps, sps);
						if (IMMDevices.Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1000, 0, &wf, NULL) == S_OK)
						{
							// INIT: IAudioRender
							if (IMMDevices.InitRenderClient() == S_OK)
							{
								// Thread
								//thread tests
								// Probasbly not done right but this allows the file to play
								// without holding up the UI
								DWORD id;
								
								h = CreateThread(NULL, 0, ThreadFn, hWnd, 0, &id);
								if (id == NULL) {
									return -1;
									//WaitForSingleObject(h, INFINITE);
									//CloseHandle(h);
								}
							}
							else {
								catout(L"Unable to init render client.\r\n", hwnd);
							}
						}
						else {
							catout(L"Format not supported or something else needed to process, can't play audio\r\n", hwnd);
						}
					}
					else {
						catout(L"Unable to activate client.\r\n", hwnd);
					}
				}
				else {
					catout(L"Device Disabled or unplugged!\r\n", hwnd);
				}
			}
			else {
				catout(L"Not rendering device.\r\n", hwnd);
			}
			/*HWND button3 = GetDlgItem(hWnd, IDC_BUTTON3);
			EnableWindow(button3, FALSE);*/
			SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
		}
		break;
		case IDC_BUTTON2: // OPEN file
		{
			IFileOpenDialog* pFileOpen;
			HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				__uuidof(IFileOpenDialog), reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr))
			{
				COMDLG_FILTERSPEC rgSpec[] = {{ L"Audio", L"*.wav" }};
				pFileOpen->SetFileTypes((sizeof(rgSpec) / sizeof(rgSpec[0])), rgSpec);
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							FILE* WAVEFILE;
							_wfopen_s(&WAVEFILE, pszFilePath, L"rb");
							if (WAVEFILE == NULL) return MessageBox(hWnd, L"Unable to open file!", L"Error", MB_OK);

							struct TESTWAVEFORMAT {
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

							// READ and ensure it's RIFF FILE
							fread(TWAVEFORMAT.riff, sizeof(BYTE), 4, WAVEFILE);
							if (memcmp(TWAVEFORMAT.riff, "RIFF", sizeof(BYTE)) == 0)
							{
								fread(&TWAVEFORMAT.riffsize, sizeof(DWORD), 1, WAVEFILE);

								// READ and ensure it's a WAVE format
								fread(TWAVEFORMAT.riffwave, sizeof(BYTE), 4, WAVEFILE);
								if (memcmp(TWAVEFORMAT.riffwave, "WAVE", sizeof(BYTE)) == 0)
								{
									// READ "fmt " CHUNK
									fread(TWAVEFORMAT.fmt, sizeof(BYTE), 4, WAVEFILE);
									if (memcmp(TWAVEFORMAT.fmt, "fmt ", sizeof(BYTE)) == 0)
									{
										fread(&TWAVEFORMAT.fmtsize, sizeof(DWORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.wFormatTag, sizeof(WORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.nChannels, sizeof(WORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.nSamplesPerSec, sizeof(DWORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.nAvgBytesPerSec, sizeof(DWORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.nBlockAlign, sizeof(WORD), 1, WAVEFILE);
										fread(&TWAVEFORMAT.nBitsPerSample, sizeof(WORD), 1, WAVEFILE);

										// TODO: OGG encoded wave file captured here but won't play
										if (TWAVEFORMAT.wFormatTag != WAVE_FORMAT_PCM && TWAVEFORMAT.wFormatTag != WAVE_FORMAT_IEEE_FLOAT) {
											BYTE sigbytes[11];
											fread(&sigbytes, sizeof(BYTE), 10, WAVEFILE);
										}
										// Ignore anything in the file, except data
										// // TODO: FACT, CUE, PLAYLIST, ASSOCIATED DATA LIST
										while (true) {
											fread(&TWAVEFORMAT.data, sizeof(BYTE), 4, WAVEFILE);
											if (memcmp(TWAVEFORMAT.data, "data", sizeof(BYTE)) == 0) {
												break;
											}
										}
										
										HWND hwnd = GetDlgItem(hWnd, IDC_EDIT2);
										catout(pszFilePath, hwnd);
										catout(L"\r\n", hwnd);
										if (memcmp(TWAVEFORMAT.data, "data", sizeof(BYTE)) != 0)
										{
											catout(L"NON-PCM File or header has additional chunks to check for.\r\n", hwnd);
											HWND button3 = GetDlgItem(hWnd, IDC_BUTTON3);
											EnableWindow(button3, FALSE);
										}
										else
										{
											fread(&TWAVEFORMAT.datasize, sizeof(DWORD), 1, WAVEFILE);
											float time = (float)TWAVEFORMAT.riffsize / ((float)TWAVEFORMAT.nSamplesPerSec * (float)TWAVEFORMAT.nChannels * ((float)TWAVEFORMAT.nBitsPerSample / 8));
											TCHAR formatex[2000];
											StringCbPrintf(formatex, 2000*sizeof(TCHAR),
												L"%c%c%c%c %d %c%c%c%c %c%c%c%c Size:%d Format:%d Channels:%d SamplesPerSec:%d AvgBytesPerSec:%d BlockAlign:%d BitsPerSample:%d SampleSize:%ld Time:%f\r\n",
												TWAVEFORMAT.riff[0],
												TWAVEFORMAT.riff[1],
												TWAVEFORMAT.riff[2],
												TWAVEFORMAT.riff[3],
												TWAVEFORMAT.riffsize,
												TWAVEFORMAT.riffwave[0],
												TWAVEFORMAT.riffwave[1],
												TWAVEFORMAT.riffwave[2],
												TWAVEFORMAT.riffwave[3],
												TWAVEFORMAT.fmt[0],
												TWAVEFORMAT.fmt[1],
												TWAVEFORMAT.fmt[2],
												TWAVEFORMAT.fmt[3],
												TWAVEFORMAT.fmtsize,
												TWAVEFORMAT.wFormatTag,
												TWAVEFORMAT.nChannels,
												TWAVEFORMAT.nSamplesPerSec,
												TWAVEFORMAT.nAvgBytesPerSec,
												TWAVEFORMAT.nBlockAlign,
												TWAVEFORMAT.nBitsPerSample,
												TWAVEFORMAT.datasize,
												time
												);
											catout(L"\t", hwnd);
											catout(formatex, hwnd);
											// Free the file 
											if (dat != NULL) {
												free(dat);
												dat = NULL;
											}
											// Get Data
											UINT syize = TWAVEFORMAT.datasize;
											dat = (DWORD*)calloc(TWAVEFORMAT.datasize, sizeof(DWORD));
											if (dat == NULL) return MessageBox(hWnd, L"malloc() failed. ", L"Error", MB_OK);
											fread(dat, sizeof(DWORD), syize, WAVEFILE);
											size = TWAVEFORMAT.datasize;
											align = TWAVEFORMAT.nBlockAlign;
											channels = TWAVEFORMAT.nChannels;
											bps = TWAVEFORMAT.nBitsPerSample;
											sps = TWAVEFORMAT.nSamplesPerSec;
											frmat = TWAVEFORMAT.wFormatTag;
											HWND button3 = GetDlgItem(hWnd, IDC_BUTTON3);
											EnableWindow(button3, TRUE);
											// Use Data
											// IDEA --- TODO
											// IMMDevices.play(data); <-- this way???
											// Free
											//free(dat);
										}
										SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
									}
								}
							}
							fclose(WAVEFILE);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
		}
		break;
		case IDC_COMBOBOX1:
			switch (nCode)
			{
			case CBN_SELCHANGE:
				UINT index = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
				HWND hwnd = GetDlgItem(hWnd, IDC_EDIT1);
				IMMDevices.DumpDeviceProps(hwnd, index);
				break;
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, MyAbout);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_EDIT1:
			switch (nCode) 
			{
			case EN_SETFOCUS:
				break;
			}
			break;
		case IDC_BUTTON1: {
			HWND hwnd = GetDlgItem(hWnd, IDC_COMBOBOX1);
			UINT index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
			hwnd = GetDlgItem(hWnd, IDC_EDIT2);

			// Set Device by selection on GUI
			IMMDevices.GetDeviceByIndex(index);

			// NOTE:  We can also get a default device 
			//IMMDevices.GetDefaultDevice(EDataFlow::eRender, ERole::eMultimedia);
			
			// ENSURE it's a render device
			if(wcscmp(IMMDevices.GetDeviceDataFlow(), L"eRender") == 0)
			{
				// ENSURE IT IS ACTIVE
				if (wcscmp(IMMDevices.GetState(), L"DEVICE_ACTIVE") == 0)
				{

					// Activate the AUDIO Client for the device
					if (IMMDevices.ActivateAudioClient() == S_OK)
					{
						LPCWSTR buff = IMMDevices.GetAudioClientDevicePeriod();
						catout(L"AudioClientDevicePeriod: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						catout(L"SUPPORTED: ", hwnd);
						buff = IMMDevices.MixFormatToString(IMMDevices.GetAudioClientMixFormat());
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						// DEBUG STUFF to display all 
						IMMDevices.DumpAudioClientSupportedFormats(hwnd);
					}
					else {
						catout(L"Unable to init audioclient.\n\r", hwnd);
					}
				}
				else {
					catout(L"Device Disabled or unplugged!\r\n", hwnd);
				}
			}
			else {
				catout(L"Not a rendering device.\r\n", hwnd);
			}
			SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		// NOTE:  This is done here as a means to ensure the process ends before termination
		// and that we stop playing any lingering audio
		// NOT CERTAIN this is the best way to handle this TODO
		play = FALSE;
		loop = FALSE;
		if (h != NULL) {
			WaitForSingleObject(h, timeout);
			CloseHandle(h);
			h = NULL;
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}