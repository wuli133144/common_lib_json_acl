/****
 *      stambi.c: ambiguity IdentifyASS and process file
 *      purpose : IdentifyASS two type of segment ambiguity (I_ambiguity & C_ambiguity) ,
 *                  use rule to eliminate ambiguity in section part .
 *      input   : an ambiguous section part .
 *      output  : correct segment .
 ****/
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <string.h>
#include        <stdio.h>
#include		<stdlib.h>

#include    "Seg.h"
#include	"Commfunt.h" 
#include	"Basicdat.h"


int     segf1[MAX_WordLen+1];
int     segf2[MAX_WordLen+1];		 /* ambiguous position list */
int     segf3[MAX_WordLen+1];
char    segc1[MAX_WordLen+1][MAX_WordCatesLen+1]; /* tagging flags list */
char    segc2[MAX_WordLen+1][MAX_WordCatesLen+1];
char    segc3[MAX_WordLen+1][MAX_WordCatesLen+1];
int     sectpos; /* section position determined by disambiguation function */
int     firstmax[3],secmax[3];
int     ambiflag=0;  /*  ambiguous flag for sentence  */


typedef	struct	
{
	int	SChPos;		/* the starting position of the character for a segmentation unit*/
	int	EChPos;		/* the ending position of the character for a segmentation unit	*/
	char	CateTags[MAX_WordCatesLen+1];		/* the category tags for the unit*/
} CTRec;	///CateTag record

#define	MAXPOSSUNITNUM	20
CTRec	CTArray[MAXPOSSUNITNUM];/*	the array for all possible category tags*/
int	CTAid;
	
#define	MAXSEGPOSNUM	100
/*	the array for all possible segmentation position in a ambiguous string:	*/
int	SegPosArray[MAXSEGPOSNUM];	
int	SPAid;
int	MultiIANum;		/* the number of the Intersecting segmentation parts	*/
int	SBDFlag; 		/* flag for using Statistics-Based Disambiguation method.	*/
int	BestSegPath[MAXSEGPOSNUM]; 
int	BSPid;
double	BestSegPathScore;
/* category tags of the ending word for a multiple intersecting ambiguous string : */
char	EndWCateTags[15];	


typedef struct  
{
  char    word[10];
  int     ambinum ;
} CAMBIWORD;		/*	structure for the special combining ambiguous words	*/

/*      --- data lists ---               */
CAMBIWORD  cambil[]={"他用",1, "把手",2, "的话",3, "才能",4, "常会",4, "将来",5,
					 "往后",6, "穿着",7, "补过",7, "按着",7, "朝着",7, "沉着",7,
					 "得手",8,/*"得到",8,*/ "得了",8, "正点",9, "着呢",10, "完了",11, 
					 "好不",12,	"正在",13,  "吃水",14, "到头",15,"到手",15,"到家",15,
					 "在家",15,"一头",16, "一道",16,"\0",0};  //"人才","性交","白天"

char      *transv[]={"来","去","上","上来","上去","下","下来","下去",
					 "进","进来","进去","出","出来","出去","回","回来","回去","过",
					 "过来","过去","起","起来","开","开来","到","到来","到去","\0"};

char    *nsuf1[]={"们","生","家","长","\0"}; //名词后缀
char    *nsuf2[]={"集","员","队","书","机","时","人","权","\0"};   
char    *singr[]={"这","那","哪","每","多少","各","某","几","\0"}; //代词
char    *zclist[]={"着", "过" ,"\0"};    /* Zhu Ci list */
/* the list of Shu Liang Ci in category 'm'	*/
char	*ShuLiangList[]={"一个","一些","一切","多数","各种","许多","\0"};	
/*	the words starting Bu that can't be segmented 	*/
char	*BuNoSegWL[]={"不安","不可","\0"};		


//
///// internals  
//
//
void	InitializeSegPathVars();
int     IdentifyASS(void);
void	ResetSegUnitInfo(int LFlag);
void    SetSegUnitInfo(int BaseChPos, int	num, int *SecMPos, int *FirMPos);
void  	AddToCTArray(int SChPos, int EChPos, char *CateTags);
char	*GetSegUnitCateTag(int SChPos, int EChPos);
void	AddToSegPosArray(int SecPos);
int		SechPossSegPos(int StPos, int SFlag);
void	SearchBestSegPath(int StPos, int EndPos, int *Path, int *Pathid);
double	GetSegPathScore(int *Path, int Pathid);
void	SetBestSegPath(int *Path, int Pathid);
int     interambi(void);
int     checkc(char  tag1,  char  tag2);
int     checkc2(char  tag1,  char  tag2);
int     mtincate(char  *stag, char  *cate);
int     twocombap();
int     combambi();
int     specambiw(void);
int		incambil(char  *key);

/*      ---  extern function prototype  ---     */
extern	double	CountSegPathScore(char *[], int);       	/*	defined in sbtmodel.c	*/



//
//////// externals

