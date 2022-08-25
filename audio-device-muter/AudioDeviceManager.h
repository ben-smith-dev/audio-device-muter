#pragma once

#include "AudioDevice.h"

#include <unordered_map>
#include <mmdeviceapi.h>
#include <Windows.h>

class AudioDeviceManager
{
private:
	std::unordered_map<LPWSTR, AudioDevice*> devices;

public:
	HRESULT GetDefaultAudioDevice();
	HRESULT PrintDevies();
	HRESULT PrintDevice(LPWSTR* deviceId);

	std::unordered_map<LPWSTR, AudioDevice*> GetDevices();
};