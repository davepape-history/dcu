#include <unistd.h>
#include <malloc.h>
#include <cave.h>
#include "dcuTrigger.h"
#include "dcuSphereVolume.h"


void drawfn(void)
{
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Enter(void *)
{
 printf("Enter\n");
}

void Leave(void *)
{
 printf("Leave\n");
}

main(int argc,char **argv)
{
 dcuSphereVolume *vol;
 dcuTrigger *trig;
 float trigpos[3] = {2,4,-3};
 CAVEConfigure(&argc,argv,NULL);
 vol = new dcuSphereVolume;
 trig = new dcuTrigger(vol);
 vol->setCenter(trigpos);
 vol->setRadius(1.0);
 trig->setEnterCallback(Enter,NULL);
 trig->setLeaveCallback(Leave,NULL);
 CAVEInit();
 CAVEDisplay(drawfn,0);
 while (!CAVEgetbutton(CAVE_ESCKEY))
	{
	float p[3];
	CAVEGetPosition(CAVE_WAND,p);
	trig->update(p);
	sginap(1);
	}
 CAVEExit();
 return 0;
}
