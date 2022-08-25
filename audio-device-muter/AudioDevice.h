#pragma once

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class AudioDevice
{
private:
	IMMDevice* device;
	IAudioEndpointVolume* endpointVolume;
	IPropertyStore* props;

public:
	AudioDevice(
		IMMDevice* device,
		IAudioEndpointVolume* endpointVolume,
		IPropertyStore* props
	);

	HRESULT GetFriendlyName(LPWSTR* deviceFriendlyname);
	HRESULT GetState(DWORD* state);
	HRESULT GetMute(BOOL* isMuted);
	HRESULT SetMute(BOOL muteState);
	HRESULT Print();

	int ReleaseInterfaceReferences();
};