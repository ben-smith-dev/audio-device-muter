#include "AudioDevice.h"

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
	AudioDevice::ReleaseInterfaceReferences();
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