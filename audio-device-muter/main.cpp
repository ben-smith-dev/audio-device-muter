#include "AudioDeviceManager.h"

#include <stdio.h>
#include <string>

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 3) 
	{
		printf("Please input a valid command and device option.\n");

		for (int i = 1; i < argc; i += 1)
		{
			printf("ARG:\t%S\n", argv[i]);
		}

		return 1;
	}

	for (int i = 1; i < argc; i += 1)
	{
		printf("ARG:\t%S\n", argv[i]);
	}

	const auto COMMAND_SPECIFIER = argv[1];
	const auto DEVICE_SPECIFIER = argv[2];

	HRESULT hr;

	AudioDeviceManager* deviceManager = new AudioDeviceManager;

	// Attempts to initialize the COM interface.
	hr = CoInitializeEx(nullptr, tagCOINIT::COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { return 1; }

	// Process device modifier (get device(s) specified).

	// DEFAULT DEVICE
	if (lstrcmpW(DEVICE_SPECIFIER, L"-d") == 0 ||
		lstrcmpW(DEVICE_SPECIFIER, L"default") == 0)
	{
		hr = deviceManager->GetDefaultAudioDevice();
		if (FAILED(hr)) { return 1; }
	}

	// ALL DEVICES
	else if (lstrcmpW(DEVICE_SPECIFIER, L"-a") == 0 ||
		lstrcmpW(DEVICE_SPECIFIER, L"all") == 0)
	{
		deviceManager->GetDevices();
		if (FAILED(hr)) { return 1; }
	}

	// SINGLE DEVICE (IF valid ID)
	else {
		hr = deviceManager->GetDevices();
		if (FAILED(hr)) { return 1; }

		printf("Getting all devices to search for one.");
		
		// Check if it can get device from ID.
		hr = deviceManager->PrintDevice((LPCWSTR*)&DEVICE_SPECIFIER);
		if (FAILED(hr)) { printf("FAILED\n"); return 1; }

		return 0;
	}

	// Process command modifier.

	// LIST
	if (lstrcmpW(COMMAND_SPECIFIER, L"-l") == 0 ||
		lstrcmpW(COMMAND_SPECIFIER, L"list") == 0)
	{
		hr = deviceManager->PrintDevices();
		if (FAILED(hr)) { return 1; }
	}

	// MUTE
	else if (lstrcmpW(COMMAND_SPECIFIER, L"-m") == 0 ||
		lstrcmpW(COMMAND_SPECIFIER, L"mute") == 0)
	{
		// Mute devices.
	}

	// UNMUTE
	else if (lstrcmpW(COMMAND_SPECIFIER, L"-u") == 0 ||
		lstrcmpW(COMMAND_SPECIFIER, L"unmute") == 0)
	{
		// unMute devices.
	}

	// OBSERVE
	else if (lstrcmpW(COMMAND_SPECIFIER, L"-o") == 0 ||
		lstrcmpW(COMMAND_SPECIFIER, L"observe") == 0)
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