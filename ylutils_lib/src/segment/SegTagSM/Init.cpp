//#include	<windows.h>
//#include	"stdafx.h"	//zanhy 2002-12-23

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h> //dir mng

#include	"Basicdat.h"
#include	"HBasDict.h"
#include	"HSpDict.h"
#include	"Wordform.h"
#include	"Sbtprep.h"


#ifdef UNIX_SEGTAG
	void UnloadDicts();		///in setmem.c
	#define _MAX_PATH	260
#endif


//
//////// SLex initialization
//
// 
/*
int	SlexInitialize(char *path)
{ 
	char curPath[_MAX_PATH];
	
	ErrorInit();		// initialize processing error flag

	if(path==0) {
////set the dictionaries directory: 
//			all lexicons(as well as other data) should be kept 
//			under "%cur-path%\Lexicons\"
//
	// for Word97
		//Get the current working directory
		if( getcwd( curPath, _MAX_PATH ) == NULL )
			//perror( "_getcwd error" )
			;
		strcat(curPath, "/lexicons/");
	}
	else
		strcpy(curPath, path);


	SetDictDirectory(curPath);
	
	//	read the basic/general-purpose dictionary 
	ReadBasicDict(); 
	
	//  read user-define(special) dictionaries
	ReadSpecDict();   
	
	//  input lexical_fix list
	if (InitLexFixList()) 
		return 0;       

	//  input overlapping word list
	if (InitOverlWordList()) 
		return 0;    
	
	//  initialize the data for SBT	
	InitSBTData();

	if (AnyError())
		return 0;

	return 1;
}   

*/
//
////////////////
//
//
int	SlexUninitialize(void)
{
	ErrorInit();		/* initialize processing error flag;	*/

	/*	free the allocated memory used by the path variables	*/
	FreePathMem();

	/*	free the allocated memory used by S&T dictionary	*/
    FreeSpecDictMem(); 
	FreeBasicDictMem();
    
	/*	free the allocated memory used by data file names	*/
    FreeLexDataFNMem(); 
    FreeOverlapFNMem();

#ifdef UNIX_SEGTAG
	UnloadDicts();
#endif

    
	if (AnyError())	
		return 0;
	
	return 1;
}

