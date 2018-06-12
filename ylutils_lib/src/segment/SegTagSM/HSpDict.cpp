/***
 *
 *  HSpDict.c : handle special/user-specified segmentation & tagging dictionaries(lexicons)
 *
 ***/
//#include "stdafx.h"	//zanhy 2002-12-23

#include	"DictComm.h"
#include	"HBasDict.h"
#include	"HSpDict.h"

#ifndef BING_S
	#define ThisMalloc  malloc
#else
	#define ThisMalloc	BSmalloc
#endif


//
////////////////////////// internal functions:
//
//
int		CmpSpecDictStem(const void *Stem1, const void *Stem2);
void		GenFirstCharList(int j);


//
///////////// external functions
//
// <moved to "HSpDict.h":>n
void    ReadSpecDict(void);
int		SearchFCListOfSpecDict(int DictID, char *FirstChar, int* StPos, int* FCWordSum);
char*	LookupSpecDict(int DictID, char *word, int StPos, int FCWordSum, int* DictIndex);
void	FreeSpecDictMem();


//
//
////////// read all user-defined dictionaries UsrLex<n> and q-sort them:
//
void    ReadSpecDict()
{
	char    SpFName[20], FullSpFileName[200]; 
	char	ErrorInfoStr[200];
	char	WordBufTmp[MAX_WordLen+1], CateBufTmp[MAX_WordCatesLen+1];
	int	i,j, c;
	FILE *	fp[MAX_SpecDictNum]; // ->to files usrlex<n>
 

#ifdef BING_S
	///set all the "shared" memory needed: in setmem.c
	switch(SetSpecDictMemBlock())
	{
	case 0:		///failed
		return;	

	case 1:		///created(and go on init.)
		break;

	case 2:		///get shared memory(already init.)
		return;
	}
#endif


	////should init it every time when load/reload usr-dict's:
	//
	SpecDictNum=0;

	for(j=0; j<=MAX_SpecDictNum; j++)
	{							 // ^this j++ is the last stmt(after all for(){...} stmts!)
//////Note: UsrLex1 -- UserLex30, while SpecDict[j] from SpecDict[0] to SpecDict[29]
//
		strcpy(SpFName,"UsrLex");
		sprintf(FullSpFileName, "%s%s%d", GetDictDirectory(), SpFName, j+1);
		if ((fp[j]=fopen(FullSpFileName,"r"))==0)   {
			if(SpecDictNum == 0) { //no user dictionary found 
				sprintf(ErrorInfoStr,"The user's dictionary file %s does not exist\n",
					FullSpFileName);
				//ErrorInfo(ErrorInfoStr);	//-- not a error!!
			}
		  return;
		}

		///SpecDictNum=j+1;
		if(SpecDictNum+1 > MAX_SpecDictNum) {	
			//ErrorInfo("Too many user dictionaries"); 
				// ^not a fatal error to cause init failed!
			return;
		}
		SpecDictNum++;

		//// read the content of a user dictionary
		SpecDictIndex=0;
		while (!feof(fp[j]))	
		{
			if ((SpecDictIndex+1) == MAX_NumOfASpecDictWords)    {
				ErrorInfo("Overflow in user dictionary(DictUnit list)\n");
				break;
			}

			////we now allow C block and C++ single-line comments in all the lexicons
			//
			//SkipComments(fp[j]); 
			//fscanf(fp,"%s%s",WordBufTmp,CateBufTmp);
			//// ^ To cooperate C/C++ cmmnts:
			GetNextTokenString(fp[j], WordBufTmp);
			GetNextTokenString(fp[j], CateBufTmp);
			//maybe empty strings - reach the eof:
			if(feof(fp[j]))
				break;

			// Try to store a word:
    		if ((SpecDict[j][SpecDictIndex]=(DictUnit*)ThisMalloc(sizeof(DictUnit)))==0)	
				return; //mem alloc failure, lexicon-loading ends here

#ifndef BING_S
			for (i=0; i<=MAX_WordLen; i++)	SpecDict[j][SpecDictIndex]->word[i]='\0';
    		for (i=0; i<=MAX_WordCatesLen; i++)	 SpecDict[j][SpecDictIndex]->cate[i]='\0';
#endif

			strncpy(SpecDict[j][SpecDictIndex]->word, WordBufTmp, MAX_WordLen);
    		strncpy(SpecDict[j][SpecDictIndex]->cate, CateBufTmp, MAX_WordCatesLen);


#ifndef UNIX_SEGTAG
			//// let's extend an "alias mechanism":
			/*
			GetNextTokenString(fp[j], CateBufTmp);  //get the alias flag(a single char!)
			if(CateBufTmp[0]=='0') {
				SpecDict[j][SpecDictIndex]->fAlias = 1;
				GetNextTokenString(fp[j], WordBufTmp);	// get the alias string
				if(SpecDict[j][SpecDictIndex]->pAlias = (char*)malloc(strlen(WordBufTmp)+1) ==0)
					return;
			*/
			SkipComments(fp[j]);  //no included comments in an alias
			if((c=fgetc(fp[j]))=='{') 
			{
				SpecDict[j][SpecDictIndex]->fAlias = 1;

				/// allocate mem to store the alias info:
				if((SpecDict[j][SpecDictIndex]->pAlias = (char*)/*ThisMalloc*/malloc((size_t)MAX_AliasLen+1)) ==0)
					return;
				i=0;
				while(!feof(fp[j])) {
					c=fgetc(fp[j]);
					if(c=='}')
						break; //reach the end
					if(i>=MAX_AliasLen)
						break;
					if(c=='\t'||c=='\n'||c=='\r') //white's within alias
						SpecDict[j][SpecDictIndex]->pAlias[i]=' ';
					else
						SpecDict[j][SpecDictIndex]->pAlias[i]=c;
					i++;
				}
				if(i<=MAX_AliasLen)
					SpecDict[j][SpecDictIndex]->pAlias[i]='\0';
				else 
					ErrorInfo("Buffer overflow\n");
			}
			else {
				ungetc(c, fp[j]);
				SpecDict[j][SpecDictIndex]->fAlias = 0;
			}
#endif

			SpecDictIndex++;
		}//while (!feof(fp[j]))
    
		fclose(fp[j]);

	////quick sort the dictionary entries according to the word and category information:
	//
		qsort((void *)SpecDict[j], (size_t)SpecDictIndex, sizeof(DictUnit*), CmpSpecDictStem);

		SpecWordSum[j]=SpecDictIndex; //record the number of total words

		GenFirstCharList(j);//generate the first Chinese character list of this dictionary

	#ifdef UNIX_SEGTAG
		DoExchangeSpecDict(0);	///0:  upload;  1: download
	#endif
	}
}


