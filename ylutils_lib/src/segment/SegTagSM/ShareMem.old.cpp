//=====================================================
// 	Author	:	pb,20030703
//	Notes	:	share memory interface for SegTag
//	
//=====================================================
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

#include "ShareMem.h"


//=====================================================
//	public data share with share memory
//=====================================================
//SShareDataIndex* gIndex;
//SShareDataPtr	gPointers;
//char*			gShareData;

//
//global index data (exchange with gIndex)
//	
long			BasicDictIndex/*=0*/;   // index to BasicDict[] -- the "cursor"!
long			BasicWordSum; //totaol (actually*) words in the basic Dictionary

//for BasicDictIndex:
int		BasDictFCTabIndex; //index of the Basic Dict FC Table
int		BasDictFirstCharSum; //total First-Chars in BasDict

int     preid;
int     sufid;
int     speclid;

int     maaid;
int     maabbid;
int     mabbid;
int     maliabid;

int     twlid;                          /* index counter */

int		SpecDictNum;		//the actual number of Spec-dictionaries (actually read in)
int		SpecDictIndex/*=0*/;   // index to SpecDict[] -- the "cursor"!
int		SpDictFCTabIndex; //index of SpDict FC Table

//
//global pointers array(convert from gShareData)
//
BasDictUnit *	BasicDict[MAX_NumOfBasicDictWords];		//BasicWordSum
DictFirstCharRecord * BasicDictFirstCharTable[126][191];	//all
DictUnit *		SpecDict[MAX_SpecDictNum][MAX_NumOfASpecDictWords];	//SpecDictNum,SpecWordSum[]
char*	mabb[MABBNUM]/*[6]*/;  /* ABB list */		//mabbid
char*	maliab[MALIABNUM]/*[6]*/;  /* ALiAB list */	//maliabid

//
//global array(convert from gShareData)
//
LEXFIX  *prefix;	//[PREFNUM];		/* prefix word list  */
LEXFIX  *suffix;	//[SUFFNUM];		/* suffix word list  */
SPECSUFREC  *specsl;	//[SPECSUFWNUM];	/*  special suffix information list  */
MLEX    *maa;	//[MAANUM]; /* AA list */
MLEX    *maabb;	//[MAABBNUM]; /* AABB list */
long    int     (*tagmat)[TAGNUM];   //[TAGNUM][TAGNUM]      /* tag concurrent matrix define */
long    int     *RowTSum;            //[TAGNUM]    /* tag's sum in corpus file */
double  (*PConTag)[TAGNUM];	//[TAGNUM][TAGNUM];        /* array of coocurrence probabilties  */
TWREC   *twlist;	//[MAXTWORD];               /* list of trained words */
int		*SpecWordSum;	//[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
DictFirstCharRecord   (*SpecDictFirstCharTable)[MAX_NumOfSpecDictFirstChars];	//[MAX_SpecDictNum][MAX_NumOfSpecDictFirstChars]; 
int		*SpDictFirstCharSum;	//[MAX_SpecDictNum]; //total First-Chars in each SpDict

//=====================================================
//	public interface for share data 
//=====================================================
void	InitShareData()
{
	prefix 	= (LEXFIX  *)calloc(PREFNUM,sizeof(LEXFIX));		/* prefix word list  */
	suffix	= (LEXFIX  *)calloc(SUFFNUM,sizeof(LEXFIX));		/* suffix word list  */
	tagmat 	= (long int (*)[TAGNUM])calloc(TAGNUM,sizeof(long int[TAGNUM]));
	RowTSum = (long int*)calloc(TAGNUM,sizeof(long int));

	specsl 	=(SPECSUFREC  *)calloc(SPECSUFWNUM,sizeof(SPECSUFREC));	/*  special suffix information list  */
	maa 	=(MLEX *)calloc(MAANUM,sizeof(MLEX)); /* AA list */
	maabb	=(MLEX *)calloc(MAABBNUM,sizeof(MLEX)); /* AABB list */
	PConTag =(double  (*)[TAGNUM])calloc(TAGNUM,sizeof(double[TAGNUM]));        /* array of coocurrence probabilties  */
	twlist 	=(TWREC*)calloc(MAXTWORD,sizeof(TWREC));               /* list of trained words */
	SpecWordSum	=(int*)calloc(MAX_SpecDictNum,sizeof(int)); //total actual words in each Sp- Dictionary
	SpecDictFirstCharTable	=(DictFirstCharRecord   (*)[MAX_NumOfSpecDictFirstChars])calloc(MAX_SpecDictNum,sizeof(DictFirstCharRecord  [MAX_NumOfSpecDictFirstChars])); 
	SpDictFirstCharSum	=(int*)calloc(MAX_SpecDictNum,sizeof(int)); //total First-Chars in each SpDict

}


