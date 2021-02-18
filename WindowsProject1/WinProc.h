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
BOOL loop = TRUE;//controls looping, start on
BOOL play = FALSE;//controls playing
float volume = 1.0f;//global control of volume
HANDLE h[2];//handle to thread
UINT timeout = 100;// thread timeout
const UINT flen = 10;
TCHAR frames[flen] = L"0";

// Thread for File 
DWORD WINAPI ThreadFnFile(LPVOID lParam) 
{
	HWND hWnd = (HWND)lParam;

	IFileOpenDialog* pFileOpen =  NULL;
	
	HRESULT hr = CoInitialize(pFileOpen);
	if (hr != S_OK) return -1;

	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		__uuidof(IFileOpenDialog), reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		COMDLG_FILTERSPEC rgSpec[] = { { L"Audio", L"*.wav;.wave" } };
		pFileOpen->SetFileTypes((sizeof(rgSpec) / sizeof(rgSpec[0])), rgSpec);
		// Show the Open dialog box.
		hr = pFileOpen->Show(hWnd);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// OK GOT the FILE
				if (SUCCEEDED(hr))
				{
					play = FALSE;
					FILE* WAVEFILE;
					_wfopen_s(&WAVEFILE, pszFilePath, L"rb");
					if (WAVEFILE == NULL) return MessageBox(hWnd, L"Unable to open file!", L"Error", MB_OK);

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
									TCHAR formatex[256];
									StringCbPrintf(formatex, 256 * sizeof(TCHAR),
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

									// TODO/TEST: modify the data in some form
									for (UINT index = 0; index < size/align; index++)
									{
										// Get a frame of data (4 bytes per block align)
										DWORD frame = dat[index];
										// Get bottom 2 bytes
										WORD leftch = LOWORD(frame);
										// Get upper 2 bytes
										WORD rightch = HIWORD(frame);
										// Get samples
										// Note: _int16 to get the actual sample value
										// Represented as double
										double rsample = (_int16)leftch;
										double lsample = (_int16)rightch;
										// Modify the samples by adjusting amplitude
										leftch = (WORD)(lsample * volume);
										rightch = (WORD)(rsample * volume);
										// Prepare packet
										DWORD modifiedframe = MAKELONG(leftch, rightch);
										// Write packet to frame buffer
										dat[index] = modifiedframe;
									}
								}
								SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
							}
						}
					}
					_fcloseall();
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
				pItem = NULL;
			}
		}
		pFileOpen->Release();
		pFileOpen = NULL;
	}
	return 0;
}

// Thread to output device information
DWORD WINAPI ThreadFnLog(LPVOID lParam) {
	HWND hwnd = (HWND)lParam;
	// DEBUG STUFF to display all 
	IMMDevices.DumpAudioClientSupportedFormats(hwnd, FALSE);
	SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
	return 0;
}

 // Thread function TODO:  not certain if this is where I should handle all