/*	purpose :	segmentation disambiguation processing, reset the segmentation 
 *				  information for the following words
 *  return :    1 -- if segmented word list is overflow; 0 -- if is not overflow
 */
int     SegDisambi()
{
 	int     i, sectbuf=0;
 	char	*CTbuf;
	InitializeSegPathVars();		

	switch  (IdentifyASS())  
	{
    	case  	INTERSECT :
				sectpos=interambi();		/* disambiguate the I_Ambiguity	*/
				if ((MultiIANum>1) && SBDFlag)
				{// output the segmentation results of a multiple intersecting 
				 //    ambiguous string	
					SBDFlag = 0;
					/* bs 99.7
					if (segpossible)	
						HaveSegFlag = 1;			//* set the flag that already segmented. * /
					*/
					for (i=1; i<BSPid; i++)	
					{
						if ((CTbuf = GetSegUnitCateTag(BestSegPath[i-1],BestSegPath[i]))!=NULL)
							if (seg_word((BestSegPath[i]-BestSegPath[i-1])/* *2 */,CTbuf))
								return 1;
					}

					/*ZW change .head */
           			if (BSPid<=0)
           			{
           				sectpos=firstmax[0];  
           				if (seg_word(sectpos/* *2*/,segc1[sectpos])) 
							return 1;
						break;
					}
					/*ZW change . end*/
		            
					return 0;
				}
        	
				if (sectpos == secmax[0])	
					ResetSegUnitInfo(2);
        		else	
					if (firstmax[0]-secmax[0]< 2*2) ///bytes
						ResetSegUnitInfo(3);
	       		
				if (sectpos>secmax[0])  {
					sectbuf=sectpos;
		 			sectpos=twocombap();/* disambiguate the possible C_Ambiguate	*/
                }
			
				if (seg_word(sectpos/* *2 */,segc1[sectpos])) 
					return 1;
				/*
				if (sectbuf>sectpos)    {
  					if (seg_word((sectbuf-sectpos)*2,segc2[sectbuf-sectpos])) return 1;
				}	*/
          		break;

		case   	COMBINE   :
 				if (firstmax[0]-secmax[0]< 2*2) ///bytes
					ResetSegUnitInfo(3);
	       		sectpos=twocombap();		/* disambiguate the C_Ambiguity	*/
	       		if (sectpos==secmax[0])	{    /*	segment as A and B	*/
	       			if (seg_word(sectpos/* *2 */,segc1[sectpos]))
						return 1; 
	       			if (seg_word((firstmax[0]-sectpos)/* *2 */,segc2[firstmax[0]-sectpos]))
						return 1;
	       		}
	       		else	  /* segment as AB	*/
	       			if (seg_word(sectpos/* *2 */,segc1[sectpos])) 
						return 1;
				///bs:
				  HaveSegFlag=0;
				break;

        default  :
/*			 	ResetSegUnitInfo(3);	*/
           		sectpos=firstmax[0];  
           		if (seg_word(sectpos/* *2 */,segc1[sectpos]))
					return 1;
				break;
	}//switch  (IdentifyASS())  
	
	return 0;
}


////////////////////// internals //////////////////////////

/*	purpose :	reset the information of segmentation units
 *	argument :	LFlag --- the flag of processing level
 */
void	ResetSegUnitInfo(int LFlag)
{
	int	i;
	
	maxposit = segpossible=0;
	switch	(LFlag)	
	{
		case	2 :		
           		for (i=1;  i<=MAX_WordLen;  i++)   {
               		segflag[i]  = segf2[i];
               		if (segf2[i])	{
               			segpossible++;
						//maxposit = /*i*2*/;  ///now in byte
               			  maxposit = i;			///last, Dict WordLen in byte
               		}
               		strcpy(segcate[i],segc2[i]);
               	}
				break;
      	
		case    3 :
          		for (i=1;  i<=MAX_WordLen;  i++)   {
               		segflag[i]=segf3[i];
               		if (segf3[i])	{
               			segpossible++;
						//maxposit = /*i*2*/;  ///now in byte
						maxposit = i;			///last, Dict WordLen in byte
					}
               		strcpy(segcate[i],segc3[i]);
              	}
				break;

	}      /* end of switch */
	
	if (segpossible)	
		HaveSegFlag = 1; 
}


/*	purpose :	initialize variables for the ASS processing
 */
