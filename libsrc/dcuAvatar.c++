#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <malloc.h>
#include "dcuAvatar.h"


dcuAvatar::dcuAvatar(volatile CAVE_USER_ST *user,headDrawFn headfn,handDrawFn handfn,
		bodyDrawFn bodyfn)
{
 user_ = (volatile CAVE_USER_ST **) CAVEMalloc(sizeof(CAVE_USER_ST *));
 headDraw_ = (volatile headDrawFn *) CAVEMalloc(sizeof(headDrawFn));
 handDraw_ = (volatile handDrawFn *) CAVEMalloc(sizeof(handDrawFn));
 bodyDraw_ = (volatile bodyDrawFn *) CAVEMalloc(sizeof(bodyDrawFn));
 drawData_ = (volatile void **) CAVEMalloc(sizeof(void *));
 bodyOffset_ = (volatile float *) CAVEMalloc(sizeof(float));
 setUser(user);
 setHeadDrawFn(headfn);
 setHandDrawFn(handfn);
 setBodyDrawFn(bodyfn);
 setDrawData(NULL);
 setBodyOffset(DCU_AVATAR_DEFAULT_BODY_OFFSET);
}

void dcuAvatar::setUser(volatile CAVE_USER_ST *user)
{
 *user_ = user;
}

void dcuAvatar::setHeadDrawFn(headDrawFn headfn)
{
 *headDraw_ = headfn;
}

void dcuAvatar::setHandDrawFn(handDrawFn handfn)
{
 *handDraw_ = handfn;
}

void dcuAvatar::setBodyDrawFn(bodyDrawFn bodyfn)
{
 *bodyDraw_ = bodyfn;
}

void dcuAvatar::setDrawData(void *data)
{
 *drawData_ = data;
}

void dcuAvatar::setBodyOffset(float offset)
{
 *bodyOffset_ = offset;
}

#ifdef OPENGL
#define PUSHMATRIX()		glPushMatrix()
#define POPMATRIX()		glPopMatrix()
#define TRANSLATE(x,y,z)	glTranslatef(x,y,z)
#define ROT_X(a)		glRotatef(a,1.0f,0.0f,0.0f)
#define ROT_Y(a)		glRotatef(a,0.0f,1.0f,0.0f)
#define ROT_Z(a)		glRotatef(a,0.0f,0.0f,1.0f)
#else
#define PUSHMATRIX()		pushmatrix()
#define POPMATRIX()		popmatrix()
#define TRANSLATE(x,y,z)	translate(x,y,z)
#define ROT_X(a)		rot(a,'x')
#define ROT_Y(a)		rot(a,'y')
#define ROT_Z(a)		rot(a,'z')
#endif

void dcuAvatar::draw(void)
{
 int i;
 float pos[3],ori[3];
 CAVE_USER_ST * u = (CAVE_USER_ST *) *user_;
 if (!u)
	return;
 if (*headDraw_)
	{
	PUSHMATRIX();
	 CAVEGetSensorPosition(&(u->sensor[0]),CAVE_NAV_FRAME,pos);
	 TRANSLATE(pos[0],pos[1],pos[2]);
	 CAVEGetSensorOrientation(&(u->sensor[0]),CAVE_NAV_FRAME,ori);
	 ROT_Y(ori[1]);
	 ROT_X(ori[0]);
	 ROT_Z(ori[2]);
	 (**headDraw_)(u,(void *)*drawData_);
	POPMATRIX();
	}
 if (*bodyDraw_)
	{
	PUSHMATRIX();
	 CAVEGetSensorPosition(&(u->sensor[0]),CAVE_NAV_FRAME,pos);
	 TRANSLATE(pos[0],pos[1] + *bodyOffset_,pos[2]);
	 CAVEGetSensorOrientation(&(u->sensor[0]),CAVE_NAV_FRAME,ori);
	 ROT_Y(ori[1]);
	 (**bodyDraw_)(u,(void *)*drawData_);
	POPMATRIX();
	}
 if (*handDraw_)
    for (i=1; i < u->num_sensors; i++)
	{
	PUSHMATRIX();
	 CAVEGetSensorPosition(&(u->sensor[i]),CAVE_NAV_FRAME,pos);
	 TRANSLATE(pos[0],pos[1],pos[2]);
	 CAVEGetSensorOrientation(&(u->sensor[i]),CAVE_NAV_FRAME,ori);
	 ROT_Y(ori[1]);
	 ROT_X(ori[0]);
	 ROT_Z(ori[2]);
	 (**handDraw_)(u,i,(void *)*drawData_);
	POPMATRIX();
	}
}

