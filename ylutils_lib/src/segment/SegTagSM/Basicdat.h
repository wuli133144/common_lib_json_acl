///////////////////
//
//		basicdat.h :
//				header file for some routines from CRountine.c
//
//


//
////////////	external functions	///////////////////	
//
//
extern	void	ErrorInit();
extern	void	ErrorInfo(char *ErrInfo);
extern	int		AnyError();

extern	void	InitSegTagUnitsList();
extern	int		AddWordCateInfo(char *word, char *cate);
extern	char*	GetWord(int  wpos);
extern	char*	GetCate(int  wpos);
extern	int		GetWordSum(); 
extern	char*	CombWord(int  stpos, int  num, char  *cate);

extern	void	SetDictDirectory(char *DictDirStr);
extern	char*	GetDictDirectory();
extern	void	FreePathMem();





