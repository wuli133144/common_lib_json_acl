/****
 *      RBTmodel.c :    process program of RBT(Rule-Based Tagging) model.
 *      purpose :       according context information , choice a suitable tag
 *						  from multiple tags .
 ****/
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <stdio.h>
#include        <string.h>

#include	"Basicdat.h"
#include	"Commfunt.h"




/*      --- data structure ---          */
#define AMBISTRNUM      30
typedef struct  
{
  int   start,end;
} AMBIPOS;

AMBIPOS ampos[AMBISTRNUM];

int     ampid;
char	buf[20];			/* buffer string for tags	*/

/************     --- information word list ---   *************/
char    *atestlist[]={"很","太","最","极","非常","挺","怪","至","这么","那么",
						"十分","特别","\0"}; /*  adjective word testing list  */
/*  quantitive word testing list:  */
char    *qtestlist[]={"这","那","哪","多少","每","此","各","某","几","\0"};

char    *haotestl[]={"表现","摔交","动","\0"};	/*  Hao4 testing list  */

char    *endflagl[]={";",".""?","!","；","？","！""。","\0"};	/*  sentence ending flag list*/

char    *nountestl[]={"这","那","这个","那个","这些","那些","哪些","\0"};/*  noun testing list */

char    *vppretestl[]={"为什么","怎么","如何","\0"};/*  preposition's pre_testing list  */

char    *vpposttestl[]={"成","得","不","\0"};	/*  preposition post_testing list   */

char    *specprepl[]={"给","叫","让","\0"};		/*  special preposition list  */

char	*ConjunctionWords[]={"和","与", "同", "跟", "或", "\0"};	/* conjuction word list  */

char	*VerbTestList[] = {"了","着","过","\0"};	/* verb testing list	*/

char	*HeVerbTestL[] = {"面","泥","\0"};		/*	testing word list for He	*/


/*		---- extern data structure ---	*/
extern  int	flags[];                        /*  defined in tagging.c */
extern  char    *transv[];                      /*  defined in stambi.c  */

/*       ---  macro purpose for set new tag in words list  ---         */
#define SetNewTag(index,ntag)   strcpy(GetCate(index),ntag);\
				flags[index]=0;\
				return(0)

//
/////////// internals
//
//
void	SetAmbipos(void);
void    SpecProcess(void);
void    bi_rule(int     index);
int     predisambi(int  i);
int     va_disambi(int  index);
int     vp_disambi(int  index);
int     vn_disambi(int  index);
int     vq_disambi(int  index);
int     rn_disambi(int  index);
int     vd_disambi(int  index);
int     da_disambi(int  index);
int		an_disambi(int  index);
int     WordDisambi(int  i);
int     ForCheckC(int  start, char  *dtag, char  *ptag);
int     BackCheckC(int  end, char  *ptag,  char  *dtag);
int     MInCate(char  *stag, char  *dtag);
void    tri_rule(int	index);
int     cmatch(int  pos, char  *forwc, char  *backc);
char    *intersect(char  *, char  *);
int     multy_rule(int  index);


//
//////////////// externals ////////////////////
//

/*
 *      purpose :      erase tagging ambiguity in sentence by rule
 */
void    Rule_Disambi()
{
	int     i, WSum;

	SpecProcess();                  /*  special ambiguity process  */
	WSum = GetWordSum();

	for (i=0;  i<WSum;  i++)         {
		if (strchr(GetCate(i),'-')!=NULL)
			WordDisambi(i);             /*  disambiguity according words  */
	}
	
	for (i=0;  i<WSum;  i++)         {
		predisambi(i);                /*  pre_process tagging ambiguity  */
	}
	
	SetAmbipos();                   /*  set ambiguous flag position  */
    
    for (i=0;  i<ampid;  i++)   {
		bi_rule(i);
		if (ampos[i].start<=ampos[i].end)  {  /* also have ambiguity */
			tri_rule(i);
			multy_rule(i);
		}
	}                       /* end of for */
}


//////////////////////////////// internals //////////////////////////////

/*
 *      purpose :      set  ambiguous position in sentence .
 */
