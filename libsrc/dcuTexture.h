#ifndef __dcuTexture_h__
#define __dcuTexture_h__

#include <GL/gl.h>
#include "dcuImage.h"

class dcuTexture
	{
	public:
	 dcuTexture(char *filename=NULL,int byteOrder=DCU_ABGR,int mipmap=0);
	 virtual void define(void);
	 virtual void bind(void);
	 virtual void unbind(void);
	 virtual void setImage(dcuImage *);
	 virtual void setMinFilter(GLint);
	 virtual void setMagFilter(GLint);
	 virtual void setWrapS(GLint);
	 virtual void setWrapT(GLint);
	 virtual void setWrap(GLint v) { setWrapS(v); setWrapT(v); }
	 virtual void setComponents(GLint v);
	 virtual void setMipmap(int flag=1);
	 virtual void setID(GLuint id);
	 virtual void dontChangeTextureMode(void);
	 virtual dcuImage *image(void) { return image_; }
	 virtual GLint minFilter(void) { return minFilter_; }
	 virtual GLint magFilter(void) { return magFilter_; }
	 virtual GLint wrapS(void) { return wrapS_; }
	 virtual GLint wrapT(void) { return wrapT_; }
	 virtual int mipmap(void) { return mipmap_; }
	 virtual int defined(void);
	protected:
	 dcuImage *image_;
	 GLuint id_;
	 GLint minFilter_, magFilter_, wrapS_, wrapT_, components_;
	 int mipmap_;
	 int useBindExt_;
	 int changeTextureMode_;
	};

#endif
