/////// on Windows
//
//#include	<windows.h>
//#include "stdafx.h"	//zanhy 2002-12-23

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>     
#include	<ctype.h>

#include	"Limits.h"
#include	"Basicdat.h"
#include	"HBasDict.h"
#include	"HSpDict.h"
#include	"Stproc.h"
#include	"Sbtprep.h"
#include	"Wordform.h"

#include	"sentence.h"

//extern  struct STUnit  *SegTagUnits;

	
extern STUnit SegTagUnits[MAX_NumOfWordsPerSentence];

//
///////////// does the real work:
//
//
int	 Do_LexicalAnalysis(char* pCurSentence,int* WSum)
{
	int	i; 
	//char	*CateBuf;
	
//	ErrorInit();		/* initialize processing error flag;	*/

	InitSegTagUnitsList();	/* initialize memory used by segmented units	*/
    
	if (SegVarInitial(pCurSentence)) 
		return 0;              /* initialize segmentation variables */

	Segmentation();             /* dict-matching segmentation and basical disambiguation process */

	InterProcess(); /* intermediate process,
						including basic word_formation processing and so on */
	if (CheckTagAmbi()) {
		Rule_Disambi();     /* Rule-Based POS disambiguation */
		Stat_Disambi();     /* Statistics-Based POS disambiguation */
	}
	
	WordFormProc();         /* do some post-processing in the buffer */

	/*	set the returning arguments	*/
	*WSum = GetWordSum();
	
	for (i=0; i<*WSum; i++)		
	{
		strcpy(SegTagUnits[i].Word, GetWord(i));

		///{{{bs 99.1 for "long categorie":
/*		CateBuf = GetCate(i);
		if ((*CateBuf=='A') || (*CateBuf=='N') || (*CateBuf=='V'))	
			SegTagUnits[i].Cate = 'g';
		else
			SegTagUnits[i].Cate = *CateBuf;
*/
		(SegTagUnits[i].Cate)[0]='\0';
		strcpy(SegTagUnits[i].Cate ,GetCate(i));
///bs}}}
	} 
		
	if (*WSum >= MAX_NumOfWordsPerSentence)
		return 0;
	/*
	if (AnyError())
		return 0;
	*/
	return 1;
}


//
//
//////////////////////// do some encapsulations /////////////////////
//
//

//space that holds all recognition results:


int SegTagASentence(char* pCurSentence)
{
	int		WordSum;

	if(!strlen(pCurSentence)) //empty sentence doesn't waste any time!
		return 0;

	if(Do_LexicalAnalysis(pCurSentence, &WordSum))
		return WordSum;
	else
		return 0;
}


//
////////////////////////////
//
//
extern char*	ChEOS;//="。，；：！？"; //Chinese end of sentence punctuations
extern char*	EnEOS;//=".,;:!?";			// English end of sentence punctuations 
								//     --- except fractional numbers!
