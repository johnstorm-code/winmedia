	#pragma once

/*
https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immnotificationclient
*/

#include "../immdevicesapi/NotificationClient.h"
#include "../resource.h"
#include "../application/Functions.h"

#include "../immdevicesapi/IMMDeviceCollectionApi.h"
#include "../immdevicesapi/IMMDeviceApi.h"
#include "../immdevicesapi/IPropertyStoreApi.h"

class ThroughSelect
{
	IMMDeviceCollectionApi _ODeviceCollectionApi;
	IMMDeviceApi _ODeviceApi;
	IPropertyStoreApi _OPropStoreApi;
	TCHAR _log[220];
public:
	ThroughSelect() : _log{ NULL } {}
	void Init(IMMDeviceCollection*collection)
	{
		if (collection == NULL) return;	
		_ODeviceCollectionApi.SetCollection(collection);
	}
	LPCWSTR ChangeCombobox(HWND combo, LPCWSTR pwstrDeviceId)
	{
		for (register UINT i = 0; i < _ODeviceCollectionApi.GetCount(); i++)
		{
			_ODeviceApi.SetDevice(_ODeviceCollectionApi.GetItem(i));
			if (wcscmp(_ODeviceApi.GetId(), pwstrDeviceId) == 0)
			{
				_OPropStoreApi.SetPropStore(_ODeviceApi.OpenPropertyStore());
				for (register UINT j = 0; j < _OPropStoreApi.GetCount(); j++)
				{
					if (_OPropStoreApi.GetAt(j) == PKEY_Device_FriendlyName)
					{
						SendMessage(combo, CB_DELETESTRING, i, 0);
						SendMessage(combo, CB_INSERTSTRING, i, (LPARAM)_OPropStoreApi.GetValue(_OPropStoreApi.GetAt(j)).pwszVal);
						SendMessage(combo, CB_SETCURSEL, i, 0);
						return _OPropStoreApi.GetValue(_OPropStoreApi.GetAt(j)).pwszVal;
					}
				}
			}
		}
		return L"";
	}
	LPCWSTR GetDeviceFriendlyName(LPCWSTR pwstrDeviceId)
	{
		for (register UINT i = 0; i < _ODeviceCollectionApi.GetCount(); i++)
		{
			_ODeviceApi.SetDevice(_ODeviceCollectionApi.GetItem(i));
			if (wcscmp(_ODeviceApi.GetId(), pwstrDeviceId) == 0) 
			{
				_OPropStoreApi.SetPropStore(_ODeviceApi.OpenPropertyStore());
				for (register UINT j = 0; j < _OPropStoreApi.GetCount(); j++)
				{
					if (_OPropStoreApi.GetAt(j) == PKEY_Device_FriendlyName)
					{
						return _OPropStoreApi.GetValue(_OPropStoreApi.GetAt(j)).pwszVal;
					}
				}
			}
		}
		return L"";
	}
	void log(LPCWSTR status, LPCWSTR log, HWND hwnd)
	{
		wcscpy_s(_log, L"");
		wcscat_s(_log, status);
		wcscat_s(_log, L" ");
		wcscat_s(_log, log);
		catout(L" ", hwnd);
		catout(_log, hwnd);
	}
};
IMMNotifyClient::IMMNotifyClient()
{
		_cRef = 0;
		_pEnumerator = NULL;
		Window = NULL;
}
IMMNotifyClient::~IMMNotifyClient()
{
	SAFE_RELEASE(_pEnumerator)
}
void IMMNotifyClient::caller(HWND hWnd, IMMDeviceEnumerator *_pEnumerator)
{
	this->_pEnumerator = _pEnumerator;
	this->Window = hWnd;
}
ULONG STDMETHODCALLTYPE IMMNotifyClient::AddRef()
{
		return InterlockedIncrement(&_cRef);
}
ULONG STDMETHODCALLTYPE IMMNotifyClient::Release()
{
	ULONG ulRef = InterlockedDecrement(&_cRef);
	if (0 == ulRef){
		delete this;
	}
	return ulRef;
}
HRESULT STDMETHODCALLTYPE IMMNotifyClient::QueryInterface(REFIID riid, VOID** ppvInterface)
{
	if (IID_IUnknown == riid)
	{
		AddRef();
		*ppvInterface = (IUnknown*)this;
	}
	else if (__uuidof(IMMNotificationClient) == riid)
	{
		AddRef();
		*ppvInterface = (IMMNotificationClient*)this;
	}
	else{
		*ppvInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IMMNotifyClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	// If the user has removed or disabled the default device for a particular role, 
	// and no other device is available to assume that role, then pwstrDefaultDevice is NULL.
	if (pwstrDeviceId == NULL) return S_OK;
	IMMDeviceCollection* ppdevs;
	ThroughSelect through;
	this->_pEnumerator->EnumAudioEndpoints(EDataFlow::eAll, DEVICE_STATE_ACTIVE, &ppdevs);
	through.Init(ppdevs);
	HWND hwnd;
	hwnd = GetDlgItem(this->Window, IDC_EDIT2);
	catout(L"Default Device Change: ", hwnd);
	catout(pwstrDeviceId, hwnd);
	catout(L" ", hwnd);
	catout(through.GetDeviceFriendlyName(pwstrDeviceId), hwnd);
	if (flow == EDataFlow::eCapture) {
		catout(L"eCapture", hwnd);
	}
	else if (flow == EDataFlow::eRender) {
		catout(L"eRender", hwnd);
	}
	catout(L" ", hwnd);
	if (role == ERole::eCommunications) {
		catout(L"eCommunications", hwnd);
	}
	else if (role == ERole::eConsole) {
		catout(L"eConsole", hwnd);
	}
	else if (role == ERole::eMultimedia) {
		catout(L"eMultimedia", hwnd);
	}
	catout(L"\r\n", hwnd);
	UINT lines = SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
	SendMessage(hwnd, EM_LINESCROLL, 0, lines);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IMMNotifyClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
	/*HWND hwnd;
	hwnd = GetDlgItem(this->Window, IDC_EDIT2);
	catout(L"Device Added: ", hwnd);
	catout(pwstrDeviceId, hwnd);
	catout(L" ", hwnd);
	catout(L"DEVICE_ADDED", hwnd);
	catout(L"\r\n", hwnd);
	UINT lines = SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
	SendMessage(hwnd, EM_LINESCROLL, 0, lines);*/
	return S_OK;
};
HRESULT STDMETHODCALLTYPE IMMNotifyClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
	/*HWND hwnd;
	hwnd = GetDlgItem(this->Window, IDC_EDIT2);
	catout(L"Device Removed: ", hwnd);
	catout(pwstrDeviceId, hwnd);
	catout(L" ", hwnd);
	catout(L"DEVICE_REMOVED", hwnd);
	catout(L"\r\n", hwnd);
	UINT lines = SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
	SendMessage(hwnd, EM_LINESCROLL, 0, lines);*/
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IMMNotifyClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId,	DWORD dwNewState)
{
	HWND hwnd = GetDlgItem(this->Window, IDC_EDIT2);
	catout(L"State Change: ", hwnd);
	catout(pwstrDeviceId, hwnd);
	HWND combo = GetDlgItem(this->Window, IDC_COMBOBOX1);
	IMMDeviceCollection* ppdevs;
	ThroughSelect through;
	switch (dwNewState) 
	{
	case DEVICE_STATE_ACTIVE:
		_pEnumerator->EnumAudioEndpoints(EDataFlow::eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED | DEVICE_STATE_UNPLUGGED, &ppdevs);
		through.Init(ppdevs);
		through.log(L"[DEVICE_ACTIVE]", through.ChangeCombobox(combo, pwstrDeviceId), hwnd);
		break;
	case DEVICE_STATE_DISABLED:
		_pEnumerator->EnumAudioEndpoints(EDataFlow::eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED | DEVICE_STATE_UNPLUGGED, &ppdevs);
		through.Init(ppdevs);
		through.log(L"[DEVICE_DISABLED]", through.ChangeCombobox(combo, pwstrDeviceId), hwnd);
		break;
	case DEVICE_STATE_NOTPRESENT:
		_pEnumerator->EnumAudioEndpoints(EDataFlow::eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED | DEVICE_STATE_UNPLUGGED|DEVICE_STATE_NOTPRESENT, &ppdevs);
		through.Init(ppdevs);
		through.log(L"[DEVICE_NOT_PRESENT]", through.ChangeCombobox(combo, pwstrDeviceId), hwnd);
		break;
	case DEVICE_STATE_UNPLUGGED:
		_pEnumerator->EnumAudioEndpoints(EDataFlow::eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED | DEVICE_STATE_UNPLUGGED, &ppdevs);
		through.Init(ppdevs);
		through.log(L"[DEVICE_UNPLUGGED]", through.ChangeCombobox(combo, pwstrDeviceId), hwnd);
		break;
	}
	catout(L"\r\n", hwnd);
	SendMessage(hwnd, EM_LINESCROLL, 0, SendMessage(hwnd, EM_GETLINECOUNT, 0, 0));
	return S_OK;
}
HRESULT STDMETHODCALLTYPE IMMNotifyClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId,const PROPERTYKEY key)
{
	/*HWND hwnd;
	hwnd = GetDlgItem(this->Window, IDC_EDIT2);
	catout(L"Property Change: ", hwnd);
	catout(pwstrDeviceId, hwnd);
	catout(L" ", hwnd);
	catout(L"TODO", hwnd);
	catout(L"\r\n", hwnd);
	UINT lines = SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
	SendMessage(hwnd, EM_LINESCROLL, 0, lines);*/
	return S_OK;
}