void    SetAmbipos()
{
	int     i=0,j,k;
	int     wlen, WSum;

	WSum = GetWordSum();
	/*  reset ambiguous flag list  */
	for  (i=0;  i<WSum;  i++)        {
		wlen=strlen(GetCate(i));
	  
		if (!wlen || ((wlen>2) && (strchr(GetCate(i),'-') != NULL)))
			flags[i]=1;
		else
			flags[i]=0;
	}
	/*  set ambiguous position list  */
	ampid=0;        
	i=0;
	while   (i<WSum)     
	{
		while  ((!flags[i]) && (i<WSum))      
			i++;
		j=i;
	  
		while  ((flags[i]) && (i<WSum))    
			i++;
		k=i-1;
	  
		if (j<=k)     
		{       /* ambigruous string in Wj..Wk */
			ampos[ampid].start=j;
			ampos[ampid++].end=k;    
	    
			if (ampid==AMBISTRNUM)	
			{
	    		ErrorInfo(" Too many ambiguous words in a sentence.");
	    		return;
	    	}
	    }
	}
}


/*      fuuction :      process special tagging ambiguity  .
 */
void    SpecProcess()
{
	int     i, WSum;
	WSum = GetWordSum();

	for  (i=0;  i<WSum;  i++)        
	{
		/*  process Ng  */
		if (!strcmp(GetCate(i),"Ng"))    
		{
			if (i && MInCate("m",GetCate(i-1)) && MInCate("n",GetCate(i+1))) 
			{
				strcpy(GetCate(i),"q");
				goto end;
			}
	   
			if ((i+1==WSum) && MInCate("m",GetCate(i-1)))   
			{
				strcpy(GetCate(i),"q");
				goto end;
			}
		}
	 
		/*  process multy_tag words  */
		if (strlen(GetCate(i)) > 2)           
		{
			if  (MInCate("q",GetCate(i)))   
			{       /*  --- q^ ---  */
				if (i && InWordList(GetWord(i-1),qtestlist)&&!MInCate("p",GetCate(i)))  
				{
					strcpy(GetCate(i),"q");
					goto end ;
				}
	    
				if (i && MInCate("m",GetCate(i-1)) && strcmp(GetWord(i-1),"一个"))  
				{
					if (strlen(GetWord(i-1))<=4)        
					{
						strcpy(GetCate(i),"q");
						goto end ;
					}
				}
	    
				if ((i>1) && MInCate("a",GetCate(i-1)) && MInCate("m",GetCate(i-2)))    
				{
					strcpy(GetCate(i),"q");
					goto end;
				}
	    
				/*  delete special tag from tag set  */
				strcpy(GetCate(i),DeleTag('q',GetCate(i)));
		
				if (strlen(GetCate(i))<=1)
					goto  end ;
			}//if  (MInCate("q",GetCate(i)))
	  
			if (MInCate("a",GetCate(i)))         
			{       /*  --- a^ ---  */
				if (i && InWordList(GetWord(i-1),atestlist))      
				{
					strcpy(GetCate(i),"a");
					goto  end ;
				}
			}
		
			if (MInCate("b",GetCate(i)))         
			{       /*  --- b^ ---  */
				if (MInCate("n",GetCate(i+1)))       
				{
					strcpy(GetCate(i),"b");
					goto end ;
				}
				strcpy(GetCate(i),DeleTag('b',GetCate(i)));
				if (strlen(GetCate(i))<=1)
					goto  end ;
		    }
	  
			if (MInCate("v",GetCate(i)))         
			{ /**  --- v^ ---  **/
				/*  (Bu) + v^ --> (set) v  */
				if (i && !strcmp(GetWord(i-1),"不"))      
				{
					strcpy(GetCate(i),"v");
					goto end ;
				}
	    
				if (InWordList(GetWord(i+1),transv) && !MInCate("u-d",GetCate(i)))      
				{
					strcpy(GetCate(i),"v");
					goto end ;
				}
	    
				if (InWordList(GetWord(i+1),vpposttestl) && !MInCate("c",GetCate(i)))    
				{
					strcpy(GetCate(i),"v");
					goto end ;
				}
			}
		}//if (strlen(GetCate(i)) > 2) 

end : ;
	
	}//for  (i=0;  i<WSum;  i++)


}


