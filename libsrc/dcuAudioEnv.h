#ifndef __dcuAudioEnv_h__
#define __dcuAudioEnv_h__

#include "dcuSample.h"

class dcuAudioEnv
	{
	public:
	 dcuAudioEnv(void);
	 dcuAudioEnv(char *env_file);
	 ~dcuAudioEnv(void);
	 virtual void readEnvFile(char *file);
	 virtual void update(int state=1);
	 virtual void addSample(dcuSample *s);
	 virtual void removeSample(dcuSample *s);
	 virtual void removeAllSamples(int del=0);
	 char *defaultDirectory(void) { return defaultDirectory_; }
	 void setDefaultDirectory(char *v);
	protected:
	 struct _sListEntry *sampList_;
	 char *defaultDirectory_;
	};

#endif
