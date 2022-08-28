#pragma once

#include <stdio.h>
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>

class MMDeviceNotificationClient : public IMMNotificationClient
{
private:
	LONG cRef;
	EDataFlow dataFlow;
	IMMDeviceEnumerator* deviceEnumerator;

	HRESULT CheckIfDeviceHasCorrectDataFlow(LPCWSTR deviceId, BOOL* isCorrectFlow);
	HRESULT CreateEnumerator();
	HRESULT PrintDeviceName(LPCWSTR* deviceID);

public:
	MMDeviceNotificationClient(IMMDeviceEnumerator* deviceEnumerator, EDataFlow dataFlow);
	~MMDeviceNotificationClient();

	// IUnknown methods

	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid,
		VOID** ppvInterface
	);

	// IMMNotificationClient callback methods

	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);
	
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		EDataFlow flow, 
		ERole role,
		LPCWSTR deviceId
	);
	
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
		LPCWSTR deviceId,
		DWORD newState
	);
	
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
		LPCWSTR deviceId,
		const PROPERTYKEY propKey
	);
};