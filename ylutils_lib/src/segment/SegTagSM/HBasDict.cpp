
//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "Limits.h"
#include "HBasDict.h"

extern char  CateBuf[MAX_WordCatesLen+1]; /* buffer for the category information	 
											- all possible POS (all ¡°¼æÀà¡±!!) */



#define ThisMalloc  malloc



//
////////////////////////// internal functions:
//
//
int		CmpBasDictStem(const void *Stem1, const void *Stem2);
void		GenBasDictFirstCharList();



//
///////////// external functions
//
//
void	readBasDic(char *curPath);
int		SearchFCListOfBasicDict(char* FirstChar, int* StPos, int* FCWordSum);
int	LookupBasicDict(char * word, int StPos, int FCWordSum,char *cate,char *pron, int *ruleflag, struct RuleFileItem *&head);
void	FreeBasicDictMem();

int insertRuleLink(char *tmpWord, struct RuleFileItem *head);


void readBasDic(char *curPath)
{
	char	FullBasDictFileName[300];
	char	WordBufTmp[MAX_BasWordLen+40], CateBufTmp[MAX_WordCatesLen+40];
	char	TmpStr[MAX_WordCatesLen+40];
	FILE    *fp;
	char	*tokptr, *strptr;
	char	PinyinBufTmp[MAX_PINYINLEN+40];
	char	tmpline[ MAXDICLINELEN];
	int		len, i, pinyin, j;


	strcpy(FullBasDictFileName, curPath);
	strcat(FullBasDictFileName, "NewWholePinyin.txt");

	if ((fp=fopen(FullBasDictFileName,"r"))==NULL)   {
		//AfxMessageBox("The basic dictionary file can not found");
		printf("The basic dictionary file can not found\r");	//zanhy 2002-12-23
	}
	
	//// read the content of the basic dictionary
	BasicDictIndex=0;
	while (!feof(fp))	
	{
		////we now allow C++ single-line comments in all the lexicons
		//
		//SkipComments(fp); 
		//fscanf(fp,"%s%s",WordBufTmp,CateBufTmp);
		//// ^ To cooperate C/C++ cmmnts:
		if(!fgets(tmpline, MAXDICLINELEN,fp))
			break;
		
		pinyin = 0;
		len = strlen(tmpline);
		for(i = len-1; i>3; i--)
			if(tmpline[i] == '[')
			{
				pinyin = 1;
				break;
			}
			
		strptr=tmpline;
		if((tokptr = strtok(strptr," \t")) != (char *)0)
			strcpy(WordBufTmp,tokptr);
		strptr = (char *)0;

		if(pinyin == 1)
		{
			if((tokptr = strtok(strptr,"[")) != (char *)0)
				strcpy(CateBufTmp,tokptr);
			strptr = (char *)0;
			if((tokptr = strtok(strptr,"]")) != (char *)0)
				strcpy(PinyinBufTmp,tokptr);
		}
		else
		{
			if((tokptr = strtok(strptr," \t\r\n")) != (char *)0)
				strcpy(CateBufTmp, tokptr);
			strcpy(PinyinBufTmp, WordBufTmp);
		}

		//GetNextTokenString(fp, WordBufTmp);
		//GetNextTokenString(fp, CateBufTmp);
		strcpy(TmpStr, CateBufTmp);
		i = 0;
		while((TmpStr[i] == ' ')||(TmpStr[i] == '\t')) i++;
		j = 0;
		while( (TmpStr[i] != '\0')&&(TmpStr[i] != ' ')&&(TmpStr[i] != '\t') )
			CateBufTmp[j++] = TmpStr[i++];
		CateBufTmp[j] = '\0';
		
		// Try to store a word:
		if ((BasicDict[BasicDictIndex]=(BasDictUnit*)ThisMalloc(sizeof(BasDictUnit)))==0)
			return;
		
		//for (i=0; i<=MAX_BasWordLen; i++)	BasicDict[BasicDictIndex]->word[i]='\0';
		//for (i=0; i<=MAX_WordCatesLen; i++)	BasicDict[BasicDictIndex]->cate[i]='\0';

		strcpy(BasicDict[BasicDictIndex]->word, WordBufTmp);
		strcpy(BasicDict[BasicDictIndex]->cate, CateBufTmp);
		//strcpy(BasicDict[BasicDictIndex]->pron,PinyinBufTmp);
		//BasicDict[BasicDictIndex]->ruleFlag = 0;
		//BasicDict[BasicDictIndex]->headRuleLink = NULL;
		
		if (++BasicDictIndex == MAX_NumOfBasicDictWords)    
			//AfxMessageBox("Overflow in the basic dictionary!\n");
			printf("Overflow in the basic dictionary!\n");	//zanhy 2002-12-23

	}
    
	fclose(fp);
	
	////quick sort the dictionary entries according to the word and category information:
	//
	qsort((void *)BasicDict, (size_t)BasicDictIndex, sizeof(BasDictUnit*), CmpBasDictStem);
	
	BasicWordSum=BasicDictIndex; //record the number of total words
	
	GenBasDictFirstCharList();//generate the first Chinese character list of this dictionary
	
}


