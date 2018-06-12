//=====================================
// pb>>share memory interface for SegTag
//=====================================
#ifndef _SEGTAG_SHAREMEM_H_
#define _SEGTAG_SHAREMEM_H_


#define _USE_CHINESE_NE_
				
#ifdef _USE_CHINESE_NE_				
#define _GET_IDX(word) ( (((unsigned char*)word)[0]-0x80)*128 +	(((unsigned char*)word)[1]-0x80) )	
#define _IS_VALID(word) (((unsigned char*)word)[0]>0x80 && ((unsigned char*)word)[1] >0x80 )
#endif 

#define CHARARRAYSIZE (128*128)
//=================================================================
//	C interface here
//=================================================================
#ifdef __cplusplus
extern "C" {
#endif

int		InitShareMem(char* fname);
int		MountShareMem(char* fname);
int		UnloadShareMem(char* fname);
int 	SegTagString(const char* buf,char* dest,const char* reservstr);


#ifdef __cplusplus
}
#endif

//=================================================================
//	C++ interface here
//=================================================================
#ifdef __cplusplus

#include "Limits.h"
#include "HBasDict.h"
#include "Lexfix.h"
#include "Overlap.h"
#include "Sbtprep.h"
#include "HSpDict.h"
#include "sentence.h"

//! zmh use open() read(), so include these headers.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//
//for public interface
//
void 	InitShareData();
void	SaveShareData(char* fname);

/**
 * @brief    将词典载入内存.
 *
 * @param    fname    词典文件.
 * @return   载入状态.
 * @retval   0   载入成功.
 * 	     -1  打开有错.
 * 	     -2  词典文件格式有错.
 */
int	LoadShareData(char* fname);

//! zmh 由于调用LoadShareData会有内存泄漏,所以将内存申请放大外面申请外面释放.
bool	ValidDictData(const char *pbuf, int len);
void	LoadShareDataEx(char* pbuf);

//
//for DataFile head
//
struct SShareDataHead{
	char	magic[4];
	int		len;
};

//
//for index of the arrays
//
struct SShareDataIndex{
	long			BasicDictIndex/*=0*/;   // index to BasicDict[] -- the "cursor"!
	long			BasicWordSum; //totaol (actually*) words in the basic Dictionary

	//for BasicDictIndex:
	int		BasDictFCTabIndex; //index of the Basic Dict FC Table
	int		BasDictFirstCharSum; //total First-Chars in BasDict

	int     preid;
	int     sufid;
	int     speclid;

	int     maaid;
	int     maabbid;
	int     mabbid;
	int     maliabid;

	int     twlid;                          /* index counter */

	int		SpecDictNum;		//the actual number of Spec-dictionaries (actually read in)
	int		SpecDictIndex/*=0*/;   // index to SpecDict[] -- the "cursor"!
	int		SpDictFCTabIndex; //index of SpDict FC Table

public:
	void	createsnap(){
		BasicDictIndex = ::BasicDictIndex;	
	}

	void	restoresnap(){
		::BasicDictIndex = BasicDictIndex;
	}

};

//
//for pointers to reallocate dynamically
//
struct SShareDataPtr{
	BasDictUnit *	BasicDict[MAX_NumOfBasicDictWords];
	DictFirstCharRecord * BasicDictFirstCharTable[126][191];
	LEXFIX  prefix[PREFNUM];		/* prefix word list  */
	LEXFIX  suffix[SUFFNUM];		/* suffix word list  */
	SPECSUFREC  specsl[SPECSUFWNUM];	/*  special suffix information list  */
	MLEX    maa[MAANUM]; /* AA list */
	MLEX    maabb[MAABBNUM]; /* AABB list */
	char*	mabb[MABBNUM]/*[6]*/;  /* ABB list */
	char*	maliab[MALIABNUM]/*[6]*/;  /* ALiAB list */
	long    int     (*tagmat)[TAGNUM];         /* tag concurrent matrix define */
	long    int     *RowTSum;                /* tag's sum in corpus file */
	double          PConTag[TAGNUM][TAGNUM];        /* array of coocurrence probabilties  */
	TWREC   twlist[MAXTWORD];               /* list of trained words */
	DictUnit *		SpecDict[MAX_SpecDictNum][MAX_NumOfASpecDictWords];
	int				SpecWordSum[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
	DictFirstCharRecord   SpecDictFirstCharTable[MAX_SpecDictNum][MAX_NumOfSpecDictFirstChars]; 
	int		SpDictFirstCharSum[MAX_SpecDictNum]; //total First-Chars in each SpDict

#ifdef _USE_CHINESE_NE_
	float	*psurname,*pmidname,*ptailname;
#endif

};

#endif

#endif