/*  purpose  :  search the first character in the first character list, and return 
 *				  its information.
 *  argument :  FirstChar --- the first character for searching
 *              StPos --- return the starting position in the SpecDict
 *              FCWordSum --- return the sum of all words with the same first character.
 *  return   :  the maximun word length of all words -- if found.
 *              0 --- if not found.
 */
int SearchFCListOfSpecDict(int DictID, char* FirstChar, int* StPos, int* FCWordSum)
#ifndef BING_S
{
///////// binary searching: ///////////
//
//
	int low=0, high=SpDictFirstCharSum[DictID], mid, i;

	while (low<=high)     
	{
		mid=(low+high)/2;
		i=strcmp(FirstChar, SpecDictFirstCharTable[DictID][mid].FirstChar);
	  
		if  (i<0)    {
			high=mid-1;
			continue;
		}
		if  (i>0)    {
			low=mid+1;
			continue;
		}
		if  (i==0)   
		{
			*StPos = SpecDictFirstCharTable[DictID][mid].StartPos;
			*FCWordSum = SpecDictFirstCharTable[DictID][mid].FCWordSum;
			return SpecDictFirstCharTable[DictID][mid].MaxWordLen;
		}
	}

	return 0;
}
#else
{
	int i,j;

	if( (unsigned char)FirstChar[0]<=0xFE && (unsigned char)FirstChar[1]<=0xFE ) 
	{
		i=(unsigned char)FirstChar[0]-0x81;
		j=(unsigned char)FirstChar[1]-0x40;

		if(i>=0 && j>=0)
		{
			
			if(SpecDictFirstCharTable[DictID][i][j] -> FCWordSum) /// 0 means empty
			{
				*StPos = SpecDictFirstCharTable[DictID][i][j] -> StartPos;
				*FCWordSum = SpecDictFirstCharTable[DictID][i][j] -> FCWordSum;
				
				return SpecDictFirstCharTable[DictID][i][j] -> MaxWordLen; //its Ch-char number!
			}
		}
	}
	return 0;
}
#endif


