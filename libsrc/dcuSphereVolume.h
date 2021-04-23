#ifndef __dcuSphereVolume_h__
#define __dcuSphereVolume_h__

#include "dcuVolume.h"

class dcuSphereVolume : public dcuVolume
	{
	public:
	 dcuSphereVolume(void);
	 virtual float distanceSq(float x,float y,float z);
	 virtual float distance(float x,float y,float z);
	 virtual int contains(float x,float y,float z);
	 virtual int rayIntersect(const float p0[3],const float p1[3],float **hit,
				int raytype=DCU_LINE);
	 virtual float distanceSq(const float *p)
				{ return distanceSq(p[0],p[1],p[2]); }
	 virtual float distance(const float *p)
				{ return distance(p[0],p[1],p[2]); }
	 virtual int contains(const float *p)
				{ return contains(p[0],p[1],p[2]); }
	 float radius(void) { return radius_; }
	 void setRadius(float v) { radius_ = v; }
	protected:
	 float radius_;
	};

#endif
