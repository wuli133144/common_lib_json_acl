/*
 *
 *	sbtprep.h :		header file for sbtprep.c	
 *
 */
#ifndef _SBTPREP_H
#define _SBTPREP_H

/*      data structure defination           */
#define TAGNUM  29                      /* tag set size */
extern	long    int     (*tagmat)[TAGNUM];         /* tag concurrent matrix define */
extern  long    int     *RowTSum;                /* tag's sum in corpus file */
extern  double          (*PConTag)[TAGNUM];        /* array of coocurrence probabilties  */
//extern	long    int     tagmat[TAGNUM][TAGNUM];         /* tag concurrent matrix define */
//extern  long    int     RowTSum[TAGNUM];                /* tag's sum in corpus file */
//extern  double          PConTag[TAGNUM][TAGNUM];        /* array of coocurrence probabilties  */

typedef struct  tw      
{
  char  keyword[12];        /* word */
  char  stag[5][4];         /* mutil-tag list */
  int   freq[5];            /* frequent counter list */
} TWREC;             /* training word's stem define */

#define MAXTWORD        1100            /* maximum number of trained words  */
extern  TWREC   *twlist;	//[MAXTWORD];               /* list of trained words */
extern  int     twlid;                          /* index counter */


extern	void    InitSBTData(void);                      
extern	double  ConcurP(char   *,  char  *);   
extern	double  WtagP(char  *,  char  *);       

#endif

