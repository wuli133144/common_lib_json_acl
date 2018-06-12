/*
 *
 *    SBTmodel.c :    handling the Statistic Based Tagging model 
 *
 */
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <malloc.h>

#include	"Basicdat.h"



typedef struct  
{
  int   wordpos;                /*  China word position in words[]  */
  char  singletag[4];           /*  single tag of word  */
  int   pathpt;                 /*  point to forward node in best path  */
  double opwt;                  /*  optimim number  */
}  GRAPHELM;                    /*  direct graph's node element  */

#define AMBICHLENG      50      /*  maximum length of mutil_tag string */
#define MAXTAGNUM       20       /*  maximum tag's number in a word  */
GRAPHELM    directg[AMBICHLENG][MAXTAGNUM];

int     dmanage[AMBICHLENG];    /*  dynamit manage array */
int     dmid;



//////internals
void    GraphGen(char *CateTags[], int CTNum, int StWordPos);
void    D_project(int WFlag);
void    SetNewTag(void);

/*      extern functions       */
extern  double   ConcurP(char *, char *);               /*  from SBTprep.c  */
extern  double   WtagP(char *, char *);                 /*  from SBTprep.c  */
extern  char    *GuessTag(char *word, char *ftag, char *btag);      /*  from GuessT.c  */

//
//////// Outside function prototypes	
//
//
void    Stat_Disambi(void);                    
double	CountSegPathScore(char *CateTags[], int CTNum);

/*      purpose :      implement SBT model .
 */
void    Stat_Disambi()
{
    char	*SentStTag = "^";   		/*	the starting tag for a sentence	*/ 
    char	*SentEndTag = "$";		/* the ending tag for a sentence	*/

	int     i=0,start,end,clen,CTNum=0, WSum;
	char    ftag[10],btag[10], *CateTags[AMBICHLENG+2];

	WSum = GetWordSum();
	while   (i<WSum)         
	{
		clen=strlen(GetCate(i));
		while ((i<WSum) && clen && (strchr(GetCate(i),'-')==NULL))  
			clen=strlen(GetCate(++i));

		start=i;
		while ((i<WSum) && (!clen || ((clen>2) && (strchr(GetCate(i),'-') != NULL))))        
		{
			if (!clen )  {                             /* undefined word */
				if (i==0)   
					strcpy(ftag,"^");
				else
					strcpy(ftag,GetCate(i-1));
				if (i==WSum-1)  
					strcpy(btag,"$");
				else
					strcpy(btag,GetCate(i+1));
				strcpy(GetCate(i),GuessTag(GetWord(i),ftag,btag));
			}
	    
			clen=strlen(GetCate(++i));
	    }
		end=i-1;
		if (start<=end)    
		{           /* have mutil_tag span */
	  		/*	set the pointers in category tag list	*/
	  		CTNum=0; 
	  		if (!start)	
				CateTags[CTNum++] = SentStTag;
	  		else
				CateTags[CTNum++] = GetCate(start-1);

			for (i=start; i<=end && CTNum<AMBICHLENG-1; i++)	
				CateTags[CTNum++] = GetCate(i);

			if (end+1==WSum)	
				CateTags[CTNum++] = SentEndTag;
	  		else	
				CateTags[CTNum++] = GetCate(end+1);
	    		
			
			/*	disambiguate using the Viterbi algorithm	*/
			GraphGen(CateTags,CTNum<AMBICHLENG?CTNum:AMBICHLENG-1, start-1);
			D_project(1);
			SetNewTag();
		}           /* end of if (start<... */
	}//while   (i<WSum)         
}


/*	purpose :	computer the score of a segmentation path, according the category tags in it.
 */
double	CountSegPathScore(char *CateTags[], int CTNum)
{ 
	int 	i;
	double 	PathScore=0.0;
	
    GraphGen(CateTags,CTNum,0);
	D_project(0);

	for (i=0; i<MAXTAGNUM; i++)	
		if (directg[dmid-1][i].opwt>PathScore)
			PathScore = directg[dmid-1][i].opwt;
	
	return PathScore;
}