int     predisambi(int  i)
{
	if ((flags[i]) && (strlen(GetCate(i))==3))  
	{   /* Bi_ambiguous tags */
		if (!strcmp(GetCate(i),"v-p") || !strcmp(GetCate(i),"p-v"))     
		{
			if (strcmp("打",GetWord(i)))        
				vp_disambi(i);
			return(0);
		}
	    if (!strcmp(GetCate(i),"v-a") || !strcmp(GetCate(i),"a-v"))     
		{
	      va_disambi(i);
	      return(0);
	    }
	    if (!strcmp(GetCate(i),"v-n") || !strcmp(GetCate(i),"n-v"))     
		{
	      vn_disambi(i);
	      return(0);
	    }
	    if (!strcmp(GetCate(i),"n-r") || !strcmp(GetCate(i),"r-n"))     
		{
	      rn_disambi(i);
	      return(0);
	    }
	    if (!strcmp(GetCate(i),"a-d") || !strcmp(GetCate(i),"d-a"))     
		{
	      da_disambi(i);
	      return(0);
	    }
	    if (!strcmp(GetCate(i),"v-d") || !strcmp(GetCate(i),"d-v"))     
		{
	      vd_disambi(i);
	      return(0);
	    }
	    if (!strcmp(GetCate(i),"a-n") || !strcmp(GetCate(i),"n-a"))     
		{
	      an_disambi(i);
	      return(0);
	    }
	}            /* end of if ((flags[i]... */

	return 1;
}


int     vp_disambi(int  index)
{
	int     i;

	/*  process special preposition & verb  */
	if (InWordList(GetWord(index),specprepl))      
	{
		if (index && MInCate("d",GetCate(index-1)))  
		{
			SetNewTag(index,"v");
	    }
		if (InWordList(GetWord(index+1),vppretestl))   
		{
			SetNewTag(index,"v");
	    }
		if (MInCate("v",GetCate(index-1)) && !strcmp(GetWord(index),"给"))     
		{
			/*  v + (Gei) --> (set) v  */
			SetNewTag(index,"v");
	    }
	}

	if (!strcmp("到",GetWord(index)))         {       /*  Dao   */
		for  (i=0;  i<index;  i++)
			if (!strcmp(GetWord(i),"从"))         {
				/*  (Chong) + ... + (Dao) --> (set) p  */
				SetNewTag(index,"p");
			}
			
			SetNewTag(index,"v");
	}
	
	if (InWordList(GetWord(index),transv))      
		return 0;

	if (InWordList(GetWord(index+1),vpposttestl))  
	  SetNewTag(index,"v");
	
	if (!strcmp("p",GetCate(index+1)))     
	  SetNewTag(index,"v");
	
	/*  p-v + ... + f|t --> (set) p (distance = 5) */
	for  (i=1;  i<5;  i++)
		if (MInCate("f-t",GetCate(index+i)))
			SetNewTag(index,"p");
	
	/*  v-a + v-p --> (set) p */
	if (index && MInCate("v-a",GetCate(index-1))) 
		SetNewTag(index,"p");
	
	/*  v-p + ... + v --> (set) p  */
	for (i=index+2; (i<GetWordSum()) && (strcmp(GetCate(i),"w")); i++)   
		if (MInCate("v",GetCate(i)) && !MInCate("u-n",GetCate(i)))  
			SetNewTag(index,"p");
	
	SetNewTag(index,"v");
}


int     va_disambi(int  index)
{
	/*  q + v-a + n --> (get) a  */
	if (MInCate("q",GetCate(index-1)) && MInCate("n",GetCate(index+1)))
	  SetNewTag(index,"a");

	/*  v + a-v + n|u|y --> (get) a  */
	if (!strcmp("v",GetCate(index-1)) && MInCate("u-n-y",GetCate(index+1))) 
	  SetNewTag(index,"a");

	if (MInCate("n-u-d",GetCate(index+1)))     
	  SetNewTag(index,"v");
}


int     vn_disambi(int index)
{
	if (InWordList(GetWord(index-1),nountestl)) 
	  SetNewTag(index,"n");

	if (index && !strcmp(GetWord(index-1),"把"))   /* Ba + * --> (get 'n') */
	  SetNewTag(index,"n");

	if ((index+1<GetWordSum()) && MInCate("f",GetCate(index+1)) 
		&& !MInCate("v",GetCate(index+1)))  
			SetNewTag(index,"n");

	if (index && !strcmp(GetWord(index-1),"地") && 
		!strcmp(GetCate(index-1),"u"))   /* Di/u + * --> (get 'v') */
			SetNewTag(index,"v");

	if (index && MInCate("m-b-a-q-u",GetCate(index-1)) && !MInCate("d",GetCate(index-1)))   
		SetNewTag(index,"n");

	if (MInCate("v",GetCate(index-1)) && (strlen(GetWord(index-1))>=4) 
		&& !MInCate("v",GetCate(index+1)))      
		  SetNewTag(index,"n");

	SetNewTag(index,"v");
}


