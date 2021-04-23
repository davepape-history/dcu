#include <stdio.h>
#include <math.h>
#include <string.h>
#include "dcuSample.h"
#include "dcuPointVolume.h"
#include "dcuSphereVolume.h"
#include "dcuBoxVolume.h"
#include "dcu.h"

static dcuVolume *createVolumeFromOption(char *opt);

dcuSample::dcuSample(char *file,float maxAmpl)
{
 directory_ = NULL;
 if (file)
	file_ = strdup(file);
 else
	file_ = NULL;
 length_ = 0;
 maxAmpl_ = maxAmpl;
 maxDistSq_ = DCU_DEFAULT_MAX_DIST * DCU_DEFAULT_MAX_DIST;
 actor_ = actorMessage(A_Create,"F",(float)SampleActor);
 note_ = -1;
 endTime_ = -0.5;
 ampl_ = 0;
 volume_ = new dcuVolume;
 attack_ = decay_ = 0;
}

void dcuSample::start(void)
{
}

void dcuSample::play(float *userPos,float curTime)
{
 startTime_ = curTime;
 endTime_ = curTime + length_;
 if (userPos)
	ampl_ = computeAmplitude(userPos,curTime);
 else
	ampl_ = maxAmpl_ * computeScale(curTime);
 note_ = actorMessage(A_BeginNote, "FFFS", actor_, 1.0, ampl_, file_);
}

void dcuSample::update(float *userPos,float curTime)
{
 if (note_ > -1)
	{
	if ((endTime_ > -1) && (endTime_ < curTime))
		stop();
	else
		{
		float newAmpl = computeAmplitude(userPos,curTime);
		if (fabs(newAmpl - ampl_) > .005)
			{
			ampl_ = newAmpl;
			actorMessage(A_SetAmpl,"FFF",actor_,note_,ampl_);
			}
		}
	}
}

float dcuSample::computeAmplitude(float *userPos,float curTime)
{
 float distSq = volume_->distanceSq(userPos);
 if (distSq > maxDistSq_)
	return 0;
 return maxAmpl_ * computeScale(curTime) * (maxDistSq_ - distSq) / maxDistSq_;
}

float dcuSample::computeScale(float curTime)
{
 if ((endTime_ > -1) && (curTime > endTime_ - decay_))
	return (endTime_ - curTime) / attack_;
 else if (curTime < startTime_ + attack_)
	return (curTime - startTime_) / attack_;
 else
	return 1.0f;
}

void dcuSample::stop(void)
{
 if (note_ > -1)
	{
	actorMessage(A_EndAllNotes,"F",actor_);
	note_ = -1;
	endTime_ = -0.5;
	ampl_ = 0;
	}
}

int dcuSample::isPlaying(void)
{
 if (note_ > -1)
	return 1;
 else
	return 0;
}

void dcuSample::setDirectory(char *v)
{
 if (directory_)
	free(directory_);
 if (v)
	{
	directory_ = strdup(v);
	actorMessage(A_SetDirectory,"FS",actor_,directory_);
	}
 else
	directory_ = NULL;
}

void dcuSample::setFile(char *v)
{
 if (file_)
	free(file_);
 if (v)
	file_ = strdup(v);
 else
	file_ = NULL;
}

dcuSample::~dcuSample(void)
{
 actorMessage(A_Delete,"F",actor_);
 if (directory_)
	free(directory_);
 if (file_)
	free(file_);
}

