#include "AudioDevice.h"

#include <string>
#include <stdio.h>

AudioDevice::AudioDevice(
	IMMDevice* device,
	IAudioEndpointVolume* endpointVolume,
	IPropertyStore* props
)
{
	this->device = device;
	this->endpointVolume = endpointVolume;
	this->props = props;
}

AudioDevice::~AudioDevice()
{
	// De-increments the refernce count of each interface.
	// AudioDevice::ReleaseInterfaceReferences();
}

HRESULT AudioDevice::GetFriendlyName(LPWSTR* deviceFriendlyname)
{
	// Initialize the propvariant structure.
	PROPVARIANT varName;
	PropVariantInit(&varName);

	//	Get device friendly name property
	HRESULT hr = props->GetValue(
		PKEY_Device_FriendlyName,
		&varName
	);
	if (FAILED(hr)) { return hr; }

	*deviceFriendlyname = varName.pwszVal;
	
	// Clear propvariant structure.
	PropVariantClear(&varName);
	
	return S_OK;
}

HRESULT AudioDevice::GetState(DWORD* deviceState) 
{
	HRESULT hr = device->GetState(deviceState);

	return hr;
}

HRESULT AudioDevice::GetMute(BOOL* isMuted)
{
	HRESULT hr = endpointVolume->GetMute(isMuted);

	return hr;
}

HRESULT AudioDevice::SetMute(BOOL muteState)
{	
	// TODO: find a better guidEventContext, maybe the ID of the endpointVolume interface?
	HRESULT hr = endpointVolume->SetMute(muteState, nullptr);

	return hr;
}

HRESULT AudioDevice::Print()
{
	HRESULT hr;

	// Get device name.
	LPWSTR* deviceName = nullptr;
	hr = GetFriendlyName(deviceName);
	if (FAILED(hr)) { return hr; }

	//Print device name.
	printf("Device name: %s.\n", deviceName);

	// Get the devices unique ID.
	LPWSTR* deviceID = nullptr;
	hr = device->GetId(deviceID);
	if (FAILED(hr)) { return hr; }

	// Get the devices state
	DWORD* deviceState = nullptr;
	hr = GetState(deviceState);
	if (FAILED(hr)) { return hr; }

	// Lambda function to convert state to string.
	auto stateToString = [](DWORD state) {
		switch (state) {
		case DEVICE_STATE_ACTIVE: return "ACTIVE";
		case DEVICE_STATE_DISABLED: return "DISABLED";
		case DEVICE_STATE_NOTPRESENT: return "NOT PRESENT";
		case DEVICE_STATE_UNPLUGGED: return "UN-PLUGGED";
		default: return "UNKNOWN STATE";
		}
	};

	// Print device state.
	printf("Device State: %s\n", stateToString(*deviceState));

	// Get current device mute state.
	BOOL* isMuted = nullptr;
	hr = GetMute(isMuted);
	if (FAILED(hr)) { return hr; }

	// Print mute state as string.
	auto boolToString = [](BOOL x) {return x ? "True" : "False"; };
	printf("Device is muted: %s.\n", boolToString(*isMuted));

	return S_OK;
}