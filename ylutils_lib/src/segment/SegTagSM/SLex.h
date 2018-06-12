//////////////////// The Slex APIs //////////////////////////////
//
//
#ifdef BINS_S
	#define BOOL int
#endif


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


BOOL	SlexInitialize(/*void*/char *path);
int		SlexSegTagAString(const char* pSrc, char* pRslt, 
					  int fSnT, char* pDelim1, char* pDelim2, int fUseAlias, int fShowCates);
//BOOL	SlexAddDictionary(char* pFullPath);
//BOOL	SlexDelDictionary(int dictID);
//BOOL	SlexGetDictOrder(int DictOrder[/* MAX_SpecDictNumber */]);
//BOOL	SlexSetDictOrder(int OrgPos, int NewPos);
//BOOL	SlexAddWord(char* pWordUnit, int dictID);
//BOOL	SlexDelWord(char* pWordUnit, int dictID);
BOOL	SlexUninitialize(void);

//extern	int * pSpecDictNum;
  
#ifdef __cplusplus
}
#endif // __cplusplus


