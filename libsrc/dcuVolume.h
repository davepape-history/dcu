#ifndef __dcuVolume_h__
#define __dcuVolume_h__

#include "dcu.h"
#include "dcuTransform.h"

class dcuVolume
	{
	public:
	 dcuVolume(void);
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
	 virtual int dimensions(void);
	 virtual void setDimensions(int v);
	 virtual int shape(void);
	 virtual void getCenter(float *p);
	 virtual void setCenter(float x,float y,float z);
	 virtual void setCenter(const float *p)
				{ setCenter(p[0],p[1],p[2]); }
	 virtual void setTransform(dcuTransform *t);
	 virtual dcuTransform * getTransform(void);
	protected:
	 int dimensions_;
	 float center_[3];
	 int shape_;
	 dcuTransform *transform_;
	};

#endif
