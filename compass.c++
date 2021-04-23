#include <stdio.h>
#include <math.h>
#include <unistd.h>
#define OPENGL
#include <cave_ogl.h>
#include "dcuTransform.h"
#include "dcuGrabber.h"
#include "dcuSphereVolume.h"

void bounce(dcuTransform *bouncer);
void init_gl(void);
void draw_all(void);
void navigate(void);
void doGrab(int sensorNum,dcuVolume *vol);

dcuGrabber *grabber;
dcuTransform *bouncer;

main(int argc,char **argv)
{
 dcuSphereVolume *boundVol;
 CAVEConfigure(&argc,argv,NULL);
 grabber = new dcuGrabber(CAVESENSOR(1));
 bouncer = new dcuTransform();
 boundVol = new dcuSphereVolume;
 boundVol->setRadius(1.0);
 boundVol->setTransform(grabber);
 CAVEInit();
 if (CAVEDistribMaster())
	{
	grabber->translate(3,4,-3);
	grabber->rotate(45,1,0,0);
//	bouncer->translate(5,0,-5);
	}
 CAVEInitApplication(init_gl,0);
 CAVEDisplay((CAVECALLBACK)draw_all,0);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	int cb1=CAVEButtonChange(1),cb2=CAVEButtonChange(2);
	if (cb1==1)
		doGrab(1,boundVol);
	else if ((cb1==-1) && (grabber->sensor() == CAVESENSOR(1)))
		grabber->release();
	if (cb2==1)
		doGrab(2,boundVol);
	else if ((cb2==-1) && (grabber->sensor() == CAVESENSOR(2)))
		grabber->release();
	bounce(bouncer);
	navigate();
	sginap(2);
	}
 CAVEExit();
 return 0;
}

void doGrab(int sensorNum,dcuVolume *vol)
{
 float w[3];
 CAVEGetSensorPosition(CAVESENSOR(sensorNum),CAVE_NAV_FRAME,w);
 if (vol->contains(w))
	{
	grabber->setSensor(CAVESENSOR(sensorNum));
	grabber->grab();
	}
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

GLuint light;

void init_gl(void)
{
 float lightpos[] = { 0.0, 1.0, 1.0, 0.0 };
 float lightcolor[] = { 1.0, 1.0, 1.0, 1.0 };
 int one=1;
 glClearColor(0.0,0.0,0.0,0.0);
 light = glGenLists(1);
 glNewList(light,GL_COMPILE);
 glEnable(GL_LIGHTING);
 glLightModeliv(GL_LIGHT_MODEL_TWO_SIDE,&one);
 glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
 glEnable(GL_LIGHT0);
 glEndList();
 glEnable(GL_NORMALIZE);
}

void draw_compass(void)
{
 float vert[][3] = { { -1,0,-1 }, { 1,0,-1 }, { 1,0,1 } , { -1,0,1 }};
 float lv[][3] = { { 0,0.1,0 }, { 0,0.1,1 }};
 float polePos[3] = { 0,0,0 }, mag;
 glColor3f(0.4,0.4,0.8);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[0]);
  glVertex3fv(vert[1]);
  glVertex3fv(vert[3]);
  glVertex3fv(vert[2]);
 glEnd();
 glColor3f(1.0,0.0,0.0);
 bouncer->transformPoint(polePos);
 polePos[1] = 0;
 grabber->inverseTransformPoint(polePos);
 mag = fsqrt(polePos[0]*polePos[0] + polePos[2]*polePos[2]);
 if (mag <= 0.000001f)
	{
	printf("mag: %f\n",mag);
	return;
	}
 glBegin(GL_LINE_STRIP);
  glVertex3fv(lv[0]);
  lv[1][0] = lv[0][0]+polePos[0]/mag;
  lv[1][2] = lv[0][2]+polePos[2]/mag;
  glVertex3fv(lv[1]);
 glEnd();
}

void draw_pole(void)
{
 float vert[][3] = { { -1,0,-1 }, { 1,0,-1 }, { 1,0,1 } , { -1,0,1 },
			{ -1,2,-1 }, { 1,2,-1 }, { 1,2,1 } , { -1,2,1 } };
 glNormal3f(0.0,0.0,-1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[0]);
  glVertex3fv(vert[1]);
  glVertex3fv(vert[4]);
  glVertex3fv(vert[5]);
 glEnd();
 glNormal3f(1.0,0.0,0.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[1]);
  glVertex3fv(vert[2]);
  glVertex3fv(vert[5]);
  glVertex3fv(vert[6]);
 glEnd();
 glNormal3f(0.0,0.0,1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[2]);
  glVertex3fv(vert[3]);
  glVertex3fv(vert[6]);
  glVertex3fv(vert[7]);
 glEnd();
 glNormal3f(-1.0,0.0,0.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[3]);
  glVertex3fv(vert[0]);
  glVertex3fv(vert[7]);
  glVertex3fv(vert[4]);
 glEnd();
}

void draw_all(void)
{
 float vert[3][3] = { { -20,0,-20 }, { 20,0,-20 }, { 0,0,20 } };
 glClearColor(0.4,0.6,1.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 CAVENavTransform();
 glCallList(light);
 glNormal3f(0.0,1.0,0.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[0]);
  glVertex3fv(vert[1]);
  glVertex3fv(vert[2]);
 glEnd();
 glPushMatrix();
   bouncer->glTransform();
   draw_pole();
 glPopMatrix();
 glDisable(GL_LIGHTING);
 glPushMatrix();
   grabber->glTransform();
   draw_compass();
 glPopMatrix();
}
