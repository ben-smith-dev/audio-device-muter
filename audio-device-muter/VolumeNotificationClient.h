#pragma once

#include <endpointvolume.h>
#include <stdio.h>

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

	HRESULT OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA notify);
};

