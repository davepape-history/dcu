#ifndef __dcuRandomSample_h__
#define __dcuRandomSample_h__

#include "dcuSample.h"

class dcuRandomSample : public dcuSample
	{
	public:
	 dcuRandomSample(char *file=NULL,float maxAmpl=1);
	 virtual void update(float *userPos,float curTime);
	 virtual int parseOption(char *opt);
	 float probability(void) { return probability_; }
	 void setProbability(float v) { probability_ = v; }
	 float minRandomAmplitude(void) { return minRandomAmpl_; }
	 void setMinRandomAmplitude(float v) { minRandomAmpl_ = v; }
	 float maxRandomAmplitude(void) { return maxRandomAmpl_; }
	 void setMaxRandomAmplitude(float v) { maxRandomAmpl_ = v; }
	protected:
	 float probability_;
	 float minRandomAmpl_,maxRandomAmpl_;
	};

#endif
