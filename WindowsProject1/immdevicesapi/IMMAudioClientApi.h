#pragma once

#include "IMMDevices.h"

class IMMAudioClientApi
{
private:
	IAudioClient* _pAudioClient;
	BOOL __objState__();
public:
	IMMAudioClientApi();
	~IMMAudioClientApi();
	UINT32 GetBufferSize();
	UINT32 GetCurrentPadding();
	HRESULT GetDevicePeriod(REFERENCE_TIME* rtdefault, REFERENCE_TIME* rtminimum);
	WAVEFORMATEX* GetMixFormat();
	REFERENCE_TIME GetStreamLatency();
	HRESULT GetService(REFIID riid, IAudioRenderClient** _pRenderClient);
	HRESULT Initialize(
		AUDCLNT_SHAREMODE  ShareMode, 
		DWORD StreamFlags, 
		REFERENCE_TIME hnsBufferDuration, 
		REFERENCE_TIME hnsPeriodicity,
		const WAVEFORMATEX* pFormat,
		LPCGUID AudioSessionGuid);
	HRESULT IsFormatSupported(AUDCLNT_SHAREMODE  ShareMode, const WAVEFORMATEX* pFormat, WAVEFORMATEX** ppClosestMatch);
	HRESULT Reset();
	HRESULT Start();
	HRESULT Stop();
	HRESULT LoadAudioClient(IAudioClient* _pAudioClient);
	HRESULT SetEventHandle(HANDLE _pEventHandle);
	BOOL Loaded();
};