#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "dcuBoxVolume.h"

dcuBoxVolume::dcuBoxVolume(void)
{
 shape_ = DCU_BOX;
 setCenter(0,0,0);
 setSize(0,0,0);
}

void dcuBoxVolume::getRange(float *v)
{
 v[0] = center_[0] - size_[0]/2.0f;
 v[1] = center_[0] + size_[0]/2.0f;
 v[2] = center_[1] - size_[1]/2.0f;
 v[3] = center_[1] + size_[1]/2.0f;
 v[4] = center_[2] - size_[2]/2.0f;
 v[5] = center_[2] + size_[2]/2.0f;
}

void dcuBoxVolume::setRange(float minX,float maxX,float minY,float maxY,float minZ,float maxZ)
{
 setCenter((minX+maxX)/2.0f,(minY+maxY)/2.0f,(minZ+maxZ)/2.0f);
 setSize(maxX-minX,maxY-minY,maxZ-minZ);
}

void dcuBoxVolume::setSize(float x,float y,float z)
{
 size_[0] = x;
 size_[1] = y;
 size_[2] = z;
}

void dcuBoxVolume::getSize(float *s)
{
 s[0] = size_[0];
 s[1] = size_[1];
 s[2] = size_[2];
}

float dcuBoxVolume::distanceSq(float x,float y,float z)
{
 float minX,maxX,minY,maxY,minZ,maxZ,dx=0,dy=0,dz=0,pos[3];
 pos[0] = x;
 pos[1] = y;
 pos[2] = z;
 if (transform_)
	transform_->inverseTransformPoint(pos);
 minX = center_[0] - size_[0]/2.0f;
 maxX = center_[0] + size_[0]/2.0f;
 minY = center_[1] - size_[1]/2.0f;
 maxY = center_[1] + size_[1]/2.0f;
 minZ = center_[2] - size_[2]/2.0f;
 maxZ = center_[2] + size_[2]/2.0f;

 if (dimensions_ & DCU_X_DIM)
	{
	if (pos[0] >= minX)
		if (pos[0] <= maxX)
			dx = 0;
		else
			dx = pos[0] - maxX;
	else
		dx = minX - pos[0];
	}
 if (dimensions_ & DCU_Y_DIM)
	{
	if (pos[1] >= minY)
		if (pos[1] <= maxY)
			dy = 0;
		else
			dy = pos[1] - maxY;
	else
		dy = minY - pos[1];
	}
 if (dimensions_ & DCU_Z_DIM)
	{
	if (pos[2] >= minZ)
		if (pos[2] <= maxZ)
			dz = 0;
		else
			dz = pos[2] - maxZ;
	else
		dz = minZ - pos[2];
	}
 return dx*dx + dy*dy + dz*dz;
}

int dcuBoxVolume::rayIntersect(const float inp0[3],const float inp1[3],float **hit,int raytype)
{
 float rayd[3],P0[3],P1[3],hitpt[6][3];
 int numhits,i;
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
 rayd[0] = P1[0] - P0[0];
 rayd[1] = P1[1] - P0[1];
 rayd[2] = P1[2] - P0[2];
 numhits = 0;
 for (i=0; i<3; i++)
    if (fabs(rayd[i]) > 0.000001f)
	{
	numhits += testRay(P0,rayd,i,-1,hitpt[numhits]);
	numhits += testRay(P0,rayd,i,1,hitpt[numhits]);
	}
 if ((numhits) && (hit))
	{
	*hit = (float *) malloc(numhits * 3 * sizeof(float));
	for (i=0; i<numhits; i++)
		{
		(*hit)[i*3] = hitpt[i][0];
		(*hit)[i*3+1] = hitpt[i][1];
		(*hit)[i*3+2] = hitpt[i][2];
		if (transform_)
			transform_->transformPoint(&(*hit)[i*3]);
		}
	}
 return numhits;
}

int dcuBoxVolume::testRay(float *P0,float *rayd,int dim,int dir,float *hit)
{
 float min[3],max[3],t,pt[3];
 min[0] = center_[0] - size_[0]/2.0f;
 max[0] = center_[0] + size_[0]/2.0f;
 min[1] = center_[1] - size_[1]/2.0f;
 max[1] = center_[1] + size_[1]/2.0f;
 min[2] = center_[2] - size_[2]/2.0f;
 max[2] = center_[2] + size_[2]/2.0f;
 if (dir < 0)
	t = (min[dim] - P0[dim]) / rayd[dim];
 else
	t = (max[dim] - P0[dim]) / rayd[dim];
 pt[0] = P0[0] + t*rayd[0];
 pt[1] = P0[1] + t*rayd[1];
 pt[2] = P0[2] + t*rayd[2];
 if ((pt[0] >= min[0]) && (pt[0] <= max[0]) &&
     (pt[1] >= min[1]) && (pt[1] <= max[1]) &&
     (pt[2] >= min[2]) && (pt[2] <= max[2]))
	{
	hit[0] = pt[0];
	hit[1] = pt[1];
	hit[2] = pt[2];
	return 1;
	}
 return 0;
}
