#ifndef __dcuWandTriggerSample_h__
#define __dcuWandTriggerSample_h__

#include "dcuTriggerSample.h"

class dcuWandTriggerSample : public dcuTriggerSample
	{
	public:
	 dcuWandTriggerSample(char *file=NULL,float maxAmpl=1);
	 virtual void update(float *userPos,float curTime);
	protected:
	};

#endif
