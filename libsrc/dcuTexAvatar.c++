#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <malloc.h>
#include "dcuTexAvatar.h"

static void drawHeadTex(CAVE_USER_ST *,void *data);
static void drawBodyTex(CAVE_USER_ST *,void *data);
static void drawHandTex(CAVE_USER_ST *,int,void *data);


dcuTexAvatar::dcuTexAvatar(volatile CAVE_USER_ST *user,char * headfile,
			char * handfile,char * bodyfile) : dcuAvatar(user)
{
 initData();
 if (headfile)
	setHeadTex(new dcuTexture(headfile));
 if (handfile)
	setHandTex(new dcuTexture(handfile));
 if (bodyfile)
	setBodyTex(new dcuTexture(bodyfile));
}

dcuTexAvatar::dcuTexAvatar(volatile CAVE_USER_ST *user,dcuTexture * headtex,
			dcuTexture * handtex,dcuTexture * bodytex) : dcuAvatar(user)
{
 initData();
 if (headtex)
	setHeadTex(headtex);
 if (handtex)
	setHandTex(handtex);
 if (bodytex)
	setBodyTex(bodytex);
}

void dcuTexAvatar::initData(void)
{
 headTex_ = (volatile dcuTexture **) CAVEMalloc(sizeof(dcuTexture *));
 handTex_ = (volatile dcuTexture **) CAVEMalloc(sizeof(dcuTexture *));
 bodyTex_ = (volatile dcuTexture **) CAVEMalloc(sizeof(dcuTexture *));
 *headTex_ = NULL;
 *handTex_ = NULL;
 *bodyTex_ = NULL;
 headCorners_ = (volatile corner_t *) CAVEMalloc(4*sizeof(corner_t));
 headCorners_[0][0] = -0.5;  headCorners_[0][1] = -0.5;
 headCorners_[1][0] = 0.5;  headCorners_[1][1] = -0.5;
 headCorners_[2][0] = -0.5;  headCorners_[2][1] = 0.5;
 headCorners_[3][0] = 0.5;  headCorners_[3][1] = 0.5;
 handCorners_ = (volatile corner_t *) CAVEMalloc(4*sizeof(corner_t));
 handCorners_[0][0] = -0.5;  handCorners_[0][1] = -0.5;
 handCorners_[1][0] = 0.5;  handCorners_[1][1] = -0.5;
 handCorners_[2][0] = -0.5;  handCorners_[2][1] = 0.5;
 handCorners_[3][0] = 0.5;  handCorners_[3][1] = 0.5;
 bodyCorners_ = (volatile corner_t *) CAVEMalloc(4*sizeof(corner_t));
 bodyCorners_[0][0] = -1;  bodyCorners_[0][1] = -2;
 bodyCorners_[1][0] = 1;  bodyCorners_[1][1] = -2;
 bodyCorners_[2][0] = -1;  bodyCorners_[2][1] = 2;
 bodyCorners_[3][0] = 1;  bodyCorners_[3][1] = 2;
 setHeadDrawFn(drawHeadTex);
 setHandDrawFn(drawHandTex);
 setBodyDrawFn(drawBodyTex);
 setDrawData(this);
}

void dcuTexAvatar::setHeadTex(dcuTexture *tex)
{
 *headTex_ = (volatile dcuTexture *) tex;
}

void dcuTexAvatar::setHandTex(dcuTexture *tex)
{
 *handTex_ = (volatile dcuTexture *) tex;
}

void dcuTexAvatar::setBodyTex(dcuTexture *tex)
{
 *bodyTex_ = (volatile dcuTexture *) tex;
}

static void drawHeadTex(CAVE_USER_ST *,void *data)
{
 dcuTexAvatar * avatar = (dcuTexAvatar *)data;
 dcuTexture *tex = avatar->headTex();
 if (tex)
	{
	if (!tex->defined())
		tex->define();
	tex->bind();
	tex->unbind();
	}
}

static void drawBodyTex(CAVE_USER_ST *,void *data)
{
 dcuTexAvatar * avatar = (dcuTexAvatar *)data;
 dcuTexture *tex = avatar->bodyTex();
 if (tex)
	{
	if (!tex->defined())
		tex->define();
	tex->bind();
	tex->unbind();
	}
}

static void drawHandTex(CAVE_USER_ST *,int,void *data)
{
 dcuTexAvatar * avatar = (dcuTexAvatar *)data;
 dcuTexture *tex = avatar->handTex();
 if (tex)
	{
	if (!tex->defined())
		tex->define();
	tex->bind();
	tex->unbind();
	}
}
