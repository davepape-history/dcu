#include <stdio.h>
#include <math.h>
#include <string.h>
#include "dcuLoopSample.h"

dcuLoopSample::dcuLoopSample(char *file,float maxAmpl) : dcuSample(file,maxAmpl)
{
}

void dcuLoopSample::start(void)
{
 float pos[3] = { 1000000,1000000,1000000 }; /* Dummy value for initialization */
 play(pos,0.0f);
}

void dcuLoopSample::play(float *userPos,float curTime)
{
 dcuSample::start();
 dcuSample::play(userPos,curTime);
 actorMessage(A_SetLoop, "FFF", actor_, note_, 1.0);
 endTime_ = -1;
}
