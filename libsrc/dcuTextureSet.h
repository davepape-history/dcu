#ifndef __dcuTextureSet_h__
#define __dcuTextureSet_h__

#include "dcuTexture.h"

class dcuTextureSet
	{
	public:
	 dcuTextureSet(void);
	 virtual dcuTexture * getTexture(char *filename);
	 virtual void setMinFilter(GLint);
	 virtual void setMagFilter(GLint);
	 virtual void setWrapS(GLint);
	 virtual void setWrapT(GLint);
	 virtual void setWrap(GLint v) { setWrapS(v); setWrapT(v); }
	 virtual void setComponents(GLint v);
	 virtual void setMipmap(int flag=1);
	 virtual void setByteOrder(int);
	 virtual void dontChangeTextureMode(void);
	 virtual void define(void);
	 virtual GLint minFilter(void) { return minFilter_; }
	 virtual GLint magFilter(void) { return magFilter_; }
	 virtual GLint wrapS(void) { return wrapS_; }
	 virtual GLint wrapT(void) { return wrapT_; }
	 virtual int mipmap(void) { return mipmap_; }
	 virtual int byteOrder(void) { return byteOrder_; }
	 virtual int defined(void);
	protected:
	 GLint minFilter_, magFilter_, wrapS_, wrapT_, components_;
	 int mipmap_, byteOrder_, dontChangeTextureMode_, defined_;
	 struct _dcuTexList *texList_;
	};

#endif
