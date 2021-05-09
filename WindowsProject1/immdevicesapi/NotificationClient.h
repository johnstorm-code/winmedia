#pragma once

#include "../application/framework.h"

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

//https://docs.microsoft.com/en-us/windows/win32/coreaudio/device-events
class IMMNotifyClient : public IMMNotificationClient{
	LONG _cRef;
	IMMDeviceEnumerator* _pEnumerator;
	HWND Window;
public:
	IMMNotifyClient();
	~IMMNotifyClient();
	void caller(HWND hWnd, IMMDeviceEnumerator *_pEnumerator);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface);
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role,LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId,DWORD dwNewState);
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId,const PROPERTYKEY key);
};
