/* 
 *	 HBasDict.h :  header file for HBasDict.c
 *
 *
 */
#ifndef _HBASDICT_H_
#define _HBASDICT_H_

#include "Limits.h"
struct RuleFileItem{
	char  ruleLine[200];
	char  pron[50];
		
	struct RuleFileItem *Next;
};

typedef struct
{
	char  word[MAX_BasWordLen+1];
	char  cate[MAX_WordCatesLen+1];
	//char  pron[MAX_PINYINLEN];
	//int	  ruleFlag;
	//struct RuleFileItem *headRuleLink;
} BasDictUnit;

//
/////// the internal form of BasicDicts: array of pointers
//
extern BasDictUnit *	BasicDict[MAX_NumOfBasicDictWords];
extern long			BasicDictIndex/*=0*/;   // index to BasicDict[] -- the "cursor"!
extern long			BasicWordSum; //totaol (actually*) words in the basic Dictionary
//

//above is for BasicDict



//BasicDictIndex:
////// the record of First Character - "Dict Indexing File" unit:
typedef struct
{
	char  FirstChar[3]; /* the first character of Chinese words  */
	int   StartPos;     /* the starting position of the (adjacent) words with same first 
							character in the BasDictFCTable list */
	int   FCWordSum;  /* the sum of (adjacent) words with the same first character in the list */
	int   MaxWordLen; /* the maximum character number of these adjacent (the same FC) words  */
} DictFirstCharRecord;
//

//
extern int		BasDictFCTabIndex; //index of the Basic Dict FC Table
extern int		BasDictFirstCharSum; //total First-Chars in BasDict
//
	/////// The "Index (File)" of the BasicDict: a list
//	DictFirstCharRecord   BasicDictFirstCharTable[MAX_NumOfBasicDictFirstChars]; 

extern DictFirstCharRecord * BasicDictFirstCharTable[126][191];


/*  extern functions */
extern  void    readBasDic(char *curPath);
extern	int		SearchFCListOfBasicDict(char* FirstChar, int* StPos, 
										int* FCWordSum);
extern int		LookupBasicDict(char * word, int StPos, int FCWordSum, char *cate, char *pron, int *ruleflag, struct RuleFileItem *&resulthead);
extern	void	FreeBasicDictMem();
extern int		insertRuleLink(char *tmpWord, struct RuleFileItem *head);

#endif

