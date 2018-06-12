
/****   stword.c :      segmentation processing file
 *      purpose :       segment the sentence to a serial of words .
 *      input   :       a string of sentence .
 *      output  :       a serial of words with possible categories .
 ****/
//#include "stdafx.h"

#include        <stdio.h>
#include		<stdlib.h>
#include        <string.h>
#include        <ctype.h>

#include	"Limits.h"
#include    "Basicdat.h" 
#include    "HBasDict.h"
#include    "HSpDict.h"



char*	pThisSentence; // "internal pointer" to the cur-sentence for handling
int		basept,seekpt,temppt; // lookup-indexes of cur-sentence

int		segflag[MAX_WordLen+1];               // segment flags - for every possible word length
char	segcate[MAX_WordLen+1][MAX_WordCatesLen+1]; // categories of every possible-length word !
int		maxposit;           /* maximum segment position */
int		segpossible;		// number of the possible segmentation units
int		HaveSegFlag;		// flag of the word that have be segmented 



//
///////////// external functions
//
//
int     SegVarInitial(char* pCurSentence);   
int     Segmentation(void);            
int     seg_word(int  num, char  *cate);
int     SegProcess();
int     GetWUnit(int  base, int  num,  char  *buf);

//
////////// internal functions
//
//
int     undefword(void);
int		SearchSpecialDicts(char fchar[3]);
void	SearchBasicDict(char fchar[3]);
int		IsNumber(int basept, int seekpt);

/*      ---  extern function called  ---     */
extern int SegDisambi();	 /* defined in stambi.c  */


/*	function :	initialize variables and text information used for segmentation
 */
int    SegVarInitial(char* pCurSentence)
{
	int	i, j;
	int len;
	
	// initialize all variables used
	temppt=basept=seekpt=0;
	maxposit = segpossible = HaveSegFlag = 0;

	/* modified by jin, 10-27-2003 */
	// init segflag, segcate
	len = MAX_WordLen+1;
	memset(segflag, 0, len*sizeof(int));
	memset(segcate, 0, len*(MAX_WordCatesLen+1));
/*
    for  (i=0;  i<=MAX_WordLen;  i++)  	
	{
    	segflag[i]=0;
        for (j=0; j<=MAX_WordCatesLen; j++)  segcate[i][j]='\0';
    }
*/
	/* end of modify */

////get the text for segmentation	
//
	pThisSentence=pCurSentence;  //use minimal space

	return 0;
}


/*	function :	segment a source sentence into words
     return : the number of processed characters
 */
int     Segmentation()
{
	int     len;

	len=strlen(pThisSentence);//pThisSentence: holds the sentence for segmentation
	while (basept<len)   
	{
		if (pThisSentence[basept]==' ')  //e.g., (one) space just befor EOS
		{	if(seekpt==basept)
				seekpt++;
			basept++;
		}

		// recognize any non_chinese character string as a Noun
	  	while  (((int) pThisSentence[seekpt]>0) && (seekpt<len))
		{
			seekpt++;

			if (ispunct(pThisSentence[seekpt-1]))    // EnEOS punctuation characters
			{   
				if( pThisSentence[seekpt-1]=='.' || pThisSentence[seekpt-1]==',' ||
					pThisSentence[seekpt-1]==':' )
				{ ///
				  //////maybe numbers: 
				  ///	36.68  2/3 20.56% 010-6275 10,000 9:30 ---except "xxx 2300."!
					if( isdigit(pThisSentence[seekpt-2]) && isdigit(pThisSentence[seekpt]) ) 
									continue; //part of a number
				}

				if( pThisSentence[seekpt-1]=='/' || pThisSentence[seekpt-1]=='@' ||
					pThisSentence[seekpt-1]=='+' || pThisSentence[seekpt-1]=='-' ||
					pThisSentence[seekpt-1]=='_' || pThisSentence[seekpt-1]=='#' ||
					pThisSentence[seekpt-1]=='%' ||	pThisSentence[seekpt-1]=='&') 
				{
					if( (int)pThisSentence[seekpt-2]>0 || /*isalnum(pThisSentence[seekpt-2]) &&*/
						(int)pThisSentence[seekpt-3]>0  /* && isalnum(pThisSentence[seekpt])*/ )
						continue; //part of a number or a ASCII string
				}

				////reach the EnEOS : . , ! ?
				if (seekpt-1>basept){
					if(IsNumber(basept, seekpt-1)) {
						if (seg_word(seekpt-1-basept,"m")) //is a number, maybe a freactional
							return basept;
					}
					else { //any non-number ASCII string is a Noun !
						if (seg_word(seekpt-1-basept,"n")) 
							return basept;
					}
				}
						
				if (seg_word(1,"w")) ///still left a EnEOS punctuation character
					return basept;          
			} //if(ispunct...)
		} //while(ASCII-char)

		///// save the non-digital ASCII char string as a noun: C++, Win95
		//
		if (seekpt>basept)    { 
			if(IsNumber(basept, seekpt)) {
				if (seg_word(seekpt-basept,"m")) //is a number, maybe a freactional
					return basept;
			}
			else {
				if (seg_word(seekpt-basept,"n")) //any non-number ASCII string is a Noun
					return basept;
			}
		}

		if (basept==len)	// reach the end of cur-sentence
			break;

/////handle Chinese chars followed the above ASCII string:
//
        switch   (SegProcess())	 //return: the number of possible segmented words
		{
        	case  0 :               /* undefined word */
					if (undefword()) 
						return basept;
					break;
            case  1 :				/* unique segmentation */
					///for now maxposit is the actual byte num, instead of Ch-char num!
					if (seg_word(maxposit,segcate[maxposit])) 
					  	return basept;
					break;
            default :                 /* ambiguious possible */
		 			if (SegDisambi())  
						return basept;
		 			break;
        }//switch

	}//while (basept<len)   

	return basept;
}


