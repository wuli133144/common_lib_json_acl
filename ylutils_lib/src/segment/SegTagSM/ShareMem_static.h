
//=====================================================
//	public data share with share memory
//=====================================================
//for BasicDict
BasDictUnit *	BasicDict[MAX_NumOfBasicDictWords];
long			BasicDictIndex/*=0*/;   // index to BasicDict[] -- the "cursor"!
long			BasicWordSum; //totaol (actually*) words in the basic Dictionary

//for BasicDictIndex:
int		BasDictFCTabIndex; //index of the Basic Dict FC Table
int		BasDictFirstCharSum; //total First-Chars in BasDict
DictFirstCharRecord * BasicDictFirstCharTable[126][191];


LEXFIX  prefix[PREFNUM];		/* prefix word list  */
int     preid;
LEXFIX  suffix[SUFFNUM];		/* suffix word list  */
int     sufid;
SPECSUFREC  specsl[SPECSUFWNUM];	/*  special suffix information list  */
int     speclid;


MLEX    maa[MAANUM]; /* AA list */
int     maaid;
MLEX    maabb[MAABBNUM]; /* AABB list */
int     maabbid;
char*	mabb[MABBNUM]/*[6]*/;  /* ABB list */
int     mabbid;
char*	maliab[MALIABNUM]/*[6]*/;  /* ALiAB list */
int     maliabid;

long    int     tagmat[TAGNUM][TAGNUM];         /* tag concurrent matrix define */
long    int     RowTSum[TAGNUM];                /* tag's sum in corpus file */
double          PConTag[TAGNUM][TAGNUM];        /* array of coocurrence probabilties  */
TWREC   twlist[MAXTWORD];               /* list of trained words */
int     twlid;                          /* index counter */


//========================================================
//for BasicDict
BasDictUnit *	BasicDict[MAX_NumOfBasicDictWords];
long			BasicDictIndex/*=0*/;   // index to BasicDict[] -- the "cursor"!
long			BasicWordSum; //totaol (actually*) words in the basic Dictionary

//for BasicDictIndex:
int		BasDictFCTabIndex; //index of the Basic Dict FC Table
int		BasDictFirstCharSum; //total First-Chars in BasDict
DictFirstCharRecord * BasicDictFirstCharTable[126][191];


LEXFIX  prefix[PREFNUM];		/* prefix word list  */
int     preid;
LEXFIX  suffix[SUFFNUM];		/* suffix word list  */
int     sufid;
SPECSUFREC  specsl[SPECSUFWNUM];	/*  special suffix information list  */
int     speclid;


MLEX    maa[MAANUM]; /* AA list */
int     maaid;
MLEX    maabb[MAABBNUM]; /* AABB list */
int     maabbid;
char*	mabb[MABBNUM]/*[6]*/;  /* ABB list */
int     mabbid;
char*	maliab[MALIABNUM]/*[6]*/;  /* ALiAB list */
int     maliabid;

long    int     (*tagmat)[TAGNUM];         /* tag concurrent matrix define */
//long    int     tagmat[TAGNUM][TAGNUM];         /* tag concurrent matrix define */
long    int     *RowTSum;                /* tag's sum in corpus file */
double          PConTag[TAGNUM][TAGNUM];        /* array of coocurrence probabilties  */
TWREC   twlist[MAXTWORD];               /* list of trained words */
int     twlid;                          /* index counter */


DictUnit *		SpecDict[MAX_SpecDictNum][MAX_NumOfASpecDictWords];
int				SpecDictIndex/*=0*/;   // index to SpecDict[] -- the "cursor"!
int				SpecWordSum[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
DictFirstCharRecord   SpecDictFirstCharTable[MAX_SpecDictNum][MAX_NumOfSpecDictFirstChars]; 
int		SpDictFCTabIndex; //index of SpDict FC Table
int		SpDictFirstCharSum[MAX_SpecDictNum]; //total First-Chars in each SpDict


