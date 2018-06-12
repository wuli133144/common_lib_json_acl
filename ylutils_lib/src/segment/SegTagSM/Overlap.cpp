/****
 *      overlap.c :     overlap words process file
 *      purpose :       combine overlap status and form a new word .
 *      background      :       possible overlaping word list from dictionary .
 *              ..\maa.txt --- AA overlaping words list .
 *              ..\maabb.txt --- AABB overlaping words list .
 *              ..\a_abb.txt --- ABB overlaping words list .
 *              ..\aliab.txt --- ALiAB overlaping words list .
 ****/
//#include "stdafx.h"	//zanhy 2002-12-23


#include        <string.h>
#include        <stdio.h>
#include        <malloc.h>
#include        <stdlib.h>

#include		"Basicdat.h"
#include		"Commfunt.h"

#include		"Overlap.h"

/*	data files	*/
char*	MAAFileName, *MAABBFileName, *MABBFileName, *MALABFileName;


//
/////////  Internals  
//
//
int     lookup1(char  *, MLEX  *, int );
int     lookup2(char  *, char  *[], int );
int     MaaP(int);
int     MaabbP(int);
void    MabbP(int);
int     MaabP(int);
void    MaliabP(int);
int     MabuabP(int);
char*	MCombWord(int , int , char  *);


//
//////////////////exports functions
//
//
int    InitOverlWordList(void);               
void    ProcOverlap(int); 
void	FreeOverlapFNMem();  


/*      purpose :      read in overlap word list .
 */
int    InitOverlWordList()
{
	char	*DictPath, FullFileName[200], ErrorInfoStr[200]; 
	FILE    *maafp,*maabbfp,*mabbfp,*malabfp;  
	char	StrBuf[100];

	DictPath = GetDictDirectory();

	/*	set the full names of data list files	*/
	sprintf(FullFileName,"%smaa.txt",DictPath);
	if ((MAAFileName = (char *) malloc(strlen(FullFileName)+1))==NULL)	
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(MAAFileName,FullFileName);

	sprintf(FullFileName,"%smaabb.txt",DictPath);
	if ((MAABBFileName = (char *) malloc(strlen(FullFileName)+1))==NULL) 
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(MAABBFileName,FullFileName);

	sprintf(FullFileName,"%sa_abb.txt",DictPath);
	if ((MABBFileName = (char *) malloc(strlen(FullFileName)+1))==NULL) 
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(MABBFileName,FullFileName);

	sprintf(FullFileName,"%saliab.txt",DictPath);
	if ((MALABFileName = (char *) malloc(strlen(FullFileName)+1))==NULL)
	{
	   ErrorInfo("Can't alloc memory when setting the full names of data list files");
	   return -1; 
	}  
	strcpy(MALABFileName,FullFileName);

	if ((maafp=fopen(MAAFileName,"r"))==NULL)          {
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",MAAFileName);
      	ErrorInfo(ErrorInfoStr);		 return -1;
   	}
	if ((maabbfp=fopen(MAABBFileName,"r"))==NULL)	{
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",MAABBFileName);
      	ErrorInfo(ErrorInfoStr);		 return -1;
   	}          
	if ((mabbfp=fopen(MABBFileName,"r"))==NULL)		{
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",MABBFileName);
      	ErrorInfo(ErrorInfoStr);		 return -1;
   	}
	if ((malabfp=fopen(MALABFileName,"r"))==NULL)	{
    	sprintf(ErrorInfoStr,"Can't open file %s for reading",MALABFileName);
      	ErrorInfo(ErrorInfoStr);		return -1;
   	}

	maaid=maabbid=mabbid=maliabid=1;
	while (!feof(maafp))    {
		fscanf(maafp,"%s%s",maa[maaid].word,maa[maaid++].cate);
	  	if (maaid == MAANUM)	{
	  		ErrorInfo("Overflow in AA list !\n");
			break;
  		}
  	}
	
	while (!feof(maabbfp))    {
	  	fscanf(maabbfp,"%s%s",maabb[maabbid].word,maabb[maabbid++].cate);
		if (maabbid == MAABBNUM)	{
	  		ErrorInfo("Overflow in AABB list !\n");
			break;
  		}
  	}
	
	while (!feof(mabbfp))    {
	  	fscanf(mabbfp,"%s",StrBuf);
	  	if ((mabb[mabbid]=(char *) malloc(strlen(StrBuf)+1))==NULL)
			break;
	  	strcpy(mabb[mabbid++],StrBuf);
  	  	if (mabbid == MABBNUM)	{
	  		ErrorInfo("Overflow in ABB list !\n");
			break;
	  	}
	}                                      
	
	while (!feof(malabfp))    {
	  	fscanf(malabfp,"%s",StrBuf);
	  	if ((maliab[maliabid]=(char *) malloc(strlen(StrBuf)+1))==NULL)	
			break;
	  	strcpy(maliab[maliabid++],StrBuf);
	  	if (maliabid == MALIABNUM)	{
	  		ErrorInfo("Overflow in ALIAB list !\n");		
			break;
		}
	}

	fclose(maafp);
	fclose(maabbfp);
	fclose(mabbfp);
	fclose(malabfp);
	
	return 0;
}


