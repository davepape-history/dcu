#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "dcuSphereVolume.h"

dcuSphereVolume::dcuSphereVolume(void)
{
 shape_ = DCU_SPHERE;
 radius_ = 0;
}

float dcuSphereVolume::distanceSq(float x,float y,float z)
{
 float dist=distance(x,y,z);
 return dist*dist;
}

float dcuSphereVolume::distance(float x,float y,float z)
{
 float dx=0,dy=0,dz=0,distSq,dist,pos[3];
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
 distSq = dx*dx + dy*dy + dz*dz;
 if (distSq <= radius_*radius_)
	return 0;
 dist = fsqrt(distSq) - radius_;
 return dist;
}

int dcuSphereVolume::contains(float x,float y,float z)
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
 return (dx*dx + dy*dy + dz*dz <= radius_*radius_);
}

int dcuSphereVolume::rayIntersect(const float inp0[3],const float inp1[3],float **hit,int raytype)
{
 float raydx,raydy,raydz,distx,disty,distz,A,B,C,disc,P0[3],P1[3];
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
 raydx = P1[0] - P0[0];
 raydy = P1[1] - P0[1];
 raydz = P1[2] - P0[2];
 distx = P0[0] - center_[0];
 disty = P0[1] - center_[1];
 distz = P0[2] - center_[2];
 A = raydx*raydx + raydy*raydy + raydz*raydz;
 B = 2.0f*(raydx*distx + raydy*disty + raydz*distz);
 C = distx*distx + disty*disty + distz*distz - radius_*radius_;
 disc = B*B - 4.0f*A*C;
 if (disc < 0.0f)
	return 0;
 else if (disc == 0.0f)	/* Not likely, but what the hey */
	{
	float t;
	t = -B / (2.0f * A);
	if ((t < 0.0f) && (raytype != DCU_LINE))
		return 0;
	if ((t > 1.0f) && (raytype == DCU_SEGMENT))
		return 0;
	if (hit)
		{
		*hit = (float *) malloc(3*sizeof(float));
		(*hit)[0] = P0[0] + t * raydx;
		(*hit)[1] = P0[1] + t * raydy;
		(*hit)[2] = P0[2] + t * raydz;
		if (transform_)
			transform_->transformPoint(*hit);
		}
	return 1;
	}
 else
	{
	float t0,t1,sqrtdisc = fsqrt(disc);
	int hit0=0,hit1=0;
	t0 = (sqrtdisc - B) / (2.0f * A);
	t1 = (-sqrtdisc - B) / (2.0f * A);
	if (t0 > 0.0f)
		{
		if ((raytype != DCU_SEGMENT) || (t0 < 1.0f))
			hit0 = 1;
		}
	else
		if (raytype == DCU_LINE)
			hit0 = 1;
	if (t1 > 0.0f)
		{
		if ((raytype != DCU_SEGMENT) || (t1 < 1.0f))
			hit1 = 1;
		}
	else
		if (raytype == DCU_LINE)
			hit1 = 1;
	if (hit0 + hit1 == 0)
		return 0;
	if (hit)
		{
		int index=0;
		*hit = (float *) malloc((hit0 + hit1) * 3 * sizeof(float));
		if (hit0)
			{
			(*hit)[index++] = P0[0] + t0 * raydx;
			(*hit)[index++] = P0[1] + t0 * raydy;
			(*hit)[index++] = P0[2] + t0 * raydz;
			if (transform_)
				transform_->transformPoint(&(*hit)[index-3]);
			}
		if (hit1)
			{
			(*hit)[index++] = P0[0] + t1 * raydx;
			(*hit)[index++] = P0[1] + t1 * raydy;
			(*hit)[index++] = P0[2] + t1 * raydz;
			if (transform_)
				transform_->transformPoint(&(*hit)[index-3]);
			}
		}
	return hit0 + hit1;
	}
}