//
//save share data into data file
//
void	SaveShareData(char* fname)
{
	int i,j,len;
	FILE* fdata;
	SShareDataHead head;

	if(!(fdata = fopen(fname,"w+"))){
		printf("error open file as %s\n",fname);
		return;
	}

	//write head
	fwrite(&head,sizeof(SShareDataHead),1,fdata);
	len = 0;
	
	//write the Index data
	fwrite(&BasicDictIndex,sizeof(long),1,fdata);
	len += sizeof(long);
	fwrite(&BasicWordSum,sizeof(long),1,fdata);
	len += sizeof(long);
	fwrite(&BasDictFCTabIndex,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&BasDictFirstCharSum,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&preid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&sufid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&speclid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&maaid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&maabbid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&mabbid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&maliabid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&twlid,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&SpecDictNum,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&SpecDictIndex,sizeof(int),1,fdata);
	len += sizeof(int);
	fwrite(&SpDictFCTabIndex,sizeof(int),1,fdata);
	len += sizeof(int);
	
	//write array
	fwrite(prefix,sizeof(LEXFIX),preid,fdata);		/* prefix word list  */
	len += sizeof(LEXFIX)*preid;

	fwrite(suffix,sizeof(LEXFIX),sufid,fdata);
	len += sizeof(LEXFIX)*sufid;
	
	fwrite(specsl,sizeof(SPECSUFREC),speclid,fdata);
	len += sizeof(SPECSUFREC)*speclid;

	fwrite(maa,sizeof(MLEX),maaid,fdata);
	len += sizeof(MLEX)*maaid;
	
	fwrite(maabb,sizeof(MLEX),maabbid,fdata);
	len += sizeof(MLEX)*maabbid;
	
	fwrite(tagmat,sizeof(long int[TAGNUM]),TAGNUM,fdata);
	len += sizeof(long int[TAGNUM])*TAGNUM;
	
	fwrite(RowTSum,sizeof(long int),TAGNUM,fdata);
	len += sizeof(long int)*TAGNUM;
	
	fwrite(PConTag,sizeof(double[TAGNUM]),TAGNUM,fdata);
	len += sizeof(double[TAGNUM])*TAGNUM;
	
	fwrite(twlist,sizeof(TWREC),twlid,fdata);
	len += sizeof(TWREC)*twlid;
	

	if(SpecDictNum){
		fwrite(SpecWordSum,sizeof(int),SpecDictNum,fdata);
		len += sizeof(int)*SpecDictNum;
	
		fwrite(SpecDictFirstCharTable,sizeof(DictFirstCharRecord [MAX_NumOfSpecDictFirstChars]),SpecDictNum,fdata);
		len += sizeof(DictFirstCharRecord [MAX_NumOfSpecDictFirstChars])*SpecDictNum;

		fwrite(SpDictFirstCharSum,sizeof(int),SpecDictNum,fdata); 
		len += sizeof(int)*SpecDictNum;
	}
	
	//write pointer array
	for(i=0;i<BasicWordSum;i++){
		fwrite(BasicDict[i],sizeof(BasDictUnit),1,fdata);
		len += sizeof(BasDictUnit);
	}
	
	for(i=0;i<126;i++){
		for(j=0;j<191;j++){
			fwrite(BasicDictFirstCharTable[i][j],sizeof(DictFirstCharRecord),1,fdata);
			len += sizeof(DictFirstCharRecord);
		}
	}
	
	for(i=1;i<mabbid;i++){
		fwrite(mabb[i],sizeof(char),strlen(mabb[i])+1,fdata);
		len += strlen(mabb[i])+1;
	}
	
	for(i=1;i<maliabid;i++){
		fwrite(maliab[i],sizeof(char),strlen(maliab[i])+1,fdata);
		len += strlen(maliab[i])+1;
	}
	
	if(SpecDictNum){
		for(i=0;i<SpecDictNum;i++){
			for(j=0;j<SpecWordSum[i];j++){
				fwrite(SpecDict[i][j],sizeof(DictUnit),1,fdata);
				len += sizeof(DictUnit);
			}
		}
	}

	//retreat to write head
	fseek(fdata,0,SEEK_SET);
	memcpy(head.magic,"DICT",4);
	head.len = len;
	fwrite(&head,sizeof(head),1,fdata);

	//
	fclose(fdata);
}

