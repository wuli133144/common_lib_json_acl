/*
 *    GuessT.c :      routines for guessing the tags of unregistered words .
 */
//#include "stdafx.h"	//zanhy 2002-12-23


#include        <string.h>
#include        <stdlib.h>
 
extern  char*	POSset[];
extern  double  ConcurP(char *, char *);    /*  from SBTprep.c  */
extern  double	WtagP(char *, char *);      /*  from SBTprep.c  */


//
//// internals
//
char*	GuessTag(char  *word, char  *ftag, char  *btag);  /* used by SBTmodel.c */
char*   GetMaxFtag(char  *ftag);
char*	GetMaxBtag(char  *btag);




/*      function :   deduce undefined word's tag from forward and backword
 *                     concurrent tag.
 *      argument :   word ---- undefined word
 *                   ftag ---- forward concurrent tag
 *                   btag ---- backword concurrent tag
 *      return   :   deduced tags for undefined word
 */
char*	GuessTag(char  *word, char  *ftag, char  *btag)
{
	int     flag,stid,bufid,sumid,detid;
	char    tagbuf[12],stag[4],detag[4];
	static char sumtag[12]; ////for return value

	sumid=0;
	for (flag=0;  flag<=1;  flag++) {       /*  process ftag and btag  */
		if (!flag)       
			strcpy(tagbuf,ftag);
		else 
			strcpy(tagbuf,btag);
		
		if (!strlen(tagbuf))       
			break;
		bufid=stid=0;
	  
		while  ((size_t)bufid<strlen(tagbuf))  {
			stag[stid++]=tagbuf[bufid++];
			if ((tagbuf[bufid]=='\0') || (tagbuf[bufid]=='-'))  {
				stag[stid++]='\0';
				if (!flag)
					strcpy(detag,GetMaxFtag(stag));
				else
					strcpy(detag,GetMaxBtag(stag));
	      
				for (detid=0; (size_t)detid<strlen(detag);  detid++)   /* insert to return tags */
					sumtag[sumid++]=detag[detid];
	      
				sumtag[sumid++]='-';
			}
			stid=0;
		}           /*  end of while  */
	}             /*  end of for (flag=... */
	
	if (sumid) 
		sumtag[sumid-1]='\0';
	else 
		sumtag[0]='\0';

	return(sumtag);
}


char*	GetMaxFtag(char  *ftag)
{
	int     col,maxcol;
	double   maxp,bufp;

	col=maxcol=-1;
	maxp=0.0;

	while  (POSset[++col][0]!='^')   {
		bufp=ConcurP(ftag,POSset[col]);
		if (bufp>maxp) {
			maxp=bufp;
			maxcol=col;
	    }
	}
	if  (maxcol==-1) 
		return("\0");
	else 
		return(POSset[maxcol]);
}


char*	GetMaxBtag(char  *btag)
{
	int     row,maxrow;
	double   maxp,bufp;

	row=maxrow=-1;
	maxp=0.0;

	while  (POSset[++row][0]!='^')  {
		bufp=ConcurP(POSset[row],btag);
		if (bufp>maxp)   {
			maxp=bufp;
			maxrow=row;
	    }
	}
	
	if (maxrow==-1)
		return("\n");
	else 
		return(POSset[maxrow]);
}


