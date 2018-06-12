///////
//////setmem.c	--	move all the process shared data into a common block,  Swen Bing, 1999.6
/////
////
//#include "stdafx.h"	//zanhy 2002-12-23

#ifdef	BING_S


/*static*/ char *	pMemBlock;	///the start byte of all the shared memory stuff -- freed elsewhere
static long		offset;		///the counter of used bytes
///static long		basdictsize;///the boundary


#ifdef UNIX_SEGTAG
	///UNIX shared memory stuff:
	#include	<sys/stat.h>
	#include	<sys/ipc.h>
	#include	<sys/shm.h>
	#include	<stdio.h>

	///test with VC++:
	//#define IPC_CREAT	111
	//#define IPC_RMID	222


	#define		PERMS	0777
	#define		SHMKEY_DictAll	31415
#else


#endif


#include	<stdlib.h>
#include	<memory.h>

#include	"..\TextToPinyin\Limits.h"
#include	"Basicdat.h"



///BasicDict:{{{
////// The basic-dict entry:
typedef struct
{
	char  word[MAX_BasWordLen+1];
	char  cate[MAX_WordCatesLen+1];
} BasDictUnit;
//
	extern BasDictUnit * BasicDict[MAX_NumOfBasicDictWords];
	extern int	BasicWordSum; //totaol words in the basic Dictionary
	//
#ifdef UNIX_SEGTAG
	BasDictUnit **	pBasicDict; ///to 80,000 BasicDict[]'s entry-ptr's!
	int			*	pBasicWordSum; 
	///a reference counter for the shared mem:
	int			*	pRefCounter;
#endif

///
////// the record of First Character - "Dict Indexing File" unit:
///
typedef struct
{
  char  FirstChar[3]; //* the first character of Chinese words  
  int   StartPos;     //* the starting position of the (adjacent) words with same first 
						//	character in the BasDictFCTable list 
  int   FCWordSum;  //* the sum of (adjacent) words with the same first character in the list
  int   MaxWordLen; //* the maximum character number of these adjacent (the same FC) words  
} DictFirstCharRecord ;
//
	extern DictFirstCharRecord * BasicDictFirstCharTable[126][191];
	//
#ifdef UNIX_SEGTAG
	///to the above 126x191 ptr's:
	DictFirstCharRecord *** pBasicDictFirstCharTable;
					///GBK: QuMa 0x81-0xFE, WeiMa 0x40-0xFE; 
#endif
///}}}}}


#define BasicEntryPtrs		(MAX_NumOfBasicDictWords*sizeof(BasDictUnit *))
#define BasicEntries		(MAX_NumOfBasicDictWords*sizeof(BasDictUnit))
#define BasicFirstCharTab	(126*191*sizeof(DictFirstCharRecord))
///alignment:
#define SIZE_BasDict	(BasicEntryPtrs + BasicEntries + BasicFirstCharTab \
							+ sizeof(int) + 4*5 )



//
////// a dict entry:
//
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
	extern DictUnit *SpecDict[MAX_SpecDictNum][MAX_NumOfASpecDictWords];
	extern int		SpecWordSum[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
	extern DictFirstCharRecord * SpecDictFirstCharTable[MAX_SpecDictNum][126][191];
	//
#ifdef UNIX_SEGTAG
	DictUnit **		pSpecDict[MAX_SpecDictNum];
	int		 *		pSpecWordSum[MAX_SpecDictNum];
	///to the above 126x191 ptr's:
	DictFirstCharRecord *** pSpecDictFirstCharTable[MAX_SpecDictNum];
#endif


#define SpecEntryPtrs	(MAX_SpecDictNum*MAX_NumOfASpecDictWords*sizeof(DictUnit*))
#define SpecEntries		(MAX_SpecDictNum*MAX_NumOfASpecDictWords*sizeof(DictUnit))
#define SpecFirstCharTab (MAX_SpecDictNum*BasicFirstCharTab)
#define SIZE_SpecDicts	(SpecEntryPtrs + SpecEntries + SpecFirstCharTab \
					+ MAX_SpecDictNum*sizeof(int) + 4*(5+MAX_SpecDictNum) )


///the total:
#define SIZE_AllDicts		(SIZE_BasDict + SIZE_SpecDicts)






