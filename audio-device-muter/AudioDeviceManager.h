#pragma once

#include "AudioDevice.h"

#include <unordered_map>
#include <mmdeviceapi.h>
#include <Windows.h>

class AudioDeviceManager
{
private:
	CONST EDataFlow DATA_FLOW = EDataFlow::eCapture;

	std::unordered_map<LPWSTR, AudioDevice*> devices;

	HRESULT CheckIfDeviceInMap(LPCWSTR* deviceID, BOOL* inMap);

public:
	HRESULT PrintDevices();
	HRESULT PrintDevice(LPCWSTR* deviceId);

	HRESULT GetDefaultAudioDevice(AudioDevice* defaultDevice);
	HRESULT GetDefaultAudioDevice();

	HRESULT GetDevices(std::unordered_map<LPWSTR, AudioDevice*>* devices);
	HRESULT GetDevices();
};