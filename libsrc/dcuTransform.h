#ifndef _dcuTransform_h_
#define _dcuTransform_h_

#include <cave.h>


class dcuTransform
	{
	public:
	 dcuTransform(void);
	 virtual void glTransform(void);
	 virtual void makeIdentity(void);
	 virtual void translate(float x,float y,float z);
	 virtual void rotate(float angle,float x,float y,float z);
	 virtual void scale(float x,float y,float z);
	 virtual void scale(float s) { scale(s,s,s); }
	 virtual void getMatrix(float m[4][4]);
	 virtual void getInverseMatrix(float inv[4][4]);
	 virtual void loadMatrix(float m[4][4]);
	 virtual void postMultMatrix(float m[4][4]);
	 virtual void postMultMatrix(float *m);
	 virtual void preMultMatrix(float m[4][4]);
	 virtual void preMultMatrix(float *m);
	 virtual void transformPoint(float v[3]);
	 virtual void transformVector(float v[3]);
	 virtual void inverseTransformPoint(float v[3]);
	 virtual void inverseTransformVector(float v[3]);
	private:
	 volatile float *matrix_;	/* in shared memory */
	 volatile int *changed_;
	 volatile float *invMatrix_;
	 CAVELOCK lock_;
	 void getLatestInverse(void);
	};

#endif
