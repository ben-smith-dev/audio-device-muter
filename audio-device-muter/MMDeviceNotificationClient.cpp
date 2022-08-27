#include "MMDeviceNotificationClient.h"

MMDeviceNotificationClient::MMDeviceNotificationClient(EDataFlow flow)
{
    this->cRef = 1;
    this->deviceEnumerator = nullptr;
    this->dataFlow = flow;
}

MMDeviceNotificationClient::~MMDeviceNotificationClient()
{
    this->deviceEnumerator = nullptr;

    Release();
}

ULONG MMDeviceNotificationClient::AddRef()
{
	return InterlockedIncrement(&cRef);
}

ULONG MMDeviceNotificationClient::Release()
{
    ULONG ulRef = InterlockedDecrement(&cRef);
    if (0 == ulRef)
    {
        delete this;
    }
    return ulRef;
}

HRESULT MMDeviceNotificationClient::QueryInterface(
    REFIID riid, 
    VOID** ppvInterface
)
{
    if (IID_IUnknown == riid)
    {
        AddRef();
        *ppvInterface = (IUnknown*)this;
    }
    else if (__uuidof(IMMNotificationClient) == riid)
    {
        AddRef();
        *ppvInterface = (IMMNotificationClient*)this;
    }
    else
    {
        *ppvInterface = NULL;
        return E_NOINTERFACE;
    }
    return S_OK;
}

HRESULT MMDeviceNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDefaultDeviceChanged(
    EDataFlow flow, 
    ERole role,
    LPCWSTR pwstrDeviceId)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDeviceStateChanged(
    LPCWSTR deviceId,
    DWORD newState
)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnPropertyValueChanged(
    LPCWSTR deviceId,
    const PROPERTYKEY propKey
)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::CheckDeviceDataFlow(LPCWSTR deviceID, EDataFlow* flow)
{
    if (deviceEnumerator == nullptr) { return E_NOTFOUND; }

    HRESULT hr;

    // Get device using ID.
    IMMDevice* device = nullptr;
    hr = deviceEnumerator->GetDevice(deviceID, &device);
    if (FAILED(hr)) { return hr; }

    // Get endpoint from querying device interfaces.
    IMMEndpoint* endpoint = nullptr;
    hr = device->QueryInterface(__uuidof(IMMEndpoint), (LPVOID*)&endpoint);
    if (FAILED(hr)) { device->Release(); return hr; }

    hr = endpoint->GetDataFlow(flow);

    device->Release();
    endpoint->Release();

    return hr;
}