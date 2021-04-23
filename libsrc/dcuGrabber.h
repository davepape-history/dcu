#ifndef _dcuGrabber_h_
#define _dcuGrabber_h_

#include "dcuTransform.h"

class dcuGrabber : public dcuTransform
	{
	public:
	 dcuGrabber(CAVE_SENSOR_ST *sensor=CAVESENSOR(1)); /* Default to wand sensor */
	 void setSensor(CAVE_SENSOR_ST *sensor);
	 CAVE_SENSOR_ST * sensor(void);
	 virtual void grab(void);
	 virtual void release(void);
	 virtual int grabbed(void);
	 virtual void glTransform(void);
	 virtual void transformPoint(float v[3]);
	 virtual void transformVector(float v[3]);
	 virtual void inverseTransformPoint(float v[3]);
	 virtual void inverseTransformVector(float v[3]);
	protected:
	 volatile CAVE_SENSOR_ST **sensor_;
	 volatile int *grabbed_;
	 void GetSensorMatrix(float *);
	 void GetInverseSensorMatrix(float *);
	};

#endif
