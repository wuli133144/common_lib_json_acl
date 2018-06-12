/****
 *      lexfix.c :      lexical_fix (prefix,suffix) process program
 *      purpose :       combine root and lexical fix to a word .
 ****/
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include		<malloc.h>

#include	"Basicdat.h"
#include	"Commfunt.h"
#include 	"Lexfix.h"

static char    *singlepre[]={"阿","大","老","小","\0"};
static char    *laowords[]={"一","二","三","四","五","六","七","八","九","\0"};
static char    *nksufl[]={"部","班","场","室","科","团","库","\0"};
static char    *vksufl[]={"学","费","计","节","团","化","论","品","\0"};
static char    *specsufl[]={"家","师","生","长","\0"};
static char    *ambisuf[]={"法","化","家","论","学","炎","头","\0"};

static char    *appelname[]=
		{"师傅","先生","小姐","老师","教授","大妈","大爷","奶奶",
		 "伯伯","大伯","阿姨","叔叔","大婶","\0"};	/*  appellation words list  */

static char    *namestr[]=
			{"王","李","刘","齐","汤","陈","扬","丁","严","方","赵","柴","古","吴",
			 "雷","孙","蓝","梁","曲","金","郑","史","周","仇","姜","蒋","贾","苗",
			 "武","许","徐","孔","韩","秦","蔡","卫","魏","肖","范","樊","余","朱",
			 "祖","梅","孟","戚","过","冯","罗","杜","乐","郝","葛","洪","汤","唐",
			 "屈","詹","柳","程","毕","聂","邓","晁","曹","廖","康","商","楚","季",
			 "纪","童","龚","容","张","章","穆","牟","莫","辛","邢","马","黄","胡",
			 "汪","杨","牛","俞","虞","陶","姚","段","郭","白","何","贺","林","关","\0"};

static char    *transverb[]=
			{"来","去","上","上来","上去","下","下来","下去",
			 "进","进来","进去","出","出来","出去","回","回来","回去","过",
			 "过来","过去","起","起来","开","开来","到","到来","到去","\0"};


/*	data files	*/
char	*PreFileName, *SufFileName, *SpecSufFileName;


//
///// internals
//
//
void    SetPrel(void);
void    SetSufl(void);
int		ReadSpecSufl(void);
int     PreSearch(char  *key);
int     SufSearch(char  *key);
int     PrefixP(int wlid,int pid);
int     SuffixP(int wlid,int  sid);
int     InRootWlist(char  * ,  char  *);
void    id_lexword(void);
char*	combw(int);
int     SurNameProc(int id);

/////  --- extern function called ---   
//
extern int  incate(char , char *);    // in stambi.c 


//
////// external functions
//
//
int    InitLexFixList(void);                  
void    LexFixProc(int  index); 
void	FreeLexDataFNMem();                


int   InitLexFixList()
{ 
	char	*DictPath, FullFileName[200]; 
	
	DictPath = GetDictDirectory();

	/*	set the full names of data list files	*/
	sprintf(FullFileName,"%spre.txt",DictPath);
	if ((PreFileName = (char *) malloc(strlen(FullFileName)+1))==NULL)	
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(PreFileName,FullFileName);

	sprintf(FullFileName,"%ssuf.txt",DictPath);
	if ((SufFileName = (char *) malloc(strlen(FullFileName)+1))==NULL)
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(SufFileName,FullFileName);

	sprintf(FullFileName,"%sspec.dat",DictPath);
	if ((SpecSufFileName = (char *) malloc(strlen(FullFileName)+1))==NULL)	
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(SpecSufFileName,FullFileName);
	
    preid=1;
    sufid=1;
	SetPrel();   			/* read the prefix data list	*/
    SetSufl();				/* read the suffix data list    */
	
	return ReadSpecSufl();			/* read the special suffix data list	*/
}


/*      function :      process No. (index) word in sentence .
 *      argument :      index --- word's index in sentence .
 */
void    LexFixProc(int  index)
{
	int     preindex,sufindex;

	/*  process single surname   */
	if (strlen(GetWord(index))==2)  {
	  	if (preindex=PreSearch(GetWord(index)))  {       /* is prefix word */
	      	PrefixP(index,preindex);
        }
	  	if (index && (sufindex=SufSearch(GetWord(index))))  {   /* is suffix word */
	      	SuffixP(index,sufindex);
        }
	}             /* end of if */
}


