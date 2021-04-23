#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <malloc.h>
#include "dcuTransform.h"

#define DTOR(r) ((r)*0.01745329f)

static void MatrixMultiply(float * a,float * b,float * result);
static int invertAffineMatrix(float *in,float *out);

#define TRANSFORM(i,j)  matrix_[i*4+j]

dcuTransform::dcuTransform(void)
{
 matrix_ = (float *) CAVEMalloc(16*sizeof(float));
 changed_ = (int *) CAVEMalloc(sizeof(int));
 invMatrix_ = (float *) CAVEMalloc(16*sizeof(float));
 lock_ = CAVENewLock();
 makeIdentity();
}

void dcuTransform::glTransform(void)
{
 CAVESetReadLock(lock_);
#ifdef OPENGL
 glMultMatrixf((float *)matrix_);
#else
 multmatrix((Matrix) matrix_);
#endif
 CAVEUnsetReadLock(lock_);
}

void dcuTransform::makeIdentity(void)
{
 CAVESetWriteLock(lock_);
 bzero((void *)matrix_,16*sizeof(float));
 TRANSFORM(0,0) = TRANSFORM(1,1) = TRANSFORM(2,2) = TRANSFORM(3,3) = 1;
 bzero((void *)invMatrix_,16*sizeof(float));
 invMatrix_[0] = invMatrix_[5] = invMatrix_[10] = invMatrix_[15] = 1;
 *changed_ = 0;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::loadMatrix(float m[4][4])
{
 CAVESetWriteLock(lock_);
 matrix_[0] = m[0][0];
 matrix_[1] = m[0][1];
 matrix_[2] = m[0][2];
 matrix_[3] = m[0][3];
 matrix_[4] = m[1][0];
 matrix_[5] = m[1][1];
 matrix_[6] = m[1][2];
 matrix_[7] = m[1][3];
 matrix_[8] = m[2][0];
 matrix_[9] = m[2][1];
 matrix_[10] = m[2][2];
 matrix_[11] = m[2][3];
 matrix_[12] = m[3][0];
 matrix_[13] = m[3][1];
 matrix_[14] = m[3][2];
 matrix_[15] = m[3][3];
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::getMatrix(float m[4][4])
{
 CAVESetReadLock(lock_);
 m[0][0] = matrix_[0];
 m[0][1] = matrix_[1];
 m[0][2] = matrix_[2];
 m[0][3] = matrix_[3];
 m[1][0] = matrix_[4];
 m[1][1] = matrix_[5];
 m[1][2] = matrix_[6];
 m[1][3] = matrix_[7];
 m[2][0] = matrix_[8];
 m[2][1] = matrix_[9];
 m[2][2] = matrix_[10];
 m[2][3] = matrix_[11];
 m[3][0] = matrix_[12];
 m[3][1] = matrix_[13];
 m[3][2] = matrix_[14];
 m[3][3] = matrix_[15];
 CAVEUnsetReadLock(lock_);
}

void dcuTransform::getInverseMatrix(float inv[4][4])
{
 getLatestInverse();
 CAVESetReadLock(lock_);
 inv[0][0] = invMatrix_[0];
 inv[0][1] = invMatrix_[1];
 inv[0][2] = invMatrix_[2];
 inv[0][3] = invMatrix_[3];
 inv[1][0] = invMatrix_[4];
 inv[1][1] = invMatrix_[5];
 inv[1][2] = invMatrix_[6];
 inv[1][3] = invMatrix_[7];
 inv[2][0] = invMatrix_[8];
 inv[2][1] = invMatrix_[9];
 inv[2][2] = invMatrix_[10];
 inv[2][3] = invMatrix_[11];
 inv[3][0] = invMatrix_[12];
 inv[3][1] = invMatrix_[13];
 inv[3][2] = invMatrix_[14];
 inv[3][3] = invMatrix_[15];
 CAVEUnsetReadLock(lock_);
}

void dcuTransform::postMultMatrix(float m[4][4])
{
 CAVESetWriteLock(lock_);
 MatrixMultiply((float *)matrix_,(float *)m,(float *)matrix_);
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::preMultMatrix(float m[4][4])
{
 CAVESetWriteLock(lock_);
 MatrixMultiply((float *)m,(float *)matrix_,(float *)matrix_);
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::postMultMatrix(float *m)
{
 CAVESetWriteLock(lock_);
 MatrixMultiply((float *)matrix_,m,(float *)matrix_);
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::preMultMatrix(float *m)
{
 CAVESetWriteLock(lock_);
 MatrixMultiply(m,(float *)matrix_,(float *)matrix_);
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::translate(float x,float y,float z)
{
 CAVESetWriteLock(lock_);
 TRANSFORM(3,0) += x;
 TRANSFORM(3,1) += y;
 TRANSFORM(3,2) += z;
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::rotate(float angle,float x,float y,float z)
{
 float len,sinth,costh,costhcomp,rotmat[16];
 len = fsqrt(x*x + y*y + z*z);
 if (len < 0.0000001)
	{
	fprintf(stderr,"dcuTransform::rotate warning: axis too small\n");
	return;
	}
 x /= len;
 y /= len;
 z /= len;
 angle = DTOR(angle);
 sinth = fsin(angle);
 costh = fcos(angle);
 costhcomp = 1.0f - costh;
 rotmat[0] = costhcomp*x*x + costh;
 rotmat[1] = costhcomp*x*y + sinth*z;
 rotmat[2] = costhcomp*x*z - sinth*y;
 rotmat[3] = 0;
 rotmat[4] = costhcomp*x*y - sinth*z;
 rotmat[5] = costhcomp*y*y + costh;
 rotmat[6] = costhcomp*y*z + sinth*x;
 rotmat[7] = 0;
 rotmat[8] = costhcomp*x*z + sinth*y;
 rotmat[9] = costhcomp*y*z - sinth*x;
 rotmat[10] = costhcomp*z*z + costh;
 rotmat[11] = 0;
 rotmat[12] = rotmat[13] = rotmat[14] = 0;
 rotmat[15] = 1;
 postMultMatrix(rotmat);
}

void dcuTransform::scale(float x,float y,float z)
{
 CAVESetWriteLock(lock_);
 matrix_[0] *= x;
 matrix_[1] *= y;
 matrix_[2] *= z;
 matrix_[4] *= x;
 matrix_[5] *= y;
 matrix_[6] *= z;
 matrix_[8] *= x;
 matrix_[9] *= y;
 matrix_[10] *= z;
 matrix_[12] *= x;
 matrix_[13] *= y;
 matrix_[14] *= z;
 *changed_ = 1;
 CAVEUnsetWriteLock(lock_);
}

void dcuTransform::transformPoint(float v[3])
{
 float x,y,z;
 CAVESetReadLock(lock_);
 x = v[0] * matrix_[0] + v[1] * matrix_[4] + v[2] * matrix_[8] + matrix_[12];
 y = v[0] * matrix_[1] + v[1] * matrix_[5] + v[2] * matrix_[9] + matrix_[13];
 z = v[0] * matrix_[2] + v[1] * matrix_[6] + v[2] * matrix_[10] + matrix_[14];
 CAVEUnsetReadLock(lock_);
 v[0] = x;
 v[1] = y;
 v[2] = z;
}

void dcuTransform::transformVector(float v[3])
{
 float x,y,z;
 CAVESetReadLock(lock_);
 x = v[0] * matrix_[0] + v[1] * matrix_[4] + v[2] * matrix_[8];
 y = v[0] * matrix_[1] + v[1] * matrix_[5] + v[2] * matrix_[9];
 z = v[0] * matrix_[2] + v[1] * matrix_[6] + v[2] * matrix_[10];
 CAVEUnsetReadLock(lock_);
 v[0] = x;
 v[1] = y;
 v[2] = z;
}

void dcuTransform::inverseTransformPoint(float v[3])
{
 float x,y,z;
 getLatestInverse();
 CAVESetReadLock(lock_);
 x = v[0] * invMatrix_[0] + v[1] * invMatrix_[4] + v[2] * invMatrix_[8] +
	invMatrix_[12];
 y = v[0] * invMatrix_[1] + v[1] * invMatrix_[5] + v[2] * invMatrix_[9] +
	invMatrix_[13];
 z = v[0] * invMatrix_[2] + v[1] * invMatrix_[6] + v[2] * invMatrix_[10] +
	invMatrix_[14];
 CAVEUnsetReadLock(lock_);
 v[0] = x;
 v[1] = y;
 v[2] = z;
}

void dcuTransform::inverseTransformVector(float v[3])
{
 float x,y,z;
 getLatestInverse();
 CAVESetReadLock(lock_);
 x = v[0] * invMatrix_[0] + v[1] * invMatrix_[4] + v[2] * invMatrix_[8];
 y = v[0] * invMatrix_[1] + v[1] * invMatrix_[5] + v[2] * invMatrix_[9];
 z = v[0] * invMatrix_[2] + v[1] * invMatrix_[6] + v[2] * invMatrix_[10];
 CAVEUnsetReadLock(lock_);
 v[0] = x;
 v[1] = y;
 v[2] = z;
}

void dcuTransform::getLatestInverse(void)
{
 CAVESetWriteLock(lock_);
 if (*changed_)
	{
	invertAffineMatrix((float *)matrix_,(float *)invMatrix_);
	*changed_ = 0;
	}
 CAVEUnsetWriteLock(lock_);
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


static int invertAffineMatrix(float *in,float *out)
{
 double det_l,pos=0,neg=0,temp;
#define ACCUMULATE \
	if (temp >= 0.0) pos += temp; \
	else neg += temp;
 temp = in[0] * in[5] * in[10];
 ACCUMULATE
 temp = in[1] * in[6] * in[8];
 ACCUMULATE
 temp = in[2] * in[4] * in[9];
 ACCUMULATE
 temp = -in[2] * in[5] * in[8];
 ACCUMULATE
 temp = -in[1] * in[4] * in[10];
 ACCUMULATE
 temp = -in[0] * in[6] * in[9];
 ACCUMULATE
 det_l = (pos + neg);
 if (fabs(det_l/(pos-neg)) < 1e-15)
	{
	fprintf(stderr,"ERROR: dcuTransform::invertAffineMatrix: matrix is not invertable\n");
	return 0;
	}
 det_l = 1.0/det_l;
 out[0] = (in[5]*in[10] - in[6]*in[9]) * det_l;
 out[4] = (in[6]*in[8] - in[4]*in[10]) * det_l;
 out[8] = (in[4]*in[9] - in[5]*in[8]) * det_l;
 out[1] = (in[2]*in[9] - in[1]*in[10]) * det_l;
 out[5] = (in[0]*in[10] - in[2]*in[8]) * det_l;
 out[9] = (in[1]*in[8] - in[0]*in[9]) * det_l;
 out[2] = (in[1]*in[6] - in[2]*in[5]) * det_l;
 out[6] = (in[2]*in[4] - in[0]*in[6]) * det_l;
 out[10] = (in[0]*in[5] - in[1]*in[4]) * det_l;
 out[12] = -(in[12]*out[0] + in[13]*out[4] + in[14]*out[8]);
 out[13] = -(in[12]*out[1] + in[13]*out[5] + in[14]*out[9]);
 out[14] = -(in[12]*out[2] + in[13]*out[6] + in[14]*out[10]);
 out[3] = out[7] = out[11] = 0.0;
 out[15] = 1.0;
 return 1;
}
