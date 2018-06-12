
/*	limits.h :	 */

//////	some general limitations:
//
#define MAX_NumOfWordsPerSentence	500	/* long enough for common chinese sentences */
#define SENT_BUFFER_LEN				1000/*600*/


#ifndef UNIX_SEGTAG
	#define	MAX_WordLen					30	//100	/* 50 ch-char longest words in UsrLex */
#else
	#define	MAX_WordLen					30
#endif

#define	MAX_BasWordLen				20	/* 4 ch-char long words in BasDict */
#define	MAX_WordCatesLen			16	/* the largest cate number of a word - "最大兼类"! */

//
//the max number of the words in the basic dict
#define MAX_NumOfBasicDictWords  80000	

/*
/////// all chinese chars used in the basic dict - about 5400 ("GB一级字库")!
#define MAX_NumOfBasicDictFirstChars   6000
*/
// GBK "2万":
#define MAX_NumOfBasicDictFirstChars   10000  /*20962*/

//
#ifndef UNIX_SEGTAG
	#define	MAX_SpecDictNum	7	/*30  - typically 3 to 20(small dicts)*/
#else
	#define	MAX_SpecDictNum	1	/*fasted speed */
#endif

//
//////the max number of user-defined words in each usr-dict, typically 2-30000 
#define MAX_NumOfASpecDictWords  30000 
//
/////// most-frequently used <n>x1000 ch-chars in usr-lexcons!
#define MAX_NumOfSpecDictFirstChars   5000 /*ZQ's Value=1000 */

#define MAX_WORDLEN 40
#define MAX_CATELEN 20
#define MAX_PINYINLEN 100
#define MAXWORDNUMBER 2000
#define MAX_SIZEOFASENTENCE 4000
#define MAXDICLINELEN 300
#define MAX_PINYINSIZEOFASENTENCE 9000