/*  purpose  :  segment a word
    arguments:	iBytes --- the number of bytes of the recognized word
       			cate --- the category tags of the recognized word
    return :    0 -- if not overflow; 1 -- if overflow in word list
 */
int    seg_word(int  iBytes, char  *cate)
{
	char    WordBuf[MAX_WordLen+1], CateBuf[20];
    int     i;

    if ((temppt) && (temppt<basept))  {	 // segment undefined word : twmppt != 0
    	//pb,for overflow error check
		if(basept-temppt<=MAX_WordLen){
			for (i=temppt; i<basept; i++)  
				WordBuf[i-temppt]=pThisSentence[i];
        	WordBuf[i-temppt]='\0';
        }
		else{
			strncpy(WordBuf,&pThisSentence[temppt],MAX_WordLen);
			WordBuf[MAX_WordLen] = '\0';
    	}

		CateBuf[0]='\0';
        if (AddWordCateInfo(WordBuf,CateBuf))
			return 1;
		temppt=0;
    }

	if (iBytes>0)        
	{
		if (iBytes>MAX_WordLen) {
			iBytes=MAX_WordLen;  
			ErrorInfo("Word too long");
		}

    	for (i=0; i<iBytes; i++) 	 
			WordBuf[i]=pThisSentence[basept+i];
        WordBuf[i]='\0';
        
		basept+=iBytes;
        seekpt=basept;

        if (AddWordCateInfo(WordBuf,cate))
			return 1;
    }

	return 0;
}


int SearchSpecialDicts(char fchar[3])
{
	int DictID=0;

	char    wbuf[100], *Cate;
	int     i, StPos, FCWordSum, DictIndex;
	int		maxlen;

	while(DictID<SpecDictNum) {
		if(maxlen=SearchFCListOfSpecDict(DictID, fchar,&StPos,&FCWordSum)) { //maxlen matching
			for (i=maxlen; i>=1; i--)        { ///bs 98.12: get started from maxlen!
				if (GetWUnit(basept, i/* i*2 */, wbuf)) {
					if ((Cate=LookupSpecDict(DictID, wbuf,StPos, FCWordSum, &DictIndex))!=NULL)
					{
						//maxposit = i/2 + i%2; //ASCII char in a use-def word allowed!
						maxposit = i; ///we now conut in byte
						segflag[i]=1;
						strcpy(segcate[i], Cate); /// i/2 : to save space!
						segpossible++;
			
						//////bs 98.12: should (unconditionally) finish the searching after 
						//				the first (maxlen-) occurence in a UsrLexcon!
						return 1;
						//////bs 98.12
					}
				}
				/*
				else  break;
				*/
			}//for()
		}
 
		DictID++;
	} //while()

	return 0; //not found in SpecDict's
}

