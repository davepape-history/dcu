#ifndef __dcuBoxVolume_h__
#define __dcuBoxVolume_h__

#include "dcuVolume.h"

class dcuBoxVolume : public dcuVolume
	{
	public:
	 dcuBoxVolume(void);
	 void getRange(float *v);
	 void setRange(const float *v)
			{ setRange(v[0],v[1],v[2],v[3],v[4],v[5]); }
	 void setRange(float minX,float maxX,float minY,float maxY,float minZ,float maxZ);
	 void setCenterAndSize(const float *center,const float *size)
			{ setCenter(center); setSize(size); }
	 void setSize(float x,float y,float z);
	 void setSize(const float *size)
			{ setSize(size[0],size[1],size[2]); }
	 void getSize(float *size);
	 virtual float distanceSq(float x,float y,float z);
	 virtual float distanceSq(const float *p)
				{ return distanceSq(p[0],p[1],p[2]); }
	 virtual int rayIntersect(const float p0[3],const float p1[3],float **hit,
				int raytype=DCU_LINE);
	protected:
	 float size_[3];
	 int testRay(float *P0,float *rayd,int dim,int dir,float *hit);
	};

#endif