////////////////////////// internals ////////////////////


/*      purpose :      generate tag span graph for SBT .
 *      argument :  	CateTags[] -- the pointer list of category tags
 *						CTNum -- the number of the elements in list    
 *						StWordPos -- the starting word position for the category list
 */
void    GraphGen(char *CateTags[], int CTNum, int StWordPos)
{
	int     cnum,tnum,chlen,bufid;
	char    tagbuf[50], catebuf[50];

	/*	initialize direct gragp	*/
	for (cnum=0; cnum<AMBICHLENG; cnum++)
		for (tnum=0; tnum<MAXTAGNUM; tnum++)	{
			directg[cnum][tnum].wordpos = directg[cnum][tnum].pathpt =0;
			directg[cnum][tnum].singletag[0] = '\0';
			directg[cnum][tnum].opwt = 0.0;
		}	
	for  (cnum=0;  cnum<CTNum;  cnum++)  
	{       /*  process mutil_tag chain */
		tnum=chlen=bufid=0;
		strcpy(catebuf,CateTags[cnum]);
	   
		while  ((size_t)chlen<strlen(catebuf))   {           /*  process a mutil_tag word */
			tagbuf[bufid++]=catebuf[chlen++];
			if ((catebuf[chlen]=='-') || (catebuf[chlen]=='\0'))       
			{
				tagbuf[bufid++]='\0';
				directg[cnum][tnum].wordpos=cnum+StWordPos;
				strcpy(directg[cnum][tnum].singletag,tagbuf);
				bufid=0;  
				chlen++;
				tnum++;
	       }
		}
	   
		dmanage[cnum]=tnum;
	}
	
	dmid=CTNum;
}


/*      purpose :      dynamic programming processing .
 *	argument :	WFlag --- the flag for using word/cate probability :  1 -- use, 0 -- don't use
 */
void    D_project(int WFlag)
{
	int     cnum,tnum,ftnum,optpt;
	double   maxp,bufp;

	cnum=tnum=0;                    /* initial */
	for (tnum=0; tnum<dmanage[cnum]; tnum++)	{
		directg[cnum][tnum].pathpt=-1;
		directg[cnum][tnum].opwt=1.0;
	}
	for (cnum=1;  cnum<dmid;  cnum++)       
	{
		for (tnum=0;  tnum<dmanage[cnum];  tnum++)    
		{
			ftnum=optpt=0;
	     	maxp=directg[cnum-1][ftnum].opwt *
				   ConcurP(directg[cnum-1][ftnum].singletag,directg[cnum][tnum].singletag);
	     	
			while  (++ftnum<dmanage[cnum-1])   
			{
	       		bufp=directg[cnum-1][ftnum].opwt * ConcurP(directg[cnum-1][ftnum].singletag,
															directg[cnum][tnum].singletag);
	       		if (bufp>maxp)   
				{
		 			maxp=bufp;
		 			optpt=ftnum;
		 		}
			}
	     	directg[cnum][tnum].pathpt=optpt;
	     	
			if (WFlag)	
				bufp=maxp * WtagP(GetWord(directg[cnum][tnum].wordpos),
				               directg[cnum][tnum].singletag);
	     	else	bufp=maxp;
	     	
			directg[cnum][tnum].opwt=bufp;
		}
	}
}


/*      purpose :      reverse best path to set new tags
 */
void    SetNewTag()
{
	int     pathse,newid,cnum;

	cnum=dmid-1;
	newid=directg[cnum][0].wordpos;
	pathse=directg[cnum][0].pathpt;

	while  (pathse!=-1)     {
	  strcpy(GetCate(--newid),directg[--cnum][pathse].singletag);
	  pathse=directg[cnum][pathse].pathpt;
	}
}

