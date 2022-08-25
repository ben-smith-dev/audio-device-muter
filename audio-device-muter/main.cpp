#include "AudioDeviceManager.h"

#include <stdio.h>

int main()
{
	HRESULT hr;

	AudioDeviceManager* deviceManager = new AudioDeviceManager;

	hr = deviceManager->GetDefaultAudioDevice();
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->PrintDevices();
	if (FAILED(hr)) { return 1; }

	return 0;
}
