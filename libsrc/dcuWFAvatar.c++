#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <malloc.h>
#include "dcuWFAvatar.h"

static void drawHeadObj(CAVE_USER_ST *,void *data);
static void drawBodyObj(CAVE_USER_ST *,void *data);
static void drawHandObj(CAVE_USER_ST *,int,void *data);


dcuWFAvatar::dcuWFAvatar(volatile CAVE_USER_ST *user,char * headfile,
			char * handfile,char * bodyfile) : dcuAvatar(user)
{
 headObj_ = (volatile wfObject **) CAVEMalloc(sizeof(wfObject *));
 handObj_ = (volatile wfObject **) CAVEMalloc(sizeof(wfObject *));
 bodyObj_ = (volatile wfObject **) CAVEMalloc(sizeof(wfObject *));
 if (headfile)
	setHeadObj(wfReadObject(headfile));
 if (handfile)
	setHandObj(wfReadObject(handfile));
 if (bodyfile)
	setBodyObj(wfReadObject(bodyfile));
 setHeadDrawFn(drawHeadObj);
 setHandDrawFn(drawHandObj);
 setBodyDrawFn(drawBodyObj);
 setDrawData(this);
}

void dcuWFAvatar::setHeadObj(wfObject *obj)
{
 *headObj_ = (volatile wfObject *) obj;
}

void dcuWFAvatar::setHandObj(wfObject *obj)
{
 *handObj_ = (volatile wfObject *) obj;
}

void dcuWFAvatar::setBodyObj(wfObject *obj)
{
 *bodyObj_ = (volatile wfObject *) obj;
}

static void drawHeadObj(CAVE_USER_ST *,void *data)
{
 dcuWFAvatar * avatar = (dcuWFAvatar *)data;
/* Note: if objects are to be stored in shared memory, you'll need a non-shared flag */
/*   to tell when to initialize them */
 if (avatar->headObj())
	wfDrawObject(avatar->headObj());
}

static void drawBodyObj(CAVE_USER_ST *,void *data)
{
 dcuWFAvatar * avatar = (dcuWFAvatar *)data;
 if (avatar->bodyObj())
	wfDrawObject(avatar->bodyObj());
}

static void drawHandObj(CAVE_USER_ST *,int,void *data)
{
 dcuWFAvatar * avatar = (dcuWFAvatar *)data;
 if (avatar->handObj())
	wfDrawObject(avatar->handObj());
}