/*  purpose  :  search the first character in the first character list, and return 
*				  its information.
*  argument :  FirstChar --- the first character for searching
*              StPos --- return the starting position in the FC list.
*              FCWordSum --- return the sum of all words with the same first character.
*  return   :  the maximun word length of all words -- if found.
*              0 --- if not found.
*/
int SearchFCListOfBasicDict(char* FirstChar, int* StPos, int* FCWordSum)
{
	int i,j;
	
	if( (unsigned char)FirstChar[0]<=0xFE && (unsigned char)FirstChar[1]<=0xFE ) 
	{
		i=(unsigned char)FirstChar[0]-0x81;
		j=(unsigned char)FirstChar[1]-0x40;

		if(i>=0 && j>=0)
		{
			if((* BasicDictFirstCharTable[i][j]).FCWordSum) /// 0 means empty
			{
				*StPos = (* BasicDictFirstCharTable[i][j]).StartPos;
				*FCWordSum = (* BasicDictFirstCharTable[i][j]).FCWordSum;
				
				return (* BasicDictFirstCharTable[i][j]).MaxWordLen; //Ch-char number!
			}
		}
	}	
	return 0;
}


/*  purpose :  look up the word in basic dictionary, and return all of its category tags.
*  argument :  word --- the word for looking up
*              StPos --- the starting position in the BasicDictFirstChar list - BasicDictFirstCharTab[Dict][StartPos]!
*              FCWordSum --- the sum of all words with the same first character.
*  return :    the list of all category tags for the word --- if found
*              0 --- if not found
*/
int LookupBasicDict(char * word, int StPos, int FCWordSum,char *cate,char *pron, int *ruleflag, struct RuleFileItem *&head)
{
	int     low,high,mid,i,j,k;
	int     bp=0;
	//char	tmpstr[3];
	
	low=StPos;  
	high=StPos+FCWordSum-1;
	
	while   (low<=high)     
	{
		mid=(low+high)/2;
		i=strcmp(word,BasicDict[mid]->word);
		
		if  (i<0)    {
			high=mid-1;
			continue;
		}
		if  (i>0)     {
			low=mid+1;
			continue;
		}
		if  (i==0)    
		{
			/// get the cate's of "Distributed" words
			/// not needed form our "well compact dict's" !!
			j=k=mid;
			while  ((j>=0) && !strcmp(word,BasicDict[j]->word)) ///check the front word 
				j--;
			while  ((k<BasicWordSum) && !strcmp(word,BasicDict[k]->word))///check the next wrod  
				k++;
			
			if(j+1==k-1) 
			{
				//strcpy(pron,BasicDict[mid]->pron);
				strcpy(cate,BasicDict[mid]->cate);
				//*ruleflag = BasicDict[mid]->ruleFlag;
				//head = BasicDict[mid]->headRuleLink;
				head = NULL;
				strcpy(pron,"***");
				ruleflag = 0;
				return 1;
			}
			else  ///multiple entries of the same word with multiple categories: "Distributed"!
			{
				//AfxMessageBox(word);
				strcpy(cate,"***");
				strcpy(pron,"***");
				return 0;//tmpstr;
			}
		} //if(i==0)
	} //while
	strcpy(cate,"***");
	strcpy(pron,"***");
	ruleflag = 0;
	return 0; // not found this word
}


