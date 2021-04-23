#ifndef _dcuSharedImage_h_
#define _dcuSharedImage_h_

#include "dcu.h"
#include "dcuImage.h"

class dcuSharedImage : public dcuImage
	{
	public:
	 dcuSharedImage(char *filename=NULL,int byteOrder=DCU_ABGR);
	 virtual void setByteOrder(int);
	 virtual int loadFile(char *filename);
	 virtual void setImage(unsigned long *image,int xdim,int ydim);
	 virtual void copyImage(unsigned long *image,int xdim,int ydim);
	 virtual void createImage(int xdim,int ydim);
	 virtual char *filename(void) { return (char *)*filename_; }
	 virtual unsigned long *image(void) { return (unsigned long *)*image_; }
	 virtual int xdim(void) { return (int)*xdim_; }
	 virtual int ydim(void) { return (int)*ydim_; }
	 virtual void dimensions(int *dim) {dim[0] = (int)*xdim_; dim[1] = (int)*ydim_;}
	 virtual int byteOrder(void) { return (int)*byteOrder_; }
	private:
	 volatile char **filename_;
	 volatile int *byteOrder_;
	 volatile unsigned long **image_;
	 volatile int *xdim_,*ydim_;
	 int ReadSgiImageLong(void);
	};

#endif