int dcuSample::parseOption(char *opt)
{
 enum _opt_type {
		OPT_LENGTH=1,
		OPT_POSITION,
		OPT_MAXDIST,
		OPT_MAXAMPL,
		OPT_DIMENSIONS,
		OPT_DIRECTORY,
		OPT_FILE,
		OPT_VOLUME,
		} opttype;
 struct _dcukeyword opt_table[] = {
		{ "length", OPT_LENGTH },
		{ "len", OPT_LENGTH },
		{ "duration", OPT_LENGTH },
		{ "maxdistance", OPT_MAXDIST },
		{ "maxdist", OPT_MAXDIST },
		{ "maxampl", OPT_MAXAMPL },
		{ "maxamplitude", OPT_MAXAMPL },
		{ "amplitude", OPT_MAXAMPL },
		{ "ampl", OPT_MAXAMPL },
		{ "directory", OPT_DIRECTORY },
		{ "dir", OPT_DIRECTORY },
		{ "file", OPT_FILE },
		{ "volume", OPT_VOLUME },
		{ "shape", OPT_VOLUME },
		{ "position", OPT_POSITION },
		{ "pos", OPT_POSITION },
		{ "dimensions", OPT_DIMENSIONS },
		{ "dim", OPT_DIMENSIONS },
		{ NULL, 0 }
		};
 char *eq,*val;
 float pos[3];
 int dim;
 dcuVolume *vol;
 eq = strchr(opt,'=');
 if (!eq) return 0;
 *eq = '\0';
 val = eq+1;
 opttype = (enum _opt_type) dcuGetStringID(opt,opt_table);
 switch (opttype)
	{
	case OPT_LENGTH:
		length_ = atof(val);
		break;
	case OPT_MAXDIST:
		maxDistSq_ = atof(val) * atof(val);
		break;
	case OPT_MAXAMPL:
		maxAmpl_ = atof(val);
		break;
	case OPT_DIRECTORY:
		setDirectory(val);
		break;
	case OPT_FILE:
		setFile(val);
		break;
	case OPT_VOLUME:
		dim = volume_->dimensions();
		volume_->getCenter(pos);
		vol = createVolumeFromOption(val);
		if (vol)
			{
			volume_ = vol;
			volume_->setDimensions(dim);
			volume_->setCenter(pos);
			}
		break;
	case OPT_DIMENSIONS:
		dim = 0;
		while (*val)
			{
			if ((*val == 'x') || (*val == 'X'))
				dim |= DCU_X_DIM;
			else if ((*val == 'y') || (*val == 'Y'))
				dim |= DCU_Y_DIM;
			else if ((*val == 'z') || (*val == 'Z'))
				dim |= DCU_Z_DIM;
			val++;
			}
		volume_->setDimensions(dim);
		break;
	case OPT_POSITION:
		sscanf(val,"%f,%f,%f",&pos[0],&pos[1],&pos[2]);
		volume_->setCenter(pos);
		break;
	default:
		return 0;
	}
 return 1;
}

static dcuVolume *createVolumeFromOption(char *opt)
{
 struct _dcukeyword type_table[] = {
		{ "point", DCU_POINT },
		{ "sphere", DCU_SPHERE },
		{ "box", DCU_BOX },
		{ NULL, -1 }
		};
 char *args=NULL,*comma=NULL;
 comma = strchr(opt,',');
 if (comma)
	{
	*comma = '\0';
	args = comma+1;
	}
 switch (dcuGetStringID(opt,type_table))
	{
	case DCU_POINT:
		return new dcuPointVolume;
	case DCU_SPHERE:
		{
		float radius=1;
		dcuSphereVolume *vol = new dcuSphereVolume;
		if (args)
			sscanf(args,"%f",&radius);
		vol->setRadius(radius);
		return vol;
		}
	case DCU_BOX:
		{
		float range[6] = {-1,1, -1,1, -1,1};
		dcuBoxVolume *vol = new dcuBoxVolume;
		if (args)
			sscanf(args,"%f,%f,%f,%f,%f,%f",&range[0],&range[1],
				&range[2],&range[3],&range[4],&range[5]);
		vol->setRange(range);
		return vol;
		}
	}
 return NULL;
}

dcuVolume * dcuSample::volume(void)
{
 return volume_;
}

void dcuSample::setVolume(dcuVolume *v)
{
 volume_ = v;
}

char * dcuSample::directory(void)
{
 return directory_;
}

char * dcuSample::file(void)
{
 return file_;
}

float dcuSample::length(void)
{
 return length_;
}

void dcuSample::setLength(float v)
{
 length_ = v;
}

float dcuSample::maxAmplitude(void)
{
 return maxAmpl_;
}

void dcuSample::setMaxAmplitude(float v)
{
 maxAmpl_ = v;
}

float dcuSample::maxDistance(void)
{
 return fsqrt(maxDistSq_);
}

void dcuSample::setMaxDistance(float v)
{
 maxDistSq_ = v*v;
}

float dcuSample::attack(void)
{
 return attack_;
}

void dcuSample::setAttack(float v)
{
 attack_ = v;
}

float dcuSample::decay(void)
{
 return decay_;
}

void dcuSample::setDecay(float v)
{
 decay_ = v;
}
