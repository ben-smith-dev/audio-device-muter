#pragma once

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

class AudioDevice
{
private:
	IMMDevice* device;
	IAudioEndpointVolume* endpointVolume;
	IPropertyStore* props;
};