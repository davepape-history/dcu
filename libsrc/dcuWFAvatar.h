#ifndef _dcuWFAvatar_h_
#define _dcuWFAvatar_h_

#include <cave.h>
#include "wave.h"
#include "dcuAvatar.h"

class dcuWFAvatar : public dcuAvatar
	{
	public:
	 dcuWFAvatar(volatile CAVE_USER_ST *user,char * headfile=NULL,
			char * handfile=NULL,char * bodyfile=NULL);
	 virtual void setHeadObj(wfObject *obj);
	 virtual void setHandObj(wfObject *obj);
	 virtual void setBodyObj(wfObject *obj);
	 virtual wfObject * headObj(void) { return (wfObject*)*headObj_; }
	 virtual wfObject * handObj(void) { return (wfObject*)*handObj_; }
	 virtual wfObject * bodyObj(void) { return (wfObject*)*bodyObj_; }
	protected:
	 volatile wfObject **headObj_;
	 volatile wfObject **handObj_;
	 volatile wfObject **bodyObj_;
	};

#endif
