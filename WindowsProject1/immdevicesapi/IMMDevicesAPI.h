#pragma once

#include "IMMDeviceApi.h"
#include "IMMDeviceCollectionApi.h"
#include "IMMDeviceEnumerator.h"
#include "IPropertyStoreApi.h"
#include "IMMAudioClientApi.h"
#include "IMMAudioRenderClientApi.h"

class IMMDevicesApi:
private IMMDeviceEnumeratorApi,
private IMMDeviceCollectionApi, 
private IMMAudioClientApi,
private IMMAudioRenderClientApi
{
private:
	IMMDeviceApi _oDevice;
	IPropertyStoreApi _oDeviceProps;
	IMMNotifyClient _oDeviceNotifyClient;
	LPWSTR KeyValue(PROPERTYKEY _pKey);
	HRESULT EnumEndpoints();
	LPCWSTR wfstate(HRESULT hr);
public:
	IMMDevicesApi();
	~IMMDevicesApi();
	HRESULT RegisterNotifictionClient(HWND hWnd);
	HRESULT UnregisterNotifictionClient();
	UINT Count();
	LPCWSTR GetDeviceDataFlow();
	IMMDevicesApi* GetDefaultDevice(EDataFlow flow, ERole role);
	IMMDevicesApi* GetDeviceByIndex(UINT index);
	IMMDevicesApi* GetDeviceById(LPCWSTR pwstrId);
	LPWSTR GetId();
	UINT GetStateId();
	LPWSTR GetState();
	LPWSTR GetFriendlyName();
	LPWSTR GetInterfaceFriendlyName();
	LPWSTR GetDeviceDesc();
	LPWSTR GetEnumeratorName();
	LRESULT DumpDeviceProps(HWND hwnd, UINT index);
	HRESULT ActivateAudioClient();
	LPCWSTR GetAudioClientBufferSize();
	UINT GetAudioClientBufferSizeRaw();
	LPCWSTR GetAudioClientCurrentPadding();
	UINT GetAudioClientCurrentPaddingRaw();
	LPCWSTR GetAudioClientDevicePeriod();
	HRESULT GetAudioClientDevicePeriodRaw(REFERENCE_TIME *times[]);
	WAVEFORMATEX* GetAudioClientMixFormat();
	LPWSTR GetWaveFormatString(WORD _waveFormat);
	LPCWSTR GetAudioClientStreamLatency();
	REFERENCE_TIME GetAudioClientStreamLatencyRaw();
	HRESULT AudioClientIsFormatSupported(AUDCLNT_SHAREMODE  ShareMode, const WAVEFORMATEX* pFormat, WAVEFORMATEX *ppClosestMatch);
	LRESULT DumpAudioClientSupportedFormats(HWND hWnd, BOOL supportedonly);
	LPCWSTR MixFormatToString(WAVEFORMATEX *pFormat);
	HRESULT Initialize(AUDCLNT_SHAREMODE ShareMode, DWORD StreamFlags, REFERENCE_TIME hnsBufferDuration, REFERENCE_TIME hnsPeriodicity, const WAVEFORMATEX* pFormat, LPCGUID AudioSessionGuid);
	WAVEFORMATEX BuildWaveFormatEx(WORD formatTag, WORD nChannels, WORD bitsPerSample, DWORD samplesPerSecond);
	HRESULT InitRenderClient();
	HRESULT GetRenderClientBuffer(UINT numFramesRequested, BYTE** ppData);
	HRESULT ReleaseRenderClientBuffer(UINT numFramesWritten, DWORD dwFlags);
	HRESULT Start();
	HRESULT Stop();
	HRESULT Reset();
	BOOL IsAudioClientLoaded();
	HRESULT UnregisterAudioClient();
};