/*  purpose :  look up the word in special dictionary, and return all of its category tags.
 *  argument :  word --- the word for looking up
 *              StPos --- the starting position in the SpecDictFirstChar list - SpecDictFirstCharTab[Dict][StartPos]!
 *              FCWordSum --- the sum of all words with the same first character.
 *  return :    the list of all category tags for the word --- if found
 *              0 --- if not found
 */
char * LookupSpecDict(int DictID, char * word, int StPos, int FCWordSum, int* DictIndex)
{
	int     low,high,mid,i,j,k;
	int     bp=0,iw;

	low=StPos;  
	high=StPos+FCWordSum-1;
	
	while   (low<=high)     
	{
		mid=(low+high)/2;
		i=strcmp(word,SpecDict[DictID][mid]->word);
	  
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
			///record the index/positon of the word in this SpecDict[i]
			*DictIndex=mid;

			j=k=mid;
			while  ((j>=0) && !strcmp(word,SpecDict[DictID][j]->word))     
				j--;
			while  ((k<SpecWordSum[DictID]) && !strcmp(word,SpecDict[DictID][k]->word))  
				k++;
	    
			if  (j+1==k-1) /* only one word's term -- One word, One Entry(无兼类)- 已合并!*/
			{
				return(SpecDict[DictID][mid]->cate);
			}
			else        
			{
				CateBuf[0] = '\0';
				for  (i=j+1;  i<k;  i++)            /* combining tags */
				{
					if (InTagSet(CateBuf,SpecDict[DictID][i]->cate))		
						continue;	/* process the duplicated entries	*/
					
					for (iw=0;  SpecDict[DictID][i]->cate[iw];  iw++)
						CateBuf[bp++]=SpecDict[DictID][i]->cate[iw];
			
					CateBuf[bp++]='-';
				}
		  
				CateBuf[bp-1]='\0';          /* delete the ending '-' */
				return(CateBuf);
			}
	   } //if(i==0)
	} //while

	return 0;
}


/*	purpose :	free the memory used by user's S&T dictionary and clear it
 */
void	FreeSpecDictMem()
{
	int	j,i;

	for(j=0; j<MAX_SpecDictNum; j++)
	{
		for (i=0; i<SpecWordSum[j]; i++)	{

#ifndef UNIX_SEGTAG
			if(SpecDict[j][i]->fAlias)
				free((char *)SpecDict[j][i]->pAlias); ///by malloc()
#endif

#ifndef BING_S
			free((char *)SpecDict[j][i]);
#endif

		}
		SpecWordSum[j] = 0;
	}
///freed by BisDict
}

////////////////////////////// internals ////////////////////////////////////

/*	purpose :	generate the first Chinese character list SpDF~C~L~ for searching
 */
