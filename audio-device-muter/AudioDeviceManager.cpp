#include "AudioDeviceManager.h"

AudioDeviceManager::AudioDeviceManager()
{
	deviceEnumerator = nullptr;
	mmNotificationClient = nullptr;
}

AudioDeviceManager::~AudioDeviceManager()
{
	UnregisterFromMMDeviceNotifications();
}

HRESULT AudioDeviceManager::GetDefaultAudioDevice()
{
	HRESULT hr;

	//Create device enumerator if needed.
	if (deviceEnumerator == nullptr) 
	{
		hr = CoCreateInstance(
			__uuidof(MMDeviceEnumerator),
			NULL,
			CLSCTX_ALL,
			__uuidof(IMMDeviceEnumerator),
			(LPVOID*)&deviceEnumerator
		);
		if (FAILED(hr)) { return hr; }

		// Register for MM Device notifications.
		mmNotificationClient = new MMDeviceNotificationClient(
			deviceEnumerator,
			EDataFlow::eCapture
		);
		if (FAILED(hr)) { return hr; }

		deviceEnumerator->RegisterEndpointNotificationCallback(
			mmNotificationClient
		);
		if (FAILED(hr)) { return hr; }
	}

	// Device data.
	IMMDevice* mmDevice = nullptr;

	// Gets default audio endpoint
	deviceEnumerator->GetDefaultAudioEndpoint(
		DATA_FLOW,
		ERole::eConsole,
		&mmDevice
	);
	if (FAILED(hr)) { return hr; }
	
	// Check if a device was found, return is not
	if (mmDevice == nullptr) { return E_POINTER;  }

	// Increment reference count to interfaces
	mmDevice->AddRef();

	// Push audio device to decices vector.
	devices.push_back(new AudioDevice());

	hr = devices.back()->ConstructFrom(mmDevice);
	if (FAILED(hr)) { return 1; }

	hr = devices.back()->RegisterForVolumeNotifications();
	if (FAILED(hr)) { return 1; }

	// Release interface pointers;
	mmDevice->Release();
	mmDevice = nullptr;

	CoUninitialize();

	return S_OK;
}

HRESULT AudioDeviceManager::GetDevices()
{
	HRESULT hr;

	deviceEnumerator = nullptr;
	IMMDeviceCollection* deviceCollection = nullptr;

	// Device data.
	IMMDevice* mmDevice = nullptr;
	LPCWSTR deviceID = NULL;

	// Initialize COM interface.
	hr = CoInitializeEx(nullptr, tagCOINIT::COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { return hr; }

	//Create device enumerator.
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID*)&deviceEnumerator
	);
	if (FAILED(hr)) { return hr; }

	mmNotificationClient = new MMDeviceNotificationClient(
		deviceEnumerator,
		EDataFlow::eCapture
	);
	if (FAILED(hr)) { return hr; }

	deviceEnumerator->RegisterEndpointNotificationCallback(
		mmNotificationClient
	);
	if (FAILED(hr)) { return hr; }

	// Get device collection from enumerator.
	hr = deviceEnumerator->EnumAudioEndpoints(
		DATA_FLOW,
		DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,
		&deviceCollection
	);
	if (FAILED(hr)) { return hr; }

	// Get number of devices in collection.
	UINT deviceCount = 0;
	hr = deviceCollection->GetCount(&deviceCount);
	if (FAILED(hr)) { return hr; }

	for (UINT i = 0; i < deviceCount; i += 1)
	{
		mmDevice = nullptr;
		hr = deviceCollection->Item(i, &mmDevice);
		if (FAILED(hr)) { return hr; }

		// Get device unique ID.
		deviceID = nullptr;
		hr = mmDevice->GetId((LPWSTR*)&deviceID);
		if (FAILED(hr)) { return hr; }

		BOOL inMap = false;;
		hr = IsDeviceInVector(&deviceID, &inMap);
		if (inMap) { continue; }

		// Push audio device to vector.
		devices.push_back(new AudioDevice());

		// Increment ref count to interface.
		mmDevice->AddRef();

		hr = devices.back()->ConstructFrom(mmDevice);
		if (FAILED(hr)) { return hr; }

		// Regiseter for volume change notifications.
		hr = devices.back()->RegisterForVolumeNotifications();
		if (FAILED(hr)) { return hr; }

		mmDevice->Release();
		mmDevice = nullptr;
	}

	deviceCollection->Release();
	deviceCollection = nullptr;

	CoUninitialize();

	return S_OK;
}