//
char* GetASentence(const char* pSrc, int* index_pSrc, int* fParagragh)
{
	static signed char   buffer[SENT_BUFFER_LEN];
	signed char ch;
	signed char	*pbuffer;
	char    ChPunctuation[3];    //holds possible chinese punctuation chars
	int     iChP; //counter of ChPunct

	if(!pSrc)
		return 0;

	pbuffer=buffer;
	iChP=0; //reset
	ChPunctuation[2]='\0';
	  //while(i<SENT_BUFFER_LEN) buffer[i++]='\0'; //static var: must be initialized to empty str!

/////read chars from pSrc:
//	
	while ((ch=pSrc[*index_pSrc]) != '\0')  
	{
	////recognize if a stentence starts a new paragragh:
	//  (we see a new paragragh as ending by an empty line immediately
	//   after <a puntuation of> the previous paragragh's last sentence)
		if(pSrc[*index_pSrc]=='\r' && pSrc[*index_pSrc+1]=='\n')///MS txt newline: 0D0A="\r\n"!!
		//if(pSrc[*index_pSrc]=='\n')
		{
				*fParagragh=1;
				(*index_pSrc)+=2; //may be continuing empty lines!...
				break;
		}

		(*index_pSrc)++;
		/////-> if(isspace(ch)) :	can't skip all ASCII spaces(as -67)
		if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\r') 
		{
			if( isalnum(pSrc[(*index_pSrc)-2]) &&	//English words/numbers in Chinese sentence
				((int)pSrc[(*index_pSrc)-2] > 0) )	//some Ch-char's low byte is also -alpha!
					*pbuffer++=' '; // only one space char needs to be kept!
		
			continue;
		}

		//if (pbuffer-(char*)buffer>SENT_BUFFER_LEN)  {
		if (pbuffer-buffer>SENT_BUFFER_LEN)  {

			ErrorInfo("Source sentence too long");
			//exit(1);
			break;
		}
		else
			*pbuffer++=ch; //receive it

	///// keep watching if we have reached any chinese punctuation(a 2-byte long string):
	//
		if (ch < 0) //start byte of chinese char
		{
			if (iChP<2)///iChP goes from 0++(1) to 1++(2)
				ChPunctuation[iChP++]=ch;  //holds the first/second half part of a Ch-char
			else  { ///iChP==3 : already full, and used!
				ChPunctuation[0]=ChPunctuation[1]='\0';
				  //for (iChP=0; iChP<3; iChP++) ChPunctuation[iChP]='\0';
				iChP=0;  
				ChPunctuation[iChP++]=ch; //the first byte
			}
		}
		else   //not a half part of a Ch-char
			if(strlen(ChPunctuation)) {  //not necessary if previous char is ASCII
				ChPunctuation[0]=ChPunctuation[1]='\0';
				  //for (iChP=0; iChP<3; iChP++) ChPunctuation[iChP]='\0';
				iChP=0;
			}

		if ((strlen(ChPunctuation)==2) && (strstr(ChEOS, ChPunctuation)!=NULL) 
			&& strcmp(ChPunctuation, "海")
			&& strcmp(ChPunctuation, "唬")) //bs: note some exceptions of this critiria!
			break;	//// Chinese EOS: a new sentence received

		if (((int)ch > 0) && ispunct(ch) && (strchr(EnEOS, ch)!=0))//English EOS?
		{
			if(ch=='.'|| ch==','||ch==':') //is a fractional, Tel-number, Time-num,...?
			{
				if( isdigit(pSrc[(*index_pSrc)-2]) && isdigit(pSrc[(*index_pSrc)]) ) 
					continue;
			}

			/*
			if(pSrc[*index_pSrc]=='\r' && pSrc[*index_pSrc+1]=='\n')///MS txt newline: 0D0A="\r\n"!!
			{
				*fParagragh=1;
				(*index_pSrc)+=2; //new line after sentence delimiters!
				//break;
			}
			*/
			break;  // a new sentence received
		}

	}///while ((ch=pSrc[*index_pSrc]) != '\0')  

	*pbuffer='\0';

	if(strlen((char *)buffer))
		return((char *)buffer);
	else
		return 0;
}

//Li Baoli begin
//function is same as the above one GetASentence()
/*
char *sgetsentest(const char* pSrc, int* index_pSrc, int* fParagragh)
{
	char ChnChar[3],SingleCh,FstOfChnChar,GetASentence,LeadOfChnCharGBK,NeedLookAhead;
	static char tmpsen[SENMAXLEN+1];
	int pos,spos;
	char ReachAlertPos; //标志输入句长度超过警戒值，需要强行截断

	FstOfChnChar = 1;
	GetASentence = 0;
	LeadOfChnCharGBK = 0;
	NeedLookAhead = 0;
	ReachAlertPos = 0;
	pos=0;
	spos = *index_pSrc;
	memset(ChnChar,0,sizeof(ChnChar));
	memset(tmpsen,0,sizeof(tmpsen));
	while (SingleCh=pSrc[spos++])
	{
		if ((SingleCh<0)||(LeadOfChnCharGBK))
		{
			//GBK : First_Byte< 0; -128<Second_Byte<128
			if (FstOfChnChar)
			{
				FstOfChnChar = 0;
				ChnChar[0] = SingleCh;
				LeadOfChnCharGBK = 1;
			}
			else
			{
				FstOfChnChar = 1;
				ChnChar[1] = SingleCh;
				LeadOfChnCharGBK = 0;
				//得到一个汉字
				if (!strcmp(ChnChar,"　"))
				{
//					spos++;
					if (!strlen(tmpsen))
						continue;
					else//遇到中文空格，强行切断
					{
						GetASentence = 1;
						break;
					}
				}
				memcpy(&(tmpsen[pos]),ChnChar,2);
				pos += 2;
				if ((strstr("。_，_；_：_！_？",ChnChar))||(ReachAlertPos&&(strstr("《_》_“_”_—_、_‘_’_（_）_…_【_】_〔_〕_〖_〗_［_］",ChnChar))))
				//汉字标点符号，或者长度达到警戒值并且遇到某些标点符号
				{
					GetASentence = 1;
					break;
				}
			}
		}
		else
		{
			if (SingleCh==' ' || SingleCh=='\t')
			//空白符
			{
				if (!strlen(tmpsen))
					continue;
				else//遇到空格或制表符，强行切断
				{
					GetASentence = 1;
					break;
				}
			}
			if (SingleCh == '\n')
			{
				if (!strlen(tmpsen))
					tmpsen[0] = '\n';
				else
					spos--;
//					ungetc('\n',zp);
				*fParagragh = 1;
				GetASentence = 1;
				break;
			}
			if (NeedLookAhead)
			{
				if (!isgraph(SingleCh))
				{
					spos--;
					GetASentence = 1;
					break;
				}
				else
					NeedLookAhead = 0;
			}
			tmpsen[pos++] = SingleCh;
			//英文的标点符号
			switch (SingleCh)
			{
			case ',':
			case ';':
			case '!':
			case '?':
				GetASentence = 1;
				break;
			case ':':
			case '.':
				if ((pos-2>=0) && isgraph(tmpsen[pos-2]))
					NeedLookAhead = 1;
				else
					GetASentence = 1;
				break;
			}
			if (GetASentence)
				break;
		}
		if (pos>=SENMAXLEN-2)
		{//长度达到极限，强行截取
//			fprintf(stderr,"Error : Sentence was too long(>%d bytes) in input file at %d.\n You'd better split the sentence into several parts. \nHere we split the sentence arbitrarily.\n",SENMAXLEN,ftell(zp));
			GetASentence = 1;
			break;
		}
		else
			if (pos>=SENMAXLEN-50)
				ReachAlertPos = 1;
	}
	//	if (GetASentence)
	if (!SingleCh)
		*index_pSrc = spos-1;
	else
		*index_pSrc = spos;
	if (strlen(tmpsen)>0)
	{
//		tmpsen[pos]=0;
		return tmpsen;
	}
	return NULL;
}*/

