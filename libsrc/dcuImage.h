#ifndef _dcuImage_h_
#define _dcuImage_h_

#include "dcu.h"

class dcuImage
	{
	public:
	 dcuImage(char *filename=NULL,int byteOrder=DCU_ABGR);
	 virtual void setByteOrder(int);
	 virtual int loadFile(char *filename);
	 virtual void writeFile(char *filename);
	 virtual void setImage(unsigned long *image,int xdim,int ydim);
	 virtual void copyImage(unsigned long *image,int xdim,int ydim);
	 virtual void createImage(int xdim,int ydim);
	 virtual dcuImage * resizedImage(int xdim,int ydim);
	 virtual char *filename(void) { return filename_; }
	 virtual unsigned long *image(void) { return image_; }
	 virtual int xdim(void) { return xdim_; }
	 virtual int ydim(void) { return ydim_; }
	 virtual void dimensions(int *dim) { dim[0] = xdim_; dim[1] = ydim_; }
	 virtual int byteOrder(void) { return byteOrder_; }
	private:
	 char *filename_;
	 int byteOrder_;
	 unsigned long *image_;
	 int xdim_,ydim_;
	 void MakeDummyImage(void);
	 int ReadSgiImageLong(void);
	};

#endif
