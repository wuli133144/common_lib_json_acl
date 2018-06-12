#ifndef SENT____
#define SENT____

struct	STUnit
{
	char	Word[MAX_WordLen+1];	/// word that recognized
	char	Cate[10];		/// the category of a recognized word: n, ns, Ag, ...
//	char	Pron[100];
//	int		FlagUndef;
};	

//struct STUnit	*SegTagUnits;
//STUnit SegTagUnits[MAX_NumOfWordsPerSentence];


extern int SegTagASentence(char* pCurSentence);
extern int  Do_LexicalAnalysis(char* pCurSentence, int* WSum);

#endif

