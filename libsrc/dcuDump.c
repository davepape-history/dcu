#include <stdio.h>
#include <cave.h>
#include <gl/image.h>

static void WriteSgiImageLong(char *fname,unsigned long *data,int xdim,int ydim);

#ifdef OPENGL

void dcuDumpStereoWindow(char *basename)
{
 char name[256];
 long xsize,ysize;
 unsigned long *image;
 glReadBuffer(GL_FRONT_RIGHT);
 CAVEGetWindowGeometry(NULL,NULL,&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 glReadPixels(0,0,xsize,ysize,GL_ABGR_EXT,GL_UNSIGNED_BYTE,image);
 sprintf(name,"%s.right.sgi",basename);
 WriteSgiImageLong(name,image,xsize,ysize);
 glReadBuffer(GL_FRONT_LEFT);
 glReadPixels(0,0,xsize,ysize,GL_ABGR_EXT,GL_UNSIGNED_BYTE,image);
 sprintf(name,"%s.left.sgi",basename);
 WriteSgiImageLong(name,image,xsize,ysize);
 free(image);
}

void dcuDumpWindow(char *imgname)
{
 long xsize,ysize;
 unsigned long *image;
 glReadBuffer(GL_FRONT);
 CAVEGetWindowGeometry(NULL,NULL,&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 glReadPixels(0,0,xsize,ysize,GL_ABGR_EXT,GL_UNSIGNED_BYTE,image);
 WriteSgiImageLong(imgname,image,xsize,ysize);
 free(image);
}

#else

void dcuDumpStereoWindow(char *basename)
{
 char name[256];
 long xsize,ysize;
 unsigned long *image;
 readsource(SRC_FRONTRIGHT);
 CAVEGetWindowGeometry(NULL,NULL,&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 sprintf(name,"%s.right.sgi",basename);
 WriteSgiImageLong(name,image,xsize,ysize);
 readsource(SRC_FRONTLEFT);
 lrectread(0,0,xsize-1,ysize-1,image);
 sprintf(name,"%s.left.sgi",basename);
 WriteSgiImageLong(name,image,xsize,ysize);
 free(image);
}

void dcuDumpWindow(char *imgname)
{
 long xsize,ysize;
 unsigned long *image;
 readsource(SRC_FRONT);
 CAVEGetWindowGeometry(NULL,NULL,&xsize,&ysize);
 image = (unsigned long *) malloc(xsize*ysize*sizeof(unsigned long));
 lrectread(0,0,xsize-1,ysize-1,image);
 WriteSgiImageLong(imgname,image,xsize,ysize);
 free(image);
}

#endif


static void WriteSgiImageLong(char *fname,unsigned long *data,int xdim,int ydim)
{
 int i,y;
 unsigned short *sbuf;
 IMAGE *image;
 unsigned long *p;
 sbuf = (unsigned short *) malloc(xdim*2);
 if (!(image = iopen(fname,"w",RLE(1),3,xdim,ydim,3)))
	{
	fprintf(stderr,"WriteSgiImageLong: can't open output file %s\n",
		fname);
	return;
	}
 for (y=0; y < ydim; y++, data+=xdim)
	{
	for (i=0, p=data; i<xdim; i++) sbuf[i] = (*p++) & 0xff;
	putrow(image,sbuf,y,0);
	for (i=0, p=data; i<xdim; i++) sbuf[i] = ((*p++) & 0xff00) >> 8;
	putrow(image,sbuf,y,1);
	for (i=0, p=data; i<xdim; i++) sbuf[i] = ((*p++) & 0xff0000) >> 16;
	putrow(image,sbuf,y,2);
	}
 iclose(image);
 free(sbuf);
}
