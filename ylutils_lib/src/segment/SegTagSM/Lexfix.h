#ifndef _LEXFIX_H_
#define _LEXFIX_H_

typedef struct  
{
  char  word[4];            /*  lexical fix  */
  char  combcate[8];        /*  combined new word's category  */
  char  strucate[8];        /*  possible category of root  */
}	LEXFIX;					/*  lexical fix record defination  */


#define PREFNUM         20
//
extern LEXFIX  *prefix;	//[PREFNUM];		/* prefix word list  */
extern int     preid;


#define SUFFNUM         50
//
extern LEXFIX  *suffix;	//[SUFFNUM];		/* suffix word list  */
extern int     sufid;


typedef struct  rootr   
{
  char  word[10];			/*  word information  */
  //struct  rootr *next;		/*  point to next record  */
} ROOTREC;					/*  root word record  */

#define MAXROOTRECNUM 200
//
typedef struct  specsufr   
{
  char  spsufw[3];			/*  special suffix word  */
  int	counter;
  char  word[MAXROOTRECNUM][10];		/*  coresponding root word's list  */
  //ROOTREC   rootwl[MAXROOTRECNUM];		/*  coresponding root word's list  */
} SPECSUFREC;
//
#define	SPECSUFWNUM		6
///
extern SPECSUFREC  *specsl;	//[SPECSUFWNUM];	/*  special suffix information list  */
extern int     speclid;

#endif

