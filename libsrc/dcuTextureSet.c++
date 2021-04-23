#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <GL/glu.h>
#include "dcuTextureSet.h"

struct _dcuTexList
	{
	char *filename;
	dcuTexture *texture;
	struct _dcuTexList *next;
	};

dcuTextureSet::dcuTextureSet(void)
{
 minFilter_ = GL_NEAREST;
 magFilter_ = GL_NEAREST;
 wrapS_ = GL_REPEAT;
 wrapT_ = GL_REPEAT;
 mipmap_ = 0;
 byteOrder_ = DCU_ABGR;
 components_ = 4;
 dontChangeTextureMode_ = 0;
 defined_ = 0;
 texList_ = NULL;
}

dcuTexture * dcuTextureSet::getTexture(char *filename)
{
 struct _dcuTexList *p;
 for (p=texList_; p; p=p->next)
	if (!strcmp(p->filename,filename))
		break;
 if (!p)
	{
	p = new struct _dcuTexList;
	p->filename = strdup(filename);
	p->texture = new dcuTexture(filename,byteOrder_,mipmap_);
	p->texture->setMinFilter(minFilter_);
	p->texture->setMagFilter(magFilter_);
	p->texture->setWrapT(wrapT_);
	p->texture->setWrapS(wrapS_);
	p->texture->setComponents(components_);
	if (dontChangeTextureMode_)
		p->texture->dontChangeTextureMode();
	p->next = texList_;
	texList_ = p;
	}
 return p->texture;
}

void dcuTextureSet::define(void)
{
 struct _dcuTexList *p;
 for (p=texList_; p; p=p->next)
	if (p->texture)
		p->texture->define();
 defined_ = 1;
}

void dcuTextureSet::setMinFilter(GLint v)
{
 minFilter_ = v;
}

void dcuTextureSet::setMagFilter(GLint v)
{
 magFilter_ = v;
}

void dcuTextureSet::setWrapS(GLint v)
{
 wrapS_ = v;
}

void dcuTextureSet::setWrapT(GLint v)
{
 wrapT_ = v;
}

void dcuTextureSet::setComponents(GLint v)
{
 components_ = v;
}

void dcuTextureSet::setMipmap(int v)
{
 mipmap_ = v;
}

void dcuTextureSet::setByteOrder(int v)
{
 byteOrder_ = v;
}

void dcuTextureSet::dontChangeTextureMode(void)
{
 dontChangeTextureMode_ = 0;
}

int dcuTextureSet::defined(void)
{
 return defined_;
}