/*	function :	free the memory used by data file names	
*/
void	FreeLexDataFNMem()
{
	///bs: 99.7.2
	int i;
	//struct  rootr *next1, *next2;

	/*for(i=0; i<SPECSUFWNUM; i++) {
		if(specsl[i].rootwl) 
		{
			next1=specsl[i].rootwl;
			while(next1) {
				next2=next1->next;
				free(next1);
				next1=next2;
			}
		}
	}
	*/
	///}}}

	free(PreFileName);
	free(SufFileName);
	free(SpecSufFileName);
}


//////////////////	internals  //////////////////////////////

/*      function :      set prefix information  .
 */
void    SetPrel()
{
	FILE    *fp; 
    char	ErrorInfoStr[200];

    if ((fp=fopen(PreFileName,"r"))==NULL)      {
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",PreFileName);
      	ErrorInfo(ErrorInfoStr);		return;
   	}

    while (!feof(fp))  {
		fscanf(fp,"%s%s%s",prefix[preid].word,prefix[preid].combcate,
							prefix[preid].strucate);
        if (++preid==PREFNUM)	{
        	ErrorInfo(" *** Error : Overflow in prefix list !\n");
        	break;
        }
	}
	
	fclose(fp);
}


/*      function :      set suffix information list  .
 */
void    SetSufl()
{
	FILE    *fp;
    char	ErrorInfoStr[200];

    if ((fp=fopen(SufFileName,"r"))==NULL)      {
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",SufFileName);
      	ErrorInfo(ErrorInfoStr);		return;
	}
	
	while (!feof(fp))       {
    	fscanf(fp,"%s%s%s",suffix[sufid].word,suffix[sufid].combcate,
							suffix[sufid].strucate);
        if (++sufid==SUFFNUM)	{
        	ErrorInfo(" *** Error : Overflow in suffix list !\n");
        	break;
		}
	}
	
	fclose(fp);
}


/*      function :      read in special suffix list's information .
 */
int   ReadSpecSufl()
{
	FILE		*fp;
    char		ErrorInfoStr[200];
	char		buf[50];
	//ROOTREC		*rootbuf;

	speclid = 0;
	if ((fp=fopen(SpecSufFileName,"r"))==NULL)  {
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",SpecSufFileName);
      	ErrorInfo(ErrorInfoStr);		return -1;
	}
	
	while  (!feof(fp)) {
	  	fscanf(fp,"%s",specsl[speclid].spsufw);       /*  get suffix word  */
	  	specsl[speclid].counter=0;

	  	fscanf(fp,"%s",buf);
		while (!feof(fp) && strcmp(buf,"$"))     {
	    	//if ((rootbuf=(ROOTREC *) malloc(sizeof(ROOTREC)))==NULL) {
            //	fclose(fp);
			//	ErrorInfo("Can't alloc memory when reading in special suffix list's information");
	   		//	return -1; 
			//}  
	    	//strcpy(rootbuf->word,buf);
	    	//rootbuf->next=specsl[speclid].rootwl;
	    	//specsl[speclid].rootwl=rootbuf;		///points to the lastest one!!
	    	if(specsl[speclid].counter>=MAXROOTRECNUM){
            	fclose(fp);
				ErrorInfo("Can't alloc memory when reading in special suffix list's information");
	   			return -1; 
			}
			strcpy(specsl[speclid].word[specsl[speclid].counter],buf);
	    	specsl[speclid].counter++;		///points to the lastest one!!
			
	    	fscanf(fp,"%s",buf);
		}
	  	
		if (++speclid == SPECSUFWNUM)	{
	  		ErrorInfo("Overflow in special suffix data list!\n");
	  		break;
		}
	}//while  (!feof(fp))

	fclose(fp);
	return 0;
}


/*      function :      lookup prefix list and check whether keyword is in list .
 *      argument :      key --- kegword string .
 *      return  :       index of this keyword in list  --- if in .
 *                      0 --- if not .
 */
