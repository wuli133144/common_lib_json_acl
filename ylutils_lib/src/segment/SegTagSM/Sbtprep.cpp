/***    SBTprep.c :     preprocessing program of SBT(Statistics-Based Tagging) model.
 *      purpose :       propose preprocessing function for SBT , include,
 *                              1. input statistical data .
 *                              2. calculate probabilities .
 *      sources :
 *              \ccmp\statres\*.stt --- statistical data .
 ***/
//#include "stdafx.h"	//zanhy 2002-12-23


#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include        <stdlib.h>

#include		"Basicdat.h"
#include		"Sbtprep.h"

/*      POS  set  defination    */
char    *POSset[]={"a","b","c","d","e","f","g","h","i","j","k","l","m",
					"n","o","p","q","r","s","t","u","v","w","x","y","z","^","$","\0"};

#define MINPOSSI     0.00000001         /*  minimal value for zero probability  */


///////////////// internals
//
int     SechWlist(char  *);
int     getindex(char *, char *[]);
void    ReadTword(void);
void    ReadMatrix(void);
void    SetConcurP(void);


//
////////////////////////////// exported :
//
void    InitSBTData(void);                      
double  ConcurP(char   *,  char  *);            /*  used by SBTModel.c  */
double  WtagP(char  *,  char  *);               /*  used by SBTModel.c  */

/*      purpose :      initial data for SBT
 */
void    InitSBTData()
{
	ReadTword();     /*  read data of Multi-tag words  */
	ReadMatrix();    /*  read data of Coocurrence Tag Matrix  */
	SetConcurP();    /*  set the values of the array of Coocurrence Probabilities  */
}


/*      purpose :      computer  tags' cooccurrent possibility.
 *                        P(Tj|Ti)=R(ti,tj)/R(ti)
 *      argument :      pair of cooccurrent tags  (Ti,Tj)
 *      output   :      possibility (double)
 */
double  ConcurP(char   *tagi,  char  *tagj)
{
	int     row,col,i;
	double   possi=0.0;
	char    rowt[4],colt[4];

	for (i=0; i<4; i++)     
		rowt[i]=colt[i]='\0';
	/*  process sub-category tags  */
	if (islower(tagi[0]))
		strncpy(rowt,tagi,1);
	else  strcpy(rowt,tagi);
	
	if (islower(tagj[0]))   
		strncpy(colt,tagj,1);
	else    strcpy(colt,tagj);
	
	row=getindex(rowt,POSset);
	col=getindex(colt,POSset);
	
	if ((row!=-1) && (col!=-1))     {
		possi=PConTag[row][col];
		if (possi==0)
			possi=MINPOSSI;
	  
		return(possi);
	}
	
	return(MINPOSSI);
}


/*      purpose :      computer  tag's  possibility in word.
 *                       P(W|T)=R(W,T)/R(T)
 *      argument :      pair of word_tag  (W,T)
 *      output   :      possibility (double)
 */
double   WtagP(char  *word,  char  *tag)
{
	int     wpos,cid,row;
	double   possi=0.0;

	if (wpos=SechWlist(word))
		for (cid=0;  strlen(twlist[wpos].stag[cid]) && (cid<5);  cid++)     {
			if (!strcmp(tag,twlist[wpos].stag[cid]))       {
				row=getindex(tag,POSset);
				if (RowTSum[row])
					possi=100.0*twlist[wpos].freq[cid]/RowTSum[row];
				if (possi==0) 
					possi=MINPOSSI;
	       
				return(possi);
	       }
		}     /* end of for (cid=... */
	
	return(1.0);
}


////////////////////////// internals ///////////////////////////

/*      purpose :      input word_tag list .
 */
