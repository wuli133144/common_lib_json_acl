/*
 *
 *wordform.h :	header file for the programs of word-forming 
 *					processing, which include lexfix.c and overlap.c
 *
 */

/*	extern functions in lexfix.c	*/
extern	int    InitLexFixList(void);                  
extern	void    LexFixProc(int  index); 
extern	void	FreeLexDataFNMem();                

/*	extern functions in overlap.c	*/
extern	int    InitOverlWordList(void);               
extern	void    ProcOverlap(int);   
extern	void	FreeOverlapFNMem();     

/*	extern functions in postproc.c	*/
extern	void    WordFormProc(void);                     

