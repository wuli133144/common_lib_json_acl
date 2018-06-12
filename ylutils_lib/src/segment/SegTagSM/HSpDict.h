/***
 *
 *  HSpDict.h :    header file of the HSpDict.c
 *
 ***/
#ifndef _HSPDICT_H_
#define _HSPDICT_H_

#include 	"Limits.h"
#include	"HBasDict.h"

/*  externals	*/
extern  void    ReadSpecDict(void);
extern  int		SearchFCListOfSpecDict(int DictID, char* FirstChar, 
									   int *StPos, int* FCWordSum);

extern  char*	LookupSpecDict(int DictID, char *word, int StPos, 
							   int FCWordSum, int* DictIndex);

extern	void	FreeSpecDictMem();


//
///////// use multiple user-specified dictionaries ///////
//
//
#define MAX_AliasLen (10*MAX_WordLen) 

//
////// a dict entry:
//
// <moved to "HSpDict.h">n
typedef struct
{
  char  word[MAX_WordLen+1];
  char  cate[MAX_WordCatesLen+1];

#ifndef UNIX_SEGTAG
  ///{{{ let's extend an "alias mechanism":
  char	fAlias;
  char *pAlias;
  ///}}}
#endif

} DictUnit;
//
/////// the internal form of SpecDicts: array of pointers
extern	DictUnit *		SpecDict[MAX_SpecDictNum][MAX_NumOfASpecDictWords];
extern  int				SpecDictNum;		//the actual number of Spec-dictionaries (actually read in)
extern 	int				SpecDictIndex/*=0*/;   // index to SpecDict[] -- the "cursor"!
extern int				*SpecWordSum;//[MAX_SpecDictNum];	//[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
//



	/////// The "Index (File)" of each usr-dict SpecDict[i]: each is a list
extern	DictFirstCharRecord   (*SpecDictFirstCharTable)[MAX_NumOfSpecDictFirstChars]; 
	//
extern	int		SpDictFCTabIndex; //index of SpDict FC Table
extern 	int		*SpDictFirstCharSum;	//[MAX_SpecDictNum]; //total First-Chars in each SpDict
	//

#endif

