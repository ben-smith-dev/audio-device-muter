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

HRESULT MMDeviceNotificationClient::OnDeviceAdded(LPCWSTR deviceID)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDeviceRemoved(LPCWSTR deviceID)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDefaultDeviceChanged(
    EDataFlow flow, 
    ERole role,
    LPCWSTR deviceID)
{
    HRESULT hr;

    return hr;
}

HRESULT MMDeviceNotificationClient::OnDeviceStateChanged(
    LPCWSTR deviceID,
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

HRESULT MMDeviceNotificationClient::CheckIfDeviceHasCorrectDataFlow(
    LPCWSTR deviceID, 
    BOOL* isCorrectFlow
)
{
    if (dataFlow == EDataFlow::eAll) { *isCorrectFlow = true; return S_OK; }

    HRESULT hr;

    if (deviceEnumerator == nullptr) 
    { 
        hr = CreateEnumerator();
        if (FAILED(hr)) { return hr; }
    }

    // Get device using ID.
    IMMDevice* device = nullptr;
    hr = deviceEnumerator->GetDevice(deviceID, &device);
    if (FAILED(hr)) { return hr; }

    // Get endpoint from querying device interfaces.
    IMMEndpoint* endpoint = nullptr;
    hr = device->QueryInterface(__uuidof(IMMEndpoint), (LPVOID*)&endpoint);
    if (FAILED(hr)) { device->Release(); return hr; }

    EDataFlow deviceFlow;
    hr = endpoint->GetDataFlow(&deviceFlow);

    // Compare device data flow to MMDeviceNotificationClient data flow mask.
    *isCorrectFlow = deviceFlow == dataFlow;

    device->Release();
    endpoint->Release();

    return hr;
}

HRESULT MMDeviceNotificationClient::CreateEnumerator()
{
    HRESULT hr;

    //Create device enumerator.
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (LPVOID*)&deviceEnumerator
    );

    return hr;
}