int     rn_disambi(int  index)
{
	if ((index>0) && MInCate("q-a",GetCate(index-1)))   {
		SetNewTag(index,"n");
	}
	else
		SetNewTag(index,"r");
}


int     da_disambi(int  index)
{
	if (MInCate("a-v-p-d",GetCate(index+1)) && !MInCate("u-v",GetCate(index-1)))  
	  SetNewTag(index,"d");

	SetNewTag(index,"a");
}


int     vd_disambi(int  index)
{
	if (!strcmp("a",GetCate(index+1)))  
	  SetNewTag(index,"d");

	/*  v-d + (Bu) + v|a --> (set) v  */
	if (!strcmp("不",GetWord(index+1)) && MInCate("v-a",GetCate(index+2)))  
	  SetNewTag(index,"v");

	if (MInCate("v-d",GetCate(index+1)))  
	  SetNewTag(index,"d");

	SetNewTag(index,"v");
}


int	an_disambi(int	index)
{
	int	i;

	/*	的+...+*-->n	*/
	for (i=index; i && (i>index-4); i--)
		if (!strcmp(GetWord(i),"的") && !MInCate("n-r",GetCate(index+1)))
			SetNewTag(index,"n");
		
	if (index && MInCate("d",GetCate(index-1)))	
		strcpy(GetCate(index-1),"d");
		
	SetNewTag(index,"a");
}