//
//load share data into memory
//
void	LoadShareDataEx(char* pbuf)
{
	int i,j;
	
	//write the Index data
	BasicDictIndex	= *(long*)pbuf;
	pbuf += sizeof(long);
	BasicWordSum	= *(long*)pbuf;
	pbuf += sizeof(long);
	BasDictFCTabIndex	=	*(int*)pbuf;
	pbuf += sizeof(int);
	BasDictFirstCharSum   =   *(int*)pbuf;
	pbuf += sizeof(int);
	preid   =   *(int*)pbuf;
	pbuf += sizeof(int);
	sufid   =   *(int*)pbuf;
	pbuf += sizeof(int);
	speclid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	maaid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	maabbid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	mabbid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	maliabid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	twlid  =   *(int*)pbuf;
	pbuf += sizeof(int);
	SpecDictNum  =   *(int*)pbuf;
	pbuf += sizeof(int);
	SpecDictIndex  =   *(int*)pbuf;
	pbuf += sizeof(int);
	SpDictFCTabIndex  =   *(int*)pbuf;
	pbuf += sizeof(int);
	
	//write array
	prefix = (LEXFIX*)pbuf;
	pbuf += sizeof(LEXFIX)*preid;

	suffix =(LEXFIX*)pbuf;
	pbuf += sizeof(LEXFIX)*sufid;
	
	specsl =(SPECSUFREC*)pbuf;
	pbuf += sizeof(SPECSUFREC)*speclid;

	maa =(MLEX*)pbuf;
	pbuf += sizeof(MLEX)*maaid;
	
	maabb =(MLEX*)pbuf;
	pbuf += sizeof(MLEX)*maabbid;
	
	tagmat = (long int(*)[TAGNUM])pbuf;
	pbuf += sizeof(long int[TAGNUM])*TAGNUM;
	
	RowTSum = (long int*)pbuf;
	pbuf += sizeof(long int)*TAGNUM;
	
	PConTag =(double(*)[TAGNUM])pbuf;
	pbuf += sizeof(double[TAGNUM])*TAGNUM;
	
	twlist =(TWREC*)pbuf;
	pbuf += sizeof(TWREC)*twlid;
	

	if(SpecDictNum){
		SpecWordSum =(int*)pbuf;
		pbuf += sizeof(int)*SpecDictNum;
	
		SpecDictFirstCharTable = (DictFirstCharRecord (*)[MAX_NumOfSpecDictFirstChars])pbuf;
		pbuf += sizeof(DictFirstCharRecord [MAX_NumOfSpecDictFirstChars])*SpecDictNum;

		SpDictFirstCharSum =(int*)pbuf; 
		pbuf += sizeof(int)*SpecDictNum;
	}
	
	//write pointer array
	for(i=0;i<BasicWordSum;i++){
		BasicDict[i] = (BasDictUnit*)pbuf;
		pbuf += sizeof(BasDictUnit);
	}
	
	for(i=0;i<126;i++){
		for(j=0;j<191;j++){
			BasicDictFirstCharTable[i][j] = (DictFirstCharRecord*)pbuf;
			pbuf += sizeof(DictFirstCharRecord);
		}
	}
	
	mabb[0] =NULL;
	for(i=1;i<mabbid;i++){
		mabb[i] =(char*)pbuf;
		pbuf += strlen(mabb[i])+1;
	}
	
	maliab[0] =NULL;
	for(i=1;i<maliabid;i++){
		maliab[i] =pbuf;
		pbuf += strlen(maliab[i])+1;
	}
	
	if(SpecDictNum){
		for(i=0;i<SpecDictNum;i++){
			for(j=0;j<SpecWordSum[i];j++){
				SpecDict[i][j] =(DictUnit*)pbuf;
				pbuf += sizeof(DictUnit);
			}
		}
	}
}


