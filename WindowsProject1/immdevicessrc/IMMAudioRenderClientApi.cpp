#include "../immdevicesapi/IMMAudioRenderClientApi.h"

/*
IAudioRenderClient doc
https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nn-audioclient-iaudiorenderclient
*/

#define ERROR_AUDIORENDER_NOT_INITIALIZED "IAudioClient not initialized!"

IMMAudioRenderClientApi::IMMAudioRenderClientApi()
{
	this->_pAudioRenderClient = NULL;
}
IMMAudioRenderClientApi::~IMMAudioRenderClientApi()
{
	if (this->_pAudioRenderClient != NULL)
	{
		this->_pAudioRenderClient->Release();
		this->_pAudioRenderClient = NULL;
	}
}
HRESULT IMMAudioRenderClientApi::Initialize(IAudioRenderClient* _pRenderClient)
{
	if (_pRenderClient != NULL)
	{
		this->_pAudioRenderClient = _pRenderClient;
		return S_OK;
	}
	return S_FALSE;
}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudiorenderclient-getbuffer
HRESULT IMMAudioRenderClientApi::GetBuffer(UINT numFramesRequested, BYTE **ppData) 
{
	if (this->_pAudioRenderClient == NULL) throw ERROR_AUDIORENDER_NOT_INITIALIZED;
	return this->_pAudioRenderClient->GetBuffer(numFramesRequested, ppData);
}
// https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudiorenderclient-releasebuffer
HRESULT IMMAudioRenderClientApi::ReleaseBuffer(UINT numFramesWritten, DWORD dwFlags) 
{
	if (this->_pAudioRenderClient == NULL) throw ERROR_AUDIORENDER_NOT_INITIALIZED;
	return this->_pAudioRenderClient->ReleaseBuffer(numFramesWritten, dwFlags);
}