void	GenFirstCharList(int DictID)
#ifndef BING_S
{
	int	i, WordLen; 
	
	SpDictFCTabIndex=0;
	for (i=0; i<SpecWordSum[DictID]; i++)	
	{
		if (strncmp(SpecDictFirstCharTable[DictID][SpDictFCTabIndex].FirstChar, SpecDict[DictID][i]->word, 2))  
		{/*  add a new stem in the first character list */
		
			if (++SpDictFCTabIndex == MAX_NumOfSpecDictFirstChars)   
			{
				ErrorInfo("Overflow in the first Chinese character list !\n");
				break;
			}
			
			strncpy(SpecDictFirstCharTable[DictID][SpDictFCTabIndex].FirstChar, 
													SpecDict[DictID][i]->word, 2);
			SpecDictFirstCharTable[DictID][SpDictFCTabIndex].StartPos = i;
			SpecDictFirstCharTable[DictID][SpDictFCTabIndex].FCWordSum = 1;

			///we count the byte num:
			SpecDictFirstCharTable[DictID][SpDictFCTabIndex].MaxWordLen = strlen(SpecDict[DictID][i]->word);
				//SpecDictFirstCharTable[DictID][SpDictFCTabIndex].MaxWordLen = strlen(SpecDict[DictID][i]->word)/2;
		}
		else    
		{
			SpecDictFirstCharTable[DictID][SpDictFCTabIndex].FCWordSum++;
			
			if (/*(WordLen=strlen(SpecDict[DictID][i]->word)/2)*/
				(WordLen=strlen(SpecDict[DictID][i]->word)) >
				   SpecDictFirstCharTable[DictID][SpDictFCTabIndex].MaxWordLen)
				SpecDictFirstCharTable[DictID][SpDictFCTabIndex].MaxWordLen = WordLen;
		}
	}

	SpDictFirstCharSum[DictID]=SpDictFCTabIndex; //record the total First-Char number of each SpDict
}
#else
{
	int i,j, k, WordLen;
	
	///memset(SpDictFirstCharTable, 0, size...)
	for(i=0; i<126; i++)
		for(j=0; j<191; j++) {
			SpecDictFirstCharTable[DictID][i][j] = 
				(DictFirstCharRecord *)ThisMalloc(sizeof(DictFirstCharRecord));
			SpecDictFirstCharTable[DictID][i][j] -> FCWordSum = 0;
		}
		
		
		///BasDictFCTabIndex=0;
		for (k=0; k<SpecWordSum[DictID]; k++)
		{
			if( (unsigned char)(SpecDict[DictID][k]->word)[0]<=0xFE && 
				(unsigned char)(SpecDict[DictID][k]->word)[1]<=0xFE	) 
			{
				i=(unsigned char)(SpecDict[DictID][k]->word)[0] - 0x81;
				j=(unsigned char)(SpecDict[DictID][k]->word)[1] - 0x40;
				
				if(i>=0 && j>=0)
				{
					
					if(SpecDictFirstCharTable[DictID][i][j] -> FCWordSum == 0) /// 0 means empty
					{
						/*  add a new stem in the first character list */	
						SpecDictFirstCharTable[DictID][i][j] -> StartPos = k;
						SpecDictFirstCharTable[DictID][i][j] -> FCWordSum = 1;
						SpecDictFirstCharTable[DictID][i][j] -> MaxWordLen =  ///Both Basic and SpDict's are now in byte number!!!
							strlen(SpecDict[DictID][k]->word);
					}
					else   ///there already exists
					{
						SpecDictFirstCharTable[DictID][i][j] -> FCWordSum++;
						
						if ( (WordLen=strlen(SpecDict[DictID][k]->word)) >
							SpecDictFirstCharTable[DictID][i][j] -> MaxWordLen)
							SpecDictFirstCharTable[DictID][i][j] -> MaxWordLen = WordLen;
					}
				}
			}
			
		}///for()
}
#endif

/*	purpose :	compare two stems in the user's S&T dictionary, 
				according to its word and category information.
 */
int		CmpSpecDictStem(const void *Stem1, const void *Stem2)
{
	int t;
	if ( t=strcmp( (*(DictUnit**)Stem1)->word, (*(DictUnit**)Stem2)->word ) ) 
		return t;
	else
		return  strcmp( (*(DictUnit**)Stem1)->cate, (*(DictUnit**)Stem2)->cate );
}

