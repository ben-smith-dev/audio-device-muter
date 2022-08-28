#include "VolumeNotificationClient.h"

VolumeNotificationClient::VolumeNotificationClient(BOOL muteState) : cRef(1) 
{
	this->lastMuteState = muteState;
}

