
#include		"limits.h"

//
///////////	external vars and typenames from CRoutine.c
//
//
extern char	words[MAX_NumOfWordsPerSentence][MAX_WordLen+1];/* word list for storing seg result*/
extern char	cates[MAX_NumOfWordsPerSentence][MAX_WordCatesLen+1];/* category list of "seg-ed" words */
extern int	wid;/* order number of a word in a sentence, i.e., words[index][] */
/*//
typedef struct
{
	short	DictID;
	long	index; 
} Entry;
extern 	Entry	entries[MAX_NumOfWordsPerSentence];/// word entry-info indicator

extern 	Entry	GlobalEntryBuffer;
//static  char	wc0[2];
*/
extern 	char *	DictDir;		/* dictionary and statistics directory information 	*/
extern 	int		iErrors;		/* program error flag	*/


//
/////////// external functions
//
//
void	ErrorInit();
void	ErrorInfo(char *ErrInfo);
int		AnyError();
void	SetDictDirectory(char *DictDirStr);
char*	GetDictDirectory();
void	FreePathMem();

void	InitSegTagUnitsList(); // init words[][], cates[][] and wid
  ///int		SetThisWordInfo(char *word, char *cate, Entry *entry);
int		AddWordCateInfo(char *word, char *cate);
char*	GetWord(int  wpos);
char*	GetCate(int  wpos);
///Entry*	GetEntry(int  wpos);
int		GetWordSum();
char*	CombWord(int  stpos, int  num, char  *cate);
//
//// more:
//
int     InWordList(char   *word, char  *wlist[]);
int     incate(char  tag, char  *cate);
int     IsPossC(char  * , char  * );
char*	DeleTag(char , char  * );


