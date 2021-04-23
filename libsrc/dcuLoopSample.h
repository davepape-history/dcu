#ifndef __dcuLoopSample_h__
#define __dcuLoopSample_h__

#include "dcuSample.h"

class dcuLoopSample : public dcuSample
	{
	public:
	 dcuLoopSample(char *file=NULL,float maxAmpl=1);
	 virtual void start(void);
	 virtual void play(float *userPos,float curTime);
	protected:
	};

#endif
