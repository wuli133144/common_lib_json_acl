/////
//		Common vars for Bas- and Sp- Dict handling
//	
//#include "stdafx.h"	//zanhy 2002-12-23

#include "DictComm.h"

char    CateBuf[MAX_WordCatesLen+1]; /* buffer for the category information	 

										- all possible POS (all °∞ºÊ¿‡°±!!) */
//
///////////////////////// external func's
//
//

/*	purpose :	test whether a category tag is in the previous tag set
 *	argument :	TagSet --- the previous tag set(every tag is seperated by '-')
 *				cate --- the category tag for testing
 *	return :	1 --- if in;	0 --- otherwise.
 */
int	InTagSet(char *TagSet, char *cate)
{
	int	StPos, EndPos;
	
	StPos = EndPos = 0;
	while (TagSet[StPos])	
	{
		/*	search the end position a tag in the tag set	*/
		EndPos = StPos;
		while (TagSet[EndPos] && (TagSet[EndPos]!='-'))	EndPos++;
		if (!TagSet[EndPos])
			break;		/* go to the ending position of the tag string	*/
		if ( ((size_t)(EndPos-StPos)==strlen(cate)) && 
			  !strncmp(&TagSet[StPos],cate,EndPos-StPos))
			return 1;
		StPos = EndPos+1; 		
	}
    return 0;
}


//
////// We now allow C++ single-line comments in all the lexicons; C cmmnts as well!
//
//

//(not really C++ comments: must not be put on the same line of a word entry:)
void  SkipComments(FILE * fp) 
{
	int c1=0, c2=0,  i=0;
/// Skip all C/C++ comments:
//
	while(!feof(fp))
	{	
		if( (c1=fgetc(fp))=='\n'||c1=='\r'||c1==' '||c1=='\t' ) 
			continue;
		if(c1=='/') {
			c2=fgetc(fp); //move ahead

			if(c2=='/') {  //a C++ single-line cmmnt starts
				while(!feof(fp)) {
					c2=fgetc(fp);
					if(	c2=='\n' || c2=='\r')
						break;
				}
			}	
			else 
				if(c2=='*') {  // a C cmmnt starts
					while(!feof(fp)) {
						if(	(c2=fgetc(fp))=='*') {
							if((c2=fgetc(fp))=='/')
							break;
						}
					}
				}
				else { //not a cmmnt
					ungetc(c2, fp);
					break;
				}
		}
		else { //not a cmmnt or a empty char, that's a entry string
			ungetc(c1, fp);
			break;
		}
	} //while()
/*
	int c1=0, c2=0;
  //NOTE: precedence of '=' less than that of '==' opr!
	while( !feof(fp) && ( (c1=fgetc(fp))=='/' || 
			c1=='\n' || c1=='\r' || c1==' ' || c1=='\t' ) )
	{		// ^Important: should also skip all empty chars(before a cmmnt or between 2 cmmnts)!!
		if(c1=='/' ) 
		{
			if((c2=fgetc(fp)) == '/') //a single-line cmmnt starts
			{
				while(!feof(fp)) 
				{
					c2=fgetc(fp);
					if(	c2=='\n' || c2=='\r')
						break;
				}
			}	
			else
				ungetc(c2, fp);
		}

	   //// Important: should also skip all empty lines(between 2 cmmnts)!!
	   //
		//if(	c1=='\n' || c1=='\r')
		//	continue;

	} //while()
	ungetc(c1, fp);
/// ^
/////// ^should send the byte back to the f-stream!
*/
}


extern void	 GetNextTokenString(FILE * fp, char * pBuff)
{
	int c1=0, c2=0,  i=0;
	
/// Skip all C/C++ comments:
//
	while(!feof(fp))
	{	
	//Important: should also skip all empty chars(before a cmmnt or between 2 cmmnts)!
	//  NOTE: precedence of '=' less than that of '==' opr!
	//
		if( (c1=fgetc(fp))=='\n'||c1=='\r'||c1==' '||c1=='\t' ) 
			continue;
		
		if(c1=='/') {
			c2=fgetc(fp); //move ahead

			if(c2=='/') {  //a C++ single-line cmmnt starts
				while(!feof(fp)) {
					c2=fgetc(fp);
					if(	c2=='\n' || c2=='\r')
						break;
				}
			}	
			else 
				if(c2=='*') {  // a C cmmnt starts
					while(!feof(fp)) {
						if(	(c2=fgetc(fp))=='*') {
							if((c2=fgetc(fp))=='/')
							break;
						}
					}
				}
				else { //not a cmmnt
					ungetc(c2, fp);
					break;
				}
		}
		else //not a cmmnt or a empty char, that's a entry string
			break;
	} //while()

//// now let's get the dict entry(word/category) string:
//
	//fscanf(fp,"%s",pBuff);
	while(!feof(fp)) {
		if(c1==' '||c1=='\t'||c1=='\n'||c1=='\r'||c1=='/')// maybe another C/C++ cmmnt
			break;
		pBuff[i++]=c1;
		c1=fgetc(fp);
	}
	if(c1=='/')//maybe another C/C++ cmmnt immediately after a word category!
		ungetc(c1, fp);
	if(/* i<=MAX_BasWordLen ||*/ i<=MAX_WordLen)
		pBuff[i]='\0';
	else 
		ErrorInfo("Buffer overflow\n");
}

