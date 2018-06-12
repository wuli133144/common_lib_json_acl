/*******  This project is to CHANGE text to Pinyin         ******/
/*******  READ FROM : (source text)                        ******/
/*******  USING: NewWholePinyin.txt                        ******/
/*******  WRITE TO FILE: (result file)                     ******/


#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>

#include	"HBasDict.h"
//#include	"ruleProcess.h"
#include	"Limits.h"
//#include	"textToPinyiin.h"
//#include	"textToPinyiinDlg.h"	//zanhy 2002-12-23
#include	"Wordform.h"
#include	"Sbtprep.h"
#include	"CRoutine.h"

#include 	"encode.h"
#include	"sentence.h"

STUnit SegTagUnits[MAX_NumOfWordsPerSentence];

struct	wordUnit{
	char	Word[3];	
	char	Pron[20];
	char	Cate[10];
	int		Rule;         //1: has rule in rulefile(multipinkyin), 0: otherwise
	int		Sign;         //1: chinese word, 0: chinese or english sign
	int		Multi;        //1: multipinyin, 0: otherwise
	struct wordUnit *Next;
	struct wordUnit *Last;
	
};


int moveToItemLink(char *Text, int flag);
//int convertToPinyinLink();
int writeItemLink();
int freeLink();
int IsMulti(char *pronstr);
int writeSTSentence(int count,int nocate=0);
int AddPinyin(int nWordCount);
int toGetWord(char *str, char *word, int num);
int hasMultiItem(char *tmppron);
int pickPronString(char *pron, char *theCate, char *catestr, char *pronstr);

int pickMaxPron(char *pron, char *str);

//int searchTagLink(char *pronstr, int totalNum, int tagNumber, struct RuleFileItem *resultHead);

//int searchRuleLink(char *pronstr,int totalNum, int tagNumber,struct RuleFileItem *resultHead);
//int toExamineRule(char *tmpRuleLine, int totalNum, int tagNumber);
int toGetSingleChar(char *ItemInSent, char ch, int num, int totalNum, int tagNumber);
int toGetString(char *ItemInSent, char ch, int num, int totalNum, int tagNumber);
int toGetSingleWord(char *ItemInSent, char ch, int num, int totalNum, int tagNumber);
int toGetSingleType(char *ItemInSent, char ch, int num, int totalNum, int tagNumber);
int toGetMultiWord(char ArrInSent[10][100], char ch, int num, int totalNum, int tagNumber);
int toGetMultiType(char ArrInSent[10][100], char ch, int num, int totalNum, int tagNumber);



int pickTagLast1(char *last, int length, char *thestr);
int pickTagLast2(char *last, int length, char *thestr);
int pickTagLast3(char *last, int length, char *thestr);
int pickTagNextStr(char *next, int totalNum, int tagNumber);
int	pickTagLastStr(char *laststr, int tagNumber);

int pickLastPos(char *strpos, int tagNumber);
int pickNextPos(char *strpos, int totalNum, int tagNumber);

FILE *fpPinyin, *fpText,  *fpOutText;



struct wordUnit *lineHead;
struct wordUnit	*theword, *lastword, *curword;

char	*	ChEOS="。_，_；_：_！_？"; //Chinese end of sentence punctuations
char	*	EnEOS=".,;:!? ";			// English end of sentence punctuations 

#define		SENMAXLEN	512

