#pragma once

#include <mmdeviceapi.h>

class MMDeviceNotificationClient : public IMMNotificationClient
{
private:
	LONG cRef;
	EDataFlow dataFlow;
	IMMDeviceEnumerator* deviceEnumerator;

	HRESULT CheckDeviceDataFlow(EDataFlow* dataFlow);

public:
	MMDeviceNotificationClient();
	~MMDeviceNotificationClient();

	// IUnknown methods

	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface();

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