int     WordDisambi(int  i)
{
	int     j, isech, WSum;

	WSum = GetWordSum();
	if (!strcmp(GetWord(i),"的"))                  /*  De/u-y  */
	  SetNewTag(i,"u");

	if (!strcmp(GetWord(i),"一"))     {             /*  Yi/d-m  */
		if (MInCate("a",GetCate(i+1)) && MInCate("q",GetCate(i+2)))    
			SetNewTag(i,"m");

		if (i && (!strcmp(GetWord(i),"这") || !strcmp(GetWord(i),"那")))   
	  		SetNewTag(i,"m");

	}
	
	if (!strcmp(GetWord(i),"一边"))   {           /*  YiBian/s-c  */
		for (isech=i+1;  isech<WSum;  isech++)  {
			if (!strcmp(GetWord(isech),"一边"))   {
				strcpy(GetCate(isech),"c");
				strcpy(GetCate(i),"c");
				return 0;
			}
		}
	  
		SetNewTag(i,"s");
	}
	
	if (!strcmp(GetWord(i),"了"))     
	{               /*  Le/v-u-y  */
		/*  (De) | (Bu) + (Le) ---> (set) v  */
		if (i && (!strcmp(GetWord(i-1),"得") || !strcmp(GetWord(i-1),"不")))      
			SetNewTag(i,"v");

		/*  (Le) + Ending flags --> (set) y  */
		if (InWordList(GetWord(i+1),endflagl))  
			SetNewTag(i,"y");

		if ((i+1==WSum) || !strcmp("y",GetCate(i+1)))  
	    SetNewTag(i,"y");

		/*  v|a + (Le) --> (set) u  */
		if (i && MInCate("v-a",GetCate(i-1)))  
			SetNewTag(i,"u");

	}             /*  end of Le   */
	
	if (!strcmp(GetWord(i),"过"))     
	{               /*  Guo/d-u-v  */
		if (!strcmp("a",GetWord(i+1)))  
			SetNewTag(i,"d");
		if (MInCate("v-a",GetCate(i-1)) && !MInCate("v",GetCate(i+1)))  
			SetNewTag(i,"u");

		if (strlen(GetCate(i))>1)
			SetNewTag(i,"v");
	}             /*  end of Guo  */
	
	if (!strcmp(GetWord(i),"着"))     {               /*  Zhao/n-u-v  */
		if (!strcmp("a",GetCate(i-1)) || !strcmp("m",GetCate(i-1)))    
			SetNewTag(i,"n");
		if (MInCate("v",GetCate(i-1)) && strcmp(GetWord(i+1),"了"))  
			SetNewTag(i,"u");
		if (strlen(GetCate(i))>1)
			SetNewTag(i,"v");
	}             /*  end of Zhao  */
	
	if (!strcmp(GetWord(i),"光"))     {               /*  Guang/v-a-n-d  */
		if (i && MInCate("a-b",GetCate(i-1)))   
			SetNewTag(i,"n");
		if (MInCate("a-v",GetCate(i+1)))   
			SetNewTag(i,"d");
		vn_disambi(i);
		goto  out1;
	}             /*  end of Guang  */
	
	if (!strcmp(GetWord(i),"在"))   
	{                 /*  Zhai/d-v-p  */
		for  (isech=i;  ((isech<WSum) && strcmp(GetCate(isech),"w"));  isech++)		
		{
			if (MInCate("f-t",GetCate(isech))) 
			{     		/*   在 .... f|t --> p	*/
				if (!strcmp(GetWord(isech),"上") || !strcmp(GetWord(isech),"下"))
					strcpy(GetCate(isech),"f");
	    		
				SetNewTag(i,"p");
			}
	    	if (!strcmp(GetWord(isech),"时") || !strcmp(GetWord(isech),"时候"))	
	    		SetNewTag(i,"p");
		}
	  
		if (MInCate("v-a",GetCate(i+1)))  
		{   /* ?+v|a-->(get) d */
			for (j=0; (j<5) && (i+2+j<WSum); j++)        /* backword lookup three words */
				if (!strcmp("的",GetWord(j)))         
					break;              /* De */
	    
				if ((j==5) || (i+2+j==WSum))   {
					SetNewTag(i,"d");
				}
			    else
					strcpy(GetCate(i),DeleTag('d',GetCate(i)));
	    }
	  
		vp_disambi(i);                              /* v-p process */
		goto out1;
	}          /* end of  Zhai */
	
	if (!strcmp(GetWord(i),"好"))   
	{                 /*  Hao/v-d-a  */
		if (InWordList(GetWord(i+1),haotestl))      
			SetNewTag(i,"v");
		if ((i<WSum) && (MInCate("a-m",GetCate(i+1))))     /* ?+a|m--> d */
			SetNewTag(i,"d");
		if (strlen(GetCate(i))>1)
			SetNewTag(i,"a");
	}                     /* end of Hao */
	
	if (!strcmp(GetWord(i),"就"))  
	{               /* Jiu/v-d-p-c */
		for  (isech=i-1;  isech>0;  isech--)    
		{
			/*  (Yi) + v + ... + (Jiu) ---> (set) c  */
			if (!strcmp(GetWord(isech),"一"))     
			{
				if (!strcmp(GetCate(isech+1),"v"))       
				{
					strcpy(GetCate(isech),"c");
					strcpy(GetCate(i),"d");
					return 0;
				}
			}
		}
	  
		if ((i<WSum) && (MInCate("v-a-b-m",GetCate(i+1))))     /* ?+v|a|m --> d */
			SetNewTag(i,"d");
		if (MInCate("n-u",GetCate(i+1)))      
			SetNewTag(i,"v");
	
	}      /* end of Jiu */
	
	if (!strcmp(GetWord(i),"得"))   
	{               /* De 3/u-v */
		if ((i<WSum) && MInCate("u",GetCate(i+1)) && MInCate("v",GetCate(i+1)))  
		{
			strcpy(GetCate(i+1),"v");
			flags[i+1]=0;
			SetNewTag(i,"u");
	    }
		if (i && MInCate("d",GetCate(i-1)))     
			SetNewTag(i,"v");
		if (i && MInCate("v-a",GetCate(i-1))) 
		    SetNewTag(i,"u");
		if (strlen(GetCate(i))>1)
			SetNewTag(i,"v");
	}                       /* end of De 3 */
	
	if (!strcmp(GetWord(i),"地"))     
	{            /*  Di/u-n  */
		if (i && MInCate("a-d-z",GetCate(i-1)))       
			SetNewTag(i,"u");
		if (i && MInCate("v-n",GetCate(i-1)) && MInCate("v-p-a",GetCate(i+1)))
	  		SetNewTag(i,"u");                   
	
		/*	  SetNewTag(i,"n");	*/
	}                   /* end of De 2 */
	
	if (!strcmp(GetWord(i),"用"))     
	{               /*  Yong/p-n-v  */
	  if (i && (!strcmp("v",GetCate(i-1)) || !strcmp("q",GetCate(i-1)))) 
	    SetNewTag(i,"n");
	  vp_disambi(i);
	}             /*  end of Yong  */
	
	if (!strcmp(GetWord(i),"来"))     {               /*  Lai/m-v */
		if (MInCate("m",GetCate(i-1)) && MInCate("q",GetCate(i+1))) 
			SetNewTag(i,"m");
		SetNewTag(i,"v");
	}
	
	if (!strcmp(GetWord(i),"打"))     
	{               /*  Da */
		if (i && MInCate("m",GetCate(i-1)) && (MInCate("n",GetCate(i+1)) || (i+1==WSum)))       
			SetNewTag(i,"q");

		for (j=i+2; ((j<WSum) && strcmp(GetCate(j),"w")); j++)   
		{
			if (!strcmp(GetWord(j),"往") || !strcmp(GetWord(j),"到") 
				|| !strcmp(GetWord(j),"起"))
				if (MInCate("n-f-s-t",GetCate(j-1))) 
					SetNewTag(i,"p");

		}        /* end of for */
	  
		if (strlen(GetCate(i))>1)
			SetNewTag(i,"v");
	}       /* end of Da */
	
	if (!strcmp(GetWord(i),"多"))     
	{                /* Duo (v-a-d-m) */
		if (i && MInCate("m-q",GetCate(i-1)))     
		    SetNewTag(i,"m");
		if (MInCate("a-p-v",GetCate(i+1)))  
			SetNewTag(i,"d");
		if (strlen(GetCate(i))>1)
			va_disambi(i);
	}       /* end of Duo */ 
	
	if (!strcmp(GetWord(i),"上"))	
	{			/* Shang(f-v)	*/
		if (i && MInCate("v",GetCate(i-1)) && !strcmp(GetWord(i+1),"的"))/*  v+*+的 --> f	*/
			SetNewTag(i,"f");
		if ((i-1) && !strcmp(GetWord(i-2),"的") && MInCate("v",GetCate(i-1)))/* 的+v+* --> f */
			SetNewTag(i,"f");
		if (i && MInCate("n",GetCate(i-1)) && (!strcmp(GetWord(i+1),"来")
			||!strcmp(GetWord(i+1),"去")))	/* n+*+来|去 --> f	*/
			SetNewTag(i,"f");
	}		/*	end of --Shang 	*/
	
	if (!strcmp(GetWord(i),"等"))	
	{		/* Deng(等) : d-u	*/
		if (MInCate("w",GetCate(i+1)))	/* *+w --> u 	*/
			SetNewTag(i,"u");
		
		for  (isech=i;  (isech && (strcmp(GetCate(isech),"w")
			||!strcmp(GetWord(isech),"、")));  isech--)		
		{  
			if (InWordList(GetWord(isech),ConjunctionWords) 
				|| !strcmp(GetWord(isech),"、"))	/* c|、+...+* --> u	*/
					SetNewTag(i,"u");
		}
	}	/*	end of -- Deng	*/
	
	if (!strcmp(GetWord(i),"和"))	
	{		/*	He(和) : v-p-c	*/
		if (InWordList(GetWord(i+1),transv) || InWordList(GetWord(i+1),VerbTestList))	
			SetNewTag(i,"v");
		if (InWordList(GetWord(i+1),HeVerbTestL))
			SetNewTag(i,"v");
		if (i && MInCate("d",GetCate(i-1)))
			SetNewTag(i,"p");
		for (isech=i; (isech<WSum) && strcmp(GetCate(isech),"w"); isech++)	
			if (!strcmp(GetWord(isech),"一起") || !strcmp(GetWord(isech),"一样"))
				SetNewTag(i,"p");

		SetNewTag(i,"c");
	} 		/*	end of  --He  	*/
	
	if (!strcmp(GetWord(i),"对"))	
	{		/*Dui(对) -- a-n-q-v	*/
		if (InWordList(GetWord(i+1),transv) || InWordList(GetWord(i+1),VerbTestList))
			SetNewTag(i,"v");
		SetNewTag(i,"p");
	}		/* end of --Dui   	*/
	
	if (!strcmp(GetWord(i),"中"))	
	{		/* Zhong(中) --- v-f	*/
		for (isech=i; isech && (strcmp(GetCate(isech),"w") 
			|| !strcmp(GetWord(isech),"、")); isech--)
			if (MInCate("p", GetCate(isech)))
				SetNewTag(i,"f");
	}
		
	out1 : ;
}//////int     WordDisambi(int  i)


