#include <unistd.h>
#include <malloc.h>
#include <cave.h>
#include "dcuTexture.h"

static dcuTexture *texmap;
static dcuTexture *texmap2;
GLuint light;

static void initfn(void)
{
 float lightpos[] = { 0.0, 1.0, 1.0, 0.0 };
 float lightcolor[] = { 1.0, 1.0, 1.0, 1.0 };
 light = glGenLists(1);
 glNewList(light,GL_COMPILE);
 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
 glEnable(GL_LIGHT0);
 glEndList();
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 texmap->define();
 texmap2->define();
}

static void drawfn(void)
{
 float vert[3][3] = { { -3,0,-5 }, { 3,0,-5 }, { 0,8,-5 } };
 float tc[3][2] = { { 0,0 }, { 1,0 }, { 0,1 } };
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glColor3f(1.0,0.0,1.0);
 glCallList(light);
 texmap->bind();
 glNormal3f(0.0,0.0,1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord3fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord3fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord3fv(tc[2]);
  glVertex3fv(vert[2]);
 glEnd();
 texmap2->unbind();
 texmap2->bind();
 glTranslatef(2.0,5.0,0.0);
 glNormal3f(0.0,0.0,1.0);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord3fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord3fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord3fv(tc[2]);
  glVertex3fv(vert[2]);
 glEnd();
 texmap2->unbind();
 glDisable(GL_LIGHTING);
}

main(int argc,char **argv)
{
 CAVESetOption(CAVE_GL_SAMPLES,4);
 CAVEConfigure(&argc,argv,NULL);
 texmap = new dcuTexture("texture.rgb",DCU_RGBA,1);
 texmap->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
 texmap->setMagFilter(GL_LINEAR);
// texmap->setComponents(GL_RGBA4_EXT);
 texmap2 = new dcuTexture("texture2.rgb",DCU_RGBA,1);
 texmap2->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
 texmap2->setMagFilter(GL_LINEAR);
// texmap->setComponents(GL_RGBA4_EXT);
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
