/*
 *	CRoutine.c :
 *	
 *			 common operations and basic data structure handling
 *
 *  
 */


//////if on Windows:
//#include	<windows.h>
//#include "stdafx.h"	//zanhy 2002-12-23

#include	<stdio.h>
#include	<malloc.h>
#include	<string.h>

#include		"Limits.h"

static	char    words[MAX_NumOfWordsPerSentence][MAX_WordLen+1];/* word list for storing seg result*/
static	char    cates[MAX_NumOfWordsPerSentence][MAX_WordCatesLen+1];/* category list of "seg-ed" words */
static	int		wid;/* order number of a word in a sentence, i.e., words[index][] */

static  char	wc0[2];

static	char *	DictDir;		/* dictionary and statistics directory information 	*/
static	int		iErrors;		/* program error flag	*/


//
/////// external functions
//
//
void	ErrorInit();
void	ErrorInfo(char *ErrInfo);
int		AnyError();

void	InitSegTagUnitsList(); // init words[][] and cates[][]
int		AddWordCateInfo(char *word, char *cate);
char*	GetWord(int  wpos);
char*	GetCate(int  wpos);
int		GetWordSum();
char*	CombWord(int  stpos, int  num, char  *cate);

void	SetDictDirectory(char *DictDirStr);
char*	GetDictDirectory();
void	FreePathMem();
//
//// more:
//
int     InWordList(char   *word, char  *wlist[]);
int     incate(char  tag, char  *cate);
int     IsPossC(char  * , char  * );
char*	DeleTag(char , char  * );



////////////////////////// internals  ///////////////////////////////////

/*	purpose :	move forward the words in the word list
 *	argument :	StPos --- the starting position for moving
 *				MWNum --- the number of words for moving
 */
void    ForwardMoveWords(int  StPos, int  MWNum)
{
	int     i;

	/// words[][] already got wid words:
	for (i=StPos;  i<wid-MWNum;  i++)   {
	  strcpy(words[i],words[i+MWNum]);  //overwrite the old!
	  strcpy(cates[i],cates[i+MWNum]);
	}
	wid -= MWNum;
}



////////////////////////// externals  ///////////////////////////////////
//
//

void	ErrorInit()
{
	iErrors = 0;
}


/*	purpose :	save the error information into the log file	
 *				or give a hint, and record any errors
 */
void	ErrorInfo(char *ErrInfo)
{
	//MessageBox(NULL, ErrInof, "zSegTag", MB_OK|MB_ICONEXCLAMATION);
	//fprintf(stderr, "%s\n", ErrInfo);

	iErrors++;
}


/*	purpose :	test the error status after the program processed.
 *	return :	1 --- if have errors;		0 --- otherwise
 */
int		AnyError()
{
	if (iErrors)	return 1;
	else	return 0;
}


void	InitSegTagUnitsList()
{
	int	i, j;
	int len;

	/* modified by jin, 10-27-2003 */	
	// init words, cates
	len = MAX_NumOfWordsPerSentence;
	memset(words, 0, len*(MAX_WordLen+1));
	memset(cates, 0, len*(MAX_WordCatesLen+1));
/*
	for (i=0; i<MAX_NumOfWordsPerSentence; i++)	{
		for (j=0; j<=MAX_WordLen; j++)	
			words[i][j] = '\0';

		for (j=0; j<=MAX_WordCatesLen; j++)
			cates[i][j] = '\0';
	}
*/
	/* end of modify */
	wid = 0;
}


/*	purpose :	add a segmented word with its category into the word list words[][] 
				and category list cates[][]
 *	return :	1 --- if overflow in word list;		0 --- if not
 */
int		AddWordCateInfo(char *word, char *cate)
{
////// simply store them into:
//
	if (wid == MAX_NumOfWordsPerSentence)	{
		ErrorInfo("Too many words in a sentence!");
		return 1; 
	}

	strncpy(words[wid],word, MAX_WordLen);
	strncpy(cates[wid],cate,MAX_WordCatesLen);
	wid++;

	return 0;
}

		
/*	purpose :	get a word from the word list words[][]
 *	argument :	wpos --- the word's position in a sentence
 */
