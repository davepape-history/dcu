#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <malloc.h>
#include <cave.h>
#include "dcuGrabber.h"

#define DTOR(r) ((r)*0.01745329f)

static void MatrixMultiply(float * a,float * b,float * result);

dcuGrabber::dcuGrabber(CAVE_SENSOR_ST *sensor)
{
 grabbed_ = (int *) CAVEMalloc(sizeof(int));
 *grabbed_ = 0;
 sensor_ = (CAVE_SENSOR_ST **) CAVEMalloc(sizeof(CAVE_SENSOR_ST *));
 *sensor_ = sensor;
}

void dcuGrabber::setSensor(CAVE_SENSOR_ST *sensor)
{
 int wasGrabbed = *grabbed_;
 if (wasGrabbed)
	release();
 *sensor_ = sensor;
 if (wasGrabbed)
	grab();
}

CAVE_SENSOR_ST * dcuGrabber::sensor(void)
{
 return (CAVE_SENSOR_ST *) *sensor_;
}

int dcuGrabber::grabbed(void)
{
 return *grabbed_;
}

void dcuGrabber::glTransform(void)
{
 if ((*grabbed_) && (*sensor_))
	{
	float pos[3],ori[3];
	CAVEGetSensorPosition((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,pos);
#ifdef OPENGL
	glTranslatef(pos[0],pos[1],pos[2]);
#else
	translate(pos[0],pos[1],pos[2]);
#endif
	CAVEGetSensorOrientation((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,ori);
#ifdef OPENGL
	glRotatef(ori[1],0.0f,1.0f,0.0f);
	glRotatef(ori[0],1.0f,0.0f,0.0f);
	glRotatef(ori[2],0.0f,0.0f,1.0f);
#else
	rot(ori[1],'y');
	rot(ori[0],'x');
	rot(ori[2],'z');
#endif
	}
 dcuTransform::glTransform();
}

void dcuGrabber::transformPoint(float v[3])
{
 dcuTransform::transformPoint(v);
 if ((*grabbed_) && (*sensor_))
	{
	float x,y,z,mat[16];
	GetSensorMatrix(mat);
	x = v[0] * mat[0] + v[1] * mat[4] + v[2] * mat[8] + mat[12];
	y = v[0] * mat[1] + v[1] * mat[5] + v[2] * mat[9] + mat[13];
	z = v[0] * mat[2] + v[1] * mat[6] + v[2] * mat[10] + mat[14];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	}
}

void dcuGrabber::transformVector(float v[3])
{
 dcuTransform::transformVector(v);
 if ((*grabbed_) && (*sensor_))
	{
	float x,y,z,mat[16];
	GetSensorMatrix(mat);
	x = v[0] * mat[0] + v[1] * mat[4] + v[2] * mat[8];
	y = v[0] * mat[1] + v[1] * mat[5] + v[2] * mat[9];
	z = v[0] * mat[2] + v[1] * mat[6] + v[2] * mat[10];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	}
}

void dcuGrabber::inverseTransformPoint(float v[3])
{
 if ((*grabbed_) && (*sensor_))
	{
	float x,y,z,mat[16];
	GetInverseSensorMatrix(mat);
	x = v[0] * mat[0] + v[1] * mat[4] + v[2] * mat[8] + mat[12];
	y = v[0] * mat[1] + v[1] * mat[5] + v[2] * mat[9] + mat[13];
	z = v[0] * mat[2] + v[1] * mat[6] + v[2] * mat[10] + mat[14];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	}
 dcuTransform::inverseTransformPoint(v);
}

void dcuGrabber::inverseTransformVector(float v[3])
{
 if ((*grabbed_) && (*sensor_))
	{
	float x,y,z,mat[16];
	GetInverseSensorMatrix(mat);
	x = v[0] * mat[0] + v[1] * mat[4] + v[2] * mat[8];
	y = v[0] * mat[1] + v[1] * mat[5] + v[2] * mat[9];
	z = v[0] * mat[2] + v[1] * mat[6] + v[2] * mat[10];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	}
 dcuTransform::inverseTransformVector(v);
}

void dcuGrabber::grab(void)
{
 if ((!*grabbed_) && (*sensor_))
	{
	float invmat[16];
	GetInverseSensorMatrix(invmat);
	preMultMatrix(invmat);
	}
 *grabbed_ = 1;
}

void dcuGrabber::release(void)
{
 if ((*grabbed_) && (*sensor_))
	{
	float sensormat[16];
	GetSensorMatrix(sensormat);
	preMultMatrix(sensormat);
	}
 *grabbed_ = 0;
}

void dcuGrabber::GetSensorMatrix(float * m)
{
 float xrotmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float yrotmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float zrotmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float or[3],rx,ry,rz,pos[3];
 CAVEGetSensorPosition((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,pos); 
 CAVEGetSensorOrientation((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,or); 
 rx = DTOR(or[0]);
 ry = -DTOR(or[1]);
 rz = DTOR(or[2]);
 bzero((void *)m,64);
 m[0] = m[5] = m[10] = m[15] = 1;
 m[12] = pos[0];
 m[13] = pos[1];
 m[14] = pos[2];
 yrotmat[0*4+0] = yrotmat[2*4+2] = fcos(ry);
 yrotmat[0*4+2] = fsin(ry);
 yrotmat[2*4+0] = -yrotmat[0*4+2];
 xrotmat[1*4+1] = xrotmat[2*4+2] = fcos(rx);
 xrotmat[1*4+2] = fsin(rx);
 xrotmat[2*4+1] = -xrotmat[1*4+2];
 zrotmat[0*4+0] = zrotmat[1*4+1] = fcos(rz);
 zrotmat[0*4+1] = fsin(rz);
 zrotmat[1*4+0] = -zrotmat[0*4+1];
 MatrixMultiply(m,yrotmat,m);
 MatrixMultiply(m,xrotmat,m);
 MatrixMultiply(m,zrotmat,m);
}

void dcuGrabber::GetInverseSensorMatrix(float * m)
{
 float xrotmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float yrotmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float transmat[] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
 float or[3],rx,ry,rz,pos[3];
 bzero((void *)m,64);
 m[0] = m[5] = m[10] = m[15] = 1;
 CAVEGetSensorOrientation((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,or); 
 rz = -DTOR(or[2]);
 m[0*4+0] = m[1*4+1] = fcos(rz);
 m[0*4+1] = fsin(rz);
 m[1*4+0] = -m[0*4+1];
 rx = -DTOR(or[0]);
 xrotmat[1*4+1] = xrotmat[2*4+2] = fcos(rx);
 xrotmat[1*4+2] = fsin(rx);
 xrotmat[2*4+1] = -xrotmat[1*4+2];
 MatrixMultiply(m,xrotmat,m);
 ry = DTOR(or[1]);
 yrotmat[0*4+0] = yrotmat[2*4+2] = fcos(ry);
 yrotmat[0*4+2] = fsin(ry);
 yrotmat[2*4+0] = -yrotmat[0*4+2];
 MatrixMultiply(m,yrotmat,m);
 CAVEGetSensorPosition((CAVE_SENSOR_ST *)*sensor_,CAVE_NAV_FRAME,pos); 
 transmat[12] = -pos[0];
 transmat[13] = -pos[1];
 transmat[14] = -pos[2];
 MatrixMultiply(m,transmat,m);
}

static void MatrixMultiply(float * a,float * b,float * out)
{
 int i,j,k;
 float result[4][4];
 float sum;
 for (i=0; i<4; i++)
    for (j=0; j<4; j++)
	{
	sum = 0;
	for (k=0; k<4; k++)
		sum += b[i*4+k] * a[k*4+j];
	result[i][j] = sum;
	}
 for (i=0; i<4; i++)
    for (j=0; j<4; j++)
	out[i*4+j] = result[i][j];
}