void SearchBasicDict(char fchar[3])
{

	char	wbuf[100];
	char	Cate[100], tmppron[100];
	//char    wbuf[100], *Cate;
	int     i, StPos, FCWordSum;
	int  maxlen;
	int tmpruleflag;
	struct RuleFileItem *head;

	tmpruleflag = 0;
	head = NULL;

	if(maxlen=SearchFCListOfBasicDict(fchar,&StPos,&FCWordSum))
	{ //maxlen matching
		for  (i=2; i<=maxlen; i+=2)	
		{///Both Basic and SpDict's are now in byte number!!!
			if (GetWUnit(basept,i,wbuf))    
			{
				LookupBasicDict(wbuf,StPos, FCWordSum,Cate,tmppron,&tmpruleflag, head);
				//if ((Cate=LookupBasicDict(wbuf,StPos, FCWordSum,tmpcate,tmppron,tmpruleflag, head))!=NULL)  {
				if(strcmp(Cate, "***") != 0)
				{
						segflag[i]=1;
						strcpy(segcate[i],Cate);
						segpossible++;
						  //maxposit=i;
						maxposit=i; ///we now conut in byte
				}

				//pb,040722,you can't quit here,otherwise many words are lost
				//else   
				//	break;
			}
		}//for
	}
}




/*	function :	segment words from source sentence incrementally, and set 
				the category tags for every segmented units
    return :	the number of the possible segmented words
 */
int	SegProcess()
{
	char    fchar[3];
	//char	*wbuf[100], cbuf[20];
	int     i /*,j*/;
	//int		iFCL, maxlen;

	// already segmented
	if (HaveSegFlag)	{
		HaveSegFlag = 0;
		return segpossible;
	}
    maxposit=segpossible=0;

    if (!GetWUnit(basept,2,fchar))	// get the first char!
		return(0);
	
	// initialize the segmentation information records 
	/* bs 99 7
	for (i=0; i<=MAX_WordLen; i++)   {
		segflag[i]=0;
		for (j=0; j<=MAX_WordCatesLen; j++)	
			segcate[i][j]='\0';
	}
	*/
	for (i=0; i<=MAX_WordLen; i++) 	{
		segflag[i]=0;
		segcate[i][0] = '\0';
	}
	///memset((char*)segcate, 0, MAX_WordLen*MAX_WordCatesLen);

//
////// segment begins: matching dictionaries
//
	// search all the special dictionaries
	// skip BasicDict if found!
	if(SearchSpecialDicts(fchar)==0)
	{
		// search the basic dictionary
		SearchBasicDict(fchar);
	}
	
	return(segpossible);
}


//
///// get num bytes from cur sentence -- pThisSentence
//
int     GetWUnit(int  base, int  num, char* buf)
{
        int     i;

        ////bs: we use decremental match:
		  //if ( (size_t)(base+num) > strlen(pThisSentence) )     
		if ( (size_t)num > (strlen(pThisSentence)-(size_t)base) )
			return(0);

        for (i=0; i<num; i++) 
          	buf[i]=pThisSentence[i+base];
        buf[i]='\0';
        return(1);
}


/****************	internal functions	*************************/

/*  function :  segment a undefined word
     return :    0 -- if not overflow; 1 -- if overflow in word list
 */
int     undefword()
{
	int     i;
	char    WordBuf[100], CateBuf[20];

    //if (!temppt)            /* first encounter undefined word */
    //	temppt=basept;
    
    //pb,just separate to single character
	temppt = basept;
	
	basept+=2;
    seekpt=basept;

	//if (!temppt || !pThisSentence[basept])   {      /* string's head or tail */
    //pb,just separate to single character
	if(1){
	//pb,for overflow error check
		if(basept-temppt<=MAX_WordLen){
			for (i=temppt; i<basept; i++)  
				WordBuf[i-temppt]=pThisSentence[i];
        	WordBuf[i-temppt] = '\0';
        }
		else{
			strncpy(WordBuf,&pThisSentence[temppt],MAX_WordLen);
			WordBuf[MAX_WordLen] = '\0';
		}
		
		CateBuf[0] = '\0';
        if (AddWordCateInfo(WordBuf,CateBuf))
			return 1;
    }
    //pb,just separate to single character
	temppt = basept;
	return 0;
}


//
/////// see a ASCII string is whether a number
//
int	IsNumber(int basept, int seekpt)
{
	int is_n=1 , i;

	for(i=basept; i<seekpt; i++) {
		if(isdigit(pThisSentence[i])||(pThisSentence[i]=='.')||
			(pThisSentence[i]=='-')||(pThisSentence[i]=='+')||
			(pThisSentence[i]=='/')||(pThisSentence[i]=='#')||
			(pThisSentence[i]==',')||(pThisSentence[i]==':')||
			(pThisSentence[i]==' ') )
				continue;	
			//"010-62753081", "2/3", "#5", "+36.68 "(one space-char possibly left!)
		else {
			is_n=0;
			break;
		}
	}

	if(is_n)
		return 1;
	else
		return 0;
}

