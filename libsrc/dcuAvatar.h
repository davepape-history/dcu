#ifndef _dcuAvatar_h_
#define _dcuAvatar_h_

#include <cave.h>

#define DCU_AVATAR_DEFAULT_BODY_OFFSET	-3.0f

class dcuAvatar
	{
	public:
	 typedef void (*headDrawFn)(CAVE_USER_ST *,void *);
	 typedef void (*handDrawFn)(CAVE_USER_ST *,int,void *);
	 typedef void (*bodyDrawFn)(CAVE_USER_ST *,void *);
	 dcuAvatar(volatile CAVE_USER_ST *user=NULL,headDrawFn headfn=NULL,
			handDrawFn handfn=NULL,bodyDrawFn bodyfn=NULL);
	 virtual void setDrawData(void *data);
	 virtual void setHeadDrawFn(headDrawFn headfn);
	 virtual void setHandDrawFn(handDrawFn headfn);
	 virtual void setBodyDrawFn(bodyDrawFn headfn);
	 virtual void setUser(volatile CAVE_USER_ST *user);
	 virtual void setBodyOffset(float offset);
	 virtual void draw(void);
	protected:
	 volatile CAVE_USER_ST **user_;
	 volatile headDrawFn *headDraw_;
	 volatile handDrawFn *handDraw_;
	 volatile bodyDrawFn *bodyDraw_;
	 volatile void **drawData_;
	 volatile float *bodyOffset_;
	};

#endif
