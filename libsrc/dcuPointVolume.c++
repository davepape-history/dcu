#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "dcuPointVolume.h"

dcuPointVolume::dcuPointVolume(void)
{
 shape_ = DCU_POINT;
}

float dcuPointVolume::distanceSq(float x,float y,float z)
{
 float dx=0,dy=0,dz=0,pos[3];
 pos[0] = x;
 pos[1] = y;
 pos[2] = z;
 if (transform_)
	transform_->inverseTransformPoint(pos);
 if (dimensions_ & DCU_X_DIM)
	dx = pos[0] - center_[0];
 if (dimensions_ & DCU_Y_DIM)
	dy = pos[1] - center_[1];
 if (dimensions_ & DCU_Z_DIM)
	dz = pos[2] - center_[2];
 return dx*dx + dy*dy + dz*dz;
}

int dcuPointVolume::rayIntersect(const float inp0[3],const float inp1[3],float **hit,int raytype)
{
 float t,d[3],delta,distx,disty,distz,P0[3],P1[3];
 int i,index;
 if (!dimensions_)
	return dcuVolume::rayIntersect(inp0,inp1,hit,raytype);
 P0[0] = inp0[0]; P0[1] = inp0[1]; P0[2] = inp0[2];
 P1[0] = inp1[0]; P1[1] = inp1[1]; P1[2] = inp1[2];
 if (transform_)
	{
	transform_->inverseTransformPoint(P0);
	transform_->inverseTransformPoint(P1);
	}
 if (!(dimensions_ & DCU_X_DIM))
	P0[0] = P1[0] = 0.0f;
 if (!(dimensions_ & DCU_Y_DIM))
	P0[1] = P1[1] = 0.0f;
 if (!(dimensions_ & DCU_Z_DIM))
	P0[2] = P1[2] = 0.0f;
 for (i=0; i<3; i++)
	{
	d[i] = P1[i] - P0[i];
	if ((!i) || (fabs(d[i]) > fabs(delta)))
		{
		delta = d[i];
		index = i;
		}
	}
 if (fabs(delta) < 0.0000001f)
	return 0;
 t = (center_[index] - P0[index]) / delta;
 if (((t < 0.0f) && (raytype != DCU_LINE)) ||
     ((t > 1.0f) && (raytype == DCU_SEGMENT)))
	return 0;
 distx = P0[0] + d[0] * t - center_[0];
 disty = P0[1] + d[1] * t - center_[1];
 distz = P0[2] + d[2] * t - center_[2];
 if (distx*distx + disty*disty + distz*distz > 0.000001f)
	return 0;
 if (hit)
	{
	*hit = (float *) malloc(3 * sizeof(float));
	(*hit)[0] = center_[0];
	(*hit)[1] = center_[1];
	(*hit)[2] = center_[2];
	if (transform_)
		transform_->transformPoint(*hit);
	}
 return 1;
}
