#ifndef __dcuTriggerSample_h__
#define __dcuTriggerSample_h__

#include "dcuSample.h"

class dcuTriggerSample : public dcuSample
	{
	public:
	 dcuTriggerSample(char *file=NULL,float maxAmpl=1);
	 virtual void update(float *userPos,float curTime);
	 virtual int parseOption(char *opt);
	 float radius(void) { return fsqrt(radiusSq_); }
	 void setRadius(float v) { radiusSq_ = v*v; }
	protected:
	 float radiusSq_;
	 int latch_;
	};

#endif