void	InitializeSegPathVars()
{
	int	i;
	int len,tmp;

	/* modified by jin, 10-27-2003 */
	// init segfX, segcX
	len = MAX_WordLen+1;
	memset(segf1, 0, len*sizeof(int));
	memset(segf2, 0, len*sizeof(int));
	memset(segf3, 0, len*sizeof(int));
	memset(segc1, 0, len*(MAX_WordCatesLen+1));
        memset(segc2, 0, len*(MAX_WordCatesLen+1));
        memset(segc3, 0, len*(MAX_WordCatesLen+1));
/*
	for (i=0; i<=MAX_WordLen; i++)	{
		segf1[i] = segf2[i] = segf3[i] = 0;
		segc1[i][0] = segc2[i][0] = segc3[i][0] = '\0';
	}
*/	
	// init firstmax, secmax
	memset(firstmax, 0, 3*sizeof(int));
	memset(secmax, 0, 3*sizeof(int));
/*
	for (i=0; i<3; i++)	
		firstmax[i] = secmax[i] = 0;
*/
	// init CTArray
	len = MAXPOSSUNITNUM;
	memset(CTArray, 0, len*sizeof(CTRec));
/*
	for (i=0; i<MAXPOSSUNITNUM; i++)	{
		CTArray[i].SChPos = CTArray[i].EChPos = 0;
		CTArray[i].CateTags[0] = '\0';
	}
*/
	// init SegPosArray, BestSegPath
	len = MAXSEGPOSNUM;
        memset(SegPosArray, 0, len*sizeof(int));
	memset(BestSegPath, 0xff, len*sizeof(int));
/*
	for (i=0; i<MAXSEGPOSNUM; i++)	{
		SegPosArray[i] = 0;
		BestSegPath[i] = -1;
	}
*/
	/* end of modify */

	sectpos = CTAid = BSPid = MultiIANum = SBDFlag = 0;
	SPAid = 1;
	BestSegPathScore = 0.0;
}	

	
/*	purpose :	Identify all possible Ambiguous Segmentation String(ASS) (the maximum number<7)
 *	return :	the flag of segmentation ambiguous string --- INTERSECT, COMBINE, or 0
 */
int     IdentifyASS(void)
{
	int     bp;
	int     i, /*ambiflag=0,*/ SecMPos=0, FirMPos=0, BaseChPos=0, BreakFlag=0;

	bp=basept; 
	SetSegUnitInfo(BaseChPos,0,&SecMPos,&FirMPos);
    
    for (i=1;  i<7;  i++)   {
		if (SecMPos>0)	{
           	basept += SecMPos /* *2 */;
           	BaseChPos += SecMPos;      	/*	 count the base character position for the segmented words	*/
		}
        else  {
			basept+=2;
			BaseChPos /*++*/ +=2;
		}
		/* skip the unnessceary segmentation test	*/
        if ((SegProcess()<2) && (i>=2))	
			BreakFlag++;

		if (maxposit && SecMPos && (maxposit/* /2 */ > FirMPos-SecMPos))
		  //if (maxposit && SecMPos && (maxposit>FirMPos-SecMPos))
				MultiIANum++;	/* check the multiple intersecting ambiguous parts	*/
		else	
			if (i>=2)	
					BreakFlag++;

		SetSegUnitInfo(BaseChPos, i, &SecMPos, &FirMPos);
		if (BreakFlag)	
			break;
	}

	if (MultiIANum>1)	{
		/*	get the ending word's category tags 	*/
		basept += FirMPos/* *2 */;
	if (SegProcess())
		strcpy(EndWCateTags,segcate[maxposit]);
		  //strcpy(EndWCateTags,segcate[maxposit]);
	else
		strcpy(EndWCateTags,"$");
	}

    if (firstmax[1])        {
		if (firstmax[1]>firstmax[0]-secmax[0])  
			ambiflag=INTERSECT;
		else 
			if (firstmax[1]==firstmax[0]-secmax[0])  
				ambiflag=COMBINE;
			else 
				if (firstmax[1]<firstmax[0]-secmax[0])   
					ambiflag=0;
	}

    basept=bp;
	return(ambiflag);
}


/*	purpose :	set the information of the segmented units
 *	argument :	BaseChPos --- the starting character position (relative position to <basept>) for this segmentation test. 	
 *					num --- the number of segmentation test
 *					SecMPos --- return the second maximum word position for this segmentation test
 *					FirMPos ---  return the first maximum word position for this segmentation test
 */
void    SetSegUnitInfo(int BaseChPos, int	num, int *SecMPos, int *FirMPos)
{
	int     i;

    switch  (num)   
	{
    	case   0 :
        		for  (i=1;  i<=MAX_WordLen;  i++)    {
					segf1[i]=segflag[i];
					strcpy(segc1[i],segcate[i]);
				}
				break;
       	
		case   1 :
           		for (i=1;  i<=MAX_WordLen;  i++)   {
               		segf2[i]=segflag[i];
               		strcpy(segc2[i],segcate[i]);
               	}
				break;
      	
		case   2 :
          		for (i=1;  i<=MAX_WordLen;  i++)   {
               		segf3[i]=segflag[i];
               		strcpy(segc3[i],segcate[i]);
              	}
				break;
	}             /* end of switch */

////Remember:  all counted in byte!!
///
	  //*FirMPos=maxposit/2;
	*FirMPos=maxposit;

    *SecMPos = (*FirMPos>0) ? *FirMPos- /*1*/2 : 0 ;
    
	while  (!segflag[*SecMPos] && (*SecMPos>0))  
		(*SecMPos)--;
    /*	add category tag information to category tag list	*/
    if (*SecMPos)	
		AddToCTArray(BaseChPos,BaseChPos+*SecMPos,segcate[*SecMPos]);

    if (*FirMPos)	
		AddToCTArray(BaseChPos,BaseChPos+*FirMPos,segcate[*FirMPos]);
    
	/*	add the segmentation position to the position list	*/
	AddToSegPosArray(*SecMPos+BaseChPos);
	AddToSegPosArray(*FirMPos+BaseChPos);    
    
	if (num<3)	{
    	firstmax[num] = *FirMPos;
        secmax[num] = *SecMPos;
	}
}