/*      purpose :      identify and process overlaping status .
 *      argument :      id --- word's index in sentence .
 */
void    ProcOverlap(int  id)
{
	int     len,j;
	char    bword[20];

	if(strlen(GetWord(id))==2)  {
	  	if ((id+1<GetWordSum()) && !strcmp(GetWord(id),GetWord(id+1)))  {     /* A A */
	    		MaaP(id);
		}
	  	if ((id+2<GetWordSum()) && !strcmp(GetWord(id+1),"里") && 
			!strncmp(GetWord(id),GetWord(id+2),2) && (strlen(GetWord(id+2))==4))  {
	    		/* A (Li) AB  */
	    		MaliabP(id+2);
		}
	  	if ((id+2<GetWordSum()) && !strcmp(GetWord(id+1),"不") && 
			!strncmp(GetWord(id),GetWord(id+2),2) && (strlen(GetWord(id+2))==4))  {
	    		/* A (Bu) AB  */
	    		MabuabP(id+2);
		}
	  	if ((strlen(GetWord(id+1))==4) && 
			!strncmp(GetWord(id),GetWord(id+1),2))  {/*  A AB  */
	    		MaabP(id+1);
		}
	  	if  ((id>0) && (strlen(GetWord(id-1))==4))      {
			len=strlen(GetWord(id-1));
	      	for  (j=len-2;  j<=len;  j++)             /* get the ending chinese char */
				bword[j-len+2]=GetWord(id-1)[j];
	   		
			if (!strcmp(GetWord(id),bword))         {   /* AB B .or. A AB B */
				if ((id-2>=0) && !strncmp(GetWord(id-2),GetWord(id-1),2) && 
					(strlen(GetWord(id-2))==2))
	  					/* A AB B */
	  					MaabbP(id-1);
				else            /* AB B */
	  				MabbP(id-1);
			}
		}
	}//if(strlen(GetWord(id))==2)
}


/*
 *	purpose :	free the allocated memory used by the data file names	
 */
void	FreeOverlapFNMem()
{
	///bs: 97.7.2
	int i;
	for(i=0; i<MABBNUM; i++)
		if(mabb[i])
			free(mabb[i]);
	//
	for(i=0; i<MALIABNUM; i++)
		if(maliab[i])
			free(maliab[i]);
	///}}}

	free(MAAFileName);
	free(MAABBFileName);
	free(MABBFileName);
	free(MALABFileName);
}


//////////////////////////// internals ///////////////////////////////


/*      purpose :      lookup keyword in word list .
 *      argument :      key --- keyword .
 *                      mlist --- overlaping word list .
 *                      mlid --- words number of list .
 *      return  :       index of keyword in list --- if in .
 *                      0 --- if not in .
 */
int     lookup1(char  *key, MLEX  *mlist, int  mlid)
{
	int     low,high,mid,i;

	low=1;  high=mlid-1;
	while   (low<=high)     {
	  	mid=(low+high)/2;
	  	i=strcmp(key,mlist[mid].word);
	  	if  (i<0)     high=mid-1;
	  	if  (i>0)     low=mid+1;
	  	if  (i==0)    return(mid-1);
	}
	return(0);
}



/*      purpose :      lookup keyword in word list .
 *      argument :      key --- keyword .
 *                      mlist --- overlaping word list .
 *                      mlid --- words number of list .
 *      return  :       index of keyword in list --- if in .
 *                      0 --- if not in .
 */
int     lookup2(char  *key, char  *mlist[], int  mlid)
{
	int     low,high,mid,i;

	low=1;  high=mlid-1;
	while   (low<=high)     {
	  	mid=(low+high)/2;
	  	i=strcmp(key,mlist[mid]);
	  	if  (i<0)     high=mid-1;
	  	if  (i>0)     low=mid+1;
	  	if  (i==0)    return(mid-1);
	}
	return(0);
}


/*      purpose :      process AA overlap .
 *      argument :      id --- index of first A .
 */