/*	purpose :	free the memory used by user's S&T dictionary and clear it
*/
//extern char *	pMemBlock;	///the start byte of all the shared memory stuff
void	FreeBasicDictMem()
{
	long	i;
	for (i=0; i<BasicWordSum; i++)
		free((char *)BasicDict[i]);
	BasicWordSum = 0;

}



////////////////////////////// internals ////////////////////////////////////

/*	purpose :	generate the first Chinese character list BasDictF~C~L~ for 
bi-search!
*/
void	GenBasDictFirstCharList()
{
	int i,j, k, WordLen;
	
	/*
	///memset(BasicDictFirstCharTable, 0, size...);
	for(i=0; i<126; i++)
	for(j=0; j<191; j++)
	BasicDictFirstCharTable[i][j].FCWordSum = 0;
	*/
	for(i=0; i<126; i++)
		for(j=0; j<191; j++) {
			BasicDictFirstCharTable[i][j] = 
				(DictFirstCharRecord *)ThisMalloc(sizeof(DictFirstCharRecord));
			(*(BasicDictFirstCharTable[i][j])).FCWordSum = 0;
		}
		
		
		for (k=0; k<BasicWordSum; k++)
		{
			if( (unsigned char)(BasicDict[k]->word)[0]<=0xFE && 
				(unsigned char)(BasicDict[k]->word)[1]<=0xFE ) 
			{
				i=(unsigned char)(BasicDict[k]->word)[0]-0x81;
				j=(unsigned char)(BasicDict[k]->word)[1]-0x40;
				
				if(i>=0 && j>=0)
				{
					
					if((* BasicDictFirstCharTable[i][j]).FCWordSum == 0) /// 0 means empty
					{
						/*  add a new stem in the first character list */	
						(* BasicDictFirstCharTable[i][j]).StartPos = k;
						(* BasicDictFirstCharTable[i][j]).FCWordSum = 1;
						(* BasicDictFirstCharTable[i][j]).MaxWordLen =  ///Both Basic and SpDict's are now in byte number!!!
									strlen(BasicDict[k]->word);
					}
					else   ///there already exists
					{
						(* BasicDictFirstCharTable[i][j]).FCWordSum++;
						
						if ((WordLen=strlen(BasicDict[k]->word)) >
							(* BasicDictFirstCharTable[i][j]).MaxWordLen)
								(* BasicDictFirstCharTable[i][j]).MaxWordLen = WordLen;
					}
				}
			}
		}///for()
}



/*	purpose :	compare two stems in the user's S&T dictionary, 
according to its word and category information.
*/
int	 CmpBasDictStem(const void *Stem1, const void *Stem2)
{
	int temp1, temp2;
	temp1=strcmp( (*(BasDictUnit**)Stem1)->word, (*(BasDictUnit**)Stem2)->word ) ;
	temp2=strcmp( (*(BasDictUnit**)Stem1)->cate, (*(BasDictUnit**)Stem2)->cate );
	
	if (!temp1)	
	{
		/*
		if (!temp2)
			return strcmp( (*(BasDictUnit**)Stem1)->pron, (*(BasDictUnit**)Stem2)->pron );
		else
			return temp2;
		*/
		return temp2;
	}
	else
		return temp1; 
}

int insertRuleLink(char *tmpWord, struct RuleFileItem *head)
{
	int start, firstnum;
	int j, high, mid, low;

	j = SearchFCListOfBasicDict(tmpWord, &start, &firstnum);
	if(j==0)  //not found the first char, sth.is wrong.
			//AfxMessageBox(tmpWord);
			printf(tmpWord);	//zanhy 2002-12-23
	else
	{
			low = start;
			high = start+firstnum-1;
			while(low<=high)     
			{
				mid=(low+high)/2;
				j=strcmp(tmpWord,BasicDict[mid]->word);
		
				if  (j<0)    {
					high=mid-1;
					continue;
				}
				if  (j>0)     {
					low=mid+1;
					continue;
				}
				if  (j==0) 
				{
//					BasicDict[mid]->ruleFlag  = 1;
//					BasicDict[mid]->headRuleLink = head;
					return 1;
				}
			}//end of while(low<=high)  
			
	}// end of else
	//AfxMessageBox("something is wrong with basic dict indexing");
	printf("something is wrong with basic dict indexing\r");	//zanhy 2002-12-23
	return 0;
}



