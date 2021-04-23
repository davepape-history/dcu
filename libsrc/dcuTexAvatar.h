#ifndef _dcuTexAvatar_h_
#define _dcuTexAvatar_h_

#include <cave.h>
#include "dcuTexture.h"
#include "dcuAvatar.h"

class dcuTexAvatar : public dcuAvatar
	{
	public:
	 typedef float corner_t[2];
	 dcuTexAvatar(volatile CAVE_USER_ST *user,char * headfile=NULL,
			char * handfile=NULL,char * bodyfile=NULL);
	 dcuTexAvatar(volatile CAVE_USER_ST *user,dcuTexture * headtex=NULL,
			dcuTexture * handtex=NULL,dcuTexture * bodytex=NULL);
	 virtual void setHeadTex(dcuTexture *tex);
	 virtual void setHandTex(dcuTexture *tex);
	 virtual void setBodyTex(dcuTexture *tex);
	 virtual void setHeadCorners(corner_t *c);
	 virtual void setHandCorners(corner_t *c);
	 virtual void setBodyCorners(corner_t *c);
	 virtual dcuTexture * headTex(void) { return (dcuTexture*)*headTex_; }
	 virtual dcuTexture * handTex(void) { return (dcuTexture*)*handTex_; }
	 virtual dcuTexture * bodyTex(void) { return (dcuTexture*)*bodyTex_; }
	protected:
	 volatile dcuTexture **headTex_;
	 volatile dcuTexture **handTex_;
	 volatile dcuTexture **bodyTex_;
	 volatile corner_t *headCorners_;
	 volatile corner_t *handCorners_;
	 volatile corner_t *bodyCorners_;
	private:
	 void initData(void);
	};

#endif
