#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <bstring.h>
#include <cave_ogl.h>
#include "glImage.h"
#include "dcuSharedImage.h"

static void reverseLongwords(unsigned long *img,int size);


dcuSharedImage::dcuSharedImage(char *filename,int byteOrder)
{
 filename_ = (volatile char * *) CAVEMalloc(sizeof(char *));
 byteOrder_ = (volatile int *) CAVEMalloc(sizeof(int));
 image_ = (volatile unsigned long * *) CAVEMalloc(sizeof(unsigned long *));
 xdim_ = (volatile int *) CAVEMalloc(sizeof(int));
 ydim_ = (volatile int *) CAVEMalloc(sizeof(int));
 *filename_ = NULL;
 *byteOrder_ = byteOrder;
 *image_ = NULL;
 *xdim_ = *ydim_ = 0;
 if (filename)
	loadFile(filename);
}

void dcuSharedImage::setByteOrder(int byteOrder)
{
 if ((*byteOrder_ != byteOrder) && (*image_))
	reverseLongwords((unsigned long *)*image_,(*xdim_)*(*ydim_));
 *byteOrder_ = byteOrder;
}

int dcuSharedImage::loadFile(char *filename)
{
 if (*image_)
	CAVEFree((void *)*image_);
 *image_ = NULL;
 if (*filename_)
	CAVEFree((void *)filename_);
 *filename_ = NULL;
 if (filename)
	{
	*filename_ = (volatile char *) CAVEMalloc(strlen(filename)+1);
	strcpy((char *)*filename_,filename);
	ReadSgiImageLong();
	if (!*image_)
		{
		CAVEFree((void *)filename_);
		*filename_ = NULL;
		}
	}
 if (!*image_)
	return 0;
 return 1;
}

void dcuSharedImage::setImage(unsigned long *image,int xdim,int ydim)
{
 if (*image_)
	CAVEFree((void *)*image_);
 if (*filename_)
	CAVEFree((void *)*filename_);
 *image_ = (volatile unsigned long *)image;
 *xdim_ = xdim;
 *ydim_ = ydim;
 *filename_ = (volatile char *)CAVEMalloc(1);
 **filename_ = '\0';
}

void dcuSharedImage::copyImage(unsigned long *image,int xdim,int ydim)
{
 volatile unsigned long *newimg;
 newimg = (volatile unsigned long *) CAVEMalloc(xdim*ydim*sizeof(unsigned long));
 if (!newimg)
	{
	fprintf(stderr,"ERROR: dcuSharedImage::copyImage: failed to allocate "
		"space for image\n");
	return;
	}
 memcpy((void*)newimg,(void*)image,xdim*ydim*sizeof(unsigned long));
 if (*image_)
	CAVEFree((void *)*image_);
 if (*filename_)
	CAVEFree((void *)*filename_);
 *image_ = newimg;
 *xdim_ = xdim;
 *ydim_ = ydim;
 *filename_ = (volatile char *)CAVEMalloc(1);
 **filename_ = '\0';
}

void dcuSharedImage::createImage(int xdim,int ydim)
{
 volatile unsigned long *newimg;
 newimg = (volatile unsigned long *) CAVEMalloc(xdim*ydim*sizeof(unsigned long));
 bzero((void *)newimg,xdim*ydim*sizeof(unsigned long));
 if (*image_)
	CAVEFree((void *)*image_);
 *image_ = newimg;
 *xdim_ = xdim;
 *ydim_ = ydim;
 if (filename_)
	CAVEFree((void *)filename_);
 *filename_ = (volatile char *)CAVEMalloc(1);
 **filename_ = '\0';
}

int dcuSharedImage::ReadSgiImageLong(void)
{
 register IMAGE *imagefp;
 register int x,y;
 unsigned short *rbuf,*gbuf,*bbuf,*abuf;
 unsigned long *p;
 int zdim;
 if ((imagefp=iopen((char*)*filename_,"r")) == NULL ) {
	fprintf(stderr,"dcuSharedImage::ReadSgiImageLong: can't open input file %s\n",
		*filename_);
	return 0;
	}
 *xdim_ = imagefp->xsize;
 *ydim_ = imagefp->ysize;
 zdim = imagefp->zsize;
 rbuf = (unsigned short *) malloc(imagefp->xsize*sizeof(unsigned short));
 if (zdim>1) gbuf = (unsigned short *) malloc(imagefp->xsize*sizeof(unsigned short));
 else gbuf = rbuf;
 if (zdim>2) bbuf = (unsigned short *) malloc(imagefp->xsize*sizeof(unsigned short));
 else bbuf = gbuf;
 if (zdim>3) abuf = (unsigned short *) malloc(imagefp->xsize*sizeof(unsigned short));
 *image_ = (volatile unsigned long *) CAVEMalloc(imagefp->xsize*imagefp->ysize*
						sizeof(unsigned long));
 p = (unsigned long *) *image_;
 for (y=0; y < imagefp->ysize; y++)
	{
	getrow(imagefp,rbuf,y,0);
	if (zdim > 1) getrow(imagefp,gbuf,y,1);
	if (zdim > 2) getrow(imagefp,bbuf,y,2);
	if (zdim > 3)
		{
		getrow(imagefp,abuf,y,3);
		if (*byteOrder_ == DCU_ABGR)
			for (x=0; x<imagefp->xsize; x++)
				*p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) |
					 (abuf[x]<<24);
		else
			for (x=0; x<imagefp->xsize; x++)
				*p++ = abuf[x] | (bbuf[x]<<8) | (gbuf[x]<<16) |
					 (rbuf[x]<<24);
		}
	else
		{
		if (*byteOrder_ == DCU_ABGR)
			for (x=0; x<imagefp->xsize; x++)
				*p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) |
					0xff000000;
		else
			for (x=0; x<imagefp->xsize; x++)
				*p++ = 0xff | (bbuf[x]<<8) | (gbuf[x]<<16) |
					 (rbuf[x]<<24);
		}
	}
 free(rbuf);
 if (zdim>1) free(gbuf);
 if (zdim>2) free(bbuf);
 if (zdim>3) free(abuf);
 iclose(imagefp);
 return 1;
}

static void reverseLongwords(unsigned long *img,int size)
{
 int i;
 for (i=0; i<size; i++, img++)
	*img =  ((*img & 0x000000ff) << 24) |
		((*img & 0x0000ff00) << 8) |
		((*img & 0x00ff0000) >> 8) |
		((*img & 0xff000000) >> 24);
}
