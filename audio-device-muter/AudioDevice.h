#pragma once

#include "VolumeNotificationClient.h"

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <Functiondiscoverykeys_devpkey.h>

class AudioDevice
{
private:
	IMMDevice* device;
	IAudioEndpointVolume* endpointVolume;
	IPropertyStore* props;
	VolumeNotificationClient* volumeNotificationClient;

public:
	AudioDevice();

	AudioDevice(
		IMMDevice* device,
		IAudioEndpointVolume* endpointVolume,
		IPropertyStore* props
	);

	~AudioDevice();

	HRESULT ConstructFrom(IMMDevice* device);

	HRESULT GetFriendlyName(PROPVARIANT& deviceFriendlyname);
	HRESULT GetMMDeviceID(LPWSTR* deviceID);
	HRESULT GetState(DWORD* state);
	HRESULT GetMute(BOOL* isMuted);
	HRESULT SetMute(BOOL muteState);

	HRESULT Print();

	HRESULT RegisterForVolumeNotifications();
	HRESULT UnregisterFromVolumeNotifications();

	int ReleaseInterfaceReferences();
};