void	LoadShareData(char* fname)
{
	char *pbuf;
	FILE* fdata;
	SShareDataHead head;

	if(!(fdata = fopen(fname,"r"))){
		printf("error open file as %s\n",fname);
		return;
	}

	//read head
	fread(&head,sizeof(head),1,fdata);
	if(memcmp(head.magic,"DICT",4)){
		printf("share data file format error\n");
		return;
	}
	
	pbuf = (char*)calloc(1,head.len);
	fread(pbuf,1,head.len,fdata);
	fclose(fdata);

	//exchange the data
	LoadShareDataEx(pbuf);
}

//
// read ShareData into ShareMem
//
int	InitShareMem(char* fname)
{
	key_t shm_key;
	int	shmid;
	char *pbuf;
	FILE* fdata;
	SShareDataHead head;

	if(!(fdata = fopen(fname,"r"))){
		printf("error open file as %s\n",fname);
		return -1;
	}

	//read head
	fread(&head,sizeof(head),1,fdata);
	if(memcmp(head.magic,"DICT",4)){
		printf("share data file format error\n");
		return -1;
	}
	
	//create sharemem
	shm_key = ftok(fname,'D');
    if( (shmid=shmget(shm_key, head.len, 0666|IPC_CREAT))<0){
       	printf("Can't alloc shared memory : %d\n",head.len);
		return -1;
	}
    if( (pbuf= (char *)shmat(shmid, (char *)0, 0)) == (char *) -1){
       	printf("can't attach the shared memory\n");
		return -1;
	}
	fread(pbuf,1,head.len,fdata);
	fclose(fdata);
	return 0;
}

//
// mount ShareMem and init ShareData
//
int	MountShareMem(char* fname)
{
	key_t shm_key;
	int	shmid;
	char *pbuf;

	//attach sharemem
	shm_key = ftok(fname,'D');
    if( (shmid=shmget(shm_key, 0, 0666))<0){
       	//printf("Can't get shared memory\n");
		return -1;
	}
    if( (pbuf= (char *)shmat(shmid, (char *)0, 0)) == (char *) -1){
       	printf("can't attach the shared memory\n");
		return -1;
	}
	
	//init ShareData
	LoadShareDataEx(pbuf);
	return 0;
}

//
// unload the ShareMem
//
int	UnloadShareMem(char* fname)
{
	key_t shm_key;
	int	shmid;

	//attach sharemem
	shm_key = ftok(fname,'D');
    	if( (shmid=shmget(shm_key, 0, 0666))<0){
       		printf("Can't get shared memory\n");
		return -1;	
	}
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}

