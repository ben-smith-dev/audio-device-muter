#include "AudioDeviceManager.h"

AudioDeviceManager::AudioDeviceManager()
{
	mmNotificationClient = nullptr;
}

AudioDeviceManager::~AudioDeviceManager()
{
	UnregisterFromMMDeviceNotifications();
}

HRESULT AudioDeviceManager::GetDefaultAudioDevice()
{
	HRESULT hr;

	IMMDeviceEnumerator* deviceEnumerator = nullptr;

	// Device data.
	IMMDevice* mmDevice = nullptr;
	IPropertyStore* props = nullptr;
	IAudioEndpointVolume* endpointVolume = nullptr;
	LPWSTR pwszID = NULL;

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

	// Gets default audio endpoint
	deviceEnumerator->GetDefaultAudioEndpoint(
		DATA_FLOW,
		ERole::eConsole,
		&mmDevice
	);
	if (FAILED(hr)) { return hr; }
	
	// Check if a device was found, return is not
	if (mmDevice == nullptr) { return S_OK;  }

	// Get device unique 
	hr = mmDevice->GetId(&pwszID);
	if (FAILED(hr)) { return hr; }

	// Get audi0 endpoint volume
	hr = mmDevice->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL,
		NULL,
		(LPVOID*)&endpointVolume
	);
	if (FAILED(hr)) { return hr; }

	//Gets pointer to device properties
	hr = mmDevice->OpenPropertyStore(
		STGM_READ,
		&props
	);
	if (FAILED(hr)) { return hr; }

	// Increment reference count to interfaces
	mmDevice->AddRef();
	endpointVolume->AddRef();
	props->AddRef();

	// Push audio device to decices vector.
	devices.push_back(new AudioDevice(
		mmDevice,
		endpointVolume,
		props
	));

	// Release interface pointers;
	mmDevice->Release();
	mmDevice = nullptr;

	endpointVolume->Release();
	endpointVolume = nullptr;

	props->Release();
	props = nullptr;

	deviceEnumerator->Release();
	deviceEnumerator = nullptr;

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
		hr = CheckIfDeviceInMap(&deviceID, &inMap);
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

HRESULT AudioDeviceManager::CheckIfDeviceInMap(LPCWSTR* deviceID, BOOL* inMap)
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

	for (auto& mmDevice : devices) {;
		hr = mmDevice->Print();
		if (FAILED(hr)) { return hr; }
	}

	return S_OK;
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
