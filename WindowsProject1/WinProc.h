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
			if(wcscmp(IMMDevices.GetState(),L"DEVICE_ACTIVE") == 0){
				// Activate the AUDIO Client for the device
				IMMDevices.ActivateAudioClient();
				// Print some Settings about it
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
				wf = IMMDevices.BuildWaveFormatEx(WAVE_FORMAT_PCM, 2, 16, 44100L);
				IMMDevices.Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 100, 0, &wf, NULL);
				buff = IMMDevices.GetAudioClientBufferSize();
				catout(L"AudioClientBufferSize: ", hwnd);
				catout(buff, hwnd);
				catout(L"\r\n", hwnd);
				buff = IMMDevices.GetAudioClientStreamLatency();
				catout(L"AudioClientStreamLatency: ", hwnd);
				catout(buff, hwnd);
				catout(L"\r\n", hwnd);
				// TODO PLAY a wave file 
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