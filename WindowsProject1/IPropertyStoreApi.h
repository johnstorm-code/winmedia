#pragma once

#include <mmdeviceapi.h>
#include <windowsx.h>
#include <Functiondiscoverykeys_devpkey.h>

class IPropertyStoreApi {
private:
	IPropertyStore* _pProps;
	BOOL __objState__();
public:
	IPropertyStoreApi();
	~IPropertyStoreApi();
	// TODO:  IPropertyStore::Commit
	PROPERTYKEY GetAt(DWORD index);
	DWORD GetCount();
	PROPVARIANT GetValue(REFPROPERTYKEY _pKey);
	// TODO:  IPropertyStore::SetValue
	HRESULT SetPropStore(IPropertyStore* _pStore);
};