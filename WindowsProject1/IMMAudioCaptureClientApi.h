#pragma once

#include "IMMDevices.h"

class IMMAudioCaptureClientApi
{
private:
	IAudioCaptureClient* _pAudioCaptureClient;
public:
	IMMAudioCaptureClientApi();
	~IMMAudioCaptureClientApi();
};