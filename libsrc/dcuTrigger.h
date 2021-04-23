#ifndef __dcuTrigger_h__
#define __dcuTrigger_h__

#include "dcuVolume.h"

typedef void (*dcuCallback)(void *);

class dcuTrigger
	{
	public:
	 dcuTrigger(dcuVolume *vol=NULL);
	 virtual void update(float *pos);
	 void setEnterCallback(dcuCallback callback,void *data);
	 dcuCallback enterCallback(void);
	 void * enterCallbackData(void);
	 void setLeaveCallback(dcuCallback callback,void *data);
	 dcuCallback leaveCallback(void);
	 void * leaveCallbackData(void);
	 void setVolume(dcuVolume *v);
	 dcuVolume * volume(void);
	protected:
	 dcuCallback enterCB_, leaveCB_;
	 void *enterCBData_, *leaveCBData_;
	 dcuVolume *volume_;
	 int latch_;
	};

#endif
