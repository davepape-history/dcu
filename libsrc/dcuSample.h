#ifndef __dcuSample_h__
#define __dcuSample_h__

#ifndef __GL_GL_H__
typedef int Boolean;
#endif
#include <vssClient.h>
#include "dcuVolume.h"

#define DCU_DEFAULT_MAX_DIST 50.0f


class dcuSample
	{
	public:
	 dcuSample(char *file=NULL,float maxAmpl=1);
	 ~dcuSample(void);
	 virtual void start(void);
	 virtual void play(float *userPos=NULL,float curTime=0);
	 virtual void update(float *userPos,float curTime);
	 virtual void stop(void);
	 virtual int  isPlaying(void);
	 virtual int parseOption(char *opt);
	 virtual dcuVolume *volume(void);
	 virtual void setVolume(dcuVolume *v);
	 virtual char *directory(void);
	 virtual void setDirectory(char *v);
	 virtual char *file(void);
	 virtual void setFile(char *v);
	 virtual float length(void);
	 virtual void setLength(float v);
	 virtual float maxAmplitude(void);
	 virtual void setMaxAmplitude(float v);
	 virtual float maxDistance(void);
	 virtual void setMaxDistance(float v);
	 virtual float attack(void);
	 virtual void setAttack(float v);
	 virtual float decay(void);
	 virtual void setDecay(float v);
	protected:
	 virtual float computeAmplitude(float *userPos,float curTime);
	 virtual float computeScale(float curTime);
/* Setup data */
	 char *directory_;
	 char *file_;
	 float length_;
	 float maxAmpl_;
	 float maxDistSq_;
	 dcuVolume *volume_;
	 float attack_, decay_;
/* Playback data */
	 float actor_,note_;
	 float startTime_,endTime_;
	 float ampl_;
	};

#endif