char*	GetWord(int  wpos)
{
	if (wpos<0)
	{
	  wc0[0]='\0';
	   return wc0;
	}
	return words[wpos];
}


/*	purpose :	get the category of a word from the category list cates[][]
 *	argument :	wpos --- the word's position in a sentence
 */
char*	GetCate(int  wpos)
{
	if (wpos<0)
	{
	  wc0[0]='\0';
	   return wc0;
	}
	return cates[wpos];
}


/*	purpose :	get the sum of the segmented words in a sentence.
 */
int		GetWordSum()
{
	return	wid;
}


/*	purpose :	combine several words in the word list ==>words[wid][],cates[wid][]
 *	argument :	stpos --- the starting word position for combining
 *				num --- the number of words for combining
 *				cate --- the category of the new combined word
 *	return   :	the combined new word and its category information
 */
char*	CombWord(int  stpos, int  num, char  *cate)
{
	char    buf[100];
	static char wcbuf[100];
    int     i, j, bp=0;

	for  (i=0;  i<num;  i++)
	  	for (j=0;  words[stpos+i][j];  j++)
	    	buf[bp++]=words[stpos+i][j];

	buf[bp++]='\0';
	strncpy(words[stpos],buf, MAX_WordLen);
	strncpy(cates[stpos],cate,MAX_WordCatesLen);
	ForwardMoveWords(stpos+1,num-1);  /* change wordslist and cateslist */
	sprintf(wcbuf,"%s/%s",buf,cate);  /* generate new word for return*/
	
	return(wcbuf);
}


/*	purpose :	set the dictionary directory information
 */
void	SetDictDirectory(char *DictDirStr)
{
	if ((DictDir = (char *) malloc(strlen(DictDirStr)+1))!=NULL)	
		strcpy(DictDir,DictDirStr);
	else	
		DictDir = (char *) NULL;
}
	

/*	purpose :	get the dictionary directory information 
 */
char*	GetDictDirectory()
{
	return DictDir;
}


/*	purpose :	free the memory used by path string.
 */
void	FreePathMem()
{
	free(DictDir);
}


//
///////// more common operations:
//

/*	purpose : test  whether a word is in a word list
 *	return  : 1 --- if in;	0 --- otherwise    
 */
int     InWordList(char   *word, char  *wlist[])
{
	int     i=0;

	while  (strlen(wlist[i]))
	  	if (!strcmp(word,wlist[i++]))     
			return(1);
    return(0);
}


/*	purpose  :	test if a POS tag is in a multitag string
 *	argument :	tag --- the POS tag for testing
 *				cate --- the multitag string
 *	return :	1 --- if in;	0 --- otherwise.
 */
int     incate(char  tag, char  *cate)
{
        if (strlen(cate) && (strchr(cate,tag)!=NULL))     
			return(1);
        return(0);
}


/*      purpose  :      check whether root's category is in possible category list .
 *      argument :      scate --- root category .
 *                      dcate --- possible category list .
 *      return  :       1 --- if in .
 *                      0 --- if not .
 */
int     IsPossC(char  *scate, char  *dcate)
{
        int     i,len;
        len=strlen(scate);

		if (len==1)     {
			if  (strchr(dcate,scate[0])!=NULL)   
				return(1);
		}
        else
			for (i=0;  i<len;  i++)
				if  ((scate[i]!='-') && (strchr(dcate,scate[i])!=NULL))
	                return(1);
    
		return(0);
}


/*      purpose  :      delete special tag from tag set .
 *      argument :      tag --- tag character .
 *                      catestr --- taging set .
 *      return  :       pointer of new tag .
 */
char*	DeleTag(char  tag, char  *catestr)
{
	static char buf[20];
	char*bufp,*catep;

	bufp=buf;
	catep=catestr;
	while  (*catep)   {
		if ((*catep != tag))
			*bufp++ = *catep++;
		else  {
			catep++;
			if (*catep=='-')    
				catep++;
	    }
	}
	
	if (*(bufp-1)=='-')    
		bufp--;

	*bufp++='\0';
	return buf;
}

/////////END File