char *sgetsen(FILE *zp)
//Li Baoli Added this function
{
	char ChnChar[3],SingleCh,FstOfChnChar,GetASentence,LeadOfChnCharGBK,NeedLookAhead;
	char BfByteType; //0:半角字符（非空、非换行符）; 1: 全角字符；
	static char tmpsen[SENMAXLEN+1];
	int pos;
	char ReachAlertPos; //标志输入句长度超过警戒值，需要强行截断

	FstOfChnChar = 1;
	GetASentence = 0;
	LeadOfChnCharGBK = 0;
	NeedLookAhead = 0;
	ReachAlertPos = 0;
	BfByteType = 0;
	pos = 0;
	memset(ChnChar,0,sizeof(ChnChar));
	memset(tmpsen,0,sizeof(tmpsen));
	while ((!feof(zp))&&(SingleCh=fgetc(zp)))
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
					if (!strlen(tmpsen)||BfByteType)
						continue;
					else//遇到中文空格，强行切断
					{
						GetASentence = 1;
						break;
					}
				}
				memcpy(&(tmpsen[pos]),ChnChar,2);
				pos += 2;
				BfByteType = 1;
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
				if (!strlen(tmpsen)||BfByteType)
					continue;
				else//遇到空格或制表符，强行切断
				{
					GetASentence = 1;
					break;
				}
			}
			if ((SingleCh == '\n')||(SingleCh == '\r'))
			{
				if (!strlen(tmpsen))
					tmpsen[0] = SingleCh;
				else
					ungetc(SingleCh,zp);
				GetASentence = 1;
				break;
			}
			if (NeedLookAhead)
			{
//				if (!isalnum(SingleCh))
				if (!isgraph(SingleCh))
				{
					ungetc(SingleCh,zp);
					GetASentence = 1;
					break;
				}
				else
					NeedLookAhead = 0;
			}

			tmpsen[pos++] = SingleCh;

			BfByteType = 0;

			//英文的标点符号
			switch (SingleCh)
			{
			case ',':
			case ':':
			case ';':
			case '!':
			case '?':
				GetASentence = 1;
				break;
			case '.':
				if ((pos-2>=0) && isgraph(tmpsen[pos-2]))
//				if ((pos-2>=0) && isalnum(tmpsen[pos-2]))
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
	if (strlen(tmpsen)>0)
	{
//		tmpsen[pos]=0;
		return tmpsen;
	}
	return NULL;
}

int InitConvert(char *curPath)
{
	strcat(curPath, "/lexicons/");	
	readBasDic(curPath);

	SetDictDirectory(curPath);

	if (InitLexFixList()) 
		return 0;       

	//  input overlapping word list
	if (InitOverlWordList()) 
		return 0;    
	
	//  initialize the data for SBT	
	InitSBTData();
}

int FreeConvert()
{
	FreeBasicDictMem();

    FreeLexDataFNMem(); 
    FreeOverlapFNMem();
}


int	mainConvert(char *SourFilename, char *TagFileName, int nocate)
{
	int		flagword = 0;
	long	fileindex = 0;
	int		tmpindex = 0;
	int     TotalWordNumb=0, curWordNumb=0;
	char    *pCurSentence;

	if ((fpText = fopen(SourFilename,"rt"))==NULL)
	{
		//AfxMessageBox("fail in open file source file!");
		printf("fail in open file source file!"); //zanhy 2002-12-22
		return 0;
	}


	if((fpOutText = fopen(TagFileName, "w")) == NULL)
	{
		//AfxMessageBox("fail in open outtext file!");
		printf("fail in open Tag file!"); //zanhy 2002-12-22
		return 0;
	}


	while ((pCurSentence=sgetsen(fpText))!=NULL)	{
		
		if ((strlen(pCurSentence)==1)&&(pCurSentence[0]=='\r'))
		{
			fprintf(fpOutText, "\n");
			continue;
		}
		
		if ((strlen(pCurSentence)==1)&&(pCurSentence[0]=='\n'))
		{
			fprintf(fpOutText,"\n");
			continue;
		}

		if(curWordNumb=SegTagASentence(pCurSentence))
		{
			//AddPinyin(curWordNumb); // add pinyin to SegTagUnits structure array
			writeSTSentence(curWordNumb,nocate);
		}
	}

	fclose(fpText);
	fclose(fpOutText);
	return 0;
}

int writeSTSentence(int count,int nocate /*=0*/)
{
	int i;

	for(i = 0; i < count; i++)
	{
		if(nocate){
			//prepare for word learning,only GB_CODE are left
			if(IS_GB(SegTagUnits[i].Word)){
				if(IS_GB_CODE( SegTagUnits[i].Word)){
					//filter out the 
					fprintf(fpOutText, "%s ", SegTagUnits[i].Word);
				}
			}
			else{
				//fprintf(fpOutText, "%s ", SegTagUnits[i].Word);
				if(SegTagUnits[i].Word[0]=='\n') fprintf(fpOutText,"\n");
				else fprintf(fpOutText, ": ");
			}
		}
		else
		{
			fprintf(fpOutText, "%s/%s ", SegTagUnits[i].Word,
				SegTagUnits[i].Cate);
		}
	}
	return 1;
}

