/****
 *      tagging.c :     word tagging initial process file
 *      purpose :       prepare words for tagging process and calculate 
						some statistic parameters.
 *      output  :       basic statistic result before tagging .
 *                        \ccmp\statres\pretstat.res
 ****/
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include		<stdlib.h>

#include	"Limits.h"
#include	"Basicdat.h"
#include	"Commfunt.h"
#include	"Wordform.h"


#define DIGITSTR        253
#define PUNCTUATION     254
#define OTHERS          255
#define CHINASTR        256


int     flags[MAX_NumOfWordsPerSentence]; /* per-word ambiguity info list */

/*      chinese puncture        */
char    *chinap[]={"£¬","¡¢","£»","¡£","¡¢","£§","¡¶","¡·","¡´","¡µ","\0"};


//
///// Inside functions
//
//
void    set_aflag(int  index);
void    SetAmbiFlag(int);
int     strcheck(char  *str);


//
////////////// external functions ////////////////////////
//
//
void    InterProcess(void);                  
int     CheckTagAmbi(void);               

/* 
 *     function :      tagging preprocess .
 */
void     InterProcess()
{
	int     i;
	int		j=GetWordSum();

	for (i=0;  i<j;  i++)   {
	  LexFixProc(i);                /* lexical fix process */
	  ProcOverlap(i);               /* overlap word process */
	}

	for (i=0;  i<j;  i++)   {
	  set_aflag(i);                 /* process undefined words */
	  SetAmbiFlag(i);               /* set ambiguous flag */
	}
}


/*      function :      check if there are ambiguous words for tagging processing
 */
int     CheckTagAmbi()
{
	int     i;

	for (i=0; i<GetWordSum();  i++)
	  if (flags[i]==1)      return 1;
	return 0;
}


/////////////////////////// internals ////////////////////////////
///

/*      function :  set ambiguous flag for muilt_tag word .
 *						if wordlen[i] (==0 .or. >=3)
 *							flag[i]=1;
 *						else
 *							flag[i]=0;
 *      argument :  
 *					index -- word index in word list,-->i.e., words[wid] !
 */
void    set_aflag(int  index)
{
	int     clen;
	clen = strlen(GetCate(index));

	if (!clen) /// "undefined word" - having no category from Dict's!
	{ 
		switch  (strcheck(GetWord(index)))        
		{
			case  DIGITSTR   :   /*  ASCII number string  */
					strcpy(GetCate(index),"m");
					break;

			case  PUNCTUATION  : /*  ASCII puctuating character  */
					strcpy(GetCate(index),"w");
					break;

			case  CHINASTR   :   /*  chinese  GBC string  */
					/***bs 99.3.1, for GBK
					if ( (strlen(GetWord(index)) == 2) && 
						((unsigned int) GetWord(index)[0] <176) )
													 /*not chinese character * /
					*/
					///GBK:
					strcpy(GetCate(index),"x");
					break;

			case  OTHERS       :  /*  other character string  */
					strcpy(GetCate(index),"x");
					break;
			default: 
					/* nothing */;
		} 
	} 
}


/*      function :     
 *					check the unknown word string's type.
 *      return   :
 *					DIGITSTR    :   is  ASIIC  digit string.
 *					PUNCTUATION :   is punctuation.
 *					CHINASTR    :   is  Chinese Word string.
 *					OTHERS      :   other char-type string. -->that's GBK  -bs :-)
 */
int     strcheck(char  *str)
{
        int     i,len;

        len=strlen(str);
        for  (i=0;  i<len;  i++)
		  if (((int) str[i]<0) || !isdigit((int) str[i]))         
			  break;

		/*  test  ASCII digit string */
		if (i==len)
			return (DIGITSTR);
                
		/* test punctuation */
/*zw 1997		
		if ((len==1) && ((int) str[i]>0) && (ispunct((int) str[0])))  
		return(PUNCTUATION); /* ASCII * /
*/ 
		if ((len==1) && ((int) str[i]>0) && (ispunct((int) str[0])) 
			&& (str[0]!='_') && (str[0]!='-'))
			return (PUNCTUATION); /* ASCII	*/

		if ((len==2) && InWordList(str,chinap))
			return (PUNCTUATION);  /* Chinese	*/
        
		/* test chinese character string */
        for (i=0;  i<len;  i++)         {
          if  ((int) str[i]>0)
			  break;
        }
		if (i==len)  
			return (CHINASTR);
        
		/* others string */
        return(OTHERS);
}


/*
 *      function :      set ambiguous flags
 *      argument :      index -- word index in word list, i.e., words[wid]
 */
void    SetAmbiFlag(int index)
{
	int     clen;

	clen=strlen(GetCate(index));
	if (!clen || ((clen>2) && (strchr(GetCate(index),'-') != NULL)))   {
	  flags[index]=1;
	}
	else
	  flags[index]=0;
}