/*--- functions for segmentation position array and category tag list ---*/

/*	purpose :	add a category tag record to the list
 */
void  	AddToCTArray(int SChPos, int EChPos, char *CateTags)
{
	CTArray[CTAid].SChPos = SChPos;
	CTArray[CTAid].EChPos = EChPos;
	strcpy(CTArray[CTAid].CateTags,CateTags);

	if (++CTAid == MAXPOSSUNITNUM)	{
		ErrorInfo("Overflow in category tag list !\n");		return;
	}
}


/*	purpose :	get the category tag of a segmentation unit
 *	argument :	SChPos --- the starting character position of the segmentation unit
 *					EChPos ---  the starting character position of the segmentation unit.
 *	return :	the pointer of its category tag string -- if found;  NULL -- if not found
 */
char	*GetSegUnitCateTag(int SChPos, int EChPos)
{
	int	i;
	
	for (i=0; i<CTAid; i++)	{
		if ((SChPos==CTArray[i].SChPos) && (EChPos==CTArray[i].EChPos))
			return CTArray[i].CateTags;
	}
	return NULL;
}


/*	purpose :	add the possible segmentation position to the list
 */
void	AddToSegPosArray(int SecPos)
{
	if (SecPos>SegPosArray[SPAid-1])	   
		SegPosArray[SPAid] = SecPos;
	else	return;
	
	if (++SPAid == MAXSEGPOSNUM)	{
		ErrorInfo("Overflow in segmentation position list(Array*) !\n");
		return;
	}
}
        
                       
int	SechPossSegPos(int StPos, int SFlag)
/*	purpose :	search the possible segmentation position from <StPos> in segmentation position list.
 *	argument :	StPos --- the starting searching position
 *				SFlag --- flag for searching : 1 -- search the first position, 2 -- search the second position
 *	return :	the segmentation position --- if found in list;  -1 --- if not found
 */
{
	int	i;
	
	/*	locate the starting searching position in list	*/
	for (i=0; i<SPAid; i++)	{
		if (StPos == SegPosArray[i])
			break;
	}

	switch	(SFlag)	
	{
		case	1 :
				if (i+1<SPAid)	
					return	SegPosArray[i+1];
				break;
		
		case	2 :
				if (i+2<SPAid)	
					return 	SegPosArray[i+2];
				break;
	}
	
	return -1;
}
	

/*	purpose :	search a best segmentation path for the multiple intersecting ambiguous string.
 *	argument :	StPos --- the starting character position for processing
 *					EndPos --- the ending position of the ambigous string.
 *					Path --- the expanded segmentation path. 
 *					Pathid --- the index of path.
 */
void	SearchBestSegPath(int StPos, int EndPos, int *Path, int *Pathid)
{
	int	SegPos1, SegPos2;           
	
	if ((SegPos1=SechPossSegPos(StPos,1))!= -1)	
	{
		while (*Pathid && (Path[*Pathid-1]>StPos))	
			(*Pathid)--;
		Path[(*Pathid)++]=SegPos1;	

		if (SegPos1==EndPos)	
			SetBestSegPath(Path, *Pathid);
		else
			SearchBestSegPath(SegPos1,EndPos,Path,Pathid);
	}

	if ((SegPos2=SechPossSegPos(StPos,2))!= -1)	
	{
		while (*Pathid && (Path[*Pathid-1]>StPos))	
			(*Pathid)--;
		Path[(*Pathid)++]=SegPos2;

		if (SegPos2==EndPos)	
			SetBestSegPath(Path, *Pathid);
		else
			SearchBestSegPath(SegPos2,EndPos,Path,Pathid);
	}
}


/*	purpose :	set the best path for segmentation position, according to the disambiguation results
 */
void	SetBestSegPath(int *Path, int Pathid)
{
	int	i;
	double	SPScore;
		
	if ((SPScore=GetSegPathScore(Path,Pathid))>BestSegPathScore)	{
		/*	set the best segmentation path	*/
		BSPid = 0;
		for (i=0; i<Pathid; i++)
			BestSegPath[BSPid++] = Path[i];

		BestSegPathScore = SPScore;
	}
}


/*	purpose :	get the score of the segmentation path
 */
