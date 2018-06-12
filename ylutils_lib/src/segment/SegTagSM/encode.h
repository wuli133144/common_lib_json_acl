#ifndef __ENCODE_H__
#define __ENCODE_H__

#ifdef __cplusplus
extern "C" {
#endif
/* define for share mem */
#define PR_READ		0444
#define PR_ALL		0777

//2002-12-10,pb,support GBK,refer to http://www.haiyan.com/steelk/navigator/ref/gbk
#define _USE_GBK_
//======================================================================
#ifdef _USE_GB2312_
/* define constant gb of char set */
#define CHN_CHAR_NUM	6768

#define SYM_HIMIN	0xA1	/* GB symbol : minmum value of first byte */
#define SYM_HIMAX	0xF7	/* GB symbol : maxmum value of first byte */
#define SYM_LOMIN	0xA1	/* GB symbol : minmum value of second byte */
#define SYM_LOMAX	0xFE	/* GB symbol : maxmum value of second byte */

#define CHAR_HIMIN	0xB0	/* GB char   : min - 1st byte */
#define CHAR_HIMAX	0xF7	/* GB char   : max - 1st byte */
#define CHAR_LOMIN	0xA1	/* GB char   : min - 2nd byte */
#define CHAR_LOMAX	0xFE	/* GB char   : max - 2nd byte */

#define MARK_HIMIN	0xA1	/* GB mark   : min - 1st byte */
#define MARK_HIMAX	0xAF	/* GB mark   : max - 1st byte */
#define MARK_LOMIN	0xA1	/* GB mark   : min - 2nd byte */
#define MARK_LOMAX	0xFE	/* GB mark   : max - 2nd byte */

/* define the range of gb */
#define IN_RANGE(ch, min, max) ( (((unsigned char)(ch))>=min) && (((unsigned char)(ch))<=max) )

#define IS_GB(cst)	( IN_RANGE(cst[0], SYM_HIMIN, SYM_HIMAX) && IN_RANGE(cst[1], SYM_LOMIN, SYM_LOMAX) )

#define IS_GB_CODE(cst)	( IN_RANGE(cst[0], CHAR_HIMIN, CHAR_HIMAX) && IN_RANGE(cst[1], CHAR_LOMIN, CHAR_LOMAX) )

#define IS_GB_MARK(cst)	( IN_RANGE(cst[0], MARK_HIMIN, MARK_HIMAX) && IN_RANGE(cst[1], MARK_LOMIN, MARK_LOMAX) )

#define IS_GB_SPACE(cst) ( (((unsigned char)(cst[0]))==SYM_HIMIN) && (((unsigned char)(cst[1]))==SYM_LOMIN) )

#endif

//========================================================================
#ifdef _USE_GBK_
/* define constant gb of char set */
#define CHN_CHAR_NUM	((0xfe-0x81+1)*(0xfe-0x40+1))

#define SYM_HIMIN	0x81	/* GBK symbol : minmum value of first byte */
#define SYM_HIMAX	0xFE	/* GBK symbol : maxmum value of first byte */
#define SYM_LOMIN	0x40	/* GBK symbol : minmum value of second byte */
#define SYM_LOMAX	0xFE	/* GBK symbol : maxmum value of second byte */

#define CHAR_HIMIN	0xB0	/* GB 2312 char   : min - 1st byte */
#define CHAR_HIMAX	0xF7	/* GB 2312 char   : max - 1st byte */
#define CHAR_LOMIN	0xA1	/* GB 2312 char   : min - 2nd byte */
#define CHAR_LOMAX	0xFE	/* GB 2312 char   : max - 2nd byte */

#define CHAR1_HIMIN	0x81	/* GBK 1 char   : min - 1st byte */
#define CHAR1_HIMAX	0xA0	/* GBK 1 char   : max - 1st byte */
#define CHAR1_LOMIN	0x40	/* GBK 1 char   : min - 2nd byte */
#define CHAR1_LOMAX	0xFE	/* GBK 1 char   : max - 2nd byte */

#define CHAR2_HIMIN	0xAA	/* GBK 2 char   : min - 1st byte */
#define CHAR2_HIMAX	0xFE	/* GBK 2 char   : max - 1st byte */
#define CHAR2_LOMIN	0x40	/* GBK 2 char   : min - 2nd byte */
#define CHAR2_LOMAX	0xA0	/* GBK 2 char   : max - 2nd byte */


#define MARK_HIMIN	0xA1	/* GB 2312 mark   : min - 1st byte */
#define MARK_HIMAX	0xA9	/* GB 2312 mark   : max - 1st byte */
#define MARK_LOMIN	0xA1	/* GB 2312 mark   : min - 2nd byte */
#define MARK_LOMAX	0xFE	/* GB 2312 mark   : max - 2nd byte */


#define MARK1_HIMIN	0xA8	/* GBK mark   : min - 1st byte */
#define MARK1_HIMAX	0xA9	/* GBK mark   : max - 1st byte */
#define MARK1_LOMIN	0x40	/* GBK mark   : min - 2nd byte */
#define MARK1_LOMAX	0xFE	/* GBK mark   : max - 2nd byte */

/* define the range of gb */
#define IN_RANGE(ch, min, max) ( (((unsigned char)(ch))>=min) && (((unsigned char)(ch))<=max) )

#define IS_GB(cst)	( IN_RANGE(cst[0], SYM_HIMIN, SYM_HIMAX) && IN_RANGE(cst[1], SYM_LOMIN, SYM_LOMAX) )

//! 不支持日文, zmh modify.
#define IS_GB_CODE(cst)	( IN_RANGE(cst[0], SYM_HIMIN, SYM_HIMAX) && IN_RANGE(cst[1], SYM_LOMIN, SYM_LOMAX) )

//#define IS_GB_CODE(cst)	(( IN_RANGE(cst[0], CHAR_HIMIN, CHAR_HIMAX) && IN_RANGE(cst[1], CHAR_LOMIN, CHAR_LOMAX) )  ||	\
			 ( IN_RANGE(cst[0], CHAR1_HIMIN, CHAR1_HIMAX) && IN_RANGE(cst[1], CHAR1_LOMIN, CHAR1_LOMAX) ) ||	\
			 ( IN_RANGE(cst[0], CHAR2_HIMIN, CHAR2_HIMAX) && IN_RANGE(cst[1], CHAR2_LOMIN, CHAR2_LOMAX)))

#define IS_GB_MARK(cst)	(( IN_RANGE(cst[0], MARK_HIMIN, MARK_HIMAX) && IN_RANGE(cst[1], MARK_LOMIN, MARK_LOMAX) ) || \
			 ( IN_RANGE(cst[0], MARK1_HIMIN, MARK1_HIMAX) && IN_RANGE(cst[1], MARK1_LOMIN, MARK1_LOMAX)) ) 

#define IS_GB_SPACE(cst) (( (((unsigned char)(cst[0]))==SYM_HIMIN) && (((unsigned char)(cst[1]))==SYM_LOMIN) ) || \
			  ( (((unsigned char)(cst[0]))==0xa3) && (((unsigned char)(cst[1]))==0xa0) ))

#endif

//============================================================================
/* extern functions */
int analysis_string_ext( char *s_words, char *d_words,
			int *words_code, char *words_feature );

int AnalysisInput(char *s_words, char *d_words, int *words_code);


int is_chnword(char *word);
int correct_chnword(char *word );
int is_engword(char *word);
int correct_engword(char *word);

int is_engchar(char *word);
int is_chnchar(char *word);
int is_number(char *word);
int to_eng_low(char *pstr);

#ifdef __cplusplus
}
#endif
#endif

