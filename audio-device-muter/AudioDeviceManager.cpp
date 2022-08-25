#include "AudioDeviceManager.h"

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

	// Get device unique ID
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

	// Push audio device to map
	devices[pwszID] = new AudioDevice(
		device,
		endpointVolume,
		props
	);

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