void    bi_rule(int     index)
{
	int     beg,end;
	int     isech, WSum;

	WSum = GetWordSum();        
	beg=ampos[index].start;
	end=ampos[index].end;
    
    if  (beg<=end) 	 
	{
		if (ForCheckC(ampos[index].start,"m","q"))     {       /*  m + q^ --> (get) q */
			strcpy(GetCate(beg),"q");
            ampos[index].start++;
		}
		if (ForCheckC(ampos[index].start,"v-a","u"))     
		{       /*  v + u^ --> (get) u */
			if (!strcmp(GetWord(beg),"着") && !strcmp(GetWord(beg),"过"))   
			{
				/*  (Zhao)|(Guo) + ... + (Le) --> (get) v  */
				for  (isech=beg+2;  isech<WSum;  isech++)
					if (!strcmp(GetWord(isech),"了"))         
					{
						strcpy(GetCate(beg),"v");
						ampos[index].start++;
					}
			}
			if ((beg+1<WSum) && MInCate("u",GetCate(beg+1)))    
			{
				strcpy(GetCate(beg),"v");
				ampos[index].start++;
			}
			if ((beg+1==WSum) && strcmp(GetWord(beg),"了"))          
			{
				strcpy(GetCate(beg),"u");
				ampos[index].start++;
			}
		}
		if (ForCheckC(ampos[index].start,"v","v"))     {       /*  v + Tv^ --> (get) v */
			if (InWordList(GetWord(beg),transv) && strcmp(GetWord(beg),"过"))    {
				strcpy(GetCate(beg),"v");
				ampos[index].start++;
			}
		}
		if (ForCheckC(ampos[index].start,"n-s","f"))     {       /*  n|s + f^ --> (get) f */
			strcpy(GetCate(beg),"f");
            ampos[index].start++;
		}
		if (BackCheckC(ampos[index].end,"m","m-q-n-t"))    {     /*  m^ + m|q|n|t --> (get) m */
			strcpy(GetCate(end),"m");
            ampos[index].end--;
		}
		if (BackCheckC(ampos[index].end,"p","s"))      {       /*  p^ + s --> (get) p */
			strcpy(GetCate(end),"p");
            ampos[index].end--;
		}
		if (BackCheckC(ampos[index].end,"d","p-d-v-a")) {       /*  d^ + d|a|v --> (set) d  */
			strcpy(GetCate(end),"d");
			ampos[index].end--;
		}
	}//if  (beg<=end) 
}


