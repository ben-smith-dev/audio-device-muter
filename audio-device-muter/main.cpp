#include "AudioDeviceManager.h"

#include <stdio.h>

int main()
{
	HRESULT hr;

	AudioDeviceManager* deviceManager = new AudioDeviceManager;

	// Attempts to initialize the COM interface.
	hr = CoInitializeEx(nullptr, tagCOINIT::COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->GetDefaultAudioDevice();
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->GetDevices();
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->PrintDevices();
	if (FAILED(hr)) { return 1; }


	CoUninitialize();
	return 0;
}
