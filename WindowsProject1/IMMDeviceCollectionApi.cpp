#include "IMMDeviceCollectionApi.h"

IMMDeviceCollectionApi::IMMDeviceCollectionApi() {
	_pCollection = NULL;
}
IMMDeviceCollectionApi::~IMMDeviceCollectionApi() {
	if (this->_pCollection != NULL) {
		this->_pCollection->Release();
		this->_pCollection = NULL;
	}
}
UINT IMMDeviceCollectionApi::GetCount() {
	__objState__();
	UINT count; HRESULT hr;
	if((hr = this->_pCollection->GetCount(&count)) != S_OK) return -1;
	return count;
}
IMMDevice* IMMDeviceCollectionApi::GetItem(UINT index) {
	__objState__();
	IMMDevice* device; HRESULT hr;
	if ((hr = this->_pCollection->Item(index, &device)) != S_OK) return NULL;
	return device;
}
HRESULT IMMDeviceCollectionApi::SetCollection(IMMDeviceCollection* _pCollection) {
	if (_pCollection != NULL) {
		this->_pCollection = _pCollection;
		return S_OK;
	}
	return NULL;
}
BOOL IMMDeviceCollectionApi::__objState__() {
	if (this->_pCollection == NULL) throw "IMMDeviceCollection not initialized!";
	return TRUE;
}