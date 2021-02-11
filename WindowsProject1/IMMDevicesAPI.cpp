#include "IMMDevicesAPI.h"

#include "Functions.h"

IMMDevicesApi::IMMDevicesApi()
{
	IMMDeviceEnumeratorApi::Initialize();
	EnumEndpoints();
	_oDevice.SetDevice(IMMDeviceEnumeratorApi::GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia));
	_oDeviceProps.SetPropStore(_oDevice.OpenPropertyStore());
}
IMMDevicesApi::~IMMDevicesApi() {}
HRESULT IMMDevicesApi::RegisterNotifictionClient(HWND hWnd)
{
	if (!IMMDeviceEnumeratorApi::Initialized()) throw IMMDeviceEnumerator_NOT_INITIALIZED;
	_oDeviceNotifyClient.caller(hWnd, IMMDeviceEnumeratorApi::ENUMERATOR());
	return IMMDeviceEnumeratorApi::RegisterEndpointNotificationCallback(&_oDeviceNotifyClient);
}
HRESULT IMMDevicesApi::UnregisterNotifictionClient() {
	if (!IMMDeviceEnumeratorApi::Initialized()) throw IMMDeviceEnumerator_NOT_INITIALIZED;
	return IMMDeviceEnumeratorApi::UnregisterEnpointNotificationCallback(&_oDeviceNotifyClient);
}
UINT IMMDevicesApi::Count()
{
	return IMMDeviceCollectionApi::GetCount();
}
LPCWSTR IMMDevicesApi::GetDeviceDataFlow() {
	return this->_oDevice.GetDataFlow();
}
IMMDevicesApi* IMMDevicesApi::GetDefaultDevice(EDataFlow flow, ERole role)
{
	_oDevice.SetDevice(IMMDeviceEnumeratorApi::GetDefaultAudioEndpoint(flow, role));
	_oDeviceProps.SetPropStore(_oDevice.OpenPropertyStore());
	return this;
}
IMMDevicesApi* IMMDevicesApi::GetDeviceByIndex(UINT index)
{
	_oDevice.SetDevice(IMMDeviceCollectionApi::GetItem(index));
	_oDeviceProps.SetPropStore(_oDevice.OpenPropertyStore());
	return this;
}
IMMDevicesApi* IMMDevicesApi::GetDeviceById(LPCWSTR pwstrId)
{
	_oDevice.SetDevice(IMMDeviceEnumeratorApi::GetDevice(pwstrId));
	_oDeviceProps.SetPropStore(_oDevice.OpenPropertyStore());
	return this;
}
LPWSTR IMMDevicesApi::GetId()
{
	return _oDevice.GetId();
}
UINT IMMDevicesApi::GetStateId() {
	return _oDevice.GetState();
}
LPWSTR IMMDevicesApi::GetWaveFormatString(WORD _waveFormat) {
	switch (_waveFormat) {
	case WAVE_FORMAT_EXTENSIBLE:
		return (LPWSTR)L"WAVE_FORMAT_EXTENSIBLE";
		break;
	case WAVE_FORMAT_PCM:
		return (LPWSTR)L"WAVE_FORMAT_PCM";
		break;
	case WAVE_FORMAT_MPEGLAYER3:
		return (LPWSTR)L"WAVE_FORMAT_MPEGLAYER3";
		break;
	case WAVE_FORMAT_IEEE_FLOAT:
		return (LPWSTR)L"WAVE_FORMAT_IEEE_FLOAT";
		break;
	case WAVE_FORMAT_DRM:
		return (LPWSTR)L"WAVE_FORMAT_DRM";
		break;
	case WAVE_FORMAT_ALAW:
		return (LPWSTR)L"WAVE_FORMAT_ALAW";
		break;
	case WAVE_FORMAT_MULAW:
		return (LPWSTR)L"WAVE_FORMAT_MULAW";
		break;
	case WAVE_FORMAT_ADPCM:
		return (LPWSTR)L"WAVE_FORMAT_ADPCM";
		break;
	case WAVE_FORMAT_DOLBY_AC3_SPDIF:
		return (LPWSTR)L"WAVE_FORMAT_DOLBY_AC3_SPDIF";
		break;
	case WAVE_FORMAT_WMASPDIF:
		return (LPWSTR)L"WAVE_FORMAT_WMASPDIF";
	default:
		return (LPWSTR)L"OTHER";
		break;
	}
}
LPWSTR IMMDevicesApi::GetState()
{
	DWORD state = _oDevice.GetState();
	switch (state)
	{
	case DEVICE_STATE_ACTIVE:
		return (LPWSTR)L"DEVICE_ACTIVE";
		break;
	case DEVICE_STATE_DISABLED:
		return (LPWSTR)L"DEVICE_DISABLED";
		break;
	case DEVICE_STATE_NOTPRESENT:
		return (LPWSTR)L"DEVICE_NOT_PRESENT";
		break;
	case DEVICE_STATE_UNPLUGGED:
		return (LPWSTR)L"DEVICE_UNPLUGGED";
		break;
	default:
		return (LPWSTR)L"UNKNOWN_STATE";
	}
}
LPWSTR IMMDevicesApi::KeyValue(PROPERTYKEY _pKey)
{
	PROPVARIANT propv;
	propv = _oDeviceProps.GetValue(_pKey);
	return propv.pwszVal;
}
LPWSTR IMMDevicesApi::GetFriendlyName()
{
	return KeyValue(PKEY_Device_FriendlyName);
}
LPWSTR IMMDevicesApi::GetInterfaceFriendlyName()
{
	return KeyValue(PKEY_DeviceInterface_FriendlyName);
}
LPWSTR IMMDevicesApi::GetDeviceDesc()
{
	return KeyValue(PKEY_Device_DeviceDesc);
}
LPWSTR IMMDevicesApi::GetEnumeratorName()
{
	return KeyValue(PKEY_Device_EnumeratorName);
}
HRESULT IMMDevicesApi::ActivateAudioClient() {
	IAudioClient* client;
	client = this->_oDevice.ActivateAudioClient(CLSCTX_ALL, NULL);
	IMMAudioClientApi::LoadAudioClient(client);
	return S_OK;
}
HRESULT IMMDevicesApi::GetRenderClientBuffer(UINT numFramesRequested, BYTE** ppData) 
{
	return IMMAudioRenderClientApi::GetBuffer(numFramesRequested, ppData);
}
HRESULT IMMDevicesApi::ReleaseRenderClientBuffer(UINT numFramesWritten, DWORD dwFlags) 
{
	return IMMAudioRenderClientApi::ReleaseBuffer(numFramesWritten, dwFlags);
}
UINT IMMDevicesApi::GetAudioClientBufferSizeRaw()
{
	UINT syze = IMMAudioClientApi::GetBufferSize();
	return syze;
}
LPCWSTR IMMDevicesApi::GetAudioClientBufferSize() {
	const UINT len = 20;
	TCHAR* pszTxt = new TCHAR[len];
	UINT syze = IMMAudioClientApi::GetBufferSize();
	HRESULT hr = StringCbPrintf(pszTxt, len * sizeof(TCHAR), L"%d", syze);
	if (hr != S_OK) return L"";
	return pszTxt;
}
UINT IMMDevicesApi::GetAudioClientCurrentPaddingRaw()
{
	UINT syze = IMMAudioClientApi::GetCurrentPadding();
	return syze;
}
LPCWSTR IMMDevicesApi::GetAudioClientCurrentPadding() {
	const UINT len = 20;
	TCHAR* pszTxt = new TCHAR[len];
	UINT syze = IMMAudioClientApi::GetCurrentPadding();
	HRESULT hr = StringCbPrintf(pszTxt, len * sizeof(TCHAR), L"%d", syze);
	if (hr != S_OK) return L"";
	return pszTxt;
}
HRESULT IMMDevicesApi::GetAudioClientDevicePeriodRaw(REFERENCE_TIME *times[]) 
{
	HRESULT hr = IMMAudioClientApi::GetDevicePeriod(times[0], times[1]);
	return hr;
}
LPCWSTR IMMDevicesApi::GetAudioClientDevicePeriod() {
	REFERENCE_TIME rtd;
	REFERENCE_TIME rtm;
	HRESULT hr = IMMAudioClientApi::GetDevicePeriod(&rtd, &rtm);
	UINT len = 20;
	TCHAR* pszTxt = new TCHAR[len];
	hr = StringCbPrintf(pszTxt, len * sizeof(TCHAR), L"%ld %ld", (long)rtd, (long)rtm);
	if (hr != S_OK) return L"";
	return pszTxt;
}
LPCWSTR IMMDevicesApi::MixFormatToString(WAVEFORMATEX* pFormat) {
	UINT len = 200;
	TCHAR* wfm = new TCHAR[len];
	LPCTSTR format = TEXT("Type: %s, Channels: %d, SamplesPerSec: %d, AvgBytesPerSec: %d, BlockAlign: %d, BitsPerSample: %d, cbSize:  %d");
	HRESULT hr = StringCbPrintf(wfm, len * sizeof(TCHAR), format,
		GetWaveFormatString(pFormat->wFormatTag),				/// Waveform audio type
		pFormat->nChannels,				/// 1 mono, 2 stereo
		pFormat->nSamplesPerSec,			/// sample frequency
		pFormat->nAvgBytesPerSec,		/// required avg transfer rate p/sec
		pFormat->nBlockAlign,			/// set nBlockAlign to (nChannels*wBitsPerSample)/8 for WAVE_FORMAT_PCM
		pFormat->wBitsPerSample,			/// bits per sample
		pFormat->cbSize);				/// extra info appended to end of struct
	if (hr != S_OK) return L"";
	return wfm;
}
// https://docs.microsoft.com/en-us/windows/win32/api/mmreg/ns-mmreg-waveformatex
WAVEFORMATEX *IMMDevicesApi::GetAudioClientMixFormat() {
	WAVEFORMATEX* wfmtex;
	wfmtex = IMMAudioClientApi::GetMixFormat();
	return wfmtex;
}
HRESULT IMMDevicesApi::AudioClientIsFormatSupported(AUDCLNT_SHAREMODE  ShareMode, const WAVEFORMATEX* pFormat, WAVEFORMATEX *ppClosestMatch){
	WAVEFORMATEX* pClosestMatch;
	HRESULT hr = IMMAudioClientApi::IsFormatSupported(ShareMode, pFormat, &pClosestMatch);
	if(pClosestMatch != NULL) (*ppClosestMatch) = *pClosestMatch;
	if (hr != S_OK) return  hr;
	return hr;
}
LRESULT IMMDevicesApi::DumpDeviceProps(HWND hwnd, UINT index)
{
	EnumEndpoints();
	_oDevice.SetDevice(IMMDeviceCollectionApi::GetItem(index));
	_oDeviceProps.SetPropStore(_oDevice.OpenPropertyStore());
	TCHAR dump[2000];
	wcscpy_s(dump, L"");
	wcscat_s(dump, L"Device State: ");
	wcscat_s(dump, this->GetState());
	wcscat_s(dump, L"\r\n");
	wcscat_s(dump, L"Device Id: ");
	wcscat_s(dump, this->GetId());
	wcscat_s(dump, L"\r\n");
	wcscat_s(dump, L"Device Data Flow: ");
	wcscat_s(dump, this->GetDeviceDataFlow());
	wcscat_s(dump, L"\r\n");
	PROPERTYKEY Key;
	PROPVARIANT propv;
	for (DWORD i = 0; i < _oDeviceProps.GetCount(); i++)
	{
		Key = _oDeviceProps.GetAt(i);
		propv = _oDeviceProps.GetValue(Key);
		if (propv.vt == VT_LPWSTR)
		{
			if (Key == PKEY_AudioEndpoint_GUID) {
				wcscat_s(dump, L"PKEY_AudioEndpoint_GUID: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_DeviceInterface_FriendlyName) {
				wcscat_s(dump, L"PKEY_DeviceInterface_FriendlyName: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_Device_DeviceDesc) {
				wcscat_s(dump, L"PKEY_Device_DeviceDesc: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_Device_FriendlyName) {
				wcscat_s(dump, L"PKEY_Device_FriendlyName: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_Device_EnumeratorName) {
				wcscat_s(dump, L"PKEY_Device_EnumeratorName: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_DeviceClass_IconPath) {
				wcscat_s(dump, L"PKEY_DeviceClass_IconPath: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_AudioEndpoint_JackSubType) {
				wcscat_s(dump, L"PKEY_AudioEndpoint_JackSubType: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else if (Key == PKEY_AudioEndpoint_Association) {
				wcscat_s(dump, L"PKEY_AudioEndpoint_Association: ");
				wcscat_s(dump, propv.pwszVal);
			}
			else {
				TCHAR propstr[PKEYSTR_MAX];
				PSStringFromPropertyKey(Key, propstr, PKEYSTR_MAX);
				wcscat_s(dump, propstr);
				wcscat_s(dump, L": ");
				wcscat_s(dump, propv.pwszVal);
			}
			wcscat_s(dump, L"\r\n");
		}
		PropVariantClear(&propv);
	}
	return SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)dump);
}
HRESULT IMMDevicesApi::EnumEndpoints() 
{
	return IMMDeviceCollectionApi::SetCollection(IMMDeviceEnumeratorApi::EnumAudioEndpoints());
}
REFERENCE_TIME IMMDevicesApi::GetAudioClientStreamLatencyRaw()
{
	return IMMAudioClientApi::GetStreamLatency();
}
LPCWSTR IMMDevicesApi::GetAudioClientStreamLatency() {
	REFERENCE_TIME rtslat;
	rtslat = IMMAudioClientApi::GetStreamLatency();
	const UINT len = 20;
	TCHAR* pszTxt = new TCHAR[len];
	HRESULT hr = StringCbPrintf(pszTxt, len * sizeof(TCHAR), L"%ld", (long)rtslat);
	if (hr != S_OK) return L"";
	return pszTxt;
}
WAVEFORMATEX IMMDevicesApi::BuildWaveFormatEx(WORD formatTag, WORD nChannels, WORD bitsPerSample, DWORD samplesPerSecond) {
	WAVEFORMATEX wf;
	wf.wFormatTag = formatTag;
	wf.nChannels = nChannels;
	wf.wBitsPerSample = bitsPerSample;
	wf.cbSize = 0;
	wf.nSamplesPerSec = samplesPerSecond;
	wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample) / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	return wf;
}
LRESULT IMMDevicesApi::DumpAudioClientSupportedFormats(HWND hWnd){
	WAVEFORMATEX wf;
	WAVEFORMATEX match;
	AUDCLNT_SHAREMODE mode = AUDCLNT_SHAREMODE_SHARED;
	HRESULT hr = S_OK;
	LPCWSTR format = TEXT("%.1fK %d-bit %s %s %s");
	const UINT len = 100;
	TCHAR dump[len];
	long bitRate[] = { 11025, 22050, 44100, 48000, 88200, 96000, 176400, 192000 };
	int bits[] = { 4, 8, 16, 24, 32, 64, 128, 256 };
	LPCWSTR ch[] = { L"MONO", L"STEREO" };  //0 = 1ch, 1 = 2ch, etc
	int maxChannels = 2;

	/****************** Performs Query on PCM format support ************************/
	for (int channel = 0; channel < maxChannels; channel++)
	{
		for (int bit = 0; bit < (sizeof(bits)/sizeof(bits[0])); bit++) 
		{
			for (int rate = 0; rate < (sizeof(bitRate)/sizeof(bitRate[0])); rate++) 
			{
				wf = BuildWaveFormatEx(WAVE_FORMAT_PCM, channel+1, bits[bit], bitRate[rate]);
				if ((hr = AudioClientIsFormatSupported(mode, &wf, &match)) == S_OK) {
					StringCbPrintf(dump, len * sizeof(TCHAR), format, (bitRate[rate]/1000.0), bits[bit], ch[channel], GetWaveFormatString(WAVE_FORMAT_PCM), wfstate(hr));
					catout(dump, hWnd);
					catout(L"\r\n", hWnd);
				}
				else if (hr == S_FALSE) {
					// TODO: 
					StringCbPrintf(dump, len * sizeof(TCHAR), format, (bitRate[rate] / 1000.0), bits[bit], ch[channel], GetWaveFormatString(WAVE_FORMAT_PCM), wfstate(hr));
					catout(dump, hWnd);
					catout(L"\r\n", hWnd);
					catout(MixFormatToString(&match), hWnd);
					catout(L"\r\n", hWnd);
				}
			}
		}
	}
	return S_OK;
}
LPCWSTR IMMDevicesApi::wfstate(HRESULT hr) {
	switch (hr) {
	case S_OK:
		return (LPCWSTR)L"FORMAT_SUPPORTED";
		break;
	case E_POINTER:
		return (LPCWSTR)L"E_POINTER";
		break;
	case E_INVALIDARG:
		return (LPCWSTR)L"E_INVALIDARG";
		break;
	case AUDCLNT_E_UNSUPPORTED_FORMAT:
		return (LPCWSTR)L"AUDCLNT_E_UNSUPPORTED_FORMAT";
		break;
	case S_FALSE:
		return (LPCWSTR)L"CLOSEST_MATCH_FOUND";
		break;
	}
	return L"";
}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-initialize
HRESULT IMMDevicesApi::Initialize(AUDCLNT_SHAREMODE  ShareMode,	DWORD StreamFlags, REFERENCE_TIME hnsBufferDuration, REFERENCE_TIME hnsPeriodicity, const WAVEFORMATEX* pFormat, LPCGUID AudioSessionGuid){
	return IMMAudioClientApi::Initialize(ShareMode, StreamFlags, hnsBufferDuration, hnsPeriodicity, pFormat, AudioSessionGuid);
}
HRESULT IMMDevicesApi::InitRenderClient() 
{
	IAudioRenderClient *_pRenderClient;
	HRESULT hr = IMMAudioClientApi::GetService(__uuidof(IAudioRenderClient), &_pRenderClient);
	IMMAudioRenderClientApi::Initialize(_pRenderClient);
	return hr;
}
HRESULT IMMDevicesApi::Start() {
	return IMMAudioClientApi::Start();
}
HRESULT IMMDevicesApi::Stop() {
	return IMMAudioClientApi::Stop();
}