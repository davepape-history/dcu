#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <GL/glu.h>
#include "dcuTexture.h"

dcuTexture::dcuTexture(char *filename,int byteOrder,int mipmap)
{
 id_ = 0;
 minFilter_ = GL_NEAREST;
 magFilter_ = GL_NEAREST;
 wrapS_ = GL_REPEAT;
 wrapT_ = GL_REPEAT;
 components_ = 4;
 mipmap_ = mipmap;
 useBindExt_ = 0;
 changeTextureMode_ = 1;
 image_ = new dcuImage(filename,byteOrder);
}

void dcuTexture::define(void)
{
 int format,xdim2,ydim2;
 void *imgp;
 unsigned long *rescaledImage=NULL;
 if ((!image_) || (!image_->image()))
	return;
 if (image_->byteOrder() == DCU_ABGR)
	format = GL_ABGR_EXT;
 else
	format = GL_RGBA;
 useBindExt_ = dcuQueryGLExtension("GL_EXT_texture_object");
 xdim2 = 1;
 while (xdim2 <= image_->xdim())
	xdim2 *= 2;
 xdim2 /= 2;
 ydim2 = 1;
 while (ydim2 <= image_->ydim())
	ydim2 *= 2;
 ydim2 /= 2;
 if ((image_->xdim() != xdim2) || (image_->ydim() != ydim2))
	{
	rescaledImage = (unsigned long *) malloc(xdim2 * ydim2 *
						sizeof(unsigned long));
	gluScaleImage(format,image_->xdim(),image_->ydim(),GL_UNSIGNED_BYTE,
			image_->image(), xdim2,ydim2,GL_UNSIGNED_BYTE,
			rescaledImage);
	imgp = rescaledImage;
	}
 else
	imgp = image_->image();
 if (!id_)
	{
#ifdef TEXTURE_OBJECT
	if (useBindExt_)
		glGenTexturesEXT(1,&id_);
	else
#endif
		id_ = glGenLists(1);
	}
 if (components_ != 4)
	{
	if (!dcuQueryGLExtension("GL_EXT_texture"))
		components_ = 4;
	}
#ifdef TEXTURE_OBJECT
 if (useBindExt_)
	glBindTextureEXT(GL_TEXTURE_2D,id_);
 else
#endif
	glNewList(id_,GL_COMPILE);
 if (mipmap_)
	gluBuild2DMipmaps(GL_TEXTURE_2D,components_,xdim2,ydim2,format,GL_UNSIGNED_BYTE,
			imgp);
 else
	glTexImage2D(GL_TEXTURE_2D,0,components_,xdim2,ydim2,0,format,GL_UNSIGNED_BYTE,
			imgp);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magFilter_);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter_);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS_);
 glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapT_);
#ifdef TEXTURE_OBJECT
 if (useBindExt_)
	glBindTextureEXT(GL_TEXTURE_2D,0);
 else
#endif
	glEndList();
/*
 if (rescaledImage)
	free(rescaledImage);
*/
}

void dcuTexture::bind(void)
{
 if (id_)
	{
#ifdef TEXTURE_OBJECT
	if (useBindExt_)
		glBindTextureEXT(GL_TEXTURE_2D,id_);
	else
#endif
		glCallList(id_);
	if (changeTextureMode_)
		glEnable(GL_TEXTURE_2D);
	}
 else
	{
#ifdef TEXTURE_OBJECT
	if (useBindExt_)
		glBindTextureEXT(GL_TEXTURE_2D,0);
#endif
	if (changeTextureMode_)
		glDisable(GL_TEXTURE_2D);
	}
}

void dcuTexture::unbind(void)
{
#ifdef TEXTURE_OBJECT
 if (useBindExt_)
	glBindTextureEXT(GL_TEXTURE_2D,0);
#endif
 if (changeTextureMode_)
	glDisable(GL_TEXTURE_2D);
}


void dcuTexture::setImage(dcuImage *im)
{
 image_ = im;
}

void dcuTexture::setMinFilter(GLint v)
{
 minFilter_ = v;
}

void dcuTexture::setMagFilter(GLint v)
{
 magFilter_ = v;
}

void dcuTexture::setWrapS(GLint v)
{
 wrapS_ = v;
}

void dcuTexture::setWrapT(GLint v)
{
 wrapT_ = v;
}

void dcuTexture::setComponents(GLint v)
{
 components_ = v;
}

void dcuTexture::setMipmap(int v)
{
 mipmap_ = v;
}

void dcuTexture::setID(GLuint v)
{
 id_ = v;
}

void dcuTexture::dontChangeTextureMode(void)
{
 changeTextureMode_ = 0;
}

int dcuTexture::defined(void)
{
 return (id_ != 0);
}
