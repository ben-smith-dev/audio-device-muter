#include "AudioDeviceManager.h"

#include <stdio.h>

int main()
{
	HRESULT hr;

	AudioDeviceManager* deviceManager = new AudioDeviceManager;

	// Attempts to initialize the COM interface.
	hr = CoInitializeEx(nullptr, tagCOINIT::COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { return 1; }

	//hr = deviceManager->GetDefaultAudioDevice();
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->GetDevices();
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->PrintDevices();
	if (FAILED(hr)) { return 1; }

	//LPCWSTR deviceID = (LPCWSTR)L"{0.0.1.00000000}.{a9c61a2a-bdcc-4f4b-9e98-fe919f6584c6}";
	//hr = deviceManager->PrintDevice(&deviceID);
	if (FAILED(hr)) { return 1; }

	// Testing Notifaction callbacks.
	while (true)
	{
		Sleep(10000);
		printf("...\n");
	}

	hr = deviceManager->UnregisterFromMMDeviceNotifications();
	if (FAILED(hr)) { return 1; }

	CoUninitialize();
	return 0;
}
