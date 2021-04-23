#include <unistd.h>
#include <malloc.h>
#include <cave_ogl.h>
#include "dcuWFAvatar.h"
#include "dcuTexture.h"

static dcuAvatar *avatar;
GLuint light;

static void initfn(void)
{
 float lightpos[] = { 0.0, 1.0, 1.0, 0.0 };
 float lightcolor[] = { 1.0, 1.0, 1.0, 1.0 };
 glClearColor(0.0,0.0,0.0,0.0);
 light = glGenLists(1);
 glNewList(light,GL_COMPILE);
 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
 glEnable(GL_LIGHT0);
 glEndList();
 glEnable(GL_NORMALIZE);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

static void drawfn(void)
{
 float vert[3][3] = { { -20,0,-20 }, { 20,0,-20 }, { 0,0,10 } };
 int i;
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glCallList(light);
 glColor3f(1.0,0.0,1.0);
 glNormal3f(0.0,1.0,0.0);
 glBegin(GL_TRIANGLE_STRIP);
  glVertex3fv(vert[0]);
  glVertex3fv(vert[1]);
  glVertex3fv(vert[2]);
 glEnd();
 for (i=1; i<*CAVENumUsers; i++)
	{
	avatar->setUser(CAVEUser[i]);
	avatar->draw();
	}
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_LIGHTING);
}

main(int argc,char **argv)
{
 CAVESetOption(CAVE_NET_UPDATELOCALDATA,1);
 CAVEConfigure(&argc,argv,NULL);
 avatar = new dcuWFAvatar(CAVEUser[0],"head.obj","hand.obj","body.obj");
 CAVEInit();
 CAVEInitApplication(initfn,0);
 CAVEDisplay(drawfn,0);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	sginap(10);
	}
 CAVEExit();
 return 0;
}
