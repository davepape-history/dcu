#ifndef __dcuPointVolume_h__
#define __dcuPointVolume_h__

#include "dcuVolume.h"

class dcuPointVolume : public dcuVolume
	{
	public:
	 dcuPointVolume(void);
	 virtual float distanceSq(float x,float y,float z);
	 virtual float distanceSq(const float *p)
				{ return distanceSq(p[0],p[1],p[2]); }
	 virtual int rayIntersect(const float p0[3],const float p1[3],float **hit,
				int raytype=DCU_LINE);
	protected:
	};

#endif
