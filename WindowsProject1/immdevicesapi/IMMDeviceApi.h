#pragma once

#include "IMMDevices.h"

class IMMDeviceApi
{
private:
	IMMDevice* _pDevice;
	BOOL __objState__();
public:
	IMMDeviceApi();
	~IMMDeviceApi();
	IAudioClient* ActivateAudioClient(DWORD dwClsCtx, PROPVARIANT* pActibvationParams);
	LPCWSTR GetDataFlow();
	LPWSTR GetId();
	DWORD GetState();
	HRESULT SetDevice(IMMDevice* pDevice);
	IPropertyStore* OpenPropertyStore();
};