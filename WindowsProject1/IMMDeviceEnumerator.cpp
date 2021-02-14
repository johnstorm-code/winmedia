#include "IMMDeviceEnumerator.h"

/*
IMMDeviceEnumerator
https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immdeviceenumerator
*/

#define ERROR_DEVICEENUMERATOR_NOT_INITIALIZED L"ERROR: IMMDeviceEnumerator not initialized!"

IMMDeviceEnumeratorApi::IMMDeviceEnumeratorApi()
{
	this->_pEnumerator = NULL;
}
IMMDeviceEnumeratorApi::~IMMDeviceEnumeratorApi()
{
	if (this->_pEnumerator != NULL) {
		this->_pEnumerator->Release();
		this->_pEnumerator = NULL;
	}
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-enumaudioendpoints
IMMDeviceCollection* IMMDeviceEnumeratorApi::EnumAudioEndpoints()
{
	__objState__();
	IMMDeviceCollection* collection;
	HRESULT hr = this->_pEnumerator->EnumAudioEndpoints(
		eAll,
		DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED | DEVICE_STATE_UNPLUGGED,
		&collection);
	if (hr != S_OK) return NULL;
	return collection;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-getdefaultaudioendpoint
IMMDevice* IMMDeviceEnumeratorApi::GetDefaultAudioEndpoint(EDataFlow flow, ERole role)
{
	__objState__();
	IMMDevice* device;
	HRESULT hr = this->_pEnumerator->GetDefaultAudioEndpoint(flow, role, &device);
	if (hr != S_OK) return NULL;
	return device;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-getdevice
IMMDevice* IMMDeviceEnumeratorApi::GetDevice(LPCWSTR pwstrId)
{
	__objState__();
	IMMDevice* device;
	HRESULT hr = this->_pEnumerator->GetDevice(pwstrId, &device);
	if (hr != S_OK) return NULL;
	return device;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-registerendpointnotificationcallback
HRESULT IMMDeviceEnumeratorApi::RegisterEndpointNotificationCallback(IMMNotifyClient* _pClient)
{
	__ncState__(_pClient);
	return this->_pEnumerator->RegisterEndpointNotificationCallback(_pClient);
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-unregisterendpointnotificationcallback
HRESULT IMMDeviceEnumeratorApi::UnregisterEnpointNotificationCallback(IMMNotifyClient* _pClient)
{
	__ncState__(_pClient);
	return this->_pEnumerator->UnregisterEndpointNotificationCallback(_pClient);
}
IMMDeviceEnumerator* IMMDeviceEnumeratorApi::ENUMERATOR()
{
	return this->_pEnumerator;
}
HRESULT IMMDeviceEnumeratorApi::Initialize()
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	HRESULT hr = CoInitialize(this->_pEnumerator);
	if (hr != S_OK) return  hr;
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		NULL,
		CLSCTX_ALL,
		IID_IMMDeviceEnumerator,
		(void**)&this->_pEnumerator);
	if (hr != S_OK) return hr;
	return hr;
}
BOOL IMMDeviceEnumeratorApi::Initialized()
{
	if (this->_pEnumerator != NULL) return TRUE;
	return FALSE;
}
BOOL IMMDeviceEnumeratorApi::__objState__()
{
	if (this->_pEnumerator == NULL) throw ERROR_DEVICEENUMERATOR_NOT_INITIALIZED;
	return TRUE;
}
BOOL IMMDeviceEnumeratorApi::__ncState__(IMMNotifyClient* _pClient)
{
	if (_pClient == NULL) throw L"IMMNotifyClient not initialized!";
	return TRUE;
}