//========================================================================================
// 	Author	:	pb,20030703
//	Notes	:	share memory interface for SegTag
//	
//	History	:
//
//		0311,pb,add Chinese Name Entiry recongnizer(use a simple probability model)
//			-->(Copyright by ICL/bswen,2003)<--
//
//========================================================================================
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
#define DICTPERM 0444

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
int		*SpecWordSum;//[MAX_SpecDictNum];	//[MAX_SpecDictNum]; //total actual words in each Sp- Dictionary
DictFirstCharRecord   (*SpecDictFirstCharTable)[MAX_NumOfSpecDictFirstChars];	//[MAX_SpecDictNum][MAX_NumOfSpecDictFirstChars]; 
int		*SpDictFirstCharSum;	//[MAX_SpecDictNum]; //total First-Chars in each SpDict

#ifdef _USE_CHINESE_NE_
float	*psurname,*pmidname,*ptailname;
#endif

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

#ifdef _USE_CHINESE_NE_
	psurname = (float*)calloc(CHARARRAYSIZE,sizeof(float));
	pmidname = (float*)calloc(CHARARRAYSIZE,sizeof(float));
	ptailname = (float*)calloc(CHARARRAYSIZE,sizeof(float));

	//read the array
	char fname[256];
	FILE* fin;
	unsigned char word[4];
	float val;
	
	if(fin = fopen("lexicons/surname.txt","r")){
		printf("load from surname.txt....\n");
		while(fscanf(fin,"%s %f\n",word,&val)==2){
			if(word[0]>0x80 && word[1]>0x80){
				psurname[(word[0]-0x80)*128+word[1]-0x80] = val;
			}
		}
		fclose(fin);
	}
	
	if(fin = fopen("lexicons/midname.txt","r")){
		printf("load from midname.txt....\n");
		while(fscanf(fin,"%s %f\n",word,&val)==2){
			if(word[0]>0x80 && word[1]>0x80){
				pmidname[(word[0]-0x80)*128+word[1]-0x80] = val;
			}
		}
		fclose(fin);
	}

	if(fin = fopen("lexicons/tailname.txt","r")){
		printf("load from tailname.txt....\n");
		while(fscanf(fin,"%s %f\n",word,&val)==2){
			if(word[0]>0x80 && word[1]>0x80){
				ptailname[(word[0]-0x80)*128+word[1]-0x80] = val;
			}
		}
		fclose(fin);
	}
	
#endif

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
	
	
#ifdef _USE_CHINESE_NE_
	fwrite(psurname,CHARARRAYSIZE,sizeof(float),fdata);
	fwrite(pmidname,CHARARRAYSIZE,sizeof(float),fdata);
	fwrite(ptailname,CHARARRAYSIZE,sizeof(float),fdata);
	len += CHARARRAYSIZE*3*sizeof(float);
#endif

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

#ifdef _USE_CHINESE_NE_
	psurname = (float*)pbuf;
	pbuf +=CHARARRAYSIZE*sizeof(float);
	pmidname = (float*)pbuf;
	pbuf +=CHARARRAYSIZE*sizeof(float);
	ptailname = (float*)pbuf;
	pbuf +=CHARARRAYSIZE*sizeof(float);
#endif
}

bool ValidDictData(const char *pbuf, int len)
{
	if (len < sizeof(SShareDataHead))
		return false;

	SShareDataHead *phead = (SShareDataHead *)pbuf;
	
	if (memcmp(pbuf, "DICT", 4) != 0)
		return false;
	
	if (len < (phead->len+sizeof(SShareDataHead)))
		return false;

	return true;
}

