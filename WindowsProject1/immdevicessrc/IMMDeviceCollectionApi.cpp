#include "../immdevicesapi/IMMDeviceCollectionApi.h"

/*
 IMMDeviceCollection 
 https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immdevicecollection
 */

#define ERROR_DEVICECOLLECTION_NOT_INITIALIZED L"IMMDeviceCollection not initialized!"

IMMDeviceCollectionApi::IMMDeviceCollectionApi()
{
	_pCollection = NULL;
}
IMMDeviceCollectionApi::~IMMDeviceCollectionApi()
{
	if (this->_pCollection != NULL) {
		this->_pCollection->Release();
		this->_pCollection = NULL;
	}
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevicecollection-getcount
UINT IMMDeviceCollectionApi::GetCount()
{
	__objState__();
	UINT count; HRESULT hr;
	if((hr = this->_pCollection->GetCount(&count)) != S_OK) return -1;
	return count;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdevicecollection-item
IMMDevice* IMMDeviceCollectionApi::GetItem(UINT index)
{
	__objState__();
	IMMDevice* device; HRESULT hr;
	if ((hr = this->_pCollection->Item(index, &device)) != S_OK) return NULL;
	return device;
}
HRESULT IMMDeviceCollectionApi::SetCollection(IMMDeviceCollection* _pCollection)
{
	if (_pCollection != NULL)
	{
		this->_pCollection = _pCollection;
		return S_OK;
	}
	return NULL;
}
BOOL IMMDeviceCollectionApi::__objState__()
{
	if (this->_pCollection == NULL) throw ERROR_DEVICECOLLECTION_NOT_INITIALIZED;
	return TRUE;
}