////////////////////////////////////////////////////////////
///
#ifdef UNIX_SEGTAG
	void DoExchangeBasDict(int direction);
	void DoExchangeSpecDict(int direction);
	void UnloadDicts();
#endif
void	* BSmalloc(size_t  bsize);



void	* BSmalloc(size_t  bsize)
{
	void *p; //long i=SIZE_AllDicts;

	if (offset+bsize>SIZE_AllDicts){
		ErrorInfo("Shared memeory exhausted.");
		return 0;
	}

	p=(void *)(pMemBlock+offset);
	offset += bsize+4-bsize%4;
	
	return p; 
}


#ifdef UNIX_SEGTAG
/*	Exchange pointers and counters
		0: local -->  share  : upload*
		1: share -->  local	 : download*
*/
void DoExchangeBasDict(int direction)
{
	int i,j;

	if (direction==0) ///upload
	{
		*pBasicWordSum=BasicWordSum;
		for(i=0; i<MAX_NumOfBasicDictWords; i++)
			pBasicDict[i]=BasicDict[i];
		//
		for(i=0; i<126; i++)
			for(j=0; j<191; j++)
				pBasicDictFirstCharTable[i][j]=BasicDictFirstCharTable[i][j];
	}
	else	///download
	{
		BasicWordSum= *pBasicWordSum;
		for(i=0; i<MAX_NumOfBasicDictWords; i++)
			BasicDict[i]=pBasicDict[i];
		//
		for(i=0; i<126; i++)
			for(j=0; j<191; j++)
				BasicDictFirstCharTable[i][j]=pBasicDictFirstCharTable[i][j];
	}
}

/*	Exchange pointers and counters
		0: local -->  share  : upload*
		1: share -->  local	 : download*
*/
void DoExchangeSpecDict(int direction)
{
	int i,j, DictID;

	if (direction==0) ///upload
	{
		for(DictID=0; DictID<MAX_SpecDictNum; DictID++) 
		{
			*pSpecWordSum[DictID]=SpecWordSum[DictID];
			for(i=0; i<MAX_NumOfBasicDictWords; i++)
				pSpecDict[DictID][i]=SpecDict[DictID][i];
		
			//
			for(i=0; i<126; i++)
				for(j=0; j<191; j++)
					pSpecDictFirstCharTable[DictID][i][j]
						=SpecDictFirstCharTable[DictID][i][j];
		}
	}
	else	///download
	{
		for(DictID=0; DictID<MAX_SpecDictNum; DictID++) 
		{
			SpecWordSum[DictID]=*pSpecWordSum[DictID];
			for(i=0; i<MAX_NumOfBasicDictWords; i++)
				SpecDict[DictID][i]=pSpecDict[DictID][i];
		
			//
			for(i=0; i<126; i++)
				for(j=0; j<191; j++)
					SpecDictFirstCharTable[DictID][i][j]
						=pSpecDictFirstCharTable[DictID][i][j];
		}
	}
}

////unload the shared memory:
void UnloadDicts()
{
	int shm_cindexid;
    int msize=4;

    shm_cindexid=shmget(SHMKEY_DictAll, msize, PERMS);
    if(shm_cindexid ==-1){
        ///printf("Segtag shared memory not present.\n");
		//ErrorInfo("Segtag shared memory not present.\n");
    }
    else{
		if( --(*pRefCounter) == 0) {
			shmctl(shm_cindexid, IPC_RMID, 0);
			fprintf(stderr, "Unloaded the segtag shared memory 1.\n");
		}
    }
}
#endif ///#ifdef UNIX_SEGTAG



