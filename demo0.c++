#include <cave.h>
#include <vssClient.h>
#include "dcuAudioEnv.h"

void drawfn(void)
{
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

main(int argc,char **argv)
{
 dcuAudioEnv *env;
 CAVEConfigure(&argc,argv,NULL);
 if (argc < 2)
	return 1;
 CAVEInit();
 CAVEDisplay(drawfn,0);
 if (!BeginSoundServer())
	return 1;
 env = new dcuAudioEnv();
 env->readEnvFile(argv[1]);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	env->update(1);
	sginap(1);
	}
 env->removeAllSamples(1);
 delete env;
 EndSoundServer();
 CAVEExit();
 return 0;
}