double	GetSegPathScore(int *Path, int Pathid)
{
	char	*CateTags[40], *CTBuf;
	int	i,CTNum=0, WSum;
	
	WSum = GetWordSum();
	/*	set the pointers in category tag list 	*/
	if (WSum)	
		CateTags[CTNum++] = GetCate(WSum-1);	/* the starting word's category tags	*/
	else
		CateTags[CTNum++] = "^";

	for (i=1; i<Pathid; i++)	{
		if ((CTBuf=GetSegUnitCateTag(Path[i-1], Path[i]))!=NULL)	
			CateTags[CTNum++] = CTBuf;
	}
		
	CateTags[CTNum++] = EndWCateTags;		/* the ending word's category tags		*/
	return CountSegPathScore(CateTags,CTNum);
}		


int     interambi(void)
{
	char    wbuf[50],wbuf2[50];
	char	PartA[50], PartB[50], PartC[50];
	int	i,Path[50], Pathid, WSum;

	WSum = GetWordSum();
	/*	get the three part in the intersecting ambiguous string	*/
  	GetWUnit(basept,secmax[0]/* *2 */,PartA);         
  	GetWUnit(basept+secmax[0]/* *2 */,(firstmax[0]-secmax[0])/* *2 */,PartB);
  	GetWUnit(basept+firstmax[0]/* *2 */,(firstmax[1]-firstmax[0]+secmax[0])/* *2 */,PartC);

	/*	process multiple intersecting ambiguous case	*/
  	if (MultiIANum>1)	{
  		if ((MultiIANum==2) && (firstmax[0]==2))	{
  			SBDFlag = 0;
  			return firstmax[0];
  		}
		
  		for (i=0; i<50; i++)	
			Path[i] = -1;

		Pathid = 1;  Path[0]=0;
		SearchBestSegPath(0,SegPosArray[SPAid-1],Path,&Pathid);  		
		SBDFlag = 1;
	  	return BestSegPath[1];
	}
	
	/*	process different kinds of ambiguous cases	*/
	if (checkc('p','n') || checkc('p','f') || checkc('p','s') 
		|| checkc('p','t'))/*  p+..+n|f|s|t */
			return(secmax[0]);

	/*	if ((firstmax[2]>=2) && (firstmax[1]<=2))     {           tri_overplay interambi 
		  	if (secmax[0]>=2)     {
	  			return(secmax[0]);
	  		}
	  		return(firstmax[0]);
	  	} */
	if (checkc('u','n') || checkc('u','f') || checkc('u','v')
		|| checkc('u','a'))     /*  u+..+n|f|v|a */
 			return(secmax[0]);
 	
    if (checkc('d','v'))    {                       /*  d+..+v */
		GetWUnit(basept+secmax[0]/* *2 */,firstmax[1]/* *2 */,wbuf);

        if (InWordList(wbuf,transv))   
			return(secmax[0]);
        
		if (incate('d',segc1[firstmax[0]]) && (incate('d',segc2[secmax[1]])
			|| incate('a',segc2[secmax[1]])))	
				return(secmax[0]);

        return(secmax[0]);
	}
  	
	if (checkc('a','n') || checkc('a','s'))  {      /*  a+..+n|s  */
		if (checkc2('n','v') || checkc2('t','v'))	 
			return(firstmax[0]);
    	
		if (incate('s',segc1[firstmax[0]]))   
			return(firstmax[0]);
    	
		GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
    	if (InWordList(wbuf,nsuf1))  
			return(firstmax[0]);
    	
		return(secmax[0]);
	}
	
	if (checkc('v','n') || checkc('v','f') || checkc('v','s') || checkc('v','r')
		|| checkc('v','t'))  
	{  /*  v+..+n|f|t */
			if (checkc2('t','n') || checkc2('r','n') || checkc2('f','n') || checkc('s','n'))
				return(firstmax[0]);
        
			if (checkc2('n','f') || checkc2('n','a'))	 
				return(firstmax[0]);
        
			if (WSum && incate('q',GetCate(WSum-1)) && checkc2('n','n'))	 
				return(firstmax[0]);
	    
			if (checkc2('v','u') || checkc2('a','u'))	
				return(firstmax[0]);
        
			GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
			if (InWordList(wbuf,nsuf2))    	
				return(firstmax[0]);
        
			GetWUnit(basept,firstmax[0]/* *2 */,wbuf);
			if (InWordList(wbuf,transv))   
				return(firstmax[0]);
        
			GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
			GetWUnit(basept,2,wbuf2);
			if (!strcmp(wbuf,wbuf2))      	
				return(firstmax[0]);
        
			return(secmax[0]);
	}
	
	if (checkc('v','v') || checkc('a','v'))    
	{            /*  v+..+v */
		GetWUnit(basept+secmax[0]/* *2 */,firstmax[1]/* *2 */,wbuf);
        
		if (InWordList(wbuf,transv))  
			return(secmax[0]);
        
		GetWUnit(basept,secmax[0]/* *2 */,wbuf);
        if (InWordList(wbuf,transv))   {
			GetWUnit(basept,firstmax[0]/* *2 */,wbuf);
            if (InWordList(wbuf,transv))      
				return(firstmax[0]);
            
			return(secmax[0]);
		}
	}
	
	if (checkc('v','a'))    
	{                           /*  v+..+a */
     	GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
        if (InWordList(wbuf,transv))   	 
			return(firstmax[0]);
        
		if (!strcmp("v",segc2[1])) 
			return(firstmax[0]);
        
		return(secmax[0]);
	}
	
	if (checkc('v','b') || checkc('v','d') || checkc('v','t')) 
	{            /*  v+..+b|d|t */
		if (checkc2('r','n'))      
			return(firstmax[0]);
       	
		return(secmax[0]);
	}
 	
	if (checkc('d','a'))    
	{		                /*  d+..+a */
		if (incate('d',segc1[firstmax[0]]) && (incate('d',segc2[secmax[1]])
			|| incate('a',segc2[secmax[1]])))
            return(firstmax[0]);

        return(secmax[0]);
	}
 	
	if (checkc('d','d'))    
	{                               /*  d+..+d */
       	if (mtincate("l-a",segc1[firstmax[0]]))  
			return(firstmax[0]);
        
		return(secmax[0]);
	}
	
	if (checkc('c','n') && !checkc('m','n')) 
	{           /*  c+..+n */
        if (checkc2('s','n'))        	
			return(firstmax[0]);
        
		return(secmax[0]);
	}
	
	if (checkc('r','a') || checkc('r','f') || checkc('r','d'))   
	{    /*  r+..+a|f */
        return(secmax[0]);
	}
	
	if (checkc('v','y'))    
	{              /*  v+..+y */
		return secmax[0];
	}
    
	if (incate('n',segc1[firstmax[0]]))
    {                /*  n+..+n */
		if ((secmax[0]==2) && (firstmax[1]>=2))
			return(secmax[0]);
	}
	
	if (!strcmp("n",segc1[secmax[0]]) && checkc('n','v'))  
	{       /*  n+..+n */
		GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
	  	if (InWordList(wbuf,transv) && incate('v',segc1[firstmax[0]]))	
			return(firstmax[0]);
        
		return(secmax[0]);
	}
	
	if (checkc('m','q') || checkc('q','n'))    	
		return secmax[0];
	
	if (!strcmp("f",segc1[secmax[0]]) && checkc('f','n'))     /*  f+..+n */
	  	return(secmax[0]);

	if (checkc('n','a') || checkc('n','f'))       	          /* n+..+a|f */
        return(secmax[0]);

    if (checkc('n','n'))    
	{
		GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
        if (!strcmp(wbuf,"儿") || !strcmp(wbuf,"子"))	 
			return(secmax[0]);
	}
	
	return(firstmax[0]);
}//int     interambi(void)