void    ReadTword()
{
	FILE    *fp;
	int     mtnum=0,i,cid;
	char    wbuf[20], AmbiwtFile[200], ErrorInfoStr[200];

	sprintf(AmbiwtFile,"%sambiwt.stt",GetDictDirectory());	/*	get the full file name	*/

	if ((fp=fopen(AmbiwtFile,"r"))==NULL)          {
		sprintf(ErrorInfoStr,"*** File %s not exist ! \n",AmbiwtFile);
	  	ErrorInfo(ErrorInfoStr);		return;
	}

	twlid=1;                        /*  initialize index counter  */
	while  (!feof(fp))      {
	  /* create a new stem */
	  fscanf(fp,"%s",wbuf);           /*  skip filekey  */
	  fscanf(fp,"%s",twlist[twlid].keyword);
	  fscanf(fp,"%s",wbuf);
	  cid=0;
	  while (strcmp(wbuf,"#") && !feof(fp))  {      /* isn't a ending flag */
	    strcpy(twlist[twlid].stag[cid],wbuf);
	    fscanf(fp,"%d",&twlist[twlid].freq[cid++]);
	    fscanf(fp,"%s",wbuf);
	  }
	  for  (i=cid;  i<5;  i++)      {               /*  initialize other var  */
	    twlist[twlid].stag[i][0]='\0';
	    twlist[twlid].freq[i]=0;
	  }
	  twlid++;
	  if (twlid==MAXTWORD)  {
	    ErrorInfo("--- Too many record in word-tag list !\n");
	    break;
	  }
	}//while()

	fclose(fp);
}


/*      purpose :      input tag cooccurrent matrix .
 */
void    ReadMatrix()
{
	FILE    *fp;
	int     i,j;
	char    MatrixFile[200], ErrorInfoStr[200];

	sprintf(MatrixFile,"%smatrix.stt",GetDictDirectory());	/*	get the full file name	*/

	if ((fp=fopen(MatrixFile,"r"))==NULL)          {
		sprintf(ErrorInfoStr,"*** File %s not exist ! \n",MatrixFile);
	  	ErrorInfo(ErrorInfoStr);		
		return;
  	}
	for  (i=0;  i<TAGNUM;  i++)     {
	    RowTSum[i]=0;
	    for  (j=0;  j<TAGNUM;  j++)         {
	      	fscanf(fp,"%ld",&tagmat[i][j]);
	      	RowTSum[i]+=tagmat[i][j];
		}
    }
	fclose(fp);
}

/*      purpose :      set the values of the array of Coocurrence Probabilities .
 */
void    SetConcurP()
{
	int     row,col;
	int     zeronum[TAGNUM];                /*  number of zero frequence in every row  */
	double  subvalue;                       /*  value to subtract  */

	for (row=0; row<TAGNUM; row++)       
		zeronum[row]=0;

	/*  set values of probabilities  */
	for (row=0;  row<TAGNUM;  row++)
		for (col=0;  col<TAGNUM;  col++)    {
			if (!tagmat[row][col])                  /*  zero frequence  */
				zeronum[row]++;
			else  {
				if (RowTSum[row]) {
					PConTag[row][col]=1.0*tagmat[row][col]/RowTSum[row];
				}
			}
		}

	/*  process zero probabilities  */
	for (row=0;  row<TAGNUM;  row++)        {
		if ((zeronum[row]) && (RowTSum[row]>30))     {
			for  (col=0;  col<TAGNUM;  col++)   {
				if (!tagmat[row][col])
					PConTag[row][col]=1.0/RowTSum[row];
				else {
					subvalue=1.0*zeronum[row]/RowTSum[row];
					PConTag[row][col] -= subvalue*tagmat[row][col]/RowTSum[row];
				}
			}         /*  end of for (col ...  */
		}           /*  end of if ((...  */
	}             /*  end of for (row ...  */
}


/*      purpose :      search word in word list ,
 *               if /word/ in it , then return record position, else return 0
 *      argument :      word --- word for search
 */
int     SechWlist(char  *word)
{
	int     low,high,mid,i;

	low=1;  high=twlid-1;
	while   (low<=high)     
	{
	  mid=(low+high)/2;
	  i=strcmp(word,twlist[mid].keyword);
	  if  (i<0) 
		  high=mid-1;
	  if  (i>0) 
		  low=mid+1;
	  if  (i==0) 
		  return(mid);
	}       /* end od while */
	
	return(0);
}


int     getindex(char  *word, char  *wlist[])
{
	int     i;

	for  (i=0;  strlen(wlist[i]);  i++)    {
		if (!strcmp(word,wlist[i]))
			return(i);
	}
	
	return(-1);
}

