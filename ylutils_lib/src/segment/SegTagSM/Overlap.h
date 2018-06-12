#ifndef _OVERLAP_H_
#define _OVERLAP_H_

/*      --- data structure  ---         */

typedef struct  
{
  char  word[6],cate[4];
} MLEX;                      /*  overlap word list'a record  */


#define MAANUM          700
//
extern  MLEX    *maa;	//[MAANUM]; /* AA list */
extern	int     maaid;


#define MAABBNUM        300
//
extern	MLEX    *maabb;	//[MAABBNUM]; /* AABB list */
extern	int     maabbid;


#define MABBNUM         200
//
extern	char*	mabb[MABBNUM]/*[6]*/;  /* ABB list */
extern	int     mabbid;


#define MALIABNUM       100
//
extern	char*	maliab[MALIABNUM]/*[6]*/;  /* ALiAB list */
extern	int     maliabid;

#endif