int toGetSingleChar(char *ItemInSent, char ch, int num, int totalNum, int tagNumber)
{
	char tmpstr[100], tmpstr2[100];
	int  i;
	int  j, flag;
	int  len;

	tmpstr[0] = '\0';
	tmpstr2[0] = '\0';
	if(ch == 'R')
	{
		i = tagNumber+1;
		flag = 0;
		while(i < totalNum)
		{
			strcat(tmpstr, SegTagUnits[i].Word);
			i++;
			len = strlen(tmpstr);
			if(len >= num*2)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{
			j = (num - 1)*2;
			ItemInSent[0] = tmpstr[j];
			ItemInSent[1] = tmpstr[j+1];
			ItemInSent[2] = '\0';
			return 1;
		}
		else
			return 0;
	}
	else if(ch == 'L')
	{
		i = tagNumber-1;
		flag = 0;
		while(i > 0)
		{
			strcpy(tmpstr2, SegTagUnits[i].Word);
			strcat(tmpstr,tmpstr2);
			i--;
			len = strlen(tmpstr);
			if(len >= num*2)
			{
				flag = 1;
				break;
			}

		}
		if(flag == 1)
		{
			j = num*2;
			ItemInSent[0] = tmpstr[len-j];
			ItemInSent[1] = tmpstr[len-j+1];
			ItemInSent[2] = '\0';
			return 1;
		}
		else
			return 0;
	}
	return 0;
}

int toGetString(char *ItemInSent, char ch, int num, int totalNum, int tagNumber)
{
	char tmpstr[100], tmpstr2[100];
	int  i;
	int  j, flag;
	int  k;
	int  len;

	tmpstr[0] = '\0';
	tmpstr2[0] = '\0';
	if(ch == 'R')
	{
		i = tagNumber+1;
		flag = 0;
		while(i < totalNum)
		{
			strcat(tmpstr, SegTagUnits[i].Word);
			i++;
			len = strlen(tmpstr);
			if(len >= num*2)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{
			j = num*2;
			tmpstr[j] = '\0';
			strcpy(ItemInSent, tmpstr);
			return 1;
		}
		else
			return 0;
	}

	else if(ch == 'L')
	{
		i = tagNumber-1;
		flag = 0;
		while(i > 0)
		{
			strcpy(tmpstr2, SegTagUnits[i].Word);
			strcat(tmpstr,tmpstr2);
			i--;
			len = strlen(tmpstr);
			if(len >= num*2)
			{
				flag = 1;
				break;
			}

		}
		if(flag == 1)
		{
			j = len - num*2;
			k = 0;
			while(tmpstr[j] != '\0')
				ItemInSent[k++] = tmpstr[j++];
			ItemInSent[k] = '\0';
			return 1;
		}
		else
			return 0;
	}
	return 0;
}

int toGetSingleWord(char *ItemInSent, char ch, int num, int totalNum, int tagNumber)
{
	int  i;

	if(ch == 'R')
		i = tagNumber + num;
	else
		i = tagNumber - num;
	if((i < totalNum)&&(i > 0))
	{
		strcpy(ItemInSent, SegTagUnits[i].Word);
		return 1;
	}
	else
		return 0;
}

int toGetSingleType(char *ItemInSent, char ch, int num, int totalNum, int tagNumber)
{
	int  i;

	if(ch == 'R')
		i = tagNumber + num;
	else
		i = tagNumber - num;
	if((i>0)&&(i<totalNum))
	{
		strcpy(ItemInSent, SegTagUnits[i].Cate);
		return 1;
	}
	else
		return 0;
}

int toGetMultiWord(char ArrInSent[10][100], char ch, int num, int totalNum, int tagNumber)
{
	int begin;
	int j;

	memset(ArrInSent,0,10*100);

	if(num > 10)
	{
		//AfxMessageBox("too long distance");
		printf("too long distance\r");	//zanhy 2002-12-23
		return 1;
	}
	switch (ch)
	{
	case 'R':
		begin = tagNumber+1;
		for (j=begin; j<totalNum; j++)
		{
			strcpy(ArrInSent[j-begin], SegTagUnits[j].Word);
		}
		break;
	case 'L':
		begin = tagNumber - num;
		if ( begin < 0 )
			begin = 0;
		for (j=begin; j<tagNumber; j++)
		{
			strcpy(ArrInSent[j-begin], SegTagUnits[j].Word);
		}
		break;
	}
	/*
	Modified by Li Baoli, 2002 2 4
	if(ch == 'R')
		begin = tagNumber+1;
	else
		begin = tagNumber - num;
	for(j = 0; j < num; j++)
	{
		if( (j + tagNumber) > totalNum)
			break;
		strcpy(ArrInSent[j], SegTagUnits[begin].Word);
		begin++;
	}*/
	return 1;
}

int toGetMultiType(char ArrInSent[10][100], char ch, int num, int totalNum, int tagNumber)
{
	int begin;
	int j;

	memset(ArrInSent,0,10*100);

	if(num > 10)
	{
		//AfxMessageBox("too long distance");
		printf("too long distance\r");	//zanhy 2002-12-23
		return 1;
	}

	switch (ch)
	{
	case 'R':
		begin = tagNumber+1;
		for (j=begin; j<totalNum; j++)
		{
			strcpy(ArrInSent[j-begin], SegTagUnits[j].Cate);
		}
		break;
	case 'L':
		begin = tagNumber - num;
		if (( begin < 0 ))
			begin = 0;
		for (j=begin; j<tagNumber; j++)
		{
			strcpy(ArrInSent[j-begin], SegTagUnits[j].Cate);
		}
		break;
	}

	/*
	Modified by Li Baoli, 2002 2 4
	if(ch == 'R')
		begin = tagNumber+1;
	else
		begin = tagNumber - num;
	for(j = 0; j < num; j++)
	{
		if( (j + tagNumber) > totalNum)
			break;
		strcpy(ArrInSent[j], SegTagUnits[begin].Cate);
		begin++;
	}*/
	return 1;
}

int pickNextPos(char *strpos, int totalNum, int tagNumber)
{
	int i;
	int j,k;
	char tmpstr[10];

	i = tagNumber+1;
	if(i >= totalNum)
		strpos = '\0';
	else
	{
		j = k = 0;
		strcpy(tmpstr, SegTagUnits[i].Cate);
		while(tmpstr[j] == ' ') j++;
		while((tmpstr[j] != '\0')&&(tmpstr[j] != ' '))
			strpos[k++] = tmpstr[j++];
		strpos[k] = '\0';
	}
	return 1;
}

int pickLastPos(char *strpos, int tagNumber)
{
	int i,j;
	char tmpstr[10];

	if(tagNumber == 0)
		strcpy(strpos, "w");
	else
	{
		i = j = 0;
		strcpy(tmpstr, SegTagUnits[tagNumber-1].Cate);
		while(tmpstr[i] == ' ') i++;
		while((tmpstr[i] != '\0')&&(tmpstr[i] != ' '))
			strpos[j++] = tmpstr[i++];
		strpos[j] = '\0';
	}

	return 1;
}

int pickTagNextStr(char *nextstr, int totalNum, int tagNumber)
{
	int i, len;
	char tmpstr[100];

	tmpstr[0] = '\0';
	i = tagNumber+1;
	while(i < totalNum)
	{
		strcat(tmpstr, SegTagUnits[i].Word);
		i++;
		len = strlen(tmpstr);
		if(len > 10)
		{
			//tmpstr[10] = '\0';
			break;
		}
	}
	
	strcpy(nextstr, tmpstr);
	return 1;
}

// to pick up one chinese character before it		
int pickTagLast1(char *last1str, int length, char *thestr)
{
	int i=0;

	if(length < 2)
		return 1;

	while(i<length-2)
		i++;
	last1str[0] = thestr[i];
	last1str[1] = thestr[i+1];
	last1str[2] = '\0';

	return 1;
}

// to pick up two chinese characters before it
int pickTagLast2(char *last2str, int length, char *thestr)
{
	int i=0;
	int k=0;

	if(length < 4)
		return 1;

	while(i < length-4)
		i++;
	while(k < 4)
	{
		last2str[k++] = thestr[i++];
	}
	last2str[k] = '\0';

	return 1;
}

// to pick up three chinese characters before it
int	pickTagLast3(char *last3str, int length, char *thestr)
{
	int i=0;
	int k=0;

	if(length < 6)
		return 1;

	while(i < length-6)
		i++;
	while(k < 6)
	{
		last3str[k++] = thestr[i++];
	}
	last3str[k] = '\0';

	return 1;
}

// pick the previous three word form the sentence array
int	pickTagLastStr(char *laststr, int tagNumber)
{
	char tmpstr[100];
	int i;
	
	i = tagNumber;
	tmpstr[0] = '\0';
	if(tagNumber == 0)
		return 1;
	if(tagNumber > 2)
	{
		strcpy(tmpstr, SegTagUnits[tagNumber-3].Word);
		strcat(tmpstr, SegTagUnits[tagNumber-2].Word);
		strcat(tmpstr, SegTagUnits[tagNumber-1].Word);
	}
	else if(tagNumber > 1 )
	{
		strcpy(tmpstr, SegTagUnits[tagNumber-2].Word);
		strcat(tmpstr, SegTagUnits[tagNumber-1].Word);
	}
	else 
		strcpy(tmpstr, SegTagUnits[0].Word);

	strcpy(laststr, tmpstr);
	return 1;
}

// return 1: pronstr is multipinyin string, 0 otherwise
int IsMulti(char *pronstr)
{
	int i, len;

	len = strlen(pronstr);
	for(i = 0; i < len-1; i++)
	{
		if(pronstr[i]=='<')
			return 1;
	}
	return 0;	
}

int writeItemLink()
{
	curword = lineHead;
	while(curword != NULL)
	{

		if(curword->Sign == 1)
		{
			fprintf(fpOutText, "%s",curword->Word);
			fprintf(fpPinyin,"%s",curword->Pron);
		}
		else
		{
			fprintf(fpOutText, "%s", curword->Word);
			fprintf(fpPinyin, "%s", curword->Word);
		}
/*
		if(curword->Sign == 1)
		{
			//fprintf(fpOutText, "%s",curword->Word);
			fprintf(fpPinyin,"%s/%s[%s]  ",curword->Word,curword->Cate,
				curword->Pron);
		}
		else
		{
			//fprintf(fpOutText, "%s", curword->Word);
			if(strcmp(curword->Word,"\n")==0)
				fprintf(fpPinyin,"%s", curword->Word);
			else
				fprintf(fpPinyin, "%s/%s[%s]  ", curword->Word,"w",
					curword->Word);
		}
*/
		curword = curword->Next;
	}

	return 1;

}

int pickMaxPron(char *pron, char *str)
{
	char *strptr, *tokptr;
	char needpron[20], tmpstr1[20], tmpstr2[20];
	int  maxfreq;
	int i, len;
	int flag = 0;

	len = strlen(str);
	for(i = 0; i < len-1; i++)
		if(str[i]=='<')
		{
			flag=1;
			break;
		}
	if (flag == 0)
	{
		strcpy(pron, str);
		return 1;
	}

	strptr = str;
	maxfreq = 0;

	while((tokptr = strtok(strptr,"<")) != (char *)0)
	{
		if(*tokptr == '*') tokptr++;
		strcpy(tmpstr1, tokptr);
		strptr = (char *)0;
		if( (tokptr= strtok(strptr, ">")) != (char *)0 )
				strcpy(tmpstr2, tokptr);
		
		if(atoi(tmpstr2) >= maxfreq)
		{
				maxfreq = atoi(tmpstr2);
				strcpy(needpron, tmpstr1);
		}
		strptr = (char *)0;
	}	

	strcpy(pron, needpron);
	
	return 1;
}




/*
int addToResultBuffer(int wordnum)
{
	struct resultRecordUnit * aRecord,  *cur, *former;
	int i, j, k;
	int count=0;
	int flag=0;

	for(i=0; i<wordnum; i++)
	{
		former = ResultRecordHead;
		cur = ResultRecordHead->next;
		if(cur == NULL)
		{
			aRecord=(struct resultRecordUnit *)malloc(sizeof(struct resultRecordUnit));
			if (!aRecord)
					AfxMessageBox("the aRecord malloc error");
			strcpy(aRecord->word, theCell[i].Word);
			strcpy(aRecord->cate, theCell[i].Cate);
			strcpy(aRecord->pron, theCell[i].Pron);
			aRecord->flag = theCell[i].Flag;
			aRecord->freq = 1;
			aRecord->next = NULL;

			count++;
			ResultRecordHead->next = aRecord;
			former = ResultRecordHead;
			continue;
			
		}
		while(cur)
		{
				j = strcmp(cur->word, theCell[i].Word);
				k = strcmp(cur->cate, theCell[i].Cate);
				if(j < 0)
				{
					flag=1;
					former = cur;
					cur = cur->next;
				}
				else if (j == 0)
				{
					if(k==0)
					{
						flag=0;
						cur->freq++;
						break;
					}
					else
					{
						flag = 1;
						former = cur;
						cur = cur->next;
					}
				}
				else
				{
					aRecord=(struct resultRecordUnit *)malloc(sizeof(struct resultRecordUnit));
					if (!aRecord)
						AfxMessageBox("the aRecord malloc error");
					strcpy(aRecord->word, theCell[i].Word);
					strcpy(aRecord->cate, theCell[i].Cate);
					strcpy(aRecord->pron, theCell[i].Pron);
					aRecord->flag = theCell[i].Flag;
					aRecord->freq = 1;
					aRecord->next = NULL;

					count++;

					former->next = aRecord;
					aRecord->next = cur;
					
					flag=0;
					break;
				}
		}  // end of while
		if(flag==1)
		{
			aRecord=(struct resultRecordUnit *)malloc(sizeof(struct resultRecordUnit));
			if (!aRecord)
						printf("the aRecord malloc error");
			strcpy(aRecord->word, theCell[i].Word);
			strcpy(aRecord->cate, theCell[i].Cate);
			strcpy(aRecord->pron, theCell[i].Pron);
			aRecord->flag = theCell[i].Flag;
			aRecord->freq = 1;
			aRecord->next = NULL;

			count++;

			former->next = aRecord;
			aRecord->next = cur;
		}
	
	} // end of for

	return count;
}
*/

int hasMultiItem(char *tmppron)
{
	int i;

	i = 0;
	while (tmppron[i] != '\0')
	{
		if (tmppron[i] == '-')
			return 1; // multi pinyin
		i++;
	}
	return 0;
}
				

int pickPronString(char *pron, char *theCate, char *catestr, char *pronstr)
{
	char *strptr1, *strptr2, *tokptr1, *tokptr2;
	char tmpcate[20], tmppron[100];
	int flag=0, num1=0, num2=0, count=0;

	strcpy(tmpcate, catestr);
	strcpy(tmppron, pronstr);
	strptr1 = tmpcate;
	strptr2 = tmppron;

	while((tokptr1 = strtok(strptr1,"-")) != (char *)0)
	{
		num1++;
		if(!strcmp(theCate,tokptr1))
		{
			count = num1;
			flag = 1;
			break;
		}
		strptr1 = (char *)0;
	}

	if(flag==1)
	{
		while((tokptr2 = strtok(strptr2, "-*")) != (char *)0)
		{
			num2++;
			if(num2 == count)
			{
				strcpy(pron, tokptr2);
				break;
			}
			strptr2 = (char *)0;
		}
	}
	else
	{
		strcpy(tmppron,pronstr);
		strptr2 = tmppron;
		if((tokptr2 = strtok(strptr2,"*")) != (char *)0)
		{
			strptr2 = (char *)0;
			if((tokptr2 = strtok(strptr2, "\n")) != (char *)0)
				strcpy(pron, tokptr2);
		}
	}
	return 1;
}




int toGetWord(char *str, char *word, int num)
{
	unsigned int i;
	int count;
	int len;

	count = 0;

	len =  strlen(str);
	/*
	if(num*2 >= len)
		return 0;
	*/
	if(num >= len)
		return 0;
	i = 0;
	word[0] = '\0';
	while(1)
	{
		if(str[i] == '\0')
			return 0;
		else if(  ((unsigned char)str[i] > 0xAF) && ((unsigned char)str[i] < 0xF8) )
		{
			word[0] = str[i];
			word[1] = str[i+1];
			word[2] = '\0';
			i += 2;
			if(num == count)
				return 1;
			count++;
		}
		else if( ((unsigned char)str[i]  > 0xA0) && ((unsigned char)str[i] < 0xAA) )
		{
			word[0] = str[i];
			word[1] = str[i+1];
			word[2] = '\0';
			i += 2;
			if(num == count)
				return 2;      //chinese sign
			count++;
		}
		else
		{
			word[0] = str[i];
			word[1] = '\0';

			i++;
			if(num == count)
				return 2;   //english sign
			count++;
		}
			
	}

	return 0;
}


int procER(char *tmpstr, char *pronstr)
{
	int len;
	char str[100];
	char c;

	strcpy(str, pronstr);
	len = strlen(str);
	if( ((strncmp(&str[len-3], "er5", 3) == 0)||
		 (strncmp(&str[len-3], "er2", 3) == 0)) &&
		 (isdigit(str[len-4])) )
	{
		c = str[len-4];
		str[len-4] = 'r';
		str[len-3] = c;
		str[len-2] = '\0';
		
		strcpy(tmpstr, str);
		return 1;
	}

	return 0;
}
			






