/////////
//	Common code for BasDict[] and SpecDict[j][] handling
//
//

#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<search.h>
#include	<assert.h>

#include	"Limits.h"
#include	"Basicdat.h"

 
/* buffer for the category information	- all possible POS (all °∞ºÊ¿‡°±!!) */
extern char    CateBuf[MAX_WordCatesLen+1];


extern int		InTagSet(char *TagSet, char *cate);
extern void		SkipComments(FILE * fp);
extern void		GetNextTokenString(FILE * fp, char * pBuff);


#ifdef UNIX_SEGTAG
	#define _cdecl	
#endif