HRESULT AudioDeviceManager::GetDevice(LPCWSTR* deviceID)
{
	HRESULT hr;

	IMMDevice* mmDevice = nullptr;

	//Create device enumerator.
	deviceEnumerator = nullptr;
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID*)&deviceEnumerator
	);
	if (FAILED(hr)) { return hr; }

	hr = deviceEnumerator->GetDevice(*deviceID, &mmDevice);
	if (FAILED(hr)) { return hr; }

	devices.push_back(new AudioDevice());

	mmDevice->AddRef();
	hr = devices.back()->ConstructFrom(mmDevice);

	mmDevice->Release();
	mmDevice = nullptr;

	return hr;
}

HRESULT AudioDeviceManager::IsDeviceInVector(LPCWSTR* deviceID, BOOL* inMap)
{
	HRESULT hr = S_OK;

	for (auto& mmDevice : devices) {
		LPWSTR otherID = nullptr;
		hr = mmDevice->GetMMDeviceID(&otherID);
		if (FAILED(hr)) { return hr; }

		// Compares the two IDs, if equal to 0, they are equal.
		if (lstrcmpW(*deviceID, otherID) == 0)
		{
			*inMap = true;
			return S_OK;
		}
	}

	return hr;
}

HRESULT AudioDeviceManager::PrintDevices()
{
	HRESULT hr;

	for (auto& mmDevice : devices) {
		hr = mmDevice->Print();
		if (FAILED(hr)) { return hr; }

		printf("\n");
	}

	return S_OK;
}

HRESULT AudioDeviceManager::MuteDevices()
{
	HRESULT hr;

	for (auto& mmDevice : devices) {
		hr = mmDevice->SetMute(TRUE);
		if (FAILED(hr)) { return hr; }

		printf("\n");
	}

	return S_OK;
}

HRESULT AudioDeviceManager::UnmuteDevices()
{
	HRESULT hr;

	for (auto& mmDevice : devices) {
		hr = mmDevice->SetMute(FALSE);
		if (FAILED(hr)) { return hr; }

		printf("\n");
	}

	return S_OK;
}

HRESULT AudioDeviceManager::MuteDevice(LPCWSTR* deviceID)
{
	HRESULT hr = S_OK;

	for (auto& mmDevice : devices) {
		LPWSTR otherID = nullptr;
		hr = mmDevice->GetMMDeviceID(&otherID);
		if (FAILED(hr)) { return hr; }

		// Compares the two IDs, if equal to 0 they are equal.
		if (lstrcmpW(*deviceID, otherID) == 0)
		{
			mmDevice->SetMute(TRUE);
			return S_OK;
		}
	}

	return hr;
}

HRESULT AudioDeviceManager::UnmuteDevice(LPCWSTR* deviceID)
{
	HRESULT hr = S_OK;

	for (auto& mmDevice : devices) {
		LPWSTR otherID = nullptr;
		hr = mmDevice->GetMMDeviceID(&otherID);
		if (FAILED(hr)) { return hr; }

		// Compares the two IDs, if equal to 0 they are equal.
		if (lstrcmpW(*deviceID, otherID) == 0)
		{
			mmDevice->SetMute(FALSE);
			return S_OK;
		}
	}

	return hr;
}

HRESULT AudioDeviceManager::PrintDevice(LPCWSTR* deviceID)
{
	HRESULT hr = S_OK;

	for (auto& mmDevice : devices) {
		LPWSTR otherID = nullptr;
		hr = mmDevice->GetMMDeviceID(&otherID);
		if (FAILED(hr)) { return hr; }

		// Compares the two IDs, if equal to 0 they are equal.
		if (lstrcmpW(*deviceID, otherID) == 0)
		{
			mmDevice->Print();
			return S_OK;
		}
	}

	return hr;
}

HRESULT AudioDeviceManager::RegisterForMMDeviceNotifications()
{
	if (mmNotificationClient != nullptr) { return S_OK; }

	HRESULT hr;

	//if (mmNotificationClient == nullptr) { mmNotificationClient = new MMDeviceNotificationClient(DATA_FLOW); }

	// Register for notifications.
	hr = deviceEnumerator->RegisterEndpointNotificationCallback(
		mmNotificationClient
	);

	deviceEnumerator->Release();

	return hr;
}

HRESULT AudioDeviceManager::UnregisterFromMMDeviceNotifications()
{
	if (mmNotificationClient != nullptr) { return S_OK; }

	HRESULT hr;

	IMMDeviceEnumerator* deviceEnumerator;

	//Create device enumerator.
	hr = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		NULL,
		CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator),
		(LPVOID*)&deviceEnumerator
	);
	if (FAILED(hr)) { return hr; }

	// Unregister for notifications.
	hr = deviceEnumerator->UnregisterEndpointNotificationCallback(
		mmNotificationClient
	);

	deviceEnumerator->Release();

	return hr;
}
