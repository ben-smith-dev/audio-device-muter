#include "MMDeviceNotificationClient.h"

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