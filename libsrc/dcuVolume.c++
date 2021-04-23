#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "dcuVolume.h"

dcuVolume::dcuVolume(void)
{
 dimensions_ = DCU_X_DIM | DCU_Y_DIM | DCU_Z_DIM;
 shape_ = DCU_INFINITE;
 center_[0] = center_[1] = center_[2] = 0;
 transform_ = NULL;
}

float dcuVolume::distanceSq(float,float,float)
{
 return 0;
}

float dcuVolume::distance(float x,float y,float z)
{
 return fsqrt(distanceSq(x,y,z));
}

int dcuVolume::rayIntersect(const float p0[3],const float *,float **hit,int)
{
 if (hit)
	{
	*hit = (float *) malloc(3 * sizeof(float));
	(*hit)[0] = p0[0];
	(*hit)[1] = p0[1];
	(*hit)[2] = p0[2];
	if (transform_)
		transform_->transformPoint(*hit);
	}
 return 1;
}

int dcuVolume::contains(float x,float y,float z)
{
 return (distanceSq(x,y,z) <= 0.0f);
}

int dcuVolume::dimensions(void)
{
 return dimensions_;
}

void dcuVolume::setDimensions(int v)
{
 dimensions_ = v;
}

int dcuVolume::shape(void)
{
 return shape_;
}

void dcuVolume::getCenter(float *p)
{
 p[0] = center_[0];
 p[1] = center_[1];
 p[2] = center_[2];
}

void dcuVolume::setCenter(float x,float y,float z)
{
 center_[0] = x;
 center_[1] = y;
 center_[2] = z;
}

void dcuVolume::setTransform(dcuTransform *t)
{
 transform_ = t;
}

dcuTransform * dcuVolume::getTransform(void)
{
 return transform_;
}
