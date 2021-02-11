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
DWORD* dat;
DWORD size;
DWORD align;

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
		for (UINT i = 0; i < IMMDevices.Count(); i++) {
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
		case IDC_BUTTON2:
		{
			IFileOpenDialog* pFileOpen;
			HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				__uuidof(IFileOpenDialog), reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr))
			{
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"Audio", L"*.wav" }
				};
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

								// FACT, CUE, PLAYLIST, ASSOCIATED DATA LIST

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
										fread(TWAVEFORMAT.data, sizeof(BYTE), 4, WAVEFILE);
										// IF 
										// READ "data" CHUNK
										HWND hwnd = GetDlgItem(hWnd, IDC_EDIT2);
										catout(pszFilePath, hwnd);
										catout(L"\r\n", hwnd);
										if (memcmp(TWAVEFORMAT.data, "data", sizeof(BYTE)) != 0)
										{
											catout(L"NON-PCM File or header has additional chunks to check for.\r\n", hwnd);
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
											// Get Data
											dat = (DWORD*)malloc(sizeof(DWORD) * TWAVEFORMAT.datasize);
											if (dat == NULL) return MessageBox(hWnd, L"malloc() failed. ", L"Error", MB_OK);
											fread(dat, sizeof(DWORD), TWAVEFORMAT.datasize, WAVEFILE);
											size = TWAVEFORMAT.datasize;
											align = TWAVEFORMAT.nBlockAlign;
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
			
			// Process Only Active  (ENSURE it is active)
			if(wcscmp(IMMDevices.GetState(),L"DEVICE_ACTIVE") == 0)
			{
				// ENSURE IT IS A RENDER DEVICE
				if (wcscmp(IMMDevices.GetDeviceDataFlow(), L"eRender") == 0) {

					// Activate the AUDIO Client for the device
					IMMDevices.ActivateAudioClient();
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
					// Initialize the AUDIO engine
					WAVEFORMATEX wf;
					// TODO  WAVEFORMAT from a file
					wf = IMMDevices.BuildWaveFormatEx(WAVE_FORMAT_PCM, 2, 16, 44100);
					if (IMMDevices.Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1000, 0, &wf, NULL) == S_OK)
					{
						// Print stuff (these can only be done after init)
						buff = IMMDevices.GetAudioClientBufferSize();
						catout(L"AudioClientBufferSize: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						buff = IMMDevices.GetAudioClientStreamLatency();
						catout(L"AudioClientStreamLatency: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						buff = IMMDevices.GetAudioClientDevicePeriod();
						catout(L"AudioClientDevicePeriod: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						buff = IMMDevices.GetAudioClientCurrentPadding();
						catout(L"AudioClientCurrentPadding: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						// INIT: IAudioRender
						if (IMMDevices.InitRenderClient() == S_OK) {
							UINT buffersize = IMMDevices.GetAudioClientBufferSizeRaw();
							BYTE* pData;
							// INITIALIZE (essentially silence the start of stream)
							IMMDevices.GetRenderClientBuffer(buffersize, &pData);
							memcpy_s(pData, buffersize,0,0);
							IMMDevices.ReleaseRenderClientBuffer(buffersize, 0);
							REFERENCE_TIME hnsActualDuration = (double)10000000 * buffersize / 44100;

							// START audio rendering
							IMMDevices.Start();

							UINT framesProcessed = 0;
							UINT ptr = 0;


							// PLAY THE FILE buffer
							for (; framesProcessed < size;) {
								// Per the Example 
								Sleep((DWORD)(hnsActualDuration / 10000 / 2));
								UINT pad = IMMDevices.GetAudioClientCurrentPaddingRaw();
								UINT framesAvailable = buffersize - pad;
								IMMDevices.GetRenderClientBuffer(framesAvailable, &pData);
								memcpy_s(pData, framesAvailable*align, &dat[ptr], framesAvailable*align);
								framesProcessed += framesAvailable;
								ptr += framesAvailable;
								IMMDevices.ReleaseRenderClientBuffer(framesAvailable, 0);
							}
							Sleep((DWORD)(hnsActualDuration / 10000 / 2));

							if (dat != NULL) {
								free(dat);
								dat = NULL;
							}


							// STOP RENDERING
							IMMDevices.Stop();
						}
					}
					else {
						catout(L"Format not supported!", hwnd);
						catout(L"\r\n", hwnd);
					}
				}
			}
			else {
				catout(L"Device Disabled or unplugged!", hwnd);
				catout(L"\r\n", hwnd);
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
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}