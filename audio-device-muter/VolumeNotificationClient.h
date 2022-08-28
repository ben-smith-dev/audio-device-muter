#pragma once

#include <endpointvolume.h>

class VolumeNotificationClient : public IAudioEndpointVolumeCallback
{
private:
	LONG cRef;
	
	// Used to determine if a notification is needed. 
	BOOL lastMuteState;

public:

	VolumeNotificationClient(BOOL muteState);

	// IUnkown methods

	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid,
		VOID** ppvInterface
	);

	// IAudioEndpointVolumeCallback method

	int OnNotify();
};