//
//convert a string(buffer)
//	output to buffer dest or stdout(with cate)
//
enum WORDSTATE{
	NONE,
	EWORD,
	NUMBER,
	HZSTRING
};
#define MAXHZSTRLEN (512-2)	//SENMAXLEN-2
#define MAXEWORDLEN  30
#define MAXNUMBERLEN 5
#include "encode.h"
extern STUnit SegTagUnits[MAX_NumOfWordsPerSentence];
int SegTagString(char* buf,char* dest,char* reservstr)
{
	WORDSTATE curstate;
	int     i,len,wsum,curWordNumb=0;
	unsigned char *p=(unsigned char*)buf;
	unsigned char* pword,*psav,*pend,savch;
	int		midcnt,tailcnt,valid;
	
	//init the state
	curstate = NONE;
	pword = NULL;
	midcnt = tailcnt = 0;
	wsum = 0;
	pend = p+strlen((char*)p);
	
	if(dest) dest[0]=0;
	
	//it's imporant to test the last \0 inside the loop
	while(p<=pend){
		valid = 0;
		switch(curstate){
		case NONE:
			if(IS_GB_CODE(p)){	//*p>0x80
				//should be HZSTRING
				curstate = HZSTRING;
			}
			else if(isdigit(*p)){   //(*p>='0' && *p<='9'){
				curstate = NUMBER;
			}
			else if(isalpha(*p)){
				curstate = EWORD;
			}
			else{
				//discard this character???
				if(strchr(reservstr,*p)){
					if(dest){
						dest[0] = *p;
						dest[1] = ' ';
						dest[2] = 0;
						dest +=2;	
					}
					else{
						printf("%c/x ",*p);
					}
				}
			}
			
			if(curstate!=NONE){
				len = 1;
				pword = p;
				midcnt = tailcnt = 0;
			}
			
			if(IS_GB(p)){
				 len++;
				 p+=2;
			}
			else p++;
			break;
		case EWORD:
			if(len < MAXEWORDLEN){
				if(!tailcnt && isalpha(*p)){
					valid = 1;
				}
				else if(!tailcnt && !midcnt && (*p=='-' || *p=='&')){
					if(isalpha(p[1])){
						midcnt = 1;
						valid = 1;
					}
				}
				else if(*p=='+' || *p=='#'){
					
					tailcnt++;
					valid = 1;
				}
			}
			
			if(valid){
				len ++;
				p++;
			}
			else{
				//break here
				psav = p;
				savch = *p;
				*p = 0;
				
				if(dest){
					strcat(dest,(char*)pword);
					strcat(dest," ");
					dest += strlen(dest);
				}
				else{
					printf("%s/x ",pword);
				}
				wsum++;
				

				//restore savch
				*psav = savch;
				curstate = NONE;
			}
			break;
		case NUMBER:
			if(len < MAXNUMBERLEN){
				if(isdigit(*p)){
					valid = 1;
				}
				else if(!midcnt && (*p=='.' )){
					if(isdigit(p[1])){
						midcnt = 1;
						valid = 1;
					}
				}
			}
			
			if(valid){
				len ++;
				p++;
			}
			else{
				//break here
				psav = p;
				savch = *p;
				*p = 0;
					
				if(dest){
					strcat(dest,(char*)pword);
					strcat(dest," ");
					dest += strlen(dest);
				}
				else	printf("%s/m ",pword);

				wsum++;
				//restore savch
				*psav = savch;
				curstate = NONE;
			}
			break;
		
			break;
		case HZSTRING:
			if(len >= MAXHZSTRLEN || !IS_GB_CODE(p)){
				//found a sentence
				psav = p;
				savch = *p;
				*p = 0;
			
				//call chinese segmentation 
				if(curWordNumb=SegTagASentence((char*)pword)){
					for(i = 0; i < curWordNumb; i++){
						if(dest){
							strcat(dest,SegTagUnits[i].Word);
							strcat(dest," ");
							dest += strlen(dest);
						}
						else printf("%s/%s ", SegTagUnits[i].Word,SegTagUnits[i].Cate);
						
						wsum++;
					}
				}

				//restore savch
				*psav = savch;
				curstate = NONE;
				
			}
			else{
				len+=2;
				p+=2;
			}
			break;
		}
	}
	
	return wsum;
}