// audio playing.  I would need a mixer to play multiple files at once
// possibly this is simply a set of buffers (2 channels) where data goes in mixed
// before being written to the output buffers allocated by GetRenderClientBuffer()
// To mix the various sources, I would suspect "adding" them to produce an output
// with all frequencies combined.  Problems:  audio becomes louder as multiple
// sounds are added, so a way to normalize is required
// I need to do more reserach on this
DWORD WINAPI ThreadFn(LPVOID lParam) 
{
	HWND hwnd, control, static1;
	hwnd = (HWND)lParam;

	control = GetDlgItem(hwnd, IDC_EDIT2);
	static1 = GetDlgItem(hwnd, IDC_STATIC1);

	catout(L"ENTERING THREAD--- playing audio.\r\n", control);
	SendMessage(control, EM_LINESCROLL, 0, SendMessage(control, EM_GETLINECOUNT, 0, 0));

	// Get Available Buffer size
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
	// audio buffer. The size of each fragment is defined by buffersize minus a padding value
	// obtained from the audio api.  This framesAvailable value is then the amount of buffer 
	// to request to write to.  The actual data written is framesAvailable * align
	// to access a packet of data.  A pointer variable is used to point to the data buffer
	// Thsi is still all a bit non-native to me.
	for (;;) 
	{
		// This STOPS playing audio
		if (play == FALSE) { break; }

		UINT totalFrames = size / align;
		if (framesProcessed > totalFrames) 
		{
			// If we want to loop this sample
			if (loop == TRUE){
				framesProcessed = 0;  ptr = 0;
			}
			// if we have already processed all frames, break
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
		UINT writeCount = framesAvailable * align;
		//
		//
		//
		memcpy_s(pData, writeCount, &dat[ptr], writeCount);
		// Calculate how much data we have processed
		framesProcessed += framesAvailable;
		// Point to next frame of data from audio file
		ptr += framesAvailable;
		// Writes to a static control
		StringCbPrintf(frames, flen*sizeof(TCHAR), L"%d", framesProcessed);
		SendMessage(static1, WM_SETTEXT, 0, (LPARAM)frames);
		// TESTING drawing text
		RECT r;
		GetClientRect(hwnd, &r);
		r.left = 150;
		r.top = 500;
		r.right = 300;
		r.bottom = 550;
		InvalidateRect(hwnd, &r, TRUE);
		// Once we have written to the buffer, free it, this
		// informs the audio engine to read the written data
		IMMDevices.ReleaseRenderClientBuffer(framesAvailable, 0);
	}
	// Per example (allow enough time for last frame)
	Sleep((DWORD)(hnsActualDuration / 10000 / 2));

	// STOP RENDERING
	IMMDevices.Stop();

	catout(L"LEAVING THREAD--- stopping audio.\r\n", control);
	play = FALSE;
	SendMessage(control, EM_LINESCROLL, 0, SendMessage(control, EM_GETLINECOUNT, 0, 0));
	return 0;
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// ALL SCROLL messages received here
	case WM_HSCROLL:
	{
		int id = GetDlgCtrlID((HWND)lParam);
		switch (id) {
		case IDC_TRACKBAR1:
			WORD scrollval = LOWORD(wParam);
			switch (scrollval) {
			case SB_THUMBTRACK:
				WORD pos = HIWORD(wParam);//position of scroll
				volume = (float)pos / 100;
				break;
			}
		break;
		}
	}
	break;
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
	case WM_COMMAND: // ALL MESSAGES 
	{
		int wmId = LOWORD(wParam);      // Control ID such as 102 or BN_Clicked
		int nCode = HIWORD(wParam);     // Message to this Control like WM_GETTEXT, WM_MOUSEMOVE, etc
										// lParam is Control HANDLE HWND

		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_BUTTON1: // INFO
		{
			HWND hwnd = GetDlgItem(hWnd, IDC_COMBOBOX1);
			UINT index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
			hwnd = GetDlgItem(hWnd, IDC_EDIT2);

			// Set Device by selection on GUI
			IMMDevices.GetDeviceByIndex(index);

			// NOTE:  We can also get a default device 
			//IMMDevices.GetDefaultDevice(EDataFlow::eRender, ERole::eMultimedia);

			// ENSURE it's a render device
			if (wcscmp(IMMDevices.GetDeviceDataFlow(), L"eRender") == 0)
			{
				// ENSURE IT IS ACTIVE
				if (wcscmp(IMMDevices.GetState(), L"DEVICE_ACTIVE") == 0)
				{

					// Activate the AUDIO Client for the device (returns S_OK if already loaded)
					if (IMMDevices.ActivateAudioClient() == S_OK)
					{
						SendMessage(hwnd, WM_SETTEXT, 0, 0);
						LPCWSTR buff = IMMDevices.GetAudioClientDevicePeriod();
						catout(L"AudioClientDevicePeriod: ", hwnd);
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);
						catout(L"SUPPORTED: ", hwnd);
						buff = IMMDevices.MixFormatToString(IMMDevices.GetAudioClientMixFormat());
						catout(buff, hwnd);
						catout(L"\r\n", hwnd);

						// Thread process the work 
						DWORD id;
						h[1] = CreateThread(NULL, 0, ThreadFnLog, hwnd, 0, &id);
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
		case IDC_CHECKBOX1: // LOOP Flag
		{
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
		case IDC_BUTTON4: // STOP
		{
			play = FALSE;
		}
		break;
		case IDC_BUTTON3: // PLAY/PAUSE
		{
			// Avoids resubmitting another audio processor thread
			if (play == TRUE) return 0;

			// old code that tried to do above
			play = FALSE;
			if (h[0] != NULL) {
				TerminateProcess(h[0], 0);
				//WaitForSingleObject(h[0], timeout);
				CloseHandle(h[0]);
				h[0] = NULL;
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
					IMMDevices.UnregisterAudioClient();
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
								h[0] = CreateThread(NULL, 0, ThreadFn, hWnd, 0, &id);
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
		case IDC_BUTTON2: // OPEN File
		{
			// This thread is handled locally here and can wait at adequate timeout without a problem
			DWORD id;
			HANDLE d;
			d = CreateThread(NULL, 0, ThreadFnFile, hWnd, 0, &id);
			WaitForSingleObject(d, 0);
		}
		break;
		case IDC_COMBOBOX1: // DEVICE SELECTION
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
			case EN_SETFOCUS: // WHEN THIS CONTROL RECIEVES focus
				break;
			}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT: // PAINTING
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT r;
		GetClientRect(hWnd, &r);
		SetTextColor(hdc, RGB(14, 177, 227));
		SetBkMode(hdc, TRANSPARENT);
		r.left = 150;
		r.top = 500;
		r.right = 300;
		r.bottom = 550;
		int ret = DrawText(hdc, frames, -1, &r, DT_LEFT);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY: // EXITS ALL
	{
		// NOTE:  This is done here as a means to ensure the process ends before termination
		// of the main window and that we stop playing any lingering audio, typically
		// threads have to be wait on for completion, but as we are destroying all objects
		// and the threads are not performing anything out of the ordinary, all threads are
		// Terminated forcibly.
		// NOT CERTAIN this is the best way to handle this TODO
		play = FALSE;
		loop = FALSE;
		for (int i = 0; i < sizeof(h) / sizeof(h[0]); i++) {
			if (h[i] != NULL) {
				if (TerminateThread(h[i], 0) != 0) {
					CloseHandle(h[i]);
					h[i] = NULL;
				}
			}
		}
		// Free if dat was not destroyed
		if (dat != NULL) {
			free(dat);
			dat = NULL;
		}
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}