#include "IMMAudioClientApi.h"

IMMAudioClientApi::IMMAudioClientApi() {
	this->_pAudioClient = NULL;
}
IMMAudioClientApi::~IMMAudioClientApi() {
	if (this->_pAudioClient != NULL) {
		this->_pAudioClient->Release();
		this->_pAudioClient = NULL;
	}

}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-getbuffersize
UINT IMMAudioClientApi::GetBufferSize() {
	__objState__();
	UINT32 buffsize = 0;
	HRESULT hr = this->_pAudioClient->GetBufferSize(&buffsize);
	if (hr != S_OK) throw "ERROR: GetBufferSize() IAudioClient loaded but not initialized!";
	return buffsize;
}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-getcurrentpadding
UINT IMMAudioClientApi::GetCurrentPadding() {
	__objState__();
	UINT32 currpaddframes = 0;
	HRESULT hr = this->_pAudioClient->GetCurrentPadding(&currpaddframes);
	if (hr != S_OK) throw "ERROR: GetCurrentPadding() IAudioClient loaded but not initialized!";
	return currpaddframes;
}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-getdeviceperiod
HRESULT IMMAudioClientApi::GetDevicePeriod(REFERENCE_TIME *rtdefault, REFERENCE_TIME *rtminimum) {
	__objState__();
	HRESULT hr = this->_pAudioClient->GetDevicePeriod(rtdefault, rtminimum);
	if (hr != S_OK) hr;
	return hr;
}
WAVEFORMATEX* IMMAudioClientApi::GetMixFormat() {
	__objState__();
	WAVEFORMATEX* wfmtex;
	HRESULT hr = this->_pAudioClient->GetMixFormat(&wfmtex);
	if (hr != S_OK) return NULL;
	return wfmtex;
}
REFERENCE_TIME IMMAudioClientApi::GetStreamLatency() {
	__objState__();
	REFERENCE_TIME reftime;
	HRESULT hr = this->_pAudioClient->GetStreamLatency(&reftime);
	if (hr != S_OK) throw "ERROR: GetStreamLatency() IAudioClient loaded but not initialized!";
	return reftime;
}
HRESULT IMMAudioClientApi::GetService(REFIID riid, IAudioRenderClient **_pRenderClient) {
	__objState__();
	IAudioRenderClient* client;
	HRESULT hr = this->_pAudioClient->GetService(riid, (void**)&client);
	if (hr == S_OK) (*_pRenderClient) = client;
	return hr;
}
HRESULT IMMAudioClientApi::Initialize(
	AUDCLNT_SHAREMODE  ShareMode,
	DWORD StreamFlags,
	REFERENCE_TIME hnsBufferDuration,
	REFERENCE_TIME hnsPeriodicity,
	const WAVEFORMATEX* pFormat,
	LPCGUID AudioSessionGuid) {
	__objState__();
	HRESULT hr = this->_pAudioClient->Initialize(ShareMode, StreamFlags, hnsBufferDuration, hnsPeriodicity, pFormat, AudioSessionGuid);
	if (hr != S_OK) throw hr;
	return hr;
}
HRESULT IMMAudioClientApi::IsFormatSupported(AUDCLNT_SHAREMODE ShareMode, const WAVEFORMATEX* pFormat, WAVEFORMATEX** ppClosestMatch) {
	__objState__();
	/// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-isformatsupported
	HRESULT hr = this->_pAudioClient->IsFormatSupported(ShareMode, pFormat, ppClosestMatch);
	return hr;
}
HRESULT IMMAudioClientApi::Reset() {
	__objState__();
	HRESULT hr = this->_pAudioClient->Reset();
	if (hr != S_OK) throw "ERROR: Reset() IAudioClient loaded but not initialized!";
	return hr;
}
HRESULT IMMAudioClientApi::Start() {
	__objState__();
	HRESULT hr = this->_pAudioClient->Start();
	if (hr != S_OK) throw "ERROR: Start() IAudioClient loaded but not initialized!";
	return hr;
}
HRESULT IMMAudioClientApi::Stop() {
	__objState__();
	HRESULT hr = this->_pAudioClient->Stop();
	if (hr != S_OK) throw "ERROR: Stop() IAudioClient loaded but not initialized!";
	return hr;
}
HRESULT IMMAudioClientApi::LoadAudioClient(IAudioClient* _pAudioClient) {
	if (_pAudioClient != NULL) {
		this->_pAudioClient = _pAudioClient;
		return S_OK;
	}
	return S_FALSE;
}
BOOL IMMAudioClientApi::__objState__() {
	if (this->_pAudioClient == NULL) {
		throw "IAudioClient not initialized!";
		return FALSE;
	}
	return TRUE;
}
HRESULT IMMAudioClientApi::SetEventHandle(HANDLE _pEventHandle) {
	__objState__();
	HRESULT hr = this->_pAudioClient->SetEventHandle(_pEventHandle);
	if (hr != S_OK) throw "ERROR: SetEventHandle() IAudioClient loaded but not initialized!";
	return hr;
}