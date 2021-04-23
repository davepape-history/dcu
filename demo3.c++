#include <unistd.h>
#include <malloc.h>
#include <cave.h>
#include "dcuTexture.h"
#include "dcuSharedImage.h"

static dcuTexture *texmap;
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
}

static void defineTexture(void)
{
 texmap->define();
}

static void drawfn(void)
{
 float vert[3][3] = { { -3,0,-5 }, { 3,0,-5 }, { 0,8,-5 } };
 float tc[3][2] = { { 0,0 }, { 1,0 }, { 0,1 } };
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glColor4f(1.0,0.0,1.0,0.5);
 glCallList(light);
 glEnable(GL_TEXTURE_2D);
 texmap->bind();
 glNormal3f(0.0,0.0,1.0);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2fv(tc[0]);
  glVertex3fv(vert[0]);
  glTexCoord2fv(tc[1]);
  glVertex3fv(vert[1]);
  glTexCoord2fv(tc[2]);
  glVertex3fv(vert[2]);
 glEnd();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_LIGHTING);
}

main(int argc,char **argv)
{
 dcuSharedImage *img;
 CAVEConfigure(&argc,argv,NULL);
 texmap = new dcuTexture;
 img = new dcuSharedImage;
 texmap->setImage(img);
 texmap->setMipmap();
 texmap->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
 texmap->setMagFilter(GL_LINEAR);
 CAVEInit();
 CAVEInitApplication(initfn,0);
 CAVEDisplay(drawfn,0);
 sleep(3);
 img->loadFile("texture.rgb");
 CAVEInitApplication(defineTexture,0);
 sleep(3);
 img->loadFile("texture2.rgb");
 CAVEInitApplication(defineTexture,0);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	sginap(10);
	}
 CAVEExit();
 return 0;
}
