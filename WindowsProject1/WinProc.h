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
					{ L"Audio", L"*.wav;*.mp3" }
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
										// READ "data" CHUNK
										if (memcmp(TWAVEFORMAT.data, "data", sizeof(BYTE)) == 0)
										{
											fread(&TWAVEFORMAT.datasize, sizeof(DWORD), 1, WAVEFILE);
											TWAVEFORMAT.datasize;
											// GET DATA
										}
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
			
			// Process Only Active 
			if(wcscmp(IMMDevices.GetState(),L"DEVICE_ACTIVE") == 0)
			{
				// Activate the AUDIO Client for the device
				IMMDevices.ActivateAudioClient();
				// Print some Settings about it (these can be done before init)
				LPCWSTR buff = IMMDevices.GetAudioClientDevicePeriod();
				catout(L"AudioClientDevicePeriod: ", hwnd);
				catout(buff, hwnd);
				catout(L"\r\n", hwnd);
				catout(L"SUPPORTED: ", hwnd);
				buff = IMMDevices.MixFormatToString(IMMDevices.GetAudioClientMixFormat());
				catout(buff, hwnd);
				catout(L"\r\n", hwnd);
				IMMDevices.DumpAudioClientSupportedFormats(hwnd);
				// Initialize the AUDIO engine
				WAVEFORMATEX wf;
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
					// TODO: IAudioRender
					// TODO PLAY a wave file 
					// http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
				}
				else {
					catout(L"Format not supported!", hwnd);
					catout(L"\r\n", hwnd);
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