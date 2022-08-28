#include "AudioDeviceManager.h"

#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 3) 
	{
		printf("Please input a valid command and device option.\n");
		return 1; 
	}

	const auto COMMAND_SPECIFIER = argv[1];
	const auto DEVICE_SPECIFIER = argv[2];

	HRESULT hr;

	AudioDeviceManager* deviceManager = new AudioDeviceManager;

	// Attempts to initialize the COM interface.
	hr = CoInitializeEx(nullptr, tagCOINIT::COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { return 1; }

	// Process device modifier (get device(s) specified).
	if (strcmp(DEVICE_SPECIFIER, "-d") == 0 ||
		strcmp(DEVICE_SPECIFIER, "default") == 0)
	{
		hr = deviceManager->GetDefaultAudioDevice();
		if (FAILED(hr)) { return 1; }
	}
	else if (strcmp(DEVICE_SPECIFIER, "-a") == 0 ||
		strcmp(DEVICE_SPECIFIER, "all") == 0)
	{
		deviceManager->GetDevices();
		if (FAILED(hr)) { return 1; }
	}
	else {
		deviceManager->GetDevices();
		printf("Getting all devices to search for one.");
		
		// Check if can get device from ID.
	}

	// Process command modifier.
	if (strcmp(COMMAND_SPECIFIER, "-l") == 0 ||
		strcmp(COMMAND_SPECIFIER, "list") == 0)
	{
		hr = deviceManager->PrintDevices();
		if (FAILED(hr)) { return 1; }
	}
	else if (strcmp(COMMAND_SPECIFIER, "-m") == 0 ||
		strcmp(COMMAND_SPECIFIER, "mute") == 0)
	{
		// Mute devices.
	}
	else if (strcmp(COMMAND_SPECIFIER, "-u") == 0 ||
		strcmp(COMMAND_SPECIFIER, "unmute") == 0)
	{
		// unMute devices.
	}
	else if (strcmp(COMMAND_SPECIFIER, "-o") == 0 ||
		strcmp(COMMAND_SPECIFIER, "observe") == 0)
	{
		//Print devices specified to show initial data.
		hr = deviceManager->PrintDevices();
		if (FAILED(hr)) { return 1; }

		// Register for notifications.
		hr = deviceManager->RegisterForMMDeviceNotifications();
		if (FAILED(hr)) { printf("FAILED"); return 1; }

		// Loop to observe state changes.
		while (true)
		{
			Sleep(20000);
		}
	}


	//hr = deviceManager->GetDefaultAudioDevice();
	if (FAILED(hr)) { return 1; }

	//hr = deviceManager->GetDevices();
	if (FAILED(hr)) { return 1; }

	//hr = deviceManager->PrintDevices();
	if (FAILED(hr)) { return 1; }

	//LPCWSTR deviceID = (LPCWSTR)L"{0.0.1.00000000}.{a9c61a2a-bdcc-4f4b-9e98-fe919f6584c6}";
	//hr = deviceManager->PrintDevice(&deviceID);
	if (FAILED(hr)) { return 1; }

	hr = deviceManager->UnregisterFromMMDeviceNotifications();
	if (FAILED(hr)) { return 1; }

	CoUninitialize();
	return 0;
}