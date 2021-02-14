#pragma once

#include "IMMDevices.h"

class IMMDeviceCollectionApi
{
private:
	IMMDeviceCollection* _pCollection;
	BOOL __objState__();
public:
	IMMDeviceCollectionApi();
	~IMMDeviceCollectionApi();
	UINT GetCount();
	IMMDevice* GetItem(UINT index);
	HRESULT SetCollection(IMMDeviceCollection* _pCollection);
};