int     MaaP(int  id)
{
	int     index;

	/*  process  A=v-q  status      */
	if (incate('q',GetCate(id)) && incate('v',GetCate(id)))     {
		if (incate('m',GetCate(id-1)) && strcmp(GetWord(id-1),"来"))          
		{
	    	/*  m && !(Lai) + A + A --> m + AA/q  */
	     	MCombWord(id,2,"q");
	     	return(1);
	    }
	  	MCombWord(id,2,"v");
	  	return(1);
	}
	
	/*  process  a-v + a-v + De  status  */
	if (incate('a',GetCate(id)) && incate('v',GetCate(id)) && 
		!strcmp(GetWord(id+2),"的")) {
	  		MCombWord(id,3,"z");
	  		return 1;
	}
	
	/*  process  A=v status           */
	if (incate('v',GetCate(id)) && !incate('y',GetCate(id)))      {
	  	MCombWord(id,2,"v");
	  	return 1;
	}
	
	/*  process  A=d  status        */
	if (incate('d',GetCate(id)))      {
	  	MCombWord(id,2,"d");
	  	return 1;
	}
	
	/*  process  A=m  status        */
	if (incate('m',GetCate(id)))      {
	  	MCombWord(id,2,"m");
	  	return 1;
	}
	
	/*  process  A=n  status        */
	if (incate('n',GetCate(id)))      {
	  	MCombWord(id,2,"n");
	  	return 1;
	}
	
	/*  process  A=a  status        */
	if (index=lookup1(GetWord(id),maa,maaid))   {
		if (!strcmp("a",maa[index].cate))     {

			if (!strcmp(GetWord(id+2),"的"))      {
	      		/*  a + a + (De) -> aa(De)/z  */
	      		MCombWord(id,3,"z");
	      		return 1;
	      	}
	    
			if (incate('v',GetCate(id+2)))    {
	      		/*  a + a + v --> aa/d + v  */
	      		MCombWord(id,2,"d");
	      		return 1;
	      	}
	    
			MCombWord(id,2,"z-d");
		}
	}

	return 0;
}


/*      purpose :      process AAB overlap .
 *      argument :      id --- index of word AB .
 */
int     MaabP(int   id)
{
	/*  process  VVO  status   */
	if (!strcmp("v",GetCate(id-1))  && !strcmp("v",GetCate(id)))        {
	  	MCombWord(id,2,"v");
	  	return(1);
	}
	return 0;
}


/*      purpose :      process AABB overlap .
 *      argument :      id --- index of word AB .
 */
int     MaabbP(int  id)
{
	int     index;

	/*    process special status    */
	if (!strcmp("n",GetCate(id)) && !strcmp("a",GetCate(id-1))) 
	{ /* (Da) + (DaXiao) + (Xiao) --> AABB/z */
	  	MCombWord(id-1,3,"z");
	  	return(0);
	}
	
	if (!strcmp("n",GetCate(id)))     {       /*  n-AABB  */
	  	MCombWord(id-1,3,"n");
	  	return(0);
	}
	
	if (!strcmp("m",GetCate(id)))     {       /*  m-AABB  */
	  	MCombWord(id-1,3,"m");
	  	return(0);
	}
	
	if (index=lookup1(GetWord(id),maabb,maabbid))   {
	  	if (!strcmp("v",maabb[index].cate))  
			MCombWord(id-1,3,"v");
	  	if (!strcmp("a",maabb[index].cate))   
			MCombWord(id-1,3,"z");
	}

	return 1;
}


/*      purpose :      process ABB overlap .
 *      argument :      id --- index of word AB .
 */
void    MabbP(int  id)
{
	if (lookup2(GetWord(id),mabb,mabbid))         {
	  	MCombWord(id,2,"z");
	}
}


/*      purpose :      process ALiBB overlap .
 *      argument :      id --- index of word AB .
 */
void    MaliabP(int  id)
{
	if (lookup2(GetWord(id),maliab,maliabid))         
		  MCombWord(id-2,3,"z");
}


/*      purpose :      process ABuAB overlap .
 *      argument :      id --- index of word AB .
 */
int     MabuabP(int  id)
{
	if (incate('v',GetCate(id)))              {
	  	MCombWord(id-2,3,"v");
	  	return 0;
	}
	if (incate('a',GetCate(id)))              {
	  	MCombWord(id-2,3,"a");
	  	return 0;
	}

	return 1;
}


char    *MCombWord(int  id, int  num, char  *cate)
{
	static char   wbuf[50], baseb[30]; ////return value

	strcpy(baseb,CombWord(id,num,cate));

	if ((id>0) && (id+num<GetWordSum()))    {
	  sprintf(wbuf,"%s/%s %s %s/%s",GetWord(id-1),GetCate(id-1),baseb,GetWord(id+num),GetCate(id+num));
	  return(wbuf);
	}
	
	if ((id==0) && (id+num<GetWordSum()))     {
	  sprintf(wbuf,"^ %s %s/%s",baseb,GetWord(id+num),GetCate(id+num));
	  return(wbuf);
	}
	
	if ((id>0) && (id+num==GetWordSum()))         {
	  sprintf(wbuf,"%s/%s %s $",GetWord(id-1),GetCate(id-1),baseb);
	  return(wbuf);
	}
	
	return(baseb);
}


