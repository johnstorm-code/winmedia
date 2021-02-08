#include "IPropertyStoreApi.h"

IPropertyStoreApi::IPropertyStoreApi() {
	this->_pProps = NULL;
}
IPropertyStoreApi::~IPropertyStoreApi() {
	if (this->_pProps != NULL) {
		this->_pProps->Release();
		this->_pProps = NULL;
	}
}
PROPERTYKEY IPropertyStoreApi::GetAt(DWORD index) {
	__objState__();
	PROPERTYKEY key;
	HRESULT hr = _pProps->GetAt(index, &key);
	if (hr != S_OK) throw hr;
	return key;
}
DWORD IPropertyStoreApi::GetCount() {
	__objState__();
	DWORD count;
	HRESULT hr = this->_pProps->GetCount(&count);
	if (hr != S_OK) return -1;
	return count;
}
PROPVARIANT IPropertyStoreApi::GetValue(REFPROPERTYKEY _pKey) {
	__objState__();
	PROPVARIANT propv;
	HRESULT hr = this->_pProps->GetValue(_pKey, &propv);
	if (hr != S_OK) throw hr;
	return propv;
}
HRESULT IPropertyStoreApi::SetPropStore(IPropertyStore* _pProps) {
	if (_pProps != NULL) {
		this->_pProps = _pProps;
	}
	return NULL;
}
BOOL IPropertyStoreApi::__objState__() {
	if (this->_pProps == NULL) throw L"IPropertyStore not initialized!";
	return TRUE;
}