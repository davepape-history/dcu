#include <stdio.h>
#include <math.h>
#include <string.h>
#include "dcuTriggerSample.h"
#include "dcu.h"

dcuTriggerSample::dcuTriggerSample(char *file,float maxAmpl) : dcuSample(file,maxAmpl)
{
 radiusSq_ = 1.0;
 latch_ = 0;
}

void dcuTriggerSample::update(float *userPos,float curTime)
{
 float distSq;
 distSq = volume_->distanceSq(userPos);
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

int dcuTriggerSample::parseOption(char *opt)
{
 enum _opt_type {
		OPT_RADIUS
		} opttype;
 struct _dcukeyword opt_table[] = {
		{ "radius", OPT_RADIUS },
		{ "rad", OPT_RADIUS },
		{ NULL, 0 }
		};
 char *eq,*val;
 eq = strchr(opt,'=');
 if (!eq) return 0;
 *eq = '\0';
 val = eq+1;
 opttype = (enum _opt_type) dcuGetStringID(opt,opt_table);
 switch (opttype)
	{
	case OPT_RADIUS:
		radiusSq_ = atof(val) * atof(val);
		break;
	default:
		*eq = '=';
		return dcuSample::parseOption(opt);
	}
 return 1;
}