int     PreSearch(char  *key)
{
        int     low,high,mid,i;

        low=1;  high=preid-1;

        while(low<=high)     
		{
			mid=(low+high)/2;
			i=strcmp(key,prefix[mid].word);

			if  (i<0)
				high=mid-1;
			if  (i>0)
				low=mid+1;
			if  (i==0)
				return(mid);
        }       /* end od while */
        return(0);
}


/*      function :      lookup suffix list and check whether keyword is in list .
 *      argument :      key --- kegword string .
 *      return  :       index of this keyword in list  --- if in .
 *                      0 --- if not .
 */
int     SufSearch(char  *key)
{
        int     low,high,mid,i;

        low=1;  high=sufid-1;
        while   (low<=high)     
		{
			mid=(low+high)/2;
			i=strcmp(key,suffix[mid].word);
          
			if  (i<0) 
				high=mid-1;
			if  (i>0)
				low=mid+1;
			if  (i==0)
				return(mid);
        }       /* end od while */
        return(0);
}


/*      function :      combine prefix and root to a new words .
 *      argument :      wlid --- prefix word's index in sentence .
 *                      pid --- prefix word's index in prefix list .
 *      return  :       1 --- if not combine .
 *                      0 --- if combine .
 */
int     PrefixP(int  wlid,int  pid)
{
	if (!strcmp(GetWord(wlid),"大") || !strcmp(GetWord(wlid),"老") || !strcmp(GetWord(wlid),"小"))        
	{
	  /*  Lao | Xiao | Da + Surname ---> combine  */
		if ((strlen(GetWord(wlid+1))==2) && InWordList(GetWord(wlid+1),namestr))   {
			CombWord(wlid,2,"nr");
			return(0);
	    }
	}
	
	if (!strcmp(GetWord(wlid),"小"))      {        /*  Xiao  */
		if (!strcmp(GetWord(wlid+1),"伙子") || !strcmp(GetWord(wlid+1),"媳妇")) {
			/* Xiao + HuoZhi|XiFu --> (comb) */
			CombWord(wlid,2,"n");
			return(0);
	    }
	  
		if ((strlen(GetWord(wlid+1))==2) && !strcmp(GetCate(wlid+1),"d"))     {
			CombWord(wlid,2,"nr");
			return(0);
	    }
	  
		if ((strlen(GetWord(wlid+1))==2) && !strcmp(GetCate(wlid+1),"a") && 
			(strchr(GetCate(wlid+2),'n')==NULL)) {
			/*    Xiao + a + !n --> (Xiao a)/ng + !n  */
			CombWord(wlid,2,"nr");
			return(0);
	    }

		if (!strcmp(GetWord(wlid+1),"二") || !strcmp(GetWord(wlid+1),"三")) {
			/*   Xiao + (Er)|(Shan) ---> (comb)   */
			CombWord(wlid,2,"n");
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('h',GetCate(wlid)));
			return (1);
	}   /*  end of Xiao  */
	
	if (!strcmp(GetWord(wlid),"老")) {       /*  Lao   */
		/*  Lao + m + !q ---> combine  */
		if ((strlen(GetWord(wlid+1))==2) && InWordList(GetWord(wlid+1),laowords) 
			&& !incate('q',GetCate(wlid+2)))     {
				CombWord(wlid,2,"n");
				return(0);
		}
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('h',GetCate(wlid)));
		
		return (1);
	}             /*  end of Lao  */
	
	if (!strcmp(GetWord(wlid),"阿"))    {       /*  Ah  */
		if (strlen(GetWord(wlid+1))==2)         {
			CombWord(wlid,2,"nr");
			return(0);
		}
	  
		return (1);
	}             /*  end of A  */
	
	if (!strcmp(GetWord(wlid),"大"))  {       /*  Da  */
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('h',GetCate(wlid)));
		return (1);
	}             /*  end of Da  */
	
	///if ((strlen(GetWord(wlid+1))>=4) && strcmp(GetWord(wlid),"过") && strcmp(GetWord(wlid),"自"))
	if ((strlen(GetWord(wlid+1))>=4) && !strcmp(GetWord(wlid),"过") && !strcmp(GetWord(wlid),"自"))
		if (IsPossC(GetCate(wlid+1),prefix[pid].combcate))    
		{
			CombWord(wlid,2,prefix[pid].strucate);
			return (0);
		}
	//////////////////

	if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('h',GetCate(wlid)));
	
	return (1);
}


