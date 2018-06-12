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

			///Chinese chars, only combin "ȫ������" and "��������":
			///	"ȫ������" �������������������� -- 0xA3(B0,B1,...B9)
			///	"��������" ��һ�����������߰˾�ʮ��ǧ����
			if(wordbuf1[0]<0 && wordbuf2[0]<0 ) 
			{
					if(
						strstr("�ڣ�������������������",word1fchar)&&strstr("��������������������������ʮ��ǧ����",word2fchar) 
						||
						strstr("�ڡ�һ�����������߰˾�ʮ��ǧ���ڼ�إ��",word1fchar)&&strstr("��������һ�����������߰˾�ʮ��ǧ���ڼ�",word2fchar)
						
					  )
					{
						CombWord(i,2,"m");
						j=GetWordSum();
						i=k; //i=0;
						continue;
					}
			}
		}
		//// "��/��" + "ʮ��ǧ����" --> m :
		if( (!strcmp(wordbuf1,"��")||!strcmp(wordbuf1,"��")) &&
			strstr("ʮ��ǧ����",word2fchar) )
		{
			CombWord(i,2,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "��" + m --> m :
		if( !strcmp(wordbuf1,"��") && !strcmp(GetCate(i+1),"m") )
		{
			CombWord(i,2,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// m + "��֮" + m --> m :
		if( !strcmp(GetCate(i),"m") && !strcmp(wordbuf2,"��֮") && !strcmp(GetCate(i+2),"m") )
		{
			CombWord(i,3,"m");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "19xx/����xx/һ��xx" + "��" --> t :
		if( ( strstr(wordbuf1,"19")||strstr(wordbuf1,"����")||strstr(wordbuf1,"һ��")
			  || strstr(wordbuf1,"20")||strstr(wordbuf1,"����") 
			 )
			&& !strcmp(wordbuf2,"��") )
		{
			CombWord(i,2,"t");
			j=GetWordSum();
			i=k;
			continue;
		}
		//// combine "x" + "��" --> t :
		if( !strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"��")
			||
		//// combine "x" + "��" --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"��") 
		  )
		{
			CombWord(i,2,"t");
			j=GetWordSum();
			i=k;
			continue;
		}

		//// combine "x" + "ʱ" --> t :
		if( !strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"ʱ")
			||
		//// combine "x" + "��" + ("x��") --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"��") && !strcmp(GetCate(i+2),"m")/* && !strcmp(GetWord(i+3),"��") */
			||
		/// combine ��"xʱ"�� + "x" + "��" --> t :
			!strcmp(GetCate(i),"m") && !strcmp(GetWord(i+1),"��") &&
			(strstr(GetWord(i-1),"ʱ")||strstr(GetWord(i-1),"��"))
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
		//"ʡ"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"��"
		if( !strcmp(GetCate(i),"ns") && 
			(!strcmp(wordbuf2,"ʡ") ||!strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��") ||
			 !strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��") ||
			 !strcmp(wordbuf2,"��") ||
			 !strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��") ||
			 !strcmp(wordbuf2,"��") ||!strcmp(wordbuf2,"��")
			) 
		  )
		{
			CombWord(i,2,"ns");
			j=GetWordSum();
			i=k; //i=0;
			continue;
		}

		/// "��" + "��" --> "����"
		if( !strcmp(wordbuf1,"��") && !strcmp(wordbuf2,"��") ||
			!strcmp(wordbuf1,"��") && !strcmp(wordbuf2,"��") ||
			!strcmp(wordbuf1,"����") && !strcmp(wordbuf2,"��") ||
			!strcmp(wordbuf1,"��") && !strcmp(wordbuf2,"��") )
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
