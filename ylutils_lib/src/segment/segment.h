/*
 * =====================================================================================
 * 
 *        Filename:  segment.h
 * 
 *     Description:  包含原分词库中的函数声明.
 * 
 *         Version:  1.0
 *         Created:  2006年07月01日 16时22分07秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup segment
 *  
 *  
 *  @{
 */

#ifndef SEGMENT_H_ZMH_2006_07_01
#define SEGMENT_H_ZMH_2006_07_01

#include <iostream>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

int		InitShareMem(char* fname);
int		MountShareMem(char* fname);
int		UnloadShareMem(char* fname);
int 	SegTagString(const char* buf,char* dest,const char* reservstr);


#ifdef __cplusplus
}
#endif

void    SetDictDirectory(char *DictDirStr);
extern  void    ReadSpecDict(void);



//! for public interface
void 	InitShareData();
//! for public interface

void	SaveShareData(char* fname);

/**
 * @brief    将词典载入内存.
 *
 * @param    fname    词典文件.
 * @return   载入状态.
 * @retval   0   载入成功.
 * 	     -1  打开有错.
 * 	     -2  词典文件格式有错.
 */
int	LoadShareData(char* fname);

//! zmh 由于调用LoadShareData会有内存泄漏,所以将内存申请放大外面申请外面释放.
bool	ValidDictData(const char *pbuf, int len);
void	LoadShareDataEx(char* pbuf);

//
////for DataFile head
////
struct SShareDataHead{
        char    magic[4];
	int     len;
};




//!  SegTagSM/segtagProcess.h
int mainConvert(char *, char *, int nocate);
//!  SegTagSM/segtagProcess.h
void mainConvertStr(char* buf);
//!  SegTagSM/segtagProcess.h);
int InitConvert(char *curPath);
//!  SegTagSM/segtagProcess.h
int FreeConvert();
#endif //SEGMENT_H_ZMH_2006_07_01
/** @} */ //
