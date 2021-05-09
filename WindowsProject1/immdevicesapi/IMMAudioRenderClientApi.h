#pragma once

#include "IMMDevices.h"

class IMMAudioRenderClientApi
{
private:
	IAudioRenderClient* _pAudioRenderClient;
public:
	IMMAudioRenderClientApi();
	~IMMAudioRenderClientApi();
	HRESULT Initialize(IAudioRenderClient *_pRenderClient);
	HRESULT GetBuffer(UINT numFramesRequested, BYTE **ppData);
	HRESULT ReleaseBuffer(UINT numFramesWritten, DWORD dwFlags);
};