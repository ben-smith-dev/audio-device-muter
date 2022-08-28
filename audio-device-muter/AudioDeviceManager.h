#pragma once

#include "AudioDevice.h"
#include "MMDeviceNotificationClient.h"

#include <vector>
#include <mmdeviceapi.h>
#include <Windows.h>

class AudioDeviceManager
{
private:
	CONST EDataFlow DATA_FLOW = EDataFlow::eCapture;

	std::vector<AudioDevice*> devices;

	MMDeviceNotificationClient* mmNotificationClient;
	IMMDeviceEnumerator* deviceEnumerator;

	HRESULT IsDeviceInVector(LPCWSTR* deviceID, BOOL* inMap);

public:
	AudioDeviceManager();
	~AudioDeviceManager();

	HRESULT RegisterForMMDeviceNotifications();
	HRESULT UnregisterFromMMDeviceNotifications();

	HRESULT PrintDevices();
	HRESULT PrintDevice(LPCWSTR* deviceId);

	HRESULT MuteDevices();
	HRESULT UnmuteDevices();

	HRESULT MuteDevice(LPCWSTR* deviceID);
	HRESULT UnmuteDevice(LPCWSTR* deviceID);

	HRESULT GetDefaultAudioDevice();

	HRESULT GetDevices();
};