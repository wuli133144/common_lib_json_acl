/*	stproc.h :	header file for segmentation and POS 
				tagging processing	*/

/*//////	extern functions prototypes	*/
/*	extern functions from   stword.c  	*/
extern	int    SegVarInitial(char* pCurSentence);   
extern	int     Segmentation(void);            
/*	extern functions from	stambi.c	*/
extern	int     SegDisambi();                     
/*	extern functions from	tagging.c	*/
extern	void    InterProcess(void);                  
extern	int     CheckTagAmbi(void);               
/*	extern functions from 	rbtmodel.c	*/
extern	void    Rule_Disambi(void);                     
/*	extern functions from   sbtmodel.c 	*/
extern	void    Stat_Disambi(void);                    

