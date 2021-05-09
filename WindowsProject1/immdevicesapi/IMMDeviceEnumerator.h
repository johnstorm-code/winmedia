#pragma once

#include "IMMDevices.h"
#include "NotificationClient.h"

class IMMDeviceEnumeratorApi
{
private:
	IMMDeviceEnumerator* _pEnumerator;
	BOOL __objState__();
	BOOL __ncState__(IMMNotifyClient*client);
public:
	IMMDeviceCollection* EnumAudioEndpoints();
	IMMDevice* GetDefaultAudioEndpoint(EDataFlow flow, ERole role);
	IMMDevice* GetDevice(LPCWSTR pwstrId);
	IMMDeviceEnumeratorApi();
	~IMMDeviceEnumeratorApi();
	IMMDeviceEnumerator* ENUMERATOR();
	HRESULT Initialize();
	BOOL Initialized();
	HRESULT RegisterEndpointNotificationCallback(IMMNotifyClient* _pClient);
	HRESULT UnregisterEnpointNotificationCallback(IMMNotifyClient* _pClient);
};