int     checkc(char  tag1,  char  tag2)
{
        if (!incate(tag1,segc1[secmax[0]]))     
			return(0);
        if (!incate(tag2,segc2[firstmax[1]]))  
			return(0);
        return(1);
}


int     checkc2(char  tag1,  char tag2)
{
        if (!incate(tag1,segc1[firstmax[0]])) 
			return(0);
        if (!incate(tag2,segc3[firstmax[2]]))    
			return(0);
        return(1);
}

int     mtincate(char  *stag, char  *cate)
{
	while (*stag)  {
	  if ((*stag!='-') && (strchr(cate,*stag)!=NULL)) 
		  return(1);
	  stag++;
	}
	return(0);
}

int     twocombap()
{
	int     ip;

	ip=combambi();
	if (ip==firstmax[0])
		ip=specambiw();
	return(ip);
}

/*	purpose :	process combining ambiguous strings (A|B=W)
 */
int     combambi()
{
	char    wbufA[50],wbufB[50],wbuf[50];
	char    wtest[100];
	int     WSum;

	WSum = GetWordSum();
	/*	get two word part A and B	*/
  	GetWUnit(basept,secmax[0]/* *2 */,wbufA);
  	GetWUnit(basept+secmax[0]/* *2 */,(firstmax[0]-secmax[0])/* *2 */,wbufB);

	/*	process different kinds of ambiguous cases	*/
	if (checkc('q','n') && (WSum>0))    {     /*  r|m + q^n --> r|m + q + n */
	  	/* not process Nian or Fen   */
		if (!strcmp("年",wbufA) || !strcmp("分",wbufA))      
			return(firstmax[0]);
	  	if (!strcmp("一",GetWord(WSum-1)) && incate('v',segc1[firstmax[0]]))	    
			return(firstmax[0]);
	  	if (incate('m',GetCate(WSum-1)) && !InWordList(GetWord(WSum-1),ShuLiangList))	
			return(secmax[0]);
	  	if (InWordList(GetWord(WSum-1),singr) && !mtincate("v-f",segc1[firstmax[0]]))
			return(secmax[0]);
	}		/*	end of Rule : r|m+q^n-->S 	*/
	
	if (checkc('n','f') && (WSum>0))    {     /*  p + n^f --> p + n + f */
		if (mtincate("n-s-t-f",segc1[firstmax[0]]))	  
			return(firstmax[0]);
        if (incate('p',GetCate(WSum-1)))
			return(secmax[0]);
	}			/*	end of  Rule : p+n^f-->S	*/
	
	if (checkc('u','n') && (WSum>0))    {    /*  v + u^n --> v + u + n */
		if (InWordList(wbufA,zclist) && !strcmp("v",GetCate(WSum-1)))
			return(secmax[0]);
	}		/*		end of Rule : v+u^n-->S	*/
	
	if (checkc('v','n') && (WSum>0))   {             /*  --- v^n  ---    */
		 /* v(dan) + V^n + Le|Lai --> (seg) */
	  	if (InWordList(wbufA,transv) && incate('v',GetCate(WSum-1)) && 
			!mtincate("s-f",segc1[firstmax[0]]))  {
	    		GetWUnit(basept+firstmax[0]/* *2 */,2,wbuf);
	    		if (!strcmp(wbuf,"了") || !strcmp(wbuf,"来"))	        
					return(secmax[0]);
		}
	  	/*  v + v^n + f --> (seg)   (daotou, daoshou)  */
/*	  	if (incate('v',GetCate(WSum-1)) && incate('f',segc3[firstmax[2]]) && !incate('v',segc2[firstmax[1]]))	{
			return(secmax[0]);
			}
*/
	  	/*  v^n/n + f --> (seg)   */
/*	  	if (!strcmp("n",segc1[firstmax[0]]) && incate('f',segc3[firstmax[2]]))	{
			return(secmax[0]);
			}
*/
	  	/*  v + (bu) + v^n|!a --> (seg)  */
	  	if ((WSum>1) && incate('v',GetCate(WSum-2)) && !strcmp(GetWord(WSum-1),"不")
			&& !incate('a',segc1[firstmax[0]]))	
				return(secmax[0]);
	}/*	end of Rule : --v^n--	*/
	
	if (checkc('n','v'))   {             /* n^v/n + V --> n + v + V */
		GetWUnit(basept+firstmax[0]/* *2 */,firstmax[2]/* *2 */,wbuf);
	  	if (InWordList(wbuf,transv) && !incate('v',segc1[firstmax[0]]))      
/*	    	for (i=WSum-1; i>0; i--)       {
	      		if (incate('p',cates[i]))	{
					return(secmax[0]);
					}
	      		} 
*/
	    	return(firstmax[0]);

	}/*	end of Rule : n^v+V-->S	*/
	
	if (checkc('m','q'))            {               /*  --- m^q ---  */
		if (mtincate("l-m-b-d",segc1[firstmax[0]]))		/* delete d from l-m-d(95.10.12)	*/
	    	return(firstmax[0]);

	  	/* m^q + n --> m + q + n */
	  	if (incate('n',segc3[firstmax[2]]))	  
			return(secmax[0]);

	  	/* v + m^q/d --> v + m + q   (yidao) */
	  	if (WSum && incate('v',GetCate(WSum-1)))	
			return(secmax[0]);
	}/*	end of Rule : --m^q--	*/
	
	if (checkc('f','v'))            {               /*  --- f^v ---  */
	  	/*  n&!v + f^v&!v --> n + f + v  */
	  	if (WSum && incate('n',GetCate(WSum-1)) && !incate('v',GetCate(WSum-1)) 
			&& !mtincate("v-a",segc1[firstmax[0]]))	
				return(secmax[0]);

	  	/*  (wang) + f^v --> (seg)  */
	  	if (WSum && !strcmp("往",GetWord(WSum-1)))	
			return(secmax[0]);
	}/*	end of Rule : --f^v--	*/
	
	if (checkc('d','v'))            {               /*  --- d^v ---  */
	  	/*  v + (bu)^v(single) --> v + (bu) + v  */  
		GetWUnit(basept,firstmax[0]/* *2 */,wbuf);
	  	if (!strcmp(wbufA,"不") && (firstmax[0]==2) && WSum && 
			incate('v',GetCate(WSum-1)) && !InWordList(wbuf,BuNoSegWL))	
	    	return(secmax[0]);
	}/*	end of Rule : --d^v--	*/
	
	if (checkc('v','v'))            {               /*  --- v^V ---  */
	  	/*  v^V + ... + (Lai | Le) --> (seg)  */
/*	  	GetWUnit(basept+secmax[0]*2,2,wbuf);
	  	if (InWordList(wbuf,transv))      {
	    	GetWUnit(basept+(firstmax[0]+firstmax[2])*2,40,wtest);
	    	if ((strstr(wtest,"了") != NULL) || (strstr(wtest,"来")!=NULL))	    
				return(secmax[0]);
		}
*/
	}/*	end of Rule : --v^v--	*/
	
	/*  forword overlap processing*/
	/* !!hong!!
	if ((WSum>0) && !strcmp(GetWord(WSum-1),wbufA))  {      
		if (incate('v',GetCate(WSum-1)))
	    	if (incate('v',segc1[firstmax[0]]) || incate('d',segc1[firstmax[0]]))
				return(1);
          	if (incate('a',GetCate(WSum-1)) && incate('v',segc1[firstmax[0]]))
                return(1);
	}
	*/
	/*	end of forward overlap processing	*/
	
	/* backword overlap processing*/
    GetWUnit(basept+secmax[0]/* *2 */,2,wbuf);
    GetWUnit(basept+firstmax[0]/* *2 */,2,wtest);
    
	if (!strcmp(wbuf,wtest))        {
    	if (checkc('d','a'))                 /*  d^A + A --> d + A + A */
  			return(secmax[0]);

		if (WSum && checkc('n','v') && incate('p',GetCate(WSum-1))) /* p+n^V+V --> p+n+V+V */
			return(secmax[0]);
        
		if (incate('v',segc1[firstmax[0]]) && incate('v',segc2[1]))
            return(secmax[0]);
	}/*	end of backward overlap processing	*/
	
	/* other cases: ...*/

	return(firstmax[0]);
}//int     combambi()


