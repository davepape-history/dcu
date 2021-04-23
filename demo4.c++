#include <unistd.h>
#include <malloc.h>
#include <cave_ogl.h>
#include "dcuAvatar.h"
#include "dcuTexture.h"

static dcuTexture *headTexmap;
static dcuTexture *bodyTexmap;
static dcuTexture *handTexmap;
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
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 headTexmap->define();
 bodyTexmap->define();
 handTexmap->define();
}

static void headdrawfn(CAVE_USER_ST *,void *)
{
 float vert[4][3] = { { -0.5,-0.5,0 }, { 0.5,-0.5,0 }, { -0.5,0.5,0 }, { 0.5,0.5,0 } };
 float tc[4][2] = { { 0,0 }, { 1,0 }, { 0,1 }, { 1,1 } };
 headTexmap->bind();
 glNormal3f(0.0,0.0,1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord3fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord3fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord3fv(tc[2]);
  glVertex3fv(vert[2]);
  glTexCoord3fv(tc[3]);
  glVertex3fv(vert[3]);
 glEnd();
}

static void handdrawfn(CAVE_USER_ST *,int,void *)
{
 float vert[4][3] = { { -0.5,0,-0.5 }, { 0.5,0,-0.5 }, { -0.5,0,0.5 }, { 0.5,0,0.5 } };
 float tc[4][2] = { { 0,0 }, { 1,0 }, { 0,1 }, { 1,1 } };
 handTexmap->bind();
 glNormal3f(0.0,1.0,0.0);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord3fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord3fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord3fv(tc[2]);
  glVertex3fv(vert[2]);
  glTexCoord3fv(tc[3]);
  glVertex3fv(vert[3]);
 glEnd();
}

static void bodydrawfn(CAVE_USER_ST *,void *)
{
 float vert[4][3] = { { -1,-2,0 }, { 1,-2,0 }, { -1,2,0 }, { 1,2,0 } };
 float tc[4][2] = { { 0,0 }, { 1,0 }, { 0,1 }, { 1,1 } };
 bodyTexmap->bind();
 glNormal3f(0.0,0.0,1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord3fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord3fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord3fv(tc[2]);
  glVertex3fv(vert[2]);
  glTexCoord3fv(tc[3]);
  glVertex3fv(vert[3]);
 glEnd();
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
 headTexmap = new dcuTexture("head.rgb");
 handTexmap = new dcuTexture("hand.rgb");
 bodyTexmap = new dcuTexture("body.rgb");
 avatar = new dcuAvatar(CAVEUser[0],headdrawfn,handdrawfn,bodydrawfn);
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