/*      function :      combine suffix and root to a new word .
 *      argument :      wlid --- suffix word's index in sentence .
 *                      pid --- suffix word's index in suffix list .
 */
int     SuffixP(int  wlid, int  sid)
{
	char    wbuf[3];
	int     wlen;

	/*  process special suffix  */
	if (InWordList(GetWord(wlid),specsufl))  {
		if (!strcmp(GetWord(wlid),"家"))   {       /*  Jia   */
			/*  get last chinese character of  root   */
			wlen=strlen(GetWord(wlid-1));
			wbuf[0]=GetWord(wlid-1)[wlen-2];
			wbuf[1]=GetWord(wlid-1)[wlen-1];
			wbuf[2]='\0';
	    
			if ((!strcmp(wbuf,"学")) && !strcmp("n",GetCate(wlid-1))) {
				/*  *(Xue) + (Jia) --> combine  */
				CombWord(wlid-1,2,"n");
				return(0);
			}
	    }           /*  end of Jia  */
	  
		if (InRootWlist(GetWord(wlid-1),GetWord(wlid)))   {
			CombWord(wlid-1,2,suffix[sid].strucate);
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
	  
		return (1);                   /*  not process  */
	}
	
	/*  process v-k suffix   */
	if (InWordList(GetWord(wlid),vksufl))      {
		if (incate('u',GetCate(wlid+1)) || InWordList(GetWord(wlid+1),transverb)) {
			if (strlen(GetCate(wlid))>1)
				strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
			return (1);                 /*  not process  */
	    }
	  
		if ((strlen(GetWord(wlid-1))>=4) && 
			IsPossC(GetCate(wlid-1),suffix[sid].combcate))  {
				CombWord(wlid-1,2,suffix[sid].strucate);
				return(0);
		}
	  
		if (!strcmp(GetWord(wlid),"化") && !strcmp(GetCate(wlid-1),"Vg"))	{	/*	Vg+化--- new */
	  		CombWord(wlid-1,2,"v");
	  		return 0;
	  	}
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
		
		return (1);
	}

	/*  process n-k suffix   */
	if (InWordList(GetWord(wlid),nksufl))         {
		if (incate('f',GetCate(wlid+1)))        {
			if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
			return 1;                   /*  not process  */
	    }
	  
		if ((strlen(GetWord(wlid-1))>=4) && IsPossC(GetCate(wlid-1),suffix[sid].combcate)) {
			CombWord(wlid-1,2,suffix[sid].strucate);
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
	  
		return (1);
	}
	
	/*  process other suffix  */
	if (!strcmp(GetWord(wlid),"机") || !strcmp(GetWord(wlid),"剂")) {       /*  Ji1 .or. Ji4  */
		/*  ZW 1997/5/23 BEGIN*/
		if ((strlen(GetWord(wlid-2))==2) && (strlen(GetWord(wlid-1))==2) && 
			(strcmp(GetCate(wlid-2),"w")!=0) && (strcmp(GetCate(wlid-1),"w")!=0)) {
		/*  ZW 1997/5/23 END*/
			CombWord(wlid-2,3,suffix[sid].strucate);
			return(0);
	    }
	}
	
	if (!strcmp(GetWord(wlid),"儿"))       {       /* Er */
		if (incate('q',GetCate(wlid-1)))          {
			CombWord(wlid-1,2,"q");
			return(0);
	    }
	  
		if (!strcmp(GetCate(wlid-1),"v"))       {
			CombWord(wlid-1,2,"v");
			return(0);
	    }
	  
		if (!strcmp(GetCate(wlid-1),"a") && (strlen(GetWord(wlid-1))>=4))  {
			CombWord(wlid-1,2,"a");
			return(0);
	    }
	  
		if (IsPossC(GetCate(wlid-1),suffix[sid].combcate))      {
			CombWord(wlid-1,2,"n");
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
	  
		return (1);           /*  not process  */
	}             /*  end of Er  */

	if (!strcmp(GetWord(wlid),"子"))      {           /*  Zi  */
		if (incate('q',GetCate(wlid-1)))          {
			CombWord(wlid-1,2,"q");
			return(0);
	    }
	  
		if (IsPossC(GetCate(wlid-1),suffix[sid].combcate))      {
			CombWord(wlid-1,2,"n");
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
	  
		return (1);
	}             /*  end of Zi  */
	
	if (!strcmp(GetWord(wlid),"头"))     {       /*  Tou  */
		if (incate('f',GetCate(wlid+1)))    {
			if (strlen(GetCate(wlid))>1)
				strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
			return 1;                   /*  not process  */
	    }
	  
		if (incate('f',GetCate(wlid-1)) && (strlen(GetWord(wlid-1))==2))  {
			/*  f + (Tou) --> f  */
			CombWord(wlid-1,2,"f");
			return(0);
	    }
	  
		if (IsPossC(GetCate(wlid-1),suffix[sid].combcate))       {
			CombWord(wlid-1,2,"n");
			return(0);
	    }
	  
		if (strlen(GetCate(wlid))>1)
			strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
			return (1);
	}             /* end of Tou  */
	
	if (IsPossC(GetCate(wlid-1),suffix[sid].combcate))  {
	    CombWord(wlid-1,2,suffix[sid].strucate);
	    return(0);
	}
	
	if (strlen(GetCate(wlid))>1)
		strcpy(GetCate(wlid),DeleTag('k',GetCate(wlid)));
	
	return (1);
}


/*      function :      check whether root is in root list .
 *      argument :      rword --- root word .
 *                      sufword --- suffix word .
 *      return  :       1 --- if in .
 *                      0 --- if not .
 */
int     InRootWlist(char  *rword,  char  *sufword)
{
	int     i,j;
	//ROOTREC         *rtest;
	char*	rtest=NULL;

	for  (i=0;  i<speclid;  i++)    {
		if (!strcmp(sufword,specsl[i].spsufw))   {
			rtest=specsl[i].word[0];
			j=0;
			break;
	    }
	}
	
	while  (rtest != NULL && j<MAXROOTRECNUM)  {
		if (!strcmp(rword,rtest))      
			return 1;
		rtest=specsl[i].word[j++];
	}

	return 0;
}


void    id_lexword()
{
        int     i,pid,sid;

        for (i=0;  i<GetWordSum();  i++) 
		{
			if (strlen(GetWord(i))==2)     
			{
				if (pid=PreSearch(GetWord(i))) 
				{
					//is combining single word prefix
					if (InWordList(prefix[pid].word,singlepre))
						if (strlen(GetWord(i+1))==2)
							combw(i);
						else
							if ((strlen(GetWord(i+1))>2) 
								&& (IsPossC(GetCate(i+1),prefix[pid].combcate) 
								|| (strlen(GetCate(i+1))==0)))
									combw(i);
				}
            
				if ((sid=SufSearch(GetWord(i))) && (i>0))
					if (IsPossC(GetCate(i-1),suffix[sid].combcate) || strlen(GetCate(i-1))==0)
						combw(i-1);
			}//if
		}//for
}


char*	combw(int  pos)
{
	static char    buf[100];

	if (pos+1<GetWordSum())
		sprintf(buf,"%s/%s %s/%s ", GetWord(pos), GetCate(pos),
									GetWord(pos+1), GetCate(pos+1));
	else
		sprintf(buf,"%s/%s %s/%s ", GetWord(pos-1), GetCate(pos-1),
									GetWord(pos), GetCate(pos));

	return(buf);
}


/*      function :      process single surname in sentence .
 *      argument :      id --- word's index in sentence .
 */
int     SurNameProc(int id)
{
	if ((strlen(GetWord(id))==2) && InWordList(GetWord(id),namestr))  
	{
		if (InWordList(GetWord(id+1),appelname))
			strcpy(GetCate(id),"nr");
	  
		if (!strcmp(GetWord(id+1),"总") || !strcmp(GetWord(id+1),"老"))
			CombWord(id,2,"nr");
	    
		return(0);
	}

	return 0;
}



