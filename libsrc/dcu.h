#ifndef __dcu_h__
#define __dcu_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Volume shapes */
#define DCU_INFINITE	2000
#define DCU_POINT	2001
#define DCU_SPHERE	2002
#define DCU_BOX		2003
#define DCU_CYLINDER	2004

/* Dimensions for volumes */
#define DCU_X_DIM	1
#define DCU_Y_DIM	2
#define DCU_Z_DIM	4

/* Ray types for volume->rayIntersect() */
#define DCU_LINE	100
#define DCU_RAY		101
#define DCU_SEGMENT	102

/* Byte orders for images */
#define DCU_ABGR	200
#define DCU_RGBA	201	


struct _dcukeyword {
		char *str;
		int id;
		};


void dcuDumpStereoWindow(char *basename);
void dcuDumpWindow(char *imgname);
char *dcuNextToken(char **string);
int dcuGetStringID(char *str,struct _dcukeyword *table);
int dcuQueryGLExtension(char *name);

extern char *dcuVersion;


#ifdef __cplusplus
}
#endif

#endif
