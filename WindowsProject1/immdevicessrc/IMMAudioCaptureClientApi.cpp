#include "../immdevicesapi/IMMAudioCaptureClientApi.h"

/*
 IAudioCaptureClient
 https://docs.microsoft.com/en-us/windows/win32/api/audioclient/nn-audioclient-iaudiocaptureclient
 */

#define ERROR_AUDIOCAPTURE_NOT_INITIALIZED L"IAudioCaptureClient not initialized!";

IMMAudioCaptureClientApi::IMMAudioCaptureClientApi()
{
	_pAudioCaptureClient = NULL;
}
IMMAudioCaptureClientApi::~IMMAudioCaptureClientApi()
{
	if (_pAudioCaptureClient != NULL) {
		_pAudioCaptureClient->Release();
		_pAudioCaptureClient = NULL;
	}
}