//Li Baoli end

//
//////////// give it out
//
//
void GiveOutSTSentence(char* pRslt, int curWordNumb, 
				   int fSnT, char* pDelim1, char* pDelim2, int* fParagragh, 
				   int fUseAlias, int fShowCates)
{
	char STWords[MAX_NumOfWordsPerSentence*(MAX_WordLen+MAX_WordCatesLen)];/*STWord[512]*/
	int i;

	char *tmppron;
	int *tmpruleflag;
	struct RuleFileItem *head;

	tmpruleflag = 0;
	head = NULL;
	tmppron = NULL;


/*	for (i=0;  i<curWordNumb;  i++) {
		if(fSnT)	{
///{{		///////we use the 98'863 23-tag set
			//
			if(SegTagUnits[i].Cate=='i'||SegTagUnits[i].Cate=='l')
				SegTagUnits[i].Cate='i';
			if(SegTagUnits[i].Cate=='x'||SegTagUnits[i].Cate=='g')
				SegTagUnits[i].Cate='x';
			if(SegTagUnits[i].Cate=='w')// punct's
				SegTagUnits[i].Cate='l';
////}}
			sprintf(STWords,"%s%s%c%s", SegTagUnits[i].Word, 
					pDelim1, SegTagUnits[i].Cate, pDelim2);
		}
		else //Seg only
			sprintf(STWords,"%s%s", SegTagUnits[i].Word, pDelim2);

		strcat(pRslt, STWords);
	}//for()
*/
//
////////use "alias" mechanism:
//
	char	fchar[3];
	int		DictID, DictIndex, StPos,FCWordSum,  fHaveAlias;
///for "show all cates":
	//char *	pAllCates;
	char	pAllCates[100];

	fchar[2]='\0';

	for (i=0;  i<curWordNumb;  i++) {
////////////sometimes "goto" clearfies the whole situation!!
///
/*		if(fUseAlias) {
			//get the first-char to search the usr-dict's:
			fchar[0]=SegTagUnits[i].Word[0];
			fchar[1]=SegTagUnits[i].Word[1];
			fchar[2]='\0';
	
			DictID=0;
			fHaveAlias=0;
			while(DictID<SpecDictNum) 
			{
				if(SearchFCListOfSpecDict(DictID, fchar, &StPos, &FCWordSum)) 
					if (LookupSpecDict(DictID, SegTagUnits[i].Word, StPos, FCWordSum, &DictIndex))  
						if(SpecDict[DictID][DictIndex]->fAlias == 1)
						{ 
							fHaveAlias=1;
							break;
						}

				DictID++;
			} //while()

			if(fHaveAlias)
					sprintf(STWords,"%s%s", SpecDict[DictID][DictIndex]->pAlias, pDelim2);
			else
			{
				if(fSnT) 
					sprintf(STWords,"%s%s%s%s", SegTagUnits[i].Word,
									pDelim1, SegTagUnits[i].Cate, pDelim2);
				else
					sprintf(STWords,"%s%s", SegTagUnits[i].Word, pDelim2);
			}
		}
		else { //do not UseAlias:
			if(fSnT) {
				if(fShowCates) {
					fchar[0]=SegTagUnits[i].Word[0];
					fchar[1]=SegTagUnits[i].Word[1];
					fchar[2]='\0';

					///we only need the basic dict word's cate's:
					//
					if(SearchFCListOfBasicDict(fchar,&StPos,&FCWordSum)) 
					{
						if(pAllCates=LookupBasicDict(SegTagUnits[i].Word, StPos, FCWordSum))
						  sprintf(STWords,"%s%s%s(%s)%s", SegTagUnits[i].Word, pDelim1, 
								SegTagUnits[i].Cate, pAllCates,	pDelim2);
					}
					else //a usr-def word:
						sprintf(STWords,"%s%s%s%s", SegTagUnits[i].Word,
								pDelim1, SegTagUnits[i].Cate, pDelim2);
				}
				else 
					sprintf(STWords,"%s%s%s%s", SegTagUnits[i].Word,
								pDelim1, SegTagUnits[i].Cate, pDelim2);
			} 
			else //seg only:
				sprintf(STWords,"%s%s", SegTagUnits[i].Word, pDelim2);
		}
*/ 
////////////sometimes "goto" clearfies the whole situation!!
///
#ifndef UNIX_SEGTAG

		if(fUseAlias) { ///don't care about fSnT and fShowCates !!
			//get the first-char to search the usr-dict's:
			fchar[0]=SegTagUnits[i].Word[0];
			fchar[1]=SegTagUnits[i].Word[1];
			//fchar[2]='\0';
	
			DictID=0;
			fHaveAlias=0;
			while(DictID<SpecDictNum) 
			{
				if(SearchFCListOfSpecDict(DictID, fchar, &StPos, &FCWordSum)) 
				  if (LookupSpecDict(DictID, SegTagUnits[i].Word, StPos, FCWordSum, &DictIndex))
						if(SpecDict[DictID][DictIndex]->fAlias == 1)
						{ 
							fHaveAlias=1;
							break;
						}

				DictID++;
			} //while()

			if(fHaveAlias)
			{
				sprintf(STWords,"%s%s", SpecDict[DictID][DictIndex]->pAlias, pDelim2);
				goto OutPut;
			}
			else
				goto NoAlias;
		}

NoAlias:
		if(fShowCates) {
			fchar[0]=SegTagUnits[i].Word[0];
			fchar[1]=SegTagUnits[i].Word[1];
			//fchar[2]='\0';

			///we only need the basic dict word's cate's:
			//
			if(SearchFCListOfBasicDict(fchar,&StPos,&FCWordSum)) 
			{
				LookupBasicDict(SegTagUnits[i].Word, StPos, FCWordSum, pAllCates, tmppron,tmpruleflag, head);
				if(strcmp(pAllCates, "***") != 0)
				//if(pAllCates=LookupBasicDict(SegTagUnits[i].Word, StPos, FCWordSum, tmpcate, tmppron,tmpruleflag, head))
				{ 
					//if(!strcmp(pAllCates, "w")) ///don't care about all punct's
					if(strchr(pAllCates, '-')==NULL) ///show multi-cate only
						goto NoBasicCates;

					sprintf(STWords,"%s%s%s(%s)%s", SegTagUnits[i].Word, pDelim1, 
							SegTagUnits[i].Cate, pAllCates,	pDelim2);
					goto OutPut;
				}
				else //a usr-def word:
					goto NoBasicCates;
			}
		}
		//else  goto NoBasicCates;
#endif ///#ifndef UNIX_SEGTAG

		
NoBasicCates:
		if(fSnT)
			sprintf(STWords,"%s%s%s%s", SegTagUnits[i].Word,
								pDelim1, SegTagUnits[i].Cate, pDelim2);
		else //seg only:
				sprintf(STWords,"%s%s", SegTagUnits[i].Word, pDelim2);

OutPut: //to result str
		strcat(pRslt, STWords);
	}//for()


	/////recover the line/paragrah status of pSrc[] to pRslt[]: 
	//		<is up to caller!!>N.
/*	if (ParagraghFlag)    
	{
		fprintf(tagetfp,"\n    ");
		ParagraghFlag = 0;		// clean the paragragh flag  
	}
	// ......
*/
	if(*fParagragh) {

#ifndef UNIX_SEGTAG
		//strcat(pRslt, "\r\n\r\n    "); //"0D0A"
		//strcat(pRslt, "\r\n");
		strcat(pRslt, "\n");
#else
		strcat(pRslt, "\n");
#endif

		*fParagragh=0;
	}
}
