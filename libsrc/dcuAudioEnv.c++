#include <stdio.h>
#include <math.h>
#include <string.h>
#include <bstring.h>
#include <cave.h>
#include <vssClient.h>
#include "dcuAudioEnv.h"
#include "dcuSample.h"
#include "dcuLoopSample.h"
#include "dcuTriggerSample.h"
#include "dcuWandTriggerSample.h"
#include "dcuRandomSample.h"
#include "dcu.h"


typedef struct _sListEntry sListEntry_t;

struct _sListEntry
	{
	int prevState;
	dcuSample *sample;
	sListEntry_t *prev,*next;
	};


static dcuSample *makeNewSample(char *type);
static void parseSampleDescr(dcuSample *sample,char *args,int linenum,char *file);


dcuAudioEnv::dcuAudioEnv(void)
{
 sampList_ = NULL;
 defaultDirectory_ = strdup(".");
}

dcuAudioEnv::dcuAudioEnv(char *env_file)
{
 sampList_ = NULL;
 defaultDirectory_ = strdup(".");
 readEnvFile(env_file);
}

void dcuAudioEnv::setDefaultDirectory(char *v)
{
 if (defaultDirectory_)
	free(defaultDirectory_);
 defaultDirectory_ = strdup(v);
}

void dcuAudioEnv::addSample(dcuSample *sample)
{
 sListEntry_t *s = new sListEntry_t;
 s->sample = sample;
 s->prevState = 0;
 s->prev = NULL;
 s->next = sampList_;
 if (sampList_)
	sampList_->prev = s;
 sampList_ = s;
/* Set sample directory, if it doesn't have one already */
 if (defaultDirectory_)
	if (!sample->directory())
		sample->setDirectory(defaultDirectory_);
}

void dcuAudioEnv::removeSample(dcuSample *sample)
{
 sListEntry_t *s;
 for (s = sampList_; s; s = s->next)
	if (s->sample == sample)
		break;
 if (s)
	{
	if (s->next)
		s->next->prev = s->prev;
	if (s->prev)
		s->prev->next = s->next;
	else
		sampList_ = s->next;
	if (s->prevState)
		sample->stop();
	delete s;
	}
}

#define WHITESPACE " \t"

void dcuAudioEnv::readEnvFile(char *env_file)
{
 FILE *fp;
 char line[256],*keyword,*args,*p;
 int linenum=0;
 if (!(fp = fopen(env_file,"r")))
	{
	perror(env_file);
	return;
	}
 while (fgets(line,256,fp))
	{
	linenum++;
	if (p = strchr(line,'\n'))
		*p = '\0';
	args = line;
	keyword = dcuNextToken(&args);
	if ((!keyword) || (!*keyword) || (*keyword == '#'))
		continue;
	args += strspn(args,WHITESPACE);
	if ((!strcasecmp(keyword,"dir")) || (!strcasecmp(keyword,"directory")))
		setDefaultDirectory(args);
	else
		{
		dcuSample *sample = makeNewSample(keyword);
		if (sample)
			{
			sample->setDirectory(defaultDirectory_);
			parseSampleDescr(sample,args,linenum,env_file);
			addSample(sample);
			}
		}
	}
 fclose(fp);
}

static dcuSample *makeNewSample(char *type)
{
 typedef enum {
		LOOP_SAMPLE,
		RANDOM_SAMPLE,
		TRIGGER_SAMPLE,
		WANDTRIGGER_SAMPLE,
		} sample_type_t;
 struct _dcukeyword type_table[] = {
		{ "loop", LOOP_SAMPLE },
		{ "random", RANDOM_SAMPLE },
		{ "trigger", TRIGGER_SAMPLE },
		{ "wandtrigger", WANDTRIGGER_SAMPLE },
		{ NULL, -1 }
		};
 switch (dcuGetStringID(type,type_table))
	{
	case LOOP_SAMPLE:
		return new dcuLoopSample;
	case RANDOM_SAMPLE:
		return new dcuRandomSample;
	case TRIGGER_SAMPLE:
		return new dcuTriggerSample;
	case WANDTRIGGER_SAMPLE:
		return new dcuWandTriggerSample;
	default:
		fprintf(stderr,"dcuAudioEnv: Unrecognized sample type \"%s\"\n",
			type);
	}
 return NULL;
}

static void parseSampleDescr(dcuSample *sample,char *args,int linenum,char *file)
{
 char *token;
 if (!(token = dcuNextToken(&args)))
	{
	fprintf(stderr,"parseSampleDescr error: no file name given "
		"(line %d of \"%s\")\n",linenum,file);
	return;
	}
 sample->setFile(token);
 while (token = dcuNextToken(&args))
	if (!sample->parseOption(token))
		fprintf(stderr,"parseSampleDescr error: unrecognized option"
			" \"%s\" (line %d of \"%s\")\n", token,linenum,file);
}



void dcuAudioEnv::update(int state)
{
 sListEntry_t *s;
 if (state)
	{
	float userPos[3],curTime;
	CAVEGetPosition(CAVE_HEAD_NAV,userPos);
	curTime = CAVEGetTime();
	for (s = sampList_; s; s = s->next)
		{
		if (!s->prevState)
			s->sample->start();
		s->sample->update(userPos,curTime);
		s->prevState = state;
		}
	}
 else
	{
	for (s = sampList_; s; s = s->next)
		{
		if (s->prevState)
			s->sample->stop();
		s->prevState = state;
		}
	}
}


dcuAudioEnv::~dcuAudioEnv(void)
{
 removeAllSamples(0);
 if (defaultDirectory_)
	free(defaultDirectory_);
}

void dcuAudioEnv::removeAllSamples(int del)
{
 sListEntry_t *s,*snext=NULL;
 for (s = sampList_; s; s = snext)
	{
	s->sample->stop();
	if (del)
		delete s->sample;
	snext = s->next;
	delete s;
	}
 sampList_ = NULL;
}
