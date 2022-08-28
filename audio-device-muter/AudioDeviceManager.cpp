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
	IMMDevice* device = nullptr;
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
		&device
	);
	if (FAILED(hr)) { return hr; }
	
	// Check if a device was found, return is not
	if (device == nullptr) { return S_OK;  }

	// Get device unique 
	hr = device->GetId(&pwszID);
	if (FAILED(hr)) { return hr; }

	// Get audi0 endpoint volume
	hr = device->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL,
		NULL,
		(LPVOID*)&endpointVolume
	);
	if (FAILED(hr)) { return hr; }

	//Gets pointer to device properties
	hr = device->OpenPropertyStore(
		STGM_READ,
		&props
	);
	if (FAILED(hr)) { return hr; }

	// Increment reference count to interfaces
	device->AddRef();
	endpointVolume->AddRef();
	props->AddRef();

	// Push audio device to decices vector.
	devices.push_back(new AudioDevice(
		device,
		endpointVolume,
		props
	));

	// Release interface pointers;
	device->Release();
	device = nullptr;

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
	IMMDevice* device = nullptr;
	IPropertyStore* props = nullptr;
	IAudioEndpointVolume* endpointVolume = nullptr;
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

	// Register for notifications.
	//RegisterForMMDeviceNotifications();

	// Get device collection from enumerator.
	hr = deviceEnumerator->EnumAudioEndpoints(
		DATA_FLOW,
		DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,
		&deviceCollection
	);
	if (FAILED(hr)) { return hr; }

	// Get number of devics in collection.
	UINT deviceCount = 0;
	hr = deviceCollection->GetCount(&deviceCount);
	if (FAILED(hr)) { return hr; }

	for (UINT i = 0; i < deviceCount; i += 1)
	{
		device = nullptr;
		hr = deviceCollection->Item(i, &device);
		if (FAILED(hr)) { return hr; }

		// Get device unique 
		deviceID = nullptr;
		hr = device->GetId((LPWSTR*)&deviceID);
		if (FAILED(hr)) { return hr; }

		BOOL inMap = false;;
		hr = CheckIfDeviceInMap(&deviceID, &inMap);
		if (inMap) { continue; }

		// Get audio endpoint volume
		endpointVolume = nullptr;
		hr = device->Activate(
			__uuidof(IAudioEndpointVolume),
			CLSCTX_ALL,
			NULL,
			(LPVOID*)&endpointVolume
		);
		if (FAILED(hr)) { return hr; }

		//Gets pointer to device properties
		props = nullptr;
		hr = device->OpenPropertyStore(
			STGM_READ,
			&props
		);
		if (FAILED(hr)) { return hr; }

		// Increment reference count to interfaces
		device->AddRef();
		endpointVolume->AddRef();
		props->AddRef();

		// Push audio device to map
		devices.push_back(new AudioDevice(
			device,
			endpointVolume,
			props
		));

		// Regiseter for volume change notifications.
		hr = devices.back()->RegisterForVolumeNotifications();
		
		if (FAILED(hr)) { return hr; }

		// Release interface pointers;
		device->Release();
		device = nullptr;

		endpointVolume->Release();
		endpointVolume = nullptr;

		props->Release();
		props = nullptr;
	}

	deviceCollection->Release();
	deviceCollection = nullptr;

	CoUninitialize();

	return S_OK;
}

HRESULT AudioDeviceManager::CheckIfDeviceInMap(LPCWSTR* deviceID, BOOL* inMap)
{
	HRESULT hr = S_OK;

	for (auto& device : devices) {
		LPWSTR otherID = nullptr;
		hr = device->GetMMDeviceID(&otherID);
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

	for (auto& device : devices) {;
		hr = device->Print();
		if (FAILED(hr)) { return hr; }
	}

	return S_OK;
}

HRESULT AudioDeviceManager::PrintDevice(LPCWSTR* deviceID)
{
	HRESULT hr = S_OK;

	for (auto& device : devices) {
		LPWSTR otherID = nullptr;
		hr = device->GetMMDeviceID(&otherID);
		if (FAILED(hr)) { return hr; }

		// Compares the two IDs, if equal to 0 they are equal.
		if (lstrcmpW(*deviceID, otherID) == 0)
		{
			device->Print();
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
