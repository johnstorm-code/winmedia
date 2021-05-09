#include "../immdevicesapi/IMMDeviceApi.h"

/*
IMMDevice docs
https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immdevice
*/

#define ERROR_DEVICE_NOT_INITIALIZED L"IMMDevice not initialized!"

IMMDeviceApi::IMMDeviceApi()
{
	this->_pDevice = NULL;
}
IMMDeviceApi::~IMMDeviceApi()
{
	if (this->_pDevice != NULL)
	{
		this->_pDevice->Release();
		this->_pDevice = NULL;
	}
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevice-activate
IAudioClient *IMMDeviceApi::ActivateAudioClient(DWORD dwClsCtx, PROPVARIANT *pActibvationParams)
{
	__objState__();
	IAudioClient* ppInterface;
	HRESULT hr = this->_pDevice->Activate(__uuidof(IAudioClient), dwClsCtx, pActibvationParams, (void**)&ppInterface);
	if (hr == AUDCLNT_E_DEVICE_INVALIDATED) {}; // TODO - audio endpoints unplugged or unable for use
	if (hr != S_OK) throw hr;
	return ppInterface;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immendpoint
LPCWSTR IMMDeviceApi::GetDataFlow()
{
	IMMEndpoint* endpoint;
	EDataFlow flow;
	HRESULT hr = this->_pDevice->QueryInterface(__uuidof(IMMEndpoint),(void**)&endpoint);
	if (hr != S_OK) throw hr;
	endpoint->GetDataFlow(&flow);
	endpoint->Release(); endpoint = NULL;
	if (flow == EDataFlow::eCapture) {
		return L"eCapture";
	}
	else if (flow == EDataFlow::eRender) {
		return L"eRender";
	}
	else if (flow == EDataFlow::eAll) {
		return L"eALL";
	}
	return L"";
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevice-getid
LPWSTR IMMDeviceApi::GetId()
{
	__objState__();
	LPWSTR Id; HRESULT hr;
	if( (hr = this->_pDevice->GetId(&Id)) != S_OK) return NULL;
	return Id;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevice-getstate
DWORD IMMDeviceApi::GetState()
{
	__objState__();
	DWORD state; HRESULT hr;
	if((hr = this->_pDevice->GetState(&state)) != S_OK) return NULL;
	return state;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevice-openpropertystore
IPropertyStore* IMMDeviceApi::OpenPropertyStore()
{
	__objState__();
	IPropertyStore* props; HRESULT hr;
	if((hr = this->_pDevice->OpenPropertyStore(STGM_READ, &props)) != S_OK) return NULL;
	return props;
}
HRESULT IMMDeviceApi::SetDevice(IMMDevice* _pDevice)
{
	if (_pDevice != NULL) {
		this->_pDevice = _pDevice;
		return S_OK;
	}
	return NULL;
}
BOOL IMMDeviceApi::__objState__()
{
	if (_pDevice == NULL) {
		throw ERROR_DEVICE_NOT_INITIALIZED;
		return FALSE;
	}
	return TRUE;
}