int     ForCheckC(int  start, char  *dtag, char  *ptag)
{
	char    buftag[15];

	if (start==0)                   /*  at the begin of sentence */
		strcpy(buftag,"^");
	else
		strcpy(buftag,GetCate(start-1));

	if (MInCate(buftag,dtag) && MInCate(ptag,GetCate(start)))
		return(1);
	
	return(0);
}


int     BackCheckC(int  end, char  *ptag,  char  *dtag)
{
	char    buftag[15];

	if (end==GetWordSum()-1)                   /*  at the end of sentence */
		strcpy(buftag,"$");
	else
		strcpy(buftag,GetCate(end+1));
	
	if (MInCate(buftag,dtag) && MInCate(ptag,GetCate(end)))
		return(1);
        
	return(0);
}


int     MInCate(char  *stag,  char  *dtag)
{
	int     len,i;

	if (!strlen(dtag))             
		return 0;
	len=strlen(stag);

	if (len==1)     
	{
		if ((strstr(dtag,stag)!=NULL))     
			return(1);
		else         
			return(0);
	}
	else    
	{
		for (i=0;  i<len;  i++)
			if ((stag[i]!='-') && (strchr(dtag,stag[i])!=NULL))
				return(1);
		return(0);
	}
}


void    tri_rule(int	index)
{
	int     pos;

	if (ampos[index].start==ampos[index].end)       
	{
		pos=ampos[index].start;
		if (cmatch(pos,"v","n-r") && strlen(intersect(GetCate(pos),"p-c-u-q")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"p-u-c"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
            goto  out;
		}
	  
		if (cmatch(pos,"v-n-r","y-w") && strlen(intersect(GetCate(pos),"v-a")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"v-a"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
			goto  out;
	   }
	  
		if (cmatch(pos,"^-w-c","v") && strlen(intersect(GetCate(pos),"d")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"d"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
			goto  out;
	   }
	  
		if (cmatch(pos,"v","l-m-b-d") && strlen(intersect(GetCate(pos),"u")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"u"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
            goto  out;
		}
	  
		if (cmatch(pos,"r","n") && strlen(intersect(GetCate(pos),"p-q-a")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"p-q-a"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
	  
		if (cmatch(pos,"n-r","r") && strlen(intersect(GetCate(pos),"p-v")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"p-v"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
		
		if (cmatch(pos,"m","n") && strlen(intersect(GetCate(pos),"q-m")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"q-m"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
			goto  out;
	   }
	  
		if (cmatch(pos,"n-r","d") && strlen(intersect(GetCate(pos),"d")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"d"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
			goto  out;
	   }
	  
		if (cmatch(pos,"n-r-d","v") && strlen(intersect(GetCate(pos),"d-v")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"d-v"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
		
		if (cmatch(pos,"n-r","a") && strlen(intersect(GetCate(pos),"d-p")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"d-p"));
			if ((strlen(GetCate(pos))<=2))
				ampos[index].start++;
			goto  out;
	   }
	  
		if (cmatch(pos,"d","n-r") && strlen(intersect(GetCate(pos),"v-p")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"v-p"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
	  
		if (cmatch(pos,"d","$-w-y") && strlen(intersect(GetCate(pos),"v-a")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"v-a"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
	  
		if (cmatch(pos,"^","n-r") && strlen(intersect(GetCate(pos),"p-v")))       
		{
			strcpy(GetCate(pos),intersect(GetCate(pos),"p-v"));
			if ((strlen(GetCate(pos))<=2))
                ampos[index].start++;
            goto  out;
		}
	}//if (ampos[index].start==ampos[index].end) 
out : ;

}


int     cmatch(int  pos, char  *forwc, char  *backc)
{
	char    fbuf[15],bbuf[15];

	if (pos==0)                     /* at the begin of sentence */
		strcpy(fbuf,"^");
	else
		strcpy(fbuf,GetCate(pos-1));
        
	if (pos==GetWordSum()-1)                 /* at the end of sentence */
		strcpy(bbuf,"$");
	else
		strcpy(bbuf,GetCate(pos+1));
	
	if (MInCate(fbuf,forwc) && MInCate(bbuf,backc))
          return(1);
        
	return(0);
}


char    *intersect(char  *scate, char  *dcate)
/*      purpose :      get intersect of two tag set .
 *      argument :      scate --- source tag set .
 *                      dcate --- destination tag set .
 *      return  :       intersect result .
 */
{
	int     i,len,bp=0;
	len=strlen(dcate);
        
	for (i=0;  i<len;  i++)
		if  ((dcate[i]!='-') && (strchr(scate,dcate[i])!=NULL))   
		{
			buf[bp++]=dcate[i];
			buf[bp++]='-';
		}

	if (bp>0)       
		buf[bp-1]='\0';
    else
		strcpy(buf,"\0");
        
	return(buf);

}


int     multy_rule(int  index)
{
	int     pos;

	if (ampos[index].start<ampos[index].end)       {       /* multy undefind tag */
		for (pos=ampos[index].start; pos<ampos[index].end;  pos++)    
		{
			if (MInCate("m",GetCate(pos)) && MInCate("q",GetCate(pos+1))) 
			{
				strcpy(GetCate(pos),"m");
				strcpy(GetCate(pos+1),"q");
				flags[pos]=flags[pos+1]=0;
				return(0);
			}
			if (!strcmp("v-n",GetCate(pos)) && !strcmp("v-n",GetCate(pos+1)) 
				&& (strlen(GetWord(pos+1))==2))    
			{
				strcpy(GetCate(pos),"v");
				strcpy(GetCate(pos+1),"n");
				flags[pos]=flags[pos+1]=0;
				return(0);
			}
		}           /* end of for */
	}             /* end of if  */

	return 1;
}

