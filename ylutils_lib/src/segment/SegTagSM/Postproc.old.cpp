/***
	PostProc.c :  S-T post-processing

 		Now all seg & tag processing have benn finished;
		there is still a chance to do some post-processing in the ST-buffer 

****/
//#include "stdafx.h"	//zanhy 2002-12-23

#include        <stdio.h>
#include        <stdlib.h>
#include		<string.h>

#include		"Basicdat.h"
#include		"Limits.h"

/* The only func in the file -
 *	
 *	  do some post-processing in the ST buffer(words[wid] and cates[wid]).
 *
 */
void     WordFormProc()
{
	int	i,j,k;
	///C++ needs convertor form singned char[] to char*, but free for char[] to char* :
	  //signed char wordbuf1[MAX_WordLen+1], wordbuf2[MAX_WordLen+1];
	char wordbuf1[MAX_WordLen+1], wordbuf2[MAX_WordLen+1];
	char word1fchar[4], word2fchar[4];	

	k=0;
	j=GetWordSum();
	i=0;

	////for (i=0; i<j; i++, j=GetWordSum())	{
	while(i<j-1){

		//*	Vg+Vg-->v	
		if (!strcmp(GetCate(i),"Vg") && !strcmp(GetCate(i+1),"Vg"))	{
			CombWord(i,2,"v");    
			j=GetWordSum();
			i=k; 
			continue;
		}
		//*	Ag+Ag-->a	
		if (!strcmp(GetCate(i),"Ag") && !strcmp(GetCate(i+1),"Vg")) {
			CombWord(i,2,"a");
			j=GetWordSum();
			i=k; 
			continue;
		}

/////bs: <<China Daily 98>> demands:
///
		strcpy(wordbuf1, GetWord(i));
		strcpy(wordbuf2, GetWord(i+1));

		word1fchar[0]=wordbuf1[0];	word1fchar[1]=wordbuf1[1];
		word1fchar[2]='\0';
		word2fchar[0]=wordbuf2[0];	word2fchar[1]=wordbuf2[1];
		word2fchar[2]='\0';

		/// m + m + ... + m --> m :
		if (strstr(GetCate(i),"m") && strstr(GetCate(i+1),"m")) 
		  //if (!strcmp(GetCate(i),"m") && !strcmp(GetCate(i+1),"m")) 
		{
			///don't mix ASCII and Chinese digital char:
			if(wordbuf1[0]>0 && wordbuf2[0]>0)
			{
				CombWord(i,2,"m");
				j=GetWordSum();
				i=k; //i=0;
				continue;
			}

			///Chinese chars, only combin "全角数字" and "汉字数字":
			///	"全角数字" ０１２３４５６７８９ -- 0xA3(B0,B1,...B9)
			///	"汉字数字" ○一二三四五六七八九十百千万亿
			if(wordbuf1[0]<0 && wordbuf2[0]<0 ) 
			{
					if(
						strstr("第０１２３４５６７８９",word1fchar)&&strstr("．％·０１２３４５６７８９十百千万亿",word2fchar) 
						||
						strstr("第○一二三四五六七八九十百千万亿几廿两",word1fchar)&&strstr("．·○零一二三四五六七八九十百千万亿几",word2fchar)
						
					  )
					{
						CombWord(i,2,"m");
						j=GetWordSum();
						i=k; //i=0;
						continue;
					}
			}
		}
		//// "成/上" + "十百千万亿" --> m :
		if( (!strcmp(wordbuf1,"成")||!strcmp(wordbuf1,"上")) &&
			strstr("十百千万亿",word2fchar) )
		{
			CombWord(i,2,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "第" + m --> m :
		if( !strcmp(wordbuf1,"第") && !strcmp(GetCate(i+1),"m") )
		{
			CombWord(i,2,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// m + "分之" + m --> m :
		if( !strcmp(GetCate(i),"m") && !strcmp(wordbuf2,"分之") && !strcmp(GetCate(i+2),"m") )
		{
			CombWord(i,3,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "19xx/１９xx/一九xx" + "年" --> t :
		if( ( strstr(wordbuf1,"19")||strstr(wordbuf1,"１９")||strstr(wordbuf1,"一九")
			  || strstr(wordbuf1,"20")||strstr(wordbuf1,"２０") 
			 )
			&& !strcmp(wordbuf2,"年") )
		{
			CombWord(i,2,"t");
			j=GetWordSum();
			i=k;
			continue;
		}
		//// combine "x" + "月" --> t :
		if( !strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"月")
			||
		//// combine "x" + "日" --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"日") 
		  )
		{
			CombWord(i,2,"t");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "x" + "时" --> t :
		if( !strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"时")
			||
		//// combine "x" + "点" + ("x分") --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"点") && !strcmp(GetCate(i+2),"m")/* && !strcmp(GetWord(i+3),"分") */
			||
		/// combine （"x时"） + "x" + "分" --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"分") &&
			(strstr(GetWord(i-1),"时")||strstr(GetWord(i-1),"点"))
		  ) 
		{
			CombWord(i,2,"t");
			j=GetWordSum();
			i=k;
			continue;
		}
	/*need a second pass when the following 't' not combined!
		////// t + t + ... --> t:
		if (!strcmp(GetCate(i),"t") && !strcmp(GetCate(i+1),"t"))
		{
				CombWord(i,2,"t");
				j=GetWordSum();
				i=k; //i=0;
				continue;
		}
	*/

		/// combine some place name:
		//"省"、"市"、"县"、"区"、"乡"、"镇"、"村"、"旗"、"州"、"都"、"府"、"道"
		if( !strcmp(GetCate(i),"ns") && 
			(!strcmp(wordbuf2,"省") ||!strcmp(wordbuf2,"市") ||!strcmp(wordbuf2,"县") ||
			 !strcmp(wordbuf2,"区") ||!strcmp(wordbuf2,"乡") ||!strcmp(wordbuf2,"镇") ||
			 !strcmp(wordbuf2,"村") ||
			 !strcmp(wordbuf2,"旗") ||!strcmp(wordbuf2,"州") ||!strcmp(wordbuf2,"都") ||
			 !strcmp(wordbuf2,"府") ||!strcmp(wordbuf2,"道")
			) 
		  )
		{
			CombWord(i,2,"ns");
			j=GetWordSum();
			i=k; //i=0;
			continue;
		}

		/// "－" + "－" --> "－－"
		if( !strcmp(wordbuf1,"－") && !strcmp(wordbuf2,"－") ||
			!strcmp(wordbuf1,"—") && !strcmp(wordbuf2,"—") ||
			!strcmp(wordbuf1,"——") && !strcmp(wordbuf2,"—") ||
			!strcmp(wordbuf1,"…") && !strcmp(wordbuf2,"…") )
		{
			CombWord(i,2,"w");
			j=GetWordSum();
			i=k; //i=0;
			continue;
		}

		/// j + j --> j:
		if (!strcmp(GetCate(i),"j") && !strcmp(GetCate(i+1),"j"))
		{
				CombWord(i,2,"j");
				j=GetWordSum();
				i=k; //i=0;
				continue;
		}

		//pb,030708,not use this one for many unknow fanti chinese characters there
		/// unknown words combination:
		//if (!strcmp(GetCate(i),"x") && !strcmp(GetCate(i+1),"x"))
		//{
		//		CombWord(i,2,"x");
		//		j=GetWordSum();
		//		i=k; //i=0;
		//		continue;
		//}

		i++;
		k++;
	}///while(i<j-1)
}