int SetBasDictMemBlock(void)
{
#ifdef UNIX_SEGTAG
	/* on UNIX */
	int shmid, flag;

	flag=shmget(SHMKEY_DictAll, SIZE_AllDicts, PERMS);
	if (flag==-1) {
		shmid=shmget(SHMKEY_DictAll, SIZE_AllDicts, PERMS|IPC_CREAT);
		if (shmid<0) {
			ErrorInfo("Cannot get/create shared memory for dict's.\n");
			return 0;
		}
	}
	else ///already created
		shmid=flag;

	pMemBlock=(char *)shmat(shmid, (char *)0, 0);
	if (pMemBlock==(char *)-1) {
		ErrorInfo("Can't attach the shared memory! stdictmg.c\n");
		return 0;
	}

	offset=0;
	
	if ((pBasicDict=(BasDictUnit **)BSmalloc(MAX_NumOfBasicDictWords*sizeof(BasDictUnit *)))==NULL)
		return 0;
	if ((pBasicWordSum=(int *)BSmalloc(sizeof(int)))==NULL)
		return 0;
	///
	if ((pBasicDictFirstCharTable=(DictFirstCharRecord***)BSmalloc(126*191*sizeof(DictFirstCharRecord*)))==NULL)
		return 0;
	///
	if ((pRefCounter=(int *)BSmalloc(sizeof(int)))==NULL)
		return 0;
	

	///incr. the ref.
	*pRefCounter += 1;

	/* init. the just-created shared memory */
	if (flag==-1) {
		///for(ll=0;ll<SIZE_AllDicts;ll++)	pMemBlock[ll]=0;
		memset(pMemBlock, 0, SIZE_AllDicts);
		//basdictsize=offset;	///save the boundary!
		//offset=0;
		return 1;
	}

	DoExchangeBasDict(1);	///0:  upload;  1: download
	return 2;


//////////////////
#else	///win32:

	pMemBlock=malloc(SIZE_AllDicts);
	if(!pMemBlock) {
		ErrorInfo("SLex: cannot allocate memory for dictionaries.");
		return 0;
	}
	offset=0;

	/*
	if(	!(pBasicDict=(BasDictUnit **)BSmalloc(BasicEntries))	||
		//!(pBasicDictIndex=(int*)BSmalloc(sizeof(int)))		||
		!(pBasicWordSum=(int*)BSmalloc(sizeof(int)))		||
		///
		!(pBasicDictFirstCharTable=
			(DictFirstCharRecord***)BSmalloc(FIRSTCHARTAB))	//||
		//!(pBasDictFCTabIndex=(int*)BSmalloc(sizeof(int)))	||
		//!(pBasDictFirstCharSum=(int*)BSmalloc(sizeof(int)))
	)
	{	ErrorInfo("SLex: memory allocation error.");
		return 0;
	}
	*/

	memset(pMemBlock, 0, SIZE_AllDicts);
	//basdictsize=offset;	///save the boundary!
	//offset=0;
	return 1;
#endif
}




int		SetSpecDictMemBlock(void)
{
#ifdef UNIX_SEGTAG
	int j;

	offset=offset+4-(offset+1)%4;	///starts from the boundary

	for(j=0; j<MAX_SpecDictNum; j++) {
		if ((pSpecDict[j]=(DictUnit **)BSmalloc(MAX_NumOfASpecDictWords*sizeof(DictUnit *)))==NULL)
			return 0;
		if ((pSpecWordSum[j]=(int *)BSmalloc(sizeof(int)))==NULL)
			return 0;
		///
		if ((pSpecDictFirstCharTable[j]=(DictFirstCharRecord***)BSmalloc(126*191*sizeof(DictFirstCharRecord*)))==NULL)
			return 0;
	}

	DoExchangeSpecDict(1);	///0:  upload;  1: download
	return 2;



//////////////////
#else	///win32:

	/*
	int i=0, offset2;

	offset2=offset=basdictsize+1 +4-(basdictsize+1)%4;
	if(offset>SIZE_AllDicts) {
		ErrorInfo("SLex: cannot allocate memory for dictionaries.");
		return 0;
	}

	while(i<MAX_SpecDictNum) {
		if(
		!(pSpecDict[i]=(DictUnit **)
			BSmalloc(MAX_NumOfASpecDictWords*sizeof(DictUnit)))	||
		!(pSpecWordSum[i]=(int*)BSmalloc(sizeof(int)))			||
		///
		!(pSpecDictFirstCharTable[i]=
			(DictFirstCharRecord*)BSmalloc(FIRSTCHARTAB))
		)
		{	ErrorInfo("SLex: memory allocation error.");
			return 0;
		}
		else
			i++;
	}

	offset=offset2; ///back to the boundary for ReadSpecDict!
	*/
	return 1;
#endif
}



#endif	///#ifdef	BING_S