int	LoadShareData(char* fname)
{
	/*
	char *pbuf;
	FILE* fdata;
	SShareDataHead head;

	if(!(fdata = fopen(fname,"r"))){
		//zmh comment it, the msg will output into my logfile.
		//printf("error open file as %s\n",fname);
		return -1;		//! file error. zmh
	}

	//read head
	fread(&head,sizeof(head),1,fdata);
	if(memcmp(head.magic,"DICT",4)){
		//printf("share data file format error\n");
		return 0;		//! format error. zmh
	}
	
	pbuf = (char*)calloc(1,head.len);
	fread(pbuf,1,head.len,fdata);
	fclose(fdata);

	//exchange the data
	LoadShareDataEx(pbuf);
	*/

	//! 上面的代码没有太多的错误检查, 重写.
	
	char *pbuf = NULL;
	int fdict;
	SShareDataHead head;

	if ((fdict=open(fname, O_RDONLY)) == -1)
	{
		return -2;		//!< 打开文件错误, 可能不存在或者权限不够.
	}

	if ((read(fdict, (char *)&head, sizeof(head)) != sizeof(head))
			|| (memcmp(head.magic, "DICT", 4) != 0))
	{
		close(fdict);
		return -1;		//!< 文件长度不够或者内容标识不对, 格式不对.
	}

	pbuf = (char *)malloc(head.len);

	if (read(fdict, pbuf, head.len) != head.len)
	{
		close(fdict);
		return -1;		//!< 文件内容长度与文件头不符, 格式不对.
	}

	close(fdict);

	LoadShareDataEx(pbuf);

	return 0;
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
    if( (shmid=shmget(shm_key, 0, DICTPERM))<0){
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
    	if( (shmid=shmget(shm_key, 0, DICTPERM))<0){
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
int SegTagString(const char* buf,char* dest,const char* reservstr)
{
	WORDSTATE curstate;
	int     i,len,wsum,curWordNumb=0;
	unsigned char *p=(unsigned char*)buf;
	unsigned char* pword,*psav,*pend,savch;
	int		midcnt,tailcnt,valid;
	int		maxnumberlen;
	
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
				maxnumberlen = 0;
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
				if(!tailcnt && (isalpha(*p) || isdigit(*p))){
					if(isdigit(*p)) maxnumberlen++;
					if(maxnumberlen<5) valid = 1;
				}
				else if(!tailcnt && !midcnt && (*p=='-' || *p=='&')){
					if(isalpha(p[1]) || isdigit(p[1])){
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
					
					#ifdef _USE_CHINESE_NE_
						//check Chinese Name entity
						float surprob,midprob,tailprob,prob2,prob;
						int	suridx,mididx,tailidx;
						int nelen = 1,k;
						
						//if two single hz at least there
						if(i<curWordNumb-1 && !SegTagUnits[i].Word[2] && !SegTagUnits[i+1].Word[2]){
							suridx = _GET_IDX(SegTagUnits[i].Word);
							//first is surname
							if(_IS_VALID(SegTagUnits[i].Word) && (surprob=psurname[suridx])>0.){
								if(i<curWordNumb-2 && !SegTagUnits[i+2].Word[2]){
									//3 single hz
									mididx = _GET_IDX(SegTagUnits[i+1].Word);
									if(_IS_VALID(SegTagUnits[i+1].Word)){
										midprob = pmidname[mididx];
										
										prob2 = 1000000. * surprob * 0.001f * ptailname[mididx];
									
										tailidx = _GET_IDX(SegTagUnits[i+2].Word);
										if(_IS_VALID(SegTagUnits[i+2].Word)){
											tailprob = ptailname[tailidx];
											
											prob = 1000000. * surprob * midprob * tailprob;
											if(prob>0.12f && prob>prob2) nelen = 3;
											else if(prob2>0.12f && prob2>prob) nelen =2;
										}
										else{
											//check 2 single hz
											if(prob2>0.12f) nelen = 2;
										}
									}
								}
								else{
									//only two single hz
									tailidx = _GET_IDX(SegTagUnits[i+1].Word);
									if(_IS_VALID(SegTagUnits[i+1].Word)){
										tailprob = ptailname[tailidx];
										
										prob = 1000000. * surprob * 0.001f * tailprob;
										if(prob>0.12f){
											nelen = 2;
										}
									}
								}
							}
						}

						//output according to the nelen
						if(dest){
							for(k=0;k<nelen;k++)
								strcat(dest,SegTagUnits[i+k].Word);
							strcat(dest," ");
							dest += strlen(dest);
						}
						else{
							for(k=0;k<nelen;k++)
								printf("%s", SegTagUnits[i+k].Word);
							if(nelen>1)	printf("/ne ");
							else printf("/%s ",SegTagUnits[i].Cate);
						}

						i+=nelen-1;
					#else
						//output normally
						if(dest){
							strcat(dest,SegTagUnits[i].Word);
							strcat(dest," ");
							dest += strlen(dest);
						}
						else printf("%s/%s ", SegTagUnits[i].Word,SegTagUnits[i].Cate);
					#endif	
						
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

