#include <stdio.h>
#include <math.h>
#include <unistd.h>
#define OPENGL
#include <cave_ogl.h>
#include "wave.h"
#include "dcuTransform.h"
#include "dcuGrabber.h"
#include "dcuSphereVolume.h"

void bounce(dcuTransform *bouncer);
void init_gl(void);
void draw_object(wfObject **);
wfObject ** init_object(int argc,char **argv);
void navigate(void);

dcuGrabber *grabber;
dcuTransform *bouncer;

main(int argc,char **argv)
{
 wfObject **obj;
 dcuSphereVolume *vol;
 float center[3],radius;
 CAVEConfigure(&argc,argv,NULL);
 obj = init_object(argc,argv);
 grabber = new dcuGrabber(CAVESENSOR(1));
 bouncer = new dcuTransform();
 wfGetBoundingSphere(obj[0],center,&radius);
 vol = new dcuSphereVolume;
 vol->setCenter(center);
 vol->setRadius(radius);
 vol->setTransform(grabber);
 CAVEInit();
 if (CAVEDistribMaster())
	{
	grabber->translate(0,4,-3);
	grabber->rotate(45,1,0,0);
	bouncer->translate(5,0,-5);
	}
 CAVEInitApplication(init_gl,0);
 CAVEDisplay((CAVECALLBACK)draw_object,1,obj);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	int cb=CAVEButtonChange(1);
	if (cb==1)
		{
		float w[3];
		CAVEGetSensorPosition(grabber->sensor(),CAVE_NAV_FRAME,w);
		if (vol->contains(w))
			grabber->grab();
		}
	else if (cb==-1)
		grabber->release();
	bounce(bouncer);
/*	grabber->rotate(1,0,0,1); */
	navigate();
	sginap(2);
	}
 CAVEExit();
 return 0;
}

#define GRAV 16

void bounce(dcuTransform *bouncer)
{
 static float vy = 20, prevT=0;
 float t,dt,pos[3]={0,0,0};
 t = CAVEGetTime();
 dt = t - prevT;
 prevT = t;
 vy -= GRAV * dt;
 bouncer->translate(0.0,vy*dt,0.0);
 bouncer->transformPoint(pos);
 if (pos[1] < 0)
	vy = 20;
}

#define SPEED 5

void navigate(void)
{
 float jx=CAVE_JOYSTICK_X,jy=CAVE_JOYSTICK_Y,dt,t;
 static float prevtime = 0;
 t = CAVEGetTime();
 dt = t - prevtime;
 prevtime = t;
 if (fabs(jy)>0.2)
	{
	float wandFront[3];
	CAVEGetVector(CAVE_WAND_FRONT,wandFront);
	CAVENavTranslate(wandFront[0]*jy*SPEED*dt, wandFront[1]*jy*SPEED*dt,
			wandFront[2]*jy*SPEED*dt);
	}
 if (fabs(jx)>0.2)
	CAVENavRot(-jx*90.0f*dt,'y');
}

wfObject ** init_object(int argc,char **argv)
{
 wfObject **obj;
 obj = (wfObject **) calloc(2,sizeof(wfObject *));
 if (argc<3)
	{
	fprintf(stderr,"Usage: %s object1.obj object2.obj\n",argv[0]);
	exit(1);
	}
 if (!(obj[0] = wfReadObject(argv[1])))
	exit(1);
 if (!(obj[1] = wfReadObject(argv[2])))
	exit(1);
 return obj;
}

void init_gl(void)
{
 float lightpos[] = { 1.0, 1.0, 0.0, 0.0 };
 float lightcolor[] = { 1.0, 1.0, 1.0, 1.0 };
 glEnable(GL_LIGHTING);
 glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
 glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
 glEnable(GL_LIGHT0);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
 glEnable(GL_TEXTURE_2D);
}

void draw_object(wfObject **obj)
{
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 CAVENavTransform();
 glPushMatrix();
   grabber->glTransform();
   wfDrawObject(obj[0]);
 glPopMatrix();
 glPushMatrix();
   bouncer->glTransform();
   wfDrawObject(obj[1]);
 glPopMatrix();
}
