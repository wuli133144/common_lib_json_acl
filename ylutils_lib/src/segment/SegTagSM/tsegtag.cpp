// textToPinyiinDlg.cpp : implementation file
// created by Zhang Hong 2000, modified by Li Baoli 2001-02, transplanted by zanhy 2002-12 
// pb,test segtag functions only

#include <string.h>
#include <iostream>
#include <memory.h>
#include <stdio.h>
//#include <direct.h>
#include <unistd.h>

#include "segtagProcess.h"
#include "ShareMem.h"
#include "CRoutine.h"

using namespace std;
#define DICTDATA "dictdata.img"
#define DICTDATA2 "dictdata2.img"

void dohelp()
{
	printf("useage: tsegtag -<file|share|load|static|parser|unload> <srcfile> <destfile>\n");
}

int main(int argc,char*argv[])
{
	char sFirstPath[100];
	int	bInit = 1;

	if(getcwd(sFirstPath, 100 ) == NULL ){
		printf("Can't get current path!\r");
		return 2;
	}
	
	
	if(argc<2){
		//convert the inputstring
		printf("Please input your string...");
		if(MountShareMem(DICTDATA)){
			if(!access(DICTDATA,F_OK)){
				printf("load dictionary: static\n");
				InitShareData();
				LoadShareData(DICTDATA);

				char dict[256];
			        sprintf(dict,  "%s.new", DICTDATA);
				SetDictDirectory("./");
				ReadSpecDict();

				SaveShareData(dict);
			}
			else{
				printf("load dictionary: file\n");
				InitShareData();
				InitConvert(sFirstPath);
			}
		}
		else{
			printf("load dictionary: sharemem\n");
		}
		
		char buf[512];
		
		while(fgets(buf,512,stdin)){
			SegTagString(buf,NULL,",:.");
			printf("\n");
		}

		return 0;
	}
	
	//argc>2
	if(!strcmp(argv[1],"-static")){
		if(argc!=4){
			dohelp();
			return -1;
		}
		//doloadstatic();
		if(!access(DICTDATA,F_OK)){
			LoadShareData(DICTDATA);
			mainConvert(argv[2],argv[3],0);
		}
		else{
			printf("dict image not found\n");
		}
	}
	else if(!strcmp(argv[1],"-u")){
		if(!UnloadShareMem(DICTDATA)){
			printf("UnLoad dictdata.img into sharemem ok!\n");
		}
	}
	else if(!strcmp(argv[1],"-load")){
		if(argc!=2){
			dohelp();
			return -1;
		}
		//doinisharemem();

		if(!InitShareMem(DICTDATA)){
			printf("Load dictdata.img into sharemem ok!\n");
		}
	}
	else if(!strcmp(argv[1],"-share")){
		if(argc!=4){
			dohelp();
			return -1;
		}
		//doloadsharemem();

		if(!MountShareMem(DICTDATA)){
			mainConvert(argv[2],argv[3],0);
		}
	}
	else if(!strcmp(argv[1],"-file")){
		//do it normally
		printf("Normally segtag process....");
		InitShareData();
		InitConvert(sFirstPath);
		if(access(DICTDATA,F_OK)) SaveShareData(DICTDATA);
		
		mainConvert(argv[2],argv[3],0);
		
		//confirm that it's read-only
		if(access(DICTDATA2,F_OK)) SaveShareData(DICTDATA2);
		
		FreeConvert();
	}
	else if(argc>=4 && !strcmp(argv[1],"-parser")){
		//convert the inputstring
		printf("Please input your string...");
		if(MountShareMem(DICTDATA)){
			if(!access(DICTDATA,F_OK)){
				printf("load dictionary: static\n");
				LoadShareData(DICTDATA);
			}
			else{
				printf("load dictionary: file\n");
				InitShareData();
			}
		}
		else{
			printf("load dictionary: sharemem\n");
		}
		
		mainConvert(argv[2],argv[3],1);
	}
	else{
		dohelp();
	}
	
	return 0;

}
