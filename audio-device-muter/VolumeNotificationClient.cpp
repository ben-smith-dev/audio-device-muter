#include "VolumeNotificationClient.h"

VolumeNotificationClient::VolumeNotificationClient(BOOL muteState) : cRef(1) 
{
	this->lastMuteState = muteState;
}

ULONG VolumeNotificationClient::AddRef()
{
	return InterlockedIncrement(&cRef);
}

ULONG VolumeNotificationClient::Release()
{
	ULONG ulRef = InterlockedDecrement(&cRef);
	if (0 == ulRef)
	{
		delete this;
	}
	return ulRef;
}

HRESULT VolumeNotificationClient::QueryInterface(
	REFIID riid, 
	VOID** ppvInterface
)
{
	if (IID_IUnknown == riid)
	{
		AddRef();
		*ppvInterface = (IUnknown*)this;
	}
	else if (__uuidof(IAudioEndpointVolumeCallback) == riid)
	{
		AddRef();
		*ppvInterface = (IAudioEndpointVolumeCallback*)this;
	}
	else
	{
		*ppvInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}