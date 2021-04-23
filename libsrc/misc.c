#include <stdio.h>
#include "dcu.h"

char *dcuVersion = "libdcu Version 1.2; compiled " __DATE__;


char *dcuNextToken(char **string)
{
 char *tok;
 tok = *string + strspn(*string," \t\n");
 if (!*tok)
	return NULL;
 *string = tok + strcspn(tok," \t\n");
 if (**string)
	{
	**string = '\0';
	(*string)++;
	}
 if (!*tok)
	return NULL;
 return tok;
}

int dcuGetStringID(char *str,struct _dcukeyword *table)
{
 int n;
 if (str)
    {
    for (n=0; table[n].str; n++)
	if (!strcasecmp(str,table[n].str)) return table[n].id;
    }
 return -1;
}
