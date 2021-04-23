#include <stdio.h>
#include <strings.h>
#include <GL/gl.h>
#include "dcu.h"


int dcuQueryGLExtension(char *name)
{
 char *extensions,*p;
 int len;
 if (!(extensions = (char *) glGetString(GL_EXTENSIONS)))
	return 0;
 len = strlen(name);
 p = extensions;
 while (p = strstr(p,name))
	{
	if ((p[len] == ' ') || (p[len] == '\0'))
		return 1;
	else
		p++;
	}
 return 0;
}
