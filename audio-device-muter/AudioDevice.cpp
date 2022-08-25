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