#include <stdio.h>
#include <math.h>
#include <string.h>
#include "dcuRandomSample.h"
#include "dcu.h"

dcuRandomSample::dcuRandomSample(char *file,float maxAmpl) : dcuSample(file,maxAmpl)
{
 probability_ = 0.005;
 maxRandomAmpl_ = maxAmpl_;
 minRandomAmpl_ = maxAmpl_;
}

void dcuRandomSample::update(float *userPos,float curTime)
{
 if (note_ == -1)
	{
	if (drand48() < probability_)
		{
		maxAmpl_ = minRandomAmpl_ + drand48() *
				(maxRandomAmpl_ - minRandomAmpl_);
		play(userPos,curTime);
		}
	}
 dcuSample::update(userPos,curTime);
}

int dcuRandomSample::parseOption(char *opt)
{
 enum _opt_type {
		OPT_PROBABILITY,
		OPT_MINAMPL,
		OPT_MAXAMPL,
		OPT_AMPL
		} opttype;
 struct _dcukeyword opt_table[] = {
		{ "probability", OPT_PROBABILITY },
		{ "prob", OPT_PROBABILITY },
		{ "minamplitude", OPT_MINAMPL },
		{ "minampl", OPT_MINAMPL },
		{ "maxamplitude", OPT_MAXAMPL },
		{ "maxampl", OPT_MAXAMPL },
		{ "amplitude", OPT_AMPL },
		{ "ampl", OPT_AMPL },
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
	case OPT_PROBABILITY:
		probability_ = atof(val);
		break;
	case OPT_MINAMPL:
		minRandomAmpl_ = atof(val);
		break;
	case OPT_MAXAMPL:
		maxRandomAmpl_ = atof(val);
		break;
	case OPT_AMPL:
		minRandomAmpl_ = maxRandomAmpl_ = atof(val);
		break;
	default:
		*eq = '=';
		return dcuSample::parseOption(opt);
	}
 return 1;
}
