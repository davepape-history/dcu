#include <stdio.h>
#include <math.h>
#include <string.h>
#include "dcuTrigger.h"
#include "dcu.h"

dcuTrigger::dcuTrigger(dcuVolume *vol)
{
 enterCB_ = NULL;
 enterCBData_ = NULL;
 leaveCB_ = NULL;
 leaveCBData_ = NULL;
 if (vol)
	volume_ = vol;
 else
	volume_ = new dcuVolume;
 latch_ = 0;
}

void dcuTrigger::update(float *pos)
{
 float distSq;
 distSq = volume_->distanceSq(pos[0],pos[1],pos[2]);
 if (!latch_)
	{
	if (volume_->contains(pos))
		{
		if (enterCB_)
			(*enterCB_)(enterCBData_);
		latch_ = 1;
		}
	}
 else
	{
	if (! volume_->contains(pos))
		{
		if (leaveCB_)
			(*leaveCB_)(leaveCBData_);
		latch_ = 0;
		}
	}
}


void dcuTrigger::setEnterCallback(dcuCallback callback,void *data)
{
 enterCB_ = callback;
 enterCBData_ = data;
}

dcuCallback dcuTrigger::enterCallback(void)
{
 return enterCB_;
}

void * dcuTrigger::enterCallbackData(void)
{
 return enterCBData_;
}

void dcuTrigger::setLeaveCallback(dcuCallback callback,void *data)
{
 leaveCB_ = callback;
 leaveCBData_ = data;
}

dcuCallback dcuTrigger::leaveCallback(void)
{
 return leaveCB_;
}

void * dcuTrigger::leaveCallbackData(void)
{
 return leaveCBData_;
}

void dcuTrigger::setVolume(dcuVolume *v)
{
 volume_ = v;
}

dcuVolume * dcuTrigger::volume(void)
{
 return volume_;
}