int     specambiw()
{
	char    wbuf[50];
	int     i, WSum;

	WSum = GetWordSum();
	
	for (i=0;  i<firstmax[0]/* *2 */;  i++)       {
	  wbuf[i]=pThisSentence[basept+i];
	}
	
	wbuf[i++]='\0';
	switch  (incambil(wbuf))        
	{
		case  1 :             /*  r^n (TaYong)  */
				if ((WSum>0) && incate('v',GetWord(WSum-1)))
					return(firstmax[0]);
				else
					return(secmax[0]);
				break;

		case  2 :             /*  p^n  (BaShou)  */
				if (WSum && (mtincate("m-q",GetCate(WSum-1)) ||
					InWordList(GetWord(WSum-1),singr)))
						return(firstmax[0]);
				if (incate('v',segc3[firstmax[0]]))
					return(secmax[0]);
				break;

		case  3 :             /*  u^n (DeHua)  */
				if (incate('w',segc3[1]) || !pThisSentence[basept+4] || ((int)pThisSentence[basept+4]>0))
				/*  sentement ending */
					return(firstmax[0]);
				else
					return(secmax[0]);
				break;

		case  4 :             /* d^v (CaiNeng)  */
				if ((WSum>0) && mtincate("v-p",GetCate(WSum-1)))
					return(firstmax[0]);
				if (mtincate("v-p",segc3[firstmax[2]]))
					return(secmax[0]);
				break;

		case  5 :             /* d^v (JiangLai) */
				if (mtincate("f-s",segc3[firstmax[2]]))
					return(secmax[0]);
				break;

		case  6 :             /* p^f (WangHou) */
				if (mtincate("v",segc3[firstmax[2]]))
				return(secmax[0]);
				break;

		case  7 :             /* v^u (ChuanZhao,BuGuo,AnZhe) */
				if (mtincate("n-m",segc3[firstmax[2]]))
				return(secmax[0]);
				break;

		case  8 :             /* u^n (DeShou,...) */
				if ((WSum>0) && mtincate("v-a",GetCate(WSum-1)))
				return(secmax[0]);
				break;

		case  9 :             /* d^v (ZhenDian) */
				if (mtincate("u-m-n",segc3[firstmax[2]]))
				return(secmax[0]);
				break;

		case  10 :	           /* u^y (ZheNe)  */
				if (WSum && incate('v',GetCate(WSum-1)) && !incate('a',GetCate(WSum-1)))
				return(secmax[0]);
				break;

		case  11 :            /* v^u  (WanLiao)  */
				if ((WSum>0) && mtincate("d-v",GetCate(WSum-1)))
					return(secmax[0]);
				break;

		case  12 :            /* a^d  (HaoBu)  */
				GetWUnit(basept+firstmax[0]/* *2 */,firstmax[2],wbuf);
				if (!strcmp(wbuf,"好"))             /*  (HaoBu) + (Hao) --> (seg)  */
					return(secmax[0]);
				break;

		case  13 :            /* d^p  (ZhengZhai)  */
				if (!incate('v',segc3[firstmax[2]]))
					return(secmax[0]);
				break;

		case  14 :            /* v^n  (ChiSui)  */
				if (WSum && incate('d',GetCate(WSum-1)))
					return(secmax[0]);
				break;

		case	15 :			/*	p^n (DaoTou, DaoShou, DaoJia, ZaiJia	*/
	  			if (incate('f',segc3[firstmax[2]]))			/* p^n+f-->S	*/
	  				return(secmax[0]);
	  			break;

		case  16 :		/* m^q (Yitou, Yidao)	*/
	  			if (incate('n',segc3[firstmax[2]]))		/*	 m^q+n --> S	*/
	  				return (secmax[0]);
	  			if (incate('w',segc3[firstmax[2]]))		/*	 m^q+n -->S	*/
	  				return (secmax[0]);
	  			break;
	}//switch  (incambil(wbuf)) 
	
	return(firstmax[0]);
}


int     incambil(char  *key)
{
	int     i=0;
	while (strlen(cambil[i].word))    {
	  if (!strcmp(key,cambil[i].word)) 
		  return(cambil[i].ambinum);
	  i++;
	}

	return(0);
}


