#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cave.h>
#include "dcuWandTriggerSample.h"
#include "dcu.h"

dcuWandTriggerSample::dcuWandTriggerSample(char *file,float maxAmpl) : dcuTriggerSample(file,maxAmpl)
{
}

void dcuWandTriggerSample::update(float *userPos,float curTime)
{
 float distSq, wandPos[3];
 CAVEGetPosition(CAVE_WAND_NAV,wandPos);
 distSq = volume_->distanceSq(wandPos);
 if ((!latch_) && (note_ == -1))
	{
	if (distSq <= radiusSq_)
		{
		play(userPos,curTime);
		latch_ = 1;
		}
	}
 else
	{
	if (distSq > radiusSq_)
		latch_ = 0;
	}
 dcuSample::update(userPos,curTime);
}
