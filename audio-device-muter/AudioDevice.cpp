#include "AudioDevice.h"

#include <string>
#include <stdio.h>

AudioDevice::AudioDevice()
{
	this->mmDevice = nullptr;
	this->endpointVolume = nullptr;
	this->props = nullptr;

	this->volumeNotificationClient = new VolumeNotificationClient(FALSE);
}

AudioDevice::AudioDevice(
	IMMDevice* mmDevice,
	IAudioEndpointVolume* endpointVolume,
	IPropertyStore* props
)
{
	this->mmDevice = mmDevice;
	this->endpointVolume = endpointVolume;
	this->props = props;
	this->volumeNotificationClient = new VolumeNotificationClient(FALSE);
}

AudioDevice::~AudioDevice()
{
	// De-increments the reference count of each interface.
	ReleaseInterfaceReferences();
}

HRESULT AudioDevice::ConstructFrom(IMMDevice* mmDevice)
{
	if (mmDevice == nullptr) return E_POINTER;

	HRESULT hr;

	this->mmDevice = mmDevice;

	// Get endpointVolume interface pointer.
	hr = mmDevice->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL,
		NULL,
		(LPVOID*)&endpointVolume
	);
	if (FAILED(hr)) { return hr; }

	//Gets pointer to device properties.
	hr = mmDevice->OpenPropertyStore(
		STGM_READ,
		&props
	);
	if (FAILED(hr)) { mmDevice->Release(); }

	return hr;
}

HRESULT AudioDevice::GetFriendlyName(PROPVARIANT& varDeviceName)
{
	PROPVARIANT varName;
	PropVariantInit(&varName);

	//	Get device friendly name property
	HRESULT hr = props->GetValue(
		PKEY_Device_FriendlyName,
		&varName
	);

	PropVariantCopy(&varDeviceName, &varName);
	
	PropVariantClear(&varName);
	return hr;
}

HRESULT AudioDevice::GetState(DWORD* deviceState) 
{
	HRESULT hr = mmDevice->GetState(deviceState);

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

HRESULT AudioDevice::GetMMDeviceID(LPWSTR* deviceID)
{
	HRESULT hr;

	hr = mmDevice->GetId(deviceID);

	return hr;
}

HRESULT AudioDevice::Print()
{
	HRESULT hr;
	
	// Get device name.
	PROPVARIANT deviceNameVar;

	hr = GetFriendlyName(deviceNameVar);
	if (FAILED(hr)) { return hr; }

	// Get the devices unique ID.
	LPWSTR deviceID = new WCHAR;
	hr = mmDevice->GetId(&deviceID);
	if (FAILED(hr)) { return hr; }

	// Get the devices state
	DWORD* deviceState = new DWORD;
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

	// Get current device mute state.
	BOOL* isMuted = new BOOL;
	hr = GetMute(isMuted);
	if (FAILED(hr)) { return hr; }

	printf("Device Name:\t\t%S\n",	deviceNameVar.pwszVal);
	printf("Device ID:\t\t%S\n",	deviceID);
	printf("Device State:\t\t%s\n", stateToString(*deviceState));
	printf("Device Muted:\t\t%s\n", *isMuted? "TRUE" : "FALSE");

	// Clear property variant.
	PropVariantClear(&deviceNameVar);

	return S_OK;
}

HRESULT AudioDevice::RegisterForVolumeNotifications()
{
	HRESULT hr;

	hr = endpointVolume->RegisterControlChangeNotify(
		volumeNotificationClient
	);

	return hr;
}

HRESULT AudioDevice::UnregisterFromVolumeNotifications()
{
	HRESULT hr;

	hr = endpointVolume->UnregisterControlChangeNotify(
		volumeNotificationClient
	);

	return hr;
}

int AudioDevice::ReleaseInterfaceReferences()
{
	if (mmDevice != nullptr) mmDevice->Release();
	if (endpointVolume != nullptr) endpointVolume->Release();
	if (props != nullptr) props->Release();

	mmDevice = nullptr;
	endpointVolume = nullptr;
	props = nullptr;

	return 0;
}