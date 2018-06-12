/***
 *
 * seg.h :   data structure & function prototype for segmentation
 *
 ***/


#include	"Limits.h"

/*  extern data */
extern  char*	pThisSentence;  // the "internal" pointer to cur-sentence
extern  int     basept,seekpt,temppt;
extern  int     segflag[];                      /* segment flags */
extern  char    segcate[][MAX_WordCatesLen+1];  /* tagging flags */

extern  int     maxposit, segpossible;
extern	int		HaveSegFlag;/* flag of the word that have be segmented*/

#define INTERSECT       333
#define COMBINE         334


/*   extern functions	*/
extern	int     seg_word(int  num, char  *cate);
extern	int     SegProcess();
extern	int     GetWUnit(int  base, int  num